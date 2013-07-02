//=========================== ChamferMatching =================================


#include "chamfermatching.h"
#include <sys/timeb.h>
#include <time.h>
#include <qdir.h>
#include <cassert>
#include <algorithm>
#include <math.h>
#include <logging.h>
#include <imageoperations.h>
#include <opgrayimage.hh>
#include <fileoperations.h>
#include <stringoperations.h>
#include <qprogressdialog.h>
#include <qtimgbrowser.hh>
//#include <eimagebrowser.h>

static const unsigned int MAX_CANDIDATES = 20;


ChamferMatching::ChamferMatching()
{
  //browser = new EImageBrowser();
  //browser->show();

  _levels=3;
  _edgeNormalization=false;
  _symmetryCheck=false;
  _candidateThresh=200;
  _coverThresh=0.8;
  _cannyLow=0.6;
  _cannyHigh=0.7;
  _sigma=2.0;
  _colorCanny=false;
}

///////////////////////////////////////////////////////////////////////////////
//
// HELPER FUNCTIONS
//
//
//
//

double ChamferMatching::distanceOnFeatures( const OpGrayImage& dt,
    const EdgePtVec&   vPoints,
    int shift_x, int shift_y,
    int& count )
{
  int w = dt.width();
  int h = dt.height();
  double dist =0;
  count = 0;

  vector<QPoint>::const_iterator it;
  for( it = vPoints.begin(); it!=vPoints.end(); it++)
  {
    // check if feature is out of bounds //
    int posx = shift_x + it->x();
    int posy = shift_y + it->y();
    if( (posy >= 0) && (posy < h) &&
        (posx >= 0) && (posx < w) )
    {
      dist += dt(posx, posy).value();
      count++;
    }
  }
  return dist;
}


double ChamferMatching::distanceOnFeatures( const OpGrayImage &imgDt,
    const EdgePtVec   &vPoints,
    int shift_x, int shift_y,
    float dScaleFact,
    int &nCount )
{
  int w = imgDt.width();
  int h = imgDt.height();
  double dist =0;
  nCount = 0;

  for( int i=0; i<(int)vPoints.size(); i++ )
  {

    int cx = (int)floor(shift_x + vPoints[i].x()*dScaleFact + 0.5);
    int cy = (int)floor(shift_y + vPoints[i].y()*dScaleFact + 0.5);

    // check if feature is out of bounds //
    if( (cy >= 0) && (cy < h ) &&
        (cx >= 0) && (cx < w ) )
    {
      dist += imgDt(cx, cy).value();
      nCount++;
    }
  }

  return dist;
}


double ChamferMatching::calcDistanceAtPosition( int x, int y,
    const OpGrayImage& imgDt,
    const EdgePtVec&   vPoints)
{
  double dDist;
  int    nCount = 0;

  int    nNumPoints = (int)vPoints.size();
  //assert(vPoints.size() >0);

  //-- add distance value on feature points of template --//
  dDist = distanceOnFeatures( imgDt, vPoints, x, y, nCount );

  //-- Adding of pixel outside of the edges --//
  dDist = dDist+255.0*(nNumPoints-nCount);
  dDist = dDist/((double) nNumPoints);

  return dDist;
}


double ChamferMatching::calcDistanceAtPosition( int x, int y,
    float dScaleFact,
    const OpGrayImage &imgDt,
    const EdgePtVec   &vPoints )
{
  double dDist;
  int    nCount = 0;

  int nNumFeatures = vPoints.size();

  //-- add distance value on feature points of template --//
  dDist = distanceOnFeatures( imgDt, vPoints, x, y, dScaleFact, nCount);

  //-- Adding of pixel outside of the edges --//
  dDist = dDist + 255.0*(nNumFeatures-nCount);
  dDist = dDist/((double) nNumFeatures);

  return dDist;
}


double ChamferMatching::calcDistanceAtPosition( int x, int y,
    const QSize&         dim,
    const IntegralImage& iimg,
    const OpGrayImage&   imgDt,
    const EdgePtVec&     vPoints )
{
  double dDist = calcDistanceAtPosition( x, y, imgDt, vPoints );

  if (_edgeNormalization)
    dDist = ( dDist/iimg.getImageArea( x,y, dim.width(), dim.height() ) *
              dim.width()*dim.height() );

  //printf("iiarea: %f tplsize: %dx%d\n", ii.getImageArea(x,y,dim.width(),
  //       dim.height()), dim.width(), dim.height());
  return dDist;
}

double ChamferMatching::normalizedSymDistance(ChamferImage& patch1, ChamferImage& patch2)
{
  assert(patch1.width()==patch2.width() && patch1.height()==patch2.height());
  float dist1 = calcDistanceAtPosition(0,0, patch1.getDT(0), patch2.getFeatures(0));
  float dist2 = calcDistanceAtPosition(0,0, patch2.getDT(0), patch1.getFeatures(0));
  float dist =  1.0 - min(1.0,(dist1+dist2)/2.0/100.0);
  //printf("%.4f %.4f -> %.4f\n", dist1, dist2, dist);
  return dist;
}

double ChamferMatching::normalizedDistance(ChamferImage& patch1, ChamferImage& patch2)
{
  assert(patch1.width()<=patch2.width() && patch1.height()<=patch2.height());
  float dist1 = calcDistanceAtPosition(0,0, patch2.getDT(0), patch1.getFeatures(0));
  float dist =  1.0 - min(1.0,dist1/100.0);
  //printf("%.4f %.4f -> %.4f\n", dist1, dist2, dist);
  return dist;
}

void ChamferMatching::matchTemplateOnWindow( CandidateSet&   vCandidates,
    const OpGrayImage&   imgEdges,
    const OpGrayImage&   imgDt,
    const IntegralImage& iimg,
    const OpGrayImage&   imgTempl,
    const EdgePtVec&     vPoints,
    int x, int y, int w, int h )
{
  double dDist=0;

  int img_h = imgDt.height();
  int img_w = imgDt.width();
  int tpl_h = imgTempl.height();
  int tpl_w = imgTempl.width();

  //-- shift template of over  edge image --//
  for( int shift_y = y; shift_y<min(img_h,y+h); shift_y++ )
  {
    for( int shift_x = x; shift_x<min(img_w,x+w); shift_x++ )
    {
      dDist = calcDistanceAtPosition( shift_x, shift_y,
                                      QSize( imgTempl.width(),
                                             imgTempl.height() ),
                                      iimg, imgDt, vPoints );

      //-- add to candidateset --//
      vCandidates.insert( Candidate(QRect(shift_x, shift_y, tpl_w, tpl_h),
                                    dDist, QString::null, 0));
      if( vCandidates.size()>MAX_CANDIDATES )
        vCandidates.erase(--vCandidates.end());

    }
  }
}


void ChamferMatching::matchTemplateOnWindow( CombiCandidateSet &vCandidates,
    const OpGrayImage &imgDT,
    const OpGrayImage &imgTempl,
    const EdgePtVec &vPoints,
    int nTemplateId,
    int x, int y, int w, int h,
    int nMaxCandidates )
{
  double dDist=0;

  int img_h = imgDT.height();
  int img_w = imgDT.width();
  int tpl_h = imgTempl.height();
  int tpl_w = imgTempl.width();

  //-- shift template of over  edge image --//
  int maxx = min( img_w, x+w );
  int maxy = min( img_h, y+h );
  for( int shift_y = y; shift_y<maxy; shift_y++ )
    for( int shift_x = x; shift_x<maxx; shift_x++ )
    {

      dDist = calcDistanceAtPosition( shift_x, shift_y, imgDT, vPoints );

      //-- add to candidateset --//
      vCandidates.insert( CombiCandidate( QRect(shift_x, shift_y, tpl_w,tpl_h),
                                          dDist, nTemplateId, 0) );

      if( (int)vCandidates.size() > nMaxCandidates )
        vCandidates.erase( --vCandidates.end() );
    }
}


void ChamferMatching::matchTemplateOnWindow( CombiCandidateSet &vCandidates,
    const OpGrayImage &imgDt,
    const OpGrayImage &imgTempl,
    const EdgePtVec &vPoints,
    int nTemplateId,
    int x, int y, int w, int h,
    float dScaleFact,
    int nStepSize,
    int nMaxCandidates )
{
  double dDist=0;

  int img_h = imgDt.height();
  int img_w = imgDt.width();
  int tpl_h = (int)floor(imgTempl.height()*dScaleFact + 0.5);
  int tpl_w = (int)floor(imgTempl.width()*dScaleFact + 0.5);

  //-- shift template of over  edge image --//
  int maxx = min( img_w, x+w );
  int maxy = min( img_h, y+h );
  for( int shift_y = y; shift_y<maxy; shift_y+=nStepSize )
    for( int shift_x = x; shift_x<maxx; shift_x+=nStepSize )
    {

      dDist = calcDistanceAtPosition( shift_x, shift_y, dScaleFact,
                                      imgDt, vPoints );

      //-- add to candidateset --//
      vCandidates.insert( CombiCandidate( QRect(shift_x, shift_y, tpl_w,tpl_h),
                                          dScaleFact, dDist, nTemplateId, 0) );

      if( (int)vCandidates.size() > nMaxCandidates )
        vCandidates.erase( --vCandidates.end() );
    }
}


void ChamferMatching::getBlackPoints( const OpGrayImage& img,
                                      EdgePtVec& points,
                                      const OpGrayImage* mask )
{
  if (mask!=0)
  {
    for (int x=0; x<img.width(); x++)
      for (int y=0; y<img.height(); y++)
        if ( img(x, y).value()==0 && (*mask)(x,y).value()==0 )
          points.push_back(QPoint(x,y));

  }
  else
  {
    for (int x=0; x<img.width(); x++)
      for (int y=0; y<img.height(); y++)
        if ( img(x, y).value()==0)
          points.push_back(QPoint(x,y));
  }
}


void ChamferMatching::getDarkPoints( const OpGrayImage& img,
                                     EdgePtVec& points,
                                     const OpGrayImage* mask )
{
  int thresh=50;
  if( mask != 0 )
  {
    for (int x=0; x<img.width(); x++)
      for (int y=0; y<img.height(); y++)
        if ( img(x, y).value()<thresh && (*mask)(x,y).value()==0 )
          points.push_back(QPoint(x,y));

  }
  else
  {
    for (int x=0; x<img.width(); x++)
      for (int y=0; y<img.height(); y++)
        if ( img(x, y).value()<thresh)
          points.push_back(QPoint(x,y));
  }
}


/****************************/
/*  Preprocessing methods   */
/****************************/

EdgePtVec ChamferMatching::prepareTemplate( const OpGrayImage &imgTempl )
{
  EdgePtVec vPoints;

  for( int y=0; y<imgTempl.height(); y++ )
    for( int x=0; x<imgTempl.width(); x++ )
      if( imgTempl(x,y).value() != 0.0 )
        vPoints.push_back( QPoint(x,y) );

  return vPoints;
}


OpGrayImage ChamferMatching::getDTImage( const OpGrayImage &img, 
                                         float dThreshLo, float dThreshHi )
{
  OpGrayImage imgEdges, imgDT;

  ImageOperations::edgeCanny( img, imgEdges, dThreshLo, dThreshHi );
  ImageOperations::distanceTransform( imgEdges, imgDT );
  
  return imgDT;
}


void ChamferMatching::cleanCandidates( CandidateSet& vCandidates,
                                       float dThresh, float dCover )
{
  printf("Cleaning with thresh: %f\n", dThresh);
  CandidateSet vAccepted;
  CandidateSet::iterator it, it2;
  for( it = vCandidates.begin(); it!=vCandidates.end(); it++ )
  {
    bool bKnown = false;

    for( it2 = vAccepted.begin(); it2!=vAccepted.end(); it2++ )
    {
      if( ImgDescrList::compCover(it->getRect(), it2->getRect()) > dCover )
        bKnown = true;
    }
    if( !bKnown && (it->getDist() < dThresh) )
      vAccepted.insert(*it);
    //else printf("skipped: %d & %f\n", bKnown, it->getDist());
  }
  vCandidates = vAccepted;
}


double ChamferMatching::symmetryCheck( const OpGrayImage& imgEdges,
                                       unsigned int nTemplIdx,
                                       int x, int y)
{
  //-- get precalculated DT of template --//
  ChamferImage&  cimgTpl  = _templates[nTemplIdx];
  OpGrayImage&   imgTempl = cimgTpl.getEdges(0);
  OpGrayImage&   imgDt    = cimgTpl.getDT(0);
  OpGrayImage&   imgMask  = cimgTpl.getMask();
  IntegralImage& iimg     = cimgTpl.getIntegralImage(0);
  EdgePtVec      vPoints;

  //-- extract edge region from edge image --//
  OpGrayImage imgEdgeWin = imgEdges.extractRegion(x, y,
                           x+imgTempl.width()-1,
                           y+imgTempl.height()-1);
  //browser->pushImage(edgeWindow.getQtImage(),"");
  //browser->pushImage(mask.getQtImage(),"");
  getBlackPoints( imgEdgeWin, vPoints, &imgMask);

  //-- calc distance from edges to templ --//
  double dDist = calcDistanceAtPosition(0,0, cimgTpl.getDimensions(0),
                                        iimg, imgDt, vPoints );
  //printf("symm dist: %f\n", dist);
  return dDist;
}


void ChamferMatching::symmetryCheck( const CandidateSet& vCandidates,
                                     CandidateSet& vRet,
                                     const OpGrayImage& imgEdges)
{
  double dDist;
  CandidateSet::iterator it;
  for( it=vCandidates.begin(); it!=vCandidates.end(); it++)
  {
    cout << "dist: " << it->getDist() << " ";
    dDist = symmetryCheck( imgEdges, it->getIdx(),
                           it->getPos().x(), it->getPos().y());
    Candidate tmp=*it;
    tmp.getDist() += dDist;
    cout << "total dist: " << tmp.getDist() << endl;
    vRet.insert(tmp);
  }
}


void ChamferMatching::clear()
{
  _candidateSets.clear();
}


void ChamferMatching::setOptions( int levels, bool symmetryCheck,
                                  bool edgeNorm,
                                  float candidateThresh, float coverThresh,
                                  float tlow, float thigh,
                                  float sigma, bool colorCanny, bool planes)
{
  _levels=levels;
  _edgeNormalization=edgeNorm;
  _symmetryCheck=symmetryCheck;
  _candidateThresh=candidateThresh;
  _coverThresh=coverThresh;
  _cannyLow=tlow;
  _cannyHigh=thigh;
  _sigma=sigma;
  _colorCanny=colorCanny;
  _planes=planes;
}


void ChamferMatching::setTemplates(const QStringList& tplNames)
{
  printf("Adding Templates: ");
  _templates.clear();
  //-- store templates --//
  if (!tplNames.isEmpty())
  {
    _templateNames=tplNames;
    QStringList::const_iterator it;
    for( it=tplNames.begin(); it!=tplNames.end(); it++)
    {
      printf("*");
      _templates.push_back(ChamferImage(*it, _cannyLow, _cannyHigh, _sigma,
                                        _colorCanny, true));
      //browser->pushImage(_templates.back().getEdges(0,0).getQtImage(), "");
      //browser->pushImage(_templates.back().getEdges(0,1).getQtImage(), "");
      //browser->pushImage(_templates.back().getEdges(0,2).getQtImage(), "");
      //browser->pushImage(_templates.back().getEdges(0,3).getQtImage(), "");
      //browser->pushImage(_templates.back().getEdges(0,4).getQtImage(), "");
    }
  }
  printf("\n# templates: %d\n", _templates.size());
}


void ChamferMatching::setImages(const QStringList& imgNames)
{
  printf("Adding Images: ");
  _images.clear();
  //-- store templates --//
  if (!imgNames.isEmpty())
  {
    _imageNames=imgNames;
    QStringList::const_iterator it;
    for( it=imgNames.begin(); it!=imgNames.end(); it++)
    {
      printf("*");
      _images.push_back(ChamferImage(*it, _cannyLow, _cannyHigh, _sigma,
                                     _colorCanny, false));
    }
  }
  printf("\n# images: %d\n", _images.size());
}


void ChamferMatching::setCodebook(const vector<OpGrayImage> vPatches, bool orientationPlanes)
{

  printf("ChamferMatching::setCodebook()\n");
  printf("  Codebook Size: %d\n", vPatches.size());

  if (orientationPlanes)
  {
    for(unsigned i=0; i<vPatches.size(); i++)
    {
      vector<OpGrayImage> vPlanes = ImageOperations::split2x2(vPatches[i]);
      for(unsigned k=0; k<vPlanes.size(); k++)
      {
        //printf("  plane size: %dx%d\n", vPlanes[k].width(), vPlanes[k].height());
        m_vCodebook.push_back( ChamferImage(vPlanes[k]) );
      }
    }
  }
  else
  {
    for(unsigned i=0; i<vPatches.size(); i++)
      m_vCodebook.push_back( ChamferImage(vPatches[i]) );
  }
  printf("ChamferMatching::setCodebook() finished!!!\n");
  /*
    printf("ChamferMatching::setCodebook()\n");
    vector<OpGrayImage>::const_iterator it;
    for(it=vPatches.begin(); it!=vPatches.end(); it++)
    {
        OpGrayImage imgDt;
        EdgePtVec   vPoints;
        ImageOperations::distanceTransform( *it, imgDt );
        getBlackPoints( *it, vPoints );
        _codebookDTs.push_back( imgDt );
        _codebookFeatures.push_back( vPoints );
    }
    printf("ChamferMatching::setCodebook() finished!!!\n");
  */
}


///////////////////////////////////////////////////////////////////////////////
//
// MATCHING FUNCTIONS
//
//
//
//

float ChamferMatching::getDistance( const OpGrayImage& img1,
                                    const OpGrayImage& img2 )
{
  OpGrayImage imgDt1, imgDt2;
  EdgePtVec   f1, f2;

  ImageOperations::distanceTransform(img1, imgDt1);
  getBlackPoints(img2, f2);
  float dDist1 = calcDistanceAtPosition(0,0, imgDt1, f2);

  ImageOperations::distanceTransform(img2, imgDt2);
  getBlackPoints(img1, f1);
  float dDist2 = calcDistanceAtPosition(0,0, imgDt2, f1);

  //EImageBrowser* browser = new EImageBrowser(); browser->show();
  //char tmp[8]; sprintf(tmp, "%f", (dist1+dist2)/2);
  //browser->pushImage(img1.getQtImage(), tmp);
  //browser->pushImage(img1.getQtImage(), tmp);

  return (dDist1+dDist2)/2.0;
}


vector<float> ChamferMatching::getDistancesToCodebook(const OpGrayImage& img, bool orientationPlanes, bool symDist)
{
  //printf("ChamferMatching::getDistancesToCodebook() with patch of size: %dx%d and symdist: %d\n", img.width(),img.height(), symDist);
  vector<float> vRet;
  if (orientationPlanes)
  {
    //--- prepare patches ---//
    vector<OpGrayImage> vPatches = ImageOperations::split2x2(img);
    vector<ChamferImage> vCPatches;
    for(int k=0; k<NO_PLANES; k++)
      vCPatches.push_back(ChamferImage(vPatches[k]));

    for(unsigned i=0; i<m_vCodebook.size(); i+=NO_PLANES)
    {
      float dist=0;
      for(int k=0; k<NO_PLANES; k++)
      {
        //printf("codebook width: %d patch width: %d\n", m_vCodebook[i].width(), vCPatches[k].width());
        if (symDist) dist+=normalizedSymDistance(m_vCodebook[i+k], vCPatches[k]);
        else dist+=normalizedDistance(m_vCodebook[i+k], vCPatches[k]);
      }
      //printf("%.4f ", dist/NO_PLANES);
      vRet.push_back(dist/NO_PLANES);
    }
    //printf("\n");
  }
  else
  {
    //--- prepare patch ---//
    ChamferImage cImg(img);
    for(unsigned i=0; i<m_vCodebook.size(); i++)
    {
      float dist =0;
      if (symDist) dist = normalizedSymDistance(m_vCodebook[i], cImg);
      else dist = normalizedDistance(m_vCodebook[i], cImg);
      //printf("%.4f ", dist);
      vRet.push_back(dist);
    }
    //printf("\n");
  }

  //printf("size of distances to codebook: %d\n", vRet.size());
  return vRet;

}


void ChamferMatching::matchIDLFile(const QString& idlFile)
{
  //clear();
  _candidateSets.clear();
  _images.clear();

  int oldTime= time(0);
  int curTime;
  int elapsed=0;

  //-- read IDL file --//
  ImgDescrList descrListIn(idlFile.latin1());
  _descrList = descrListIn;
  //cout << "descrsize: "<< _descrList.size() << endl;

  //-- parse path --//
  QString dir, filename;
  FileOperations::parseFilename(idlFile, dir, filename);

  //-- store templates --//
  //setTemplates(tplNames);

  //-- show progress dialog --//
  QProgressDialog dialog(0,"Progress");
  dialog.setTotalSteps(_descrList.size());
  dialog.show();

  //-- process all test images --//
  for(int i=0; i<descrListIn.size(); i++ )
  {
    printf("\n|||||||||||||||||| Image No: %d of %d ", i, descrListIn.size());
    curTime=time(0);
    elapsed+=curTime-oldTime;
    oldTime=curTime;
    printf(" -- Elapsed time: %.1f min\n", (float) elapsed/60);

    dialog.setProgress(i+1);//update progress dialog

    //-- load image --//
    QString imgName = dir + descrListIn[i].sName.c_str();
    //_images.push_back(ChamferImage(imgName, _cannyLow, _cannyHigh, _sigma,
    //                               _colorCanny));
    ChamferImage img(imgName, _cannyLow, _cannyHigh, _sigma, _colorCanny);

    //-- perform matching --//
    CandidateSet candidates;
    doCoarseToFineMatching(candidates, img);//_images.back());

    //-- clean candidates --//
    cleanCandidates(candidates, _candidateThresh, _coverThresh);

    //-- symmetry check --//
    if(_symmetryCheck)
    {
      CandidateSet symCand;
      symmetryCheck(candidates, symCand, img.getEdges(0));
      //_images.back().getEdges(0));
      _candidateSets.push_back(symCand);

    }
    else
    {
      _candidateSets.push_back(candidates);
    }

    //-- store result in descrList --//
    CandidateSet::const_iterator it;
    for( it = _candidateSets.back().begin();
         it!=_candidateSets.back().end(); it++)
    {
      Rect r = it->getRect();
      r.score*=-1;
      _descrList[i].vRectList.push_back(r);
    }

    //_descrList.save("result-edi.idl", true);
  }

  //-- save results to file --//
  // cout << "Saving to: " << dir+"/result-edi.idl" << endl;
  // _descrList.save( (dir+"result-edi.idl").latin1() );
}

void ChamferMatching::matchImage(const QImage& img)
{
  _candidateSets.clear();
  _images.clear();

  printf("\nMatching Image ...\n");

  //-- perform matching --//
  CandidateSet candidates;
  ChamferImage cimg(img, _cannyLow, _cannyHigh, _sigma, _colorCanny, false);
  _images.push_back(cimg);
  doCoarseToFineMatching(candidates, cimg);
  //-- clean up candidates --//
  cleanCandidates(candidates, _candidateThresh, _coverThresh);

  //-- symmetry check --//
  if(_symmetryCheck)
  {
    CandidateSet symCand;
    symmetryCheck(candidates, symCand, cimg.getEdges(0));
    _candidateSets.push_back(symCand);
  }
  else
  {
    _candidateSets.push_back(candidates);
  }
}

void ChamferMatching::matchImages()
{
  //clear();
  _candidateSets.clear();

  //-- process all images --//
  for (unsigned int i=0; i<_images.size(); i++)
  {
    printf("\n|||||||||||||||||| Image No: %d of %d\n", i, _images.size());
    //printf("Image Dimensions: %dx%d\n", img.width(), img.height());

    //-- perform matching --//
    CandidateSet candidates;
    doCoarseToFineMatching(candidates, _images[i]);

    //-- clean up candidates --//
    cleanCandidates(candidates, _candidateThresh, _coverThresh);

    //-- symmetry check --//
    if(_symmetryCheck)
    {
      CandidateSet symCand;
      symmetryCheck(candidates, symCand, _images[i].getEdges(0));
      _candidateSets.push_back(symCand);

    }
    else
    {
      _candidateSets.push_back(candidates);
    }
  }
}


vector<vector<float> > ChamferMatching::getDistanceMatrixOfTemplates()
{
  //clear();
  _candidateSets.clear();

  vector<vector<float> > vvSims;
  //-- process all images --//
  for (unsigned int i=0; i<_templates.size(); i++)
  {
    vector<float> t;
    vvSims.push_back(t);
    for (unsigned int j=0; j<_templates.size(); j++)
    {
      if(i<j)
      {
        CandidateSet vCandidates;
        coarseToFineMatching( vCandidates, _templates[i], _templates[j],
                              0,0,0,1,1 );
        vvSims[i].push_back( vCandidates.begin()->getDist() );
        printf("Distance between template %d and %d: %.5f\n", i, j,
               vvSims[i][j]);
      }
      else
        vvSims[i].push_back(0.0);
    }
  }
  return vvSims;
}

void ChamferMatching::getDistanceMatrix(vector<float>& sims, const vector<OpGrayImage>& patches, bool orientationPlanes)
{

  unsigned size = patches.size();

  int idx;
  vector<ChamferImage> templates;

  if (!orientationPlanes) //no orientation planes
  {
    for(unsigned i=0; i<size; i++)
      templates.push_back(ChamferImage(patches[i]));

    //-- process all pairs --//
    for (unsigned int i=0; i<size; i++)
      for (unsigned int j=0; j<size; j++)
      {
        idx = i*size + j;
        if(i>j)
        {
          sims[idx]=normalizedSymDistance(templates[i], templates[j]);
          //printf("Distance between template %d and %d: %.5f\n", i, j, sims[idx]);
        }
      }
  }
  else
    // use orientation planes //
  {
    int dim = patches[0].width()/2;
    int x, y;

    for(unsigned i=0; i<size; i++)
      for (int j=0; j<4; j++)
      {
        x = (j%2)*dim;
        y = (j/2)*dim;
        OpGrayImage img = patches[i].extractRegion(x,y, x+dim, y+dim);
        templates.push_back(ChamferImage(img));
      }

    //-- process all pairs --//
    for (unsigned int i=0; i<size; i++)
      for (unsigned int j=0; j<size; j++)
      {
        idx = i*size + j;
        if(i>j)
        {
          sims[idx]=0;
          for (int k=0; k<4; k++)
            sims[idx]+=normalizedSymDistance(templates[4*i+k], templates[4*j+k]);
          sims[idx]/=4;//check normalization!!!
        }
      }
  }

}

vector<float> ChamferMatching::getDistanceMatrix(const vector<OpGrayImage>& patches, bool orientationPlanes)
{

  unsigned size = patches.size();
  vector<float> sims(size*size);

  int idx;

  vector<ChamferImage> templates;

  if (!orientationPlanes) //no orientation planes
  {
    for(unsigned i=0; i<size; i++)
      templates.push_back(ChamferImage(patches[i]));

    //-- process all pairs --//
    for (unsigned int i=0; i<size; i++)
      for (unsigned int j=0; j<size; j++)
      {
        idx = i*size + j;
        if(i>j)
        {
          sims[idx]=normalizedSymDistance(templates[i], templates[j]);
          //printf("Distance between template %d and %d: %.5f\n", i, j, sims[idx]);
        }
      }
  }
  else
    // use orientation planes //
  {
    int dim = patches[0].width()/2;
    int x, y;

    for(unsigned i=0; i<size; i++)
      for (int j=0; j<4; j++)
      {
        x = (j%2)*dim;
        y = (j/2)*dim;
        OpGrayImage img = patches[i].extractRegion(x,y, x+dim, y+dim);
        templates.push_back(ChamferImage(img));
      }

    //-- process all pairs --//
    for (unsigned int i=0; i<size; i++)
      for (unsigned int j=0; j<size; j++)
      {
        idx = i*size + j;
        if(i>j)
        {
          sims[idx]=0;
          for (int k=0; k<4; k++)
            sims[idx]+=normalizedSymDistance(templates[4*i+k], templates[4*j+k]);
          sims[idx]/=4;//check normalization!!!
        }
      }
  }

  return sims;

  /*
    unsigned size = patches.size();
  	vector<float> sims(size*size);
  	
      vector<ChamferImage> templates;
      for(unsigned i=0; i<size; i++)
          templates.push_back(ChamferImage(patches[i]));
   
      //-- process all images --//
      for (unsigned int i=0; i<size; i++) {
        for (unsigned int j=0; j<size; j++) {
          int idx = i*size + j;
          if(i>j) {
            float dist1 = calcDistanceAtPosition(0,0, templates[i].getDT(0), 
                                                 templates[j].getFeatures(0));
            float dist2 = calcDistanceAtPosition(0,0, templates[j].getDT(0), 
                                                 templates[i].getFeatures(0));
            float dist =  1.0 - min(1.0,(dist1+dist2)/2.0/100.0);
   
            sims[idx]=dist;
            printf("%.4f %.4f -> %.4f\n", dist1, dist2, dist);
   printf("Distance between template %d and %d: %.5f\n", i, j, dist);
   
          }
        }
      }
  	
      return sims;
  */
}

void ChamferMatching::doCoarseToFineMatching( CandidateSet& vCandidates,
    ChamferImage& cimg )
{
  //_levels=levels;
  //candidates.clear();
  printf("Doing Coarse-To-Fine Matching with %d levels\n", _levels);

  int i=0;
  cerr << "[ ";
  vector<ChamferImage>::iterator it;
  for( it = _templates.begin(); it != _templates.end(); it++, i++)
  {
    cerr << "*";
    _currentTemplateIndex=i;

    CandidateSet tmpCandidates;
    coarseToFineMatching( tmpCandidates, cimg, *it, _levels, 0, 0,
                          cimg.width(), cimg.height());
    //cout << "dist: " << tmpCandidates.begin()->distance << endl;

    vCandidates.insert( tmpCandidates.begin(), tmpCandidates.end() );
    //cout << "total # candidates: " << candidates.size() << endl;
  }
  cerr << " ]\n";
}


void ChamferMatching::coarseToFineMatching( CandidateSet& candidates,
    ChamferImage& img,
    ChamferImage& tpl,
    int levels, int x, int y,
    int w, int h )
{
  //logLine("Coarse-to-Fine Template Matching...",2);

  //-- recursion for level>0 --//
  CandidateSet coarseCandidates;
  if (levels>0)
    coarseToFineMatching(coarseCandidates, img, tpl, levels-1, x,y,w,h);

  //-- variable declaration
  double dist;
  int count;
  int noPlanes=1;
  int firstPlane=0;
  if (_planes)
  {
    noPlanes=4;
    firstPlane=1;
  }

  //-- dimensions --//
  QSize& tplDim = tpl.getDimensions(_levels-levels);
  QSize& imgDim = img.getDimensions(_levels-levels);
  //printf("Dimensions: I:%dx%d T:%dx%d\n", imgDim.width(), imgDim.height(),
  //                                        tplDim.width(), tplDim.height());

  //-- get data for current pyramid level --//
  vector<OpGrayImage*>   vDtPlanes;
  vector<IntegralImage*> vIiPlanes;
  vector<EdgePtVec*>     vFeaturePlanes;

  for (int i=firstPlane; i<noPlanes+firstPlane; i++)
  {
    vDtPlanes.push_back     ( &img.getDT(_levels-levels, i) );
    vIiPlanes.push_back     ( &img.getIntegralImage(_levels-levels, i) );
    vFeaturePlanes.push_back( &tpl.getFeatures(_levels-levels, i) );
    //browser->pushImage(dtPlanes.back()->getQtImage(), "");
  }

  if (levels==0)
  { //base case
    // DEBUG: 4 lines//
    //EImageBrowser* browser = new EImageBrowser();
    //browser->show();
    //browser->pushImage(img.getEdges(_levels).getQtImage(),"");
    //browser->pushImage(tpl.getEdges(_levels).getQtImage(),"");

    //-- shift template of over  edge image --//
    for (int shift_y = y; shift_y<min(imgDim.height(),y+h); shift_y++)
    {
      for (int shift_x = x; shift_x<min(imgDim.width(),x+w); shift_x++)
      {
        dist = 0;
        count = 0;

        if (!_planes)
          dist = calcDistanceAtPosition( shift_x, shift_y, tplDim,
                                         *vIiPlanes[0], *vDtPlanes[0],
                                         *vFeaturePlanes[0]);
        else
          for( int i=0; i<(int)vDtPlanes.size(); i++ )
            dist +=calcDistanceAtPosition( shift_x, shift_y, tplDim,
                                           *vIiPlanes[i], *vDtPlanes[i],
                                           *vFeaturePlanes[i]);

        candidates.insert( Candidate( QRect(shift_x, shift_y,
                                            tpl.width(), tpl.height()),
                                      dist, tpl.getName(),
                                      _currentTemplateIndex));
        if (candidates.size()>MAX_CANDIDATES)
          candidates.erase(--candidates.end());

      }
    }

  }
  // DEBUG till return //
  //	for ( CandidateSet::iterator it=coarseCandidates.begin(); it!=coarseCandidates.end(); it++)
  //	{
  //		int x = it->position.x()*2;
  //        int y = it->position.y()*2;
  //		candidates.insert(Candidate(QRect(x, y, tpl.width(), tpl.height()), it->distance, tpl.getName(), _currentTemplateIndex));
  //        if (candidates.size()>MAX_CANDIDATES)
  //            candidates.erase(--candidates.end());
  //	}
  //	return;
  else
  {
    //-- Now have a look at the high resolution edges --//
    //-- window on next level --//
    x = x/2;
    y = y/2;
    w = w/2;
    h = h/2;
    CandidateSet::iterator it;
    for ( it = coarseCandidates.begin(); it!=coarseCandidates.end(); it++)
    {
      double min_dist=10000.0;
      int min_x=0, min_y=0;

      for (int inc_y=0; inc_y<1; inc_y++)
      {
        //for (int inc_y=-1; inc_y<2; inc_y++)

        for (int inc_x=0; inc_x<1; inc_x++)
        {
          //for (int inc_x=-1; inc_x<2; inc_x++)
          dist = 0;
          count = 0;

          //-- calculate shift on higher resolution --//
          int shift_x = int(it->getPos().x())*2 + inc_x;
          int shift_y = int(it->getPos().y())*2 + inc_y;
          if (shift_x<0 || shift_y<0 || shift_x>=imgDim.width() ||
              shift_y>=imgDim.height() )
            continue;

          if (!_planes)
          {
            dist = calcDistanceAtPosition( shift_x, shift_y, tplDim,
                                           *vIiPlanes[0], *vDtPlanes[0],
                                           *vFeaturePlanes[0]);
          }
          else
          {
            for( int i=0; i<(int)vDtPlanes.size(); i++ )
            {
              dist +=calcDistanceAtPosition( shift_x, shift_y, tplDim,
                                             *vIiPlanes[i], *vDtPlanes[i],
                                             *vFeaturePlanes[i]);
            }
          }

          if (dist < min_dist)
          {
            min_dist=dist;
            min_x=shift_x;
            min_y=shift_y;
          }
        }//endfor inc_x
      }//endfor inc_y

      //-- add to candidateset --//
      candidates.insert(Candidate(QRect( min_x, min_y,
                                         tpl.width(), tpl.height()), min_dist,
                                  tpl.getName(), _currentTemplateIndex));
      if (candidates.size()>MAX_CANDIDATES)
        candidates.erase(--candidates.end());


    }//endfor it
  }

  //const Candidate& best = *(candidates.begin());
  //const QRect& pos = best.position;
  //double fac = pow(2.0,_levels-levels);
  //printf("# candidates found: %d\n", candidates.size());
  //printf("Best candidate at pyramid level %d: %.0fx%.0f with dist: %f\n",
  //       levels, pos.x()*fac,pos.y()*fac, best.distance);
  //printf("Dimensions: I:%dx%d T:%dx%d\n", imgDim.width(), imgDim.height(),
  //       tplDim.width(), tplDim.height());
  //if (_planes)
  //  printf("Features: %d %d %d %d\n", featurePlanes[0]->size(),
  //         featurePlanes[1]->size(), featurePlanes[3]->size(),
  //         featurePlanes[3]->size());
  //else printf("Features: %d\n", featurePlanes[0]->size());
}


double ChamferMatching::matchTemplateAtPosition(int x, int y)
{
  return calcDistanceAtPosition(x,y, _templates[0].getDimensions(0), _images[0].getIntegralImage(0), _images[0].getDT(0), _templates[0].getFeatures(0));
  return 10000.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// DRAWING methods
//
//

void ChamferMatching::drawDT(const OpGrayImage& dt, QImage& ret)
{
  // create and fill return image //
  ret.create(dt.width(), dt.height(),32);
  for (int y=0; y<dt.height(); y++)
  {                                                   // Determine
    for (int x=0; x<dt.width(); x++)
    {                                               //  colour of each
      int col_r = int(dt(x,y).value());             //  pixel depending

      if (col_r > 255)
        col_r = 255;                                //  on distance.
      ret.setPixel(x,y, qRgb(col_r, 0, 0));
    }
  }
}


QImage ChamferMatching::drawDT( const OpGrayImage &imgDT )
{
  QImage qimgResult;

  // create and fill return image //
  qimgResult.create( imgDT.width(), imgDT.height(), 32 );
  for( int y=0; y<(int)imgDT.height(); y++ )
    for( int x=0; x<(int)imgDT.width(); x++ )
    {
      int col_r = int( imgDT(x,y).value() );

      if( col_r > 255 )
        col_r = 255;
      qimgResult.setPixel(x,y, qRgb(col_r, 0, 0));
    }

  return qimgResult;
}


void ChamferMatching::drawEdgesOnDT(const OpGrayImage& edges, QImage& dt)
{
  int img_h = edges.height();
  int img_w = edges.width();
  assert(img_h==dt.height() && img_w == dt.width());

  // draw edges on output //
  for (int img_x = 0; img_x < img_w; img_x++)
    for (int img_y = 0; img_y < img_h; img_y++)
      if( edges(img_x, img_y).value() == 0 )
        dt.setPixel(img_x, img_y, qRgb(0,0,255));
}


void ChamferMatching::drawTemplateOnImage( const OpGrayImage& templ,
    QImage& img,
    int pos_x, int pos_y, QRgb color )
{

  int tpl_h = templ.height();
  int tpl_w = templ.width();
  int img_h = img.height();
  int img_w = img.width();

  // draw template on output //
  int minx = max(0, pos_x);
  int miny = max(0, pos_y);
  int maxx = min(pos_x + tpl_w, img_w);
  int maxy = min(pos_y + tpl_h, img_h);
  for( int y = miny; y < maxy; y++ )
    for( int x = minx; x < maxx;  x++ )
      if (templ(x-pos_x, y-pos_y).value() == 0)
        img.setPixel(x, y, color);
}


void ChamferMatching::drawTemplateOnImage( const OpGrayImage& imgTempl,
    QImage& qimg,
    int pos_x, int pos_y, float dScale,
    QRgb color )
{
  int nNewWidth = (int)floor( imgTempl.width()*dScale + 0.5 );
  OpGrayImage imgScTempl = imgTempl;
  imgScTempl = imgScTempl.opRescaleToWidth( nNewWidth );

  int tpl_h = imgScTempl.height();
  int tpl_w = imgScTempl.width();
  int img_h = qimg.height();
  int img_w = qimg.width();

  // draw template on output //
  int minx = max(0, pos_x);
  int miny = max(0, pos_y);
  int maxx = min(pos_x + tpl_w, img_w);
  int maxy = min(pos_y + tpl_h, img_h);
  for( int y = miny; y < maxy; y++ )
    for( int x = minx; x < maxx;  x++ )
      //if( imgScTempl(x-pos_x, y-pos_y).value() != 0.0 )
      if( imgScTempl(x-pos_x, y-pos_y).value() >= 0.5 )
        qimg.setPixel( x, y, color );
}


void ChamferMatching::drawTemplateOnImage( const EdgePtVec &vTemplPoints,
    QImage& qimg,
    int pos_x, int pos_y, float dScale,
    QRgb color )
{
  int img_h = qimg.height();
  int img_w = qimg.width();

  // draw template on output //
  for( int i=0; i<(int)vTemplPoints.size(); i++ )
  {
    int cx = (int)floor(pos_x + vTemplPoints[i].x()*dScale + 0.5);
    int cy = (int)floor(pos_y + vTemplPoints[i].y()*dScale + 0.5);

    // check if point is out of bounds //
    if( (cy >= 0) && (cy < img_h ) &&
        (cx >= 0) && (cx < img_w ) )
      qimg.setPixel( cx, cy, color );
  }
}


QImage ChamferMatching::drawImageResult( unsigned int imgNo,
    unsigned int hypothesisNo ) const
{
  assert(_images.size()==_candidateSets.size());
  if (imgNo>=_images.size())
    return QImage();

  // draw result //
  if (imgNo>_images.size())
    return QImage();

  QRect  pos;
  QImage qimgResult;

  ChamferImage tmp = _images[imgNo];
  drawDT( tmp.getDT(0), qimgResult );
  drawEdgesOnDT( tmp.getEdges(0), qimgResult );

  // draw best no //
  CandidateSet::iterator it = _candidateSets[imgNo].begin();
  unsigned int i=0;
  for(; it!=_candidateSets[imgNo].end() && i<hypothesisNo; it++, i++)
  {
    pos = it->getPos();
    if (it->getName().empty())
    {}//drawTemplateOnImage(*_templatePyramid.begin(),
    //result, pos.x(), pos.y(), qRgb(0,255,0));
    else
      drawTemplateOnImage( QImage(it->getName().c_str()),
                           qimgResult, pos.x(), pos.y(),
                           qRgb(0,255,0));
    //drawTemplateOnImage( _templ, _dt, coarsePos.x()*2, coarsePos.y()*2,
    //                     qRgb(0,255,255));
  }

  // draw best //
  it = _candidateSets[imgNo].begin();
  if (it!=_candidateSets[imgNo].end())
  {
    pos = it->getPos();
    if (it->getName().empty())
      printf("that's it -> seg fault\n");
    //drawTemplateOnImage( *_templatePyramid.begin(), result, pos.x(), pos.y(),
    //                     qRgb(255,255,0));
    else
      drawTemplateOnImage( QImage(it->getName()), qimgResult,
                           pos.x(), pos.y(),
                           qRgb(255,255,0));
  }
  return qimgResult;
}


QImage ChamferMatching::drawImageResult( CombiCandidateSet &vCandidates,
    const vector<OpGrayImage> &vSilhouettes,
    int nNumHypos,
    const OpGrayImage imgDT )
{
  QImage qimgResult;
  QRect qrPos;

  qimgResult = drawDT       ( imgDT );
  //drawEdgesOnDT(imgEdges, qimgResult);

  // draw best hypotheses //
  CombiCandidateSet::iterator it;
  int i=0;
  for( it=vCandidates.begin(); (it!=vCandidates.end() && i<nNumHypos);
       it++, i++)
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vSilhouettes.size()) )
      drawTemplateOnImage( vSilhouettes[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(0,255,0) );
  }

  // draw very best hypothesis //
  it = vCandidates.begin();
  if( it!=vCandidates.end() )
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vSilhouettes.size()) )
      drawTemplateOnImage( vSilhouettes[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(255,255,0));
  }
  return qimgResult;
}


QImage ChamferMatching::drawImageResult( CombiCandidateSet &vCandidates,
    const vector<EdgePtVec> &vvTemplPoints,
    int nNumHypos,
    const OpGrayImage imgDT )
{
  QImage qimgResult;
  QRect qrPos;

  qimgResult = drawDT       ( imgDT );
  //drawEdgesOnDT(imgEdges, qimgResult);

  // draw best hypotheses //
  float     dMaxBhatta = 0.0;
  ;
  CombiCandidate cMaxCand;
  int i=0;
  CombiCandidateSet::iterator it;
  for( it = vCandidates.begin(); (it!=vCandidates.end() && i<nNumHypos);
       it++, i++)
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vvTemplPoints.size()) )
      drawTemplateOnImage( vvTemplPoints[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(0,255,0) );

    if( it->getBhScore() > dMaxBhatta )
    {
      dMaxBhatta = it->getBhScore();
      cMaxCand = (*it);
    }
  }

  // draw hypothesis with best Chamfer score //
  it = vCandidates.begin();
  if( it!=vCandidates.end() )
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vvTemplPoints.size()) )
      drawTemplateOnImage( vvTemplPoints[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(255,255,0));
  }

  // draw hypothesis with best Bhatta score //
  qrPos = cMaxCand.getPos();
  if( cMaxCand.getTemplateId()>0 &&
      (cMaxCand.getTemplateId()<(int)vvTemplPoints.size()) )
    drawTemplateOnImage( vvTemplPoints[cMaxCand.getTemplateId()],
                         qimgResult, qrPos.x(), qrPos.y(), cMaxCand.getScale(),
                         qRgb(0,0,255));

  return qimgResult;
}


QImage ChamferMatching::drawImageResult( vector<CombiCandidate> &vCandidates,
    const vector<EdgePtVec> &vvTemplPoints,
    int nNumHypos,
    const OpGrayImage imgDT )
{
  QImage qimgResult;
  QRect qrPos;

  qimgResult = drawDT       ( imgDT );
  //drawEdgesOnDT(imgEdges, qimgResult);

  // draw best hypotheses //
  float     dMaxBhatta = 0.0;
  ;
  float     dMaxComb = 0.0;
  ;
  CombiCandidate cMaxBhCand;
  CombiCandidate cMaxCombCand;
  int i=0;
  vector<CombiCandidate>::iterator it;
  for( it = vCandidates.begin(); (it!=vCandidates.end() && i<nNumHypos);
       it++, i++)
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vvTemplPoints.size()) )
      drawTemplateOnImage( vvTemplPoints[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(0,255,0) );

    if( it->getBhScore() > dMaxBhatta )
    {
      dMaxBhatta = it->getBhScore();
      cMaxBhCand = (*it);
    }
    if( it->getCombScore() > dMaxComb )
    {
      dMaxComb = it->getCombScore();
      cMaxCombCand = (*it);
    }
  }

  // draw hypothesis with best Chamfer score //
  it = vCandidates.begin();
  if( it!=vCandidates.end() )
  {
    qrPos = it->getPos();
    if( it->getTemplateId()>0 &&
        (it->getTemplateId()<(int)vvTemplPoints.size()) )
      drawTemplateOnImage( vvTemplPoints[it->getTemplateId()],
                           qimgResult, qrPos.x(), qrPos.y(), it->getScale(),
                           qRgb(255,255,0));
  }

  // draw hypothesis with best Bhatta score //
  qrPos = cMaxBhCand.getPos();
  if( cMaxBhCand.getTemplateId()>0 &&
      (cMaxBhCand.getTemplateId()<(int)vvTemplPoints.size()) )
    drawTemplateOnImage( vvTemplPoints[cMaxBhCand.getTemplateId()],
                         qimgResult, qrPos.x(), qrPos.y(),
                         cMaxBhCand.getScale(),
                         qRgb(255,0,255));

  // draw hypothesis with best combined score //
  qrPos = cMaxCombCand.getPos();
  if( cMaxCombCand.getTemplateId()>0 &&
      (cMaxCombCand.getTemplateId()<(int)vvTemplPoints.size()) )
    drawTemplateOnImage( vvTemplPoints[cMaxCombCand.getTemplateId()],
                         qimgResult, qrPos.x(), qrPos.y(),
                         cMaxCombCand.getScale(),
                         qRgb(0,0,255));

  return qimgResult;
}


vector<QImage> ChamferMatching::drawResults(unsigned int hypothesisNo)
{
  assert(_images.size()==_candidateSets.size());
  vector<QImage> imgVec;
  int startPlane=0;
  int noPlanes=1;

  // draw result //
  for(unsigned int i=0; i<_images.size(); i++)
  {
    QRect pos;
    const CandidateSet& candidates = _candidateSets[i];
    printf("# candidates: %d\n", candidates.size());

    if (_planes)
      noPlanes=5;

    for(int j=startPlane; j<startPlane+noPlanes; j++)
    {
      QImage result;

      //-- draw ground --//
      drawDT(_images[i].getDT(0,j), result);
      drawEdgesOnDT(_images[i].getEdges(0, j), result);

      //-- draw best hyptheses --//
      CandidateSet::const_iterator it = candidates.begin();
      unsigned int k=0;
      for(; it!=candidates.end() && k<hypothesisNo; it++, k++)
      {
        pos = it->getPos();
        drawTemplateOnImage(_templates[it->getIdx()].getEdges(0,j).getQtImage(),
                            result, pos.x(), pos.y(), qRgb(0,255,0));
      }
      // draw best //
      it = candidates.begin();
      if (it!=candidates.end())
      {
        pos = it->getPos();
        drawTemplateOnImage(_templates[it->getIdx()].getEdges(0,j).getQtImage(),
                            result, pos.x(), pos.y(), qRgb(255,255,0));
      }

      imgVec.push_back(result);
    }

  }
  printf("no. of result images: %d\n", imgVec.size());
  return imgVec;
}


/////////////////////////////////////////////////////////////////////////////
//
// getting results
//
//

Candidate ChamferMatching::getCandidate(unsigned int imgNo, unsigned int hypothesisNo) const
{
  if (imgNo<_candidateSets.size() && hypothesisNo<_candidateSets[imgNo].size())
  {
    const CandidateSet& cand = _candidateSets[imgNo];
    unsigned int i=0;
    for(CandidateSet::const_iterator it=cand.begin(); it!=cand.end(); it++, i++)
      if (i==hypothesisNo)
        return *it;
  }

  return Candidate();
}

void ChamferMatching::saveResults(const QString& filename) const
{
  cout << "Saving Results ...\n";
  if (!filename.isEmpty())
    _descrList.save(filename.latin1());
}


vector<QPoint> ChamferMatching::getContourPointsOfCandidates(int pixelDist, unsigned noOfHypotheses)
{
  assert(_images.size()==_candidateSets.size()==1);

  vector<QPoint> points;

  for(unsigned i=0; i<_candidateSets.size(); i++)
  {
    QRect pos;
    unsigned k=0;
    //int w = _images[i].width();
    //int h = _images[i].height();

    const CandidateSet& candidates = _candidateSets[i];
    CandidateSet::const_iterator it = candidates.begin();
    for(; it!=candidates.end() && k<noOfHypotheses; it++, k++)
    {
      //--- getting candidate position ---//
      pos = it->getPos();
      //--- getting contour points ---//
      vector<QPoint> tmpPoints = _templates[it->getIdx()].getFeatures(0);
      for(unsigned j=0; j<tmpPoints.size(); j++)
        tmpPoints[j] = tmpPoints[j]+pos.topLeft();
      points.insert(points.end(), tmpPoints.begin(), tmpPoints.end());
    }
  }
  //--- appending sampled points ---//
  vector<QPoint> sampled = ImageOperations::sampleFromPoints(points, pixelDist);
  return sampled;
}

















///////////////////////////////////////////////////////////////////////////////////////////////////
//
// old stuff
//
//
/*
QRgb ChamferMatching::getColumnColor(int col)
{
    switch (col)
    {
    case 0:
        return qRgb(0, 0, 255);
    case 1:
        return qRgb(0, 200, 0);
    case 2:
        return qRgb(255, 255, 0);
    case 3:
        return qRgb(255, 0, 0);
    }
 
    return qRgb(0, 255, 0);
    ;
}
 
void ChamferMatching::addOccurenceToOccurenceBox(int x, int y, QRect& box)
{
    if (x < box.left())
        box.setLeft(x);
    if (x > box.right())
        box.setRight(x);
    if (y < box.top())
        box.setTop(y);
    if (y > box.bottom())
        box.setBottom(y);
}
 
void ChamferMatching::hierarchicalMatching(CandidateSet& candidates, const QImage& edges,
                                           const QString baseDir, const QStringList& templateHierarchy)
{
    //QString dir =QDir::homeDirPath()+"/VISION/PROGRAMMING/chamfer/src/test/me_E_01L/";
 
    _levels=3; //number of levels in the hierarchy
    candidates.clear();
 
 
    //-- variable definition --//
    int noOfTemplates = templateHierarchy.count();
    QRect occurenceBox;
    bool activatedElements[noOfTemplates][4];
    QRect searchWindows[noOfTemplates][4];
    QString filenames[noOfTemplates][4];
 
 
    //-- store template filenames in filename hierarchy --//
    int col=0, row = 0;
    for (QStringList::ConstIterator it = templateHierarchy.begin(); it != templateHierarchy.end(); it++, row++)
    {
        col = 0;
        QStringList hierarchyRow = QStringList::split(HIER_ELMT_SEPARATOR, *it, true);
        for (QStringList::Iterator it2 = hierarchyRow.begin(); it2 != hierarchyRow.end(); it2++, col++)
        {
            filenames[row][col] = *it2;
        }
    }
 
    //-- init activatedElements and searchCoords --//
    for (row=0; row<noOfTemplates; row++)
    {                                                                 // Set all elements
        for (col=0; col<4; col++)
        {                                                             //  of the hierar-
            activatedElements[row][col] = false;                            //  chy apart from
            //  the root to be
            searchWindows[row][col].setX(-1);                       //  not active.
            searchWindows[row][col].setY(-1);
            searchWindows[row][col].setWidth(-1);
            searchWindows[row][col].setHeight(-1);
        }
    }
 
 
    //-- Traverse hierarchy --//
    for (int col=0; col<4; col++, _levels--)
    {
        if (col==0) //root
        {
            setCurrentEdgeImage(edges);
            activatedElements[0][0] = true;
            searchWindows[0][0].setX(0);
            searchWindows[0][0].setY(0);
            searchWindows[0][0].setWidth(_edgePyramid.back().width());
            searchWindows[0][0].setHeight(_edgePyramid.back().height());
        }
        else
        {
            _edgePyramid.pop_back();
            _edgeDimensions.pop_back();
            _dtPyramid.pop_back();
            _integralImagePyramid.pop_back();
        }
 
        int w = _edgePyramid.back().width();
        int h = _edgePyramid.back().height();
        //int fac = edges.height()/_edgePyramid.back().height();
 
        for (int row=0; row<noOfTemplates; row++)
        {
            //-- is node marked active? --//
            if (activatedElements[row][col] == false || filenames[row][col] == 0)
                continue;
 
            cout << endl << "Machting node [" << row << "/"  << col << "] on window: " << searchWindows[row][col].x()
            << "-" << searchWindows[row][col].y() <<" " << searchWindows[row][col].width() <<"-"<< searchWindows[row][col].height()
            << " with distance threshold " << 100 << " >" << endl;
 
            //-- load template --//
            _currentTemplateName=baseDir+filenames[row][col];
            setCurrentTemplate(_currentTemplateName);
            OpGrayImage& tpl = _templatePyramid.front();
 
            //-- do matching --//
            CandidateSet candidates;
            coarseMatching(candidates, searchWindows[row][col].x(), searchWindows[row][col].y(),
                           searchWindows[row][col].width(), searchWindows[row][col].height());
            cout << "Distance: " << candidates.begin()->distance << endl;
 
            //-- do i have a match? --//
            if (candidates.size()==0)
                continue;
 
            //-- reset occurenceBox --//
            occurenceBox.setLeft(w);
            occurenceBox.setRight(0);
            occurenceBox.setTop(h);
            occurenceBox.setBottom(0);
 
            // go through candidates //
            for (CandidateSet::iterator it = candidates.begin(); it != candidates.end(); it++)
            {
                QRect pos = it->position;
                double curDist = it->distance;
 
                //-- how do i deal with our normalization? --//
                float threshold = (2e-5 - _levels*sqrt(2.0)/2.0*pow(2.0,_levels))*(tpl.width()*tpl.height());
                if (curDist < threshold) //threshold has yet to be defined
                {
                    //-- store in _candidates and calculate occurence box --//
                    candidates.insert(*it);
                    if (candidates.size()>MAX_CANDIDATES)
                        candidates.erase(--candidates.end());
                    addOccurenceToOccurenceBox(pos.x(), pos.y(), occurenceBox);
                }
            }
 
            cout << "Box: " << occurenceBox.x() << "-" << occurenceBox.y() <<" "
                 << occurenceBox.width() << "-"<< occurenceBox.height() <<  endl;
 
            //-- activate children and set searchwindows --//
            if (col != 3)
            {
                int i = row;
 
                while ( i<noOfTemplates )
                {
                    //-- don't activate children of next node --//
                    if (i>row && filenames[i][col]!=NULL)
                        break;
 
                    //-- activate next child and set search window --//
                    if (filenames[i][col+1] != NULL)
                    {
                        activatedElements[i][col+1] = true;
                        int margin = 0;//20/fac;
                        searchWindows[i][col+1].setLeft(2*max(0,occurenceBox.left() - margin));
                        searchWindows[i][col+1].setRight(2*min(w, occurenceBox.right() + margin));
                        searchWindows[i][col+1].setTop(2*max(0, occurenceBox.top() - margin));
                        searchWindows[i][col+1].setBottom(2*min(h, occurenceBox.bottom() + margin));
                        //searchWindows[i][col+1].setLeft(2*0);
                        //searchWindows[i][col+1].setRight(2*w);
                        //searchWindows[i][col+1].setTop(2*0);
                        //searchWindows[i][col+1].setBottom(2*h);
                        cout << "< Node [" << i << "/" << col+1 << "] activated in window [" << searchWindows[i][col+1].x() << "-"
                        << searchWindows[i][col+1].y() << "/" << searchWindows[i][col+1].width()
                        << "-"<< searchWindows[i][col+1].height() << "] >"
                        << endl;
                    }
                    i++;
                } // end while
            } // end if (col!=3)
 
        } // end for (row)
 
    } // end for (col)
 
}
*/
