/*********************************************************************/
/*                                                                   */
/* FILE         detector-experiments.cc                              */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@visin.ee.ethz.ch                               */
/*                                                                   */
/* CONTENT      This file contains some experimental methods.        */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Fri Feb 10 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <algorithm>

#include <resources.hh>
#include <nnsearch.hh>
#include <balltree.hh>
//#include <kballtree.hh>

#include "detector.hh"

void Detector::refineHypotheses( bool bDisplayResults )
  /*******************************************************************/
  /* Refine the current hypotheses (as defined by the votes contain- */
  /* ed in m_vActiveVotes) by uniform sampling.                      */
  /*******************************************************************/
{
  unsigned nIdx=0;

  if( (m_vHyposSingle.size() == 0) ) {
    cerr << "  No hypotheses to refine!" << endl;
    return;
  } else
    cout << "  Refining " << m_vHyposSingle.size() << " hypotheses..." << endl;

  /*---------------------------*/
  /* Initialize some variables */
  /*---------------------------*/
  int   nObjWidth     = m_parReco.params()->m_nObjWidth;
  int   nObjHeight    = m_parReco.params()->m_nObjHeight;

  /***************************************/
  /*   Initialize the global variables   */
  /***************************************/
  m_vImagePatches.clear();
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    m_vvPointsInside[k].clear();
    m_vvFeatures[k].clear();
  }

  /******************************************************************/
  /*   Extract all patches around the current hypothesis.           */
  /******************************************************************/
  vector<InterestPoint> vPointsToRefine;
  for(unsigned i=0; i<m_vHyposSingle.size(); i++ ) {
    Hypothesis hypo = m_vHyposSingle[i];
    /* collect all points that fall into the extended bounding box */
    int x1 = hypo.nBoxX1-hypo.nBoxWidth/10;
    int y1 = hypo.nBoxY1-hypo.nBoxHeight/10;
    int x2 = (int)(hypo.nBoxX1+hypo.nBoxWidth*1.2);
    int y2 = (int)(hypo.nBoxY1+hypo.nBoxHeight*1.2);

    m_vPoints.clear();
    int minx = max( x1, m_vCues[nIdx].params()->m_nPatchSize );
    int miny = max( y1, m_vCues[nIdx].params()->m_nPatchSize );
    int maxx = min( x2, m_img.width()-m_vCues[nIdx].params()->m_nPatchSize );
    int maxy = min( y2, m_img.height()-m_vCues[nIdx].params()->m_nPatchSize );
    for (int y=miny; y < maxy; y+=m_vCues[nIdx].params()->m_nStepSize)
      for (int x=minx; x < maxx; x+=m_vCues[nIdx].params()->m_nStepSize) {
        InterestPoint ptNew;
        ptNew.x = x;
        ptNew.y = y;
        ptNew.value = 1.0;
        ptNew.scale = 1.0;
        ptNew.angle = 0.0;
        ptNew.l1    = ptNew.scale;
        ptNew.l2    = ptNew.scale;
        m_vPoints.push_back( ptNew );
      }

    /* add those points that are new */
    int nSizeBefore = vPointsToRefine.size();
    for(unsigned j=0; j<m_vPoints.size(); j++ ) {
      bool found = false;
      for( int k=0; k<nSizeBefore && !found; k++ )
        if( (m_vPoints[j].x == vPointsToRefine[k].x) &&
            (m_vPoints[j].y == vPointsToRefine[k].y) )
          found = true;
      
      if( !found )
        vPointsToRefine.push_back( m_vPoints[j] );
    }
  }
  qApp->processEvents();

  /* extract the corresponding patches */
  m_vPoints              = vPointsToRefine;
  m_vvPointsInside[nIdx] = vPointsToRefine;
  //extractAllPatches(true);
  m_vCues[nIdx].processImage( m_sImgFullName, m_grayImg, m_grayImgMap, m_img, 
                              m_nFeatureType,
                              m_vPoints, m_vvPointsInside[nIdx], 
                              m_vImagePatches, m_vvFeatures[nIdx] );

  /***************************************************/
  /*   Match the extracted patches to the codebook   */
  /***************************************************/
  cout << "    Comparing image patches with all matches..." << endl;
  compareFeatures( false );
  
  /*******************************************************/
  /*   Apply the voting procedure to obtain hypotheses   */
  /*******************************************************/
  vector<Hypothesis> vResultHypos;

  /*--------------------------------*/
  /*   Initialize the VotingSpace   */
  /*--------------------------------*/
  m_vISMReco[nIdx].setRecoParams    ( m_parReco );
  m_vISMReco[nIdx].createVotingSpace( m_grayImg.width(), m_grayImg.height(),
                                      SIZE_VOTINGBINS, true );

  /*==========================*/
  /* Apply patch-based voting */
  /*==========================*/
  m_vISMReco[nIdx].doPatchVoting( m_vvPointsInside[nIdx], 
                                  m_vMatchResults[nIdx],
                                  m_vParMatching[nIdx].params()->m_dRejectionThresh, 
                                  true );
    
  cout << "================================" << endl;

  /*--------------------------*/
  /* Test single-patch voting */
  /*--------------------------*/
  vResultHypos = m_vISMReco[nIdx].refinePatchHypotheses(m_vHyposSingle, 
                                                        m_vvPointsInside[nIdx],
                                                        bDisplayResults );

  /* adapt the hypotheses to fixed bounding boxes */
  for(unsigned i=0; i<vResultHypos.size(); i++ ) {
    vResultHypos[i].nBoxX1 = (vResultHypos[i].x - nObjWidth/2);
    vResultHypos[i].nBoxY1 = (vResultHypos[i].y - nObjHeight/2);
    vResultHypos[i].nBoxWidth  = nObjWidth;
    vResultHypos[i].nBoxHeight = nObjHeight;
  }
  
  /* prepare for displaying the results */
  FeatureVector fvWindowPos( 2 );
  vector<QImage> vPwResultImgs;
  vector<QImage> vPwSegmentImgs;
  emit sigRefreshSrcImg();
  qApp->processEvents();

  
  /* initialize the results */
  m_vHyposSingle.clear();

  /* prepare a "scene image" */
  //OpGrayImage imgAllSupport( m_grayImg.width(), m_grayImg.height() );
  OpGrayImage imgAllSegment( m_grayImg.width(), m_grayImg.height() );
  vector<HoughVote> vAllSupporting;
  vector<OpGrayImage> vImgSegment;
  vector<OpGrayImage> vImgPFig;
  vector<OpGrayImage> vImgPGnd;
  vector<float> vFigArea( vResultHypos.size() );
  vector<float> vSumPFig( vResultHypos.size() );
  int nCount = 0;
  if( m_parReco.params()->m_bDoMDL ) {
    cout << "=======================================" << endl;
    cout << "Refined Hypotheses:" << endl;
  }
  for( int i=0; i<(int)vResultHypos.size(); i++ )
    if( vResultHypos[i].dScore >= m_parReco.params()->m_dScoreThreshSingle ) {

      /* check if the hypothesis overlaps with higher-ranking hypos */
      bool bOverlaps = false;
      if( m_parReco.params()->m_bRejectOverlap )
        for( int j=0; j<i && !bOverlaps; j++ ) {
          float dOverlap = computeBoundingBoxOverlap( vResultHypos[j],
                                                      vResultHypos[i]);
          if( dOverlap >= m_parReco.params()->m_dMaxOverlap )
            bOverlaps = true;
        }
          
      QColor qcol; 
      if( !bOverlaps ) {
        m_vHyposSingle.push_back( vResultHypos[i] );

        /* draw a green rectangle around the detection */
        qcol = QColor::QColor(0, 255, 0);
      
        rsSourceImg->drawRect( vResultHypos[i].x - nObjWidth/2,
                               vResultHypos[i].y - nObjHeight/2,
                               nObjWidth, nObjHeight, qcol, true );
      
        /* extract the hypothesis support */
        fvWindowPos.setValue( 0, vResultHypos[i].x );
        fvWindowPos.setValue( 1, vResultHypos[i].y );
        
        vector<HoughVote> vSupporting;
        vSupporting = m_vISMReco[nIdx].getSupportingVotes( fvWindowPos );
        
//         OpGrayImage imgSupport = drawVotePatches( vSupporting, 
//                                                   m_bMapsOnManually, 
//                                                   m_bDrawConfidence );
        
        OpGrayImage imgPFig;
        OpGrayImage imgPGnd;
        OpGrayImage imgSeg;
        //OpGrayImage imgSegment = drawVotePatches( vSupporting, true, false ); 
        m_vISMReco[nIdx].drawSegmentation( vSupporting, 
                                           m_vvPointsInside[nIdx], 
                                           m_vCues[nIdx],
                                           imgPFig, imgPGnd, imgSeg, true );

        /*-----------------------------------------------------------*/
        /* for MDL selection, compute additional segmentation images */
        /*-----------------------------------------------------------*/
        if( m_parReco.params()->m_bDoMDL ) {
          /* one segmentation with zero background */
          vImgSegment.push_back( imgSeg );

          /* and one p(figure) image */
          vImgPFig.push_back( imgPFig );

          /* and one p(ground) image */
          vImgPGnd.push_back( imgPGnd );

          /* compute some hypothesis attributes: */
          /*   - figure area                     */
          /*   - sum(p(figure))                  */
          float dFigArea = vImgSegment[i].getSum()/255.0;
          float dSumPFig = vImgPFig[i].getSum();
          cout << "  " << setw(2) << i+1 << ". ";
          printHypothesis( vResultHypos[i] );
          cout << "          "
               << "FigArea=" << (int) dFigArea 
               << ", Sum(pfig)=" << (int) dSumPFig << endl;

          vFigArea[i] = dFigArea;
          vSumPFig[i] = dSumPFig;
          
        }

        vPwSegmentImgs.push_back( imgSeg.getQtImage() );
        
        /* Prepare a "scene picture" */
        vAllSupporting.insert( vAllSupporting.end(), vSupporting.begin(),
                               vSupporting.end() );

        imgAllSegment = imgAllSegment.add( imgSeg );
        nCount++;
      }
    }
  
  if( m_parReco.params()->m_bDoMDL ) {
    cout << "---------------------------------------" << endl;
    cout << "Interactions:" << endl;
    for(unsigned j=0; j<m_vHyposSingle.size(); j++ )
      for(unsigned k=0; k<j; k++ ) {
        /* compute the overlapping area */
        float dFigAreaOverlap = 0.0;
        float dSumPFigOverlap = 0.0;
        OpGrayImage imgAreaOverlap( m_grayImg.width(), m_grayImg.height() );
        OpGrayImage imgPFigOverlap( m_grayImg.width(), m_grayImg.height() );
        for( int y=0; y<m_grayImg.height(); y++ )
          for( int x=0; x<m_grayImg.width(); x++ ) {
            float dMinSeg  = min( vImgSegment[j](x,y).value(), 
                                  vImgSegment[k](x,y).value() );
            imgAreaOverlap(x,y) = dMinSeg;
            dFigAreaOverlap += dMinSeg;
            
            float dMinPFig = min( vImgPFig[j](x,y).value(), 
                                  vImgPFig[k](x,y).value() );
            imgPFigOverlap(x,y) = dMinPFig;
            dSumPFigOverlap += dMinPFig;
          }
        dFigAreaOverlap /= 255.0;
        cout << "  " << k+1 << " <-> " << j+1 << ": Overlapping " << setw(6) 
             << "FigArea=" << dFigAreaOverlap 
             << ", Sum(pfig)=" << dSumPFigOverlap << endl;
        
        //vImgVisualize.push_back( imgAreaOverlap );
        //vImgVisualize.push_back( imgPFigOverlap );
      }
    

    /*------------------------------*/
    /* Select consistent hypotheses */
    /*------------------------------*/
    if( m_parReco.params()->m_bRejectPFig ) {
      cout << "---------------------------------------" << endl;
      cout << "Selection:" << endl;
      
      unsigned nChosen = 0;
      vector<bool>  vChosen( vResultHypos.size(), false );
      bool bFinished = false;
      while( nChosen<vResultHypos.size() && !bFinished ) {
        
        /* find the hypothesis with max. SumPFig */
        float dMaxVal = 0.0;
        int   nMaxIdx = 0;
        for(unsigned k=0; k<vResultHypos.size(); k++ )
          if( !vChosen[k] )
            if( vSumPFig[k] > dMaxVal ) {
              dMaxVal = vSumPFig[k];
              nMaxIdx = k;
            }
        
        if( dMaxVal < m_parReco.params()->m_dMinPFigRefined )
          bFinished = true;
        
        else {
          /* select this hypothesis */
          vChosen[nMaxIdx] = true;
          nChosen++;
          cout << "  Selected hypothesis " << nMaxIdx+1 << endl;
          
          /* update the remaining hypotheses */
          for(unsigned k=0; k<vResultHypos.size(); k++ )
            if( !vChosen[k] ) {
              
              /* remove the overlapping area */
              for( int y=0; y<m_grayImg.height(); y++ )
                for( int x=0; x<m_grayImg.width(); x++ ) //{
                  if( vImgPFig[nMaxIdx](x,y).value() > 
                      vImgPGnd[nMaxIdx](x,y).value() ) {
                    /* pixel is better explained by selected hypothesis */
                    /* => remove it from this hypothesis */
                  //vImgSegment[k](x,y) = min(vImgSegment[k](x,y).value(), 
                  //                          vImgSegment[nMaxIdx](x,y).value());
                  //vImgPFig[k](x,y)    = min(vImgPFig[k](x,y).value(), 
                  //                          vImgPFig[nMaxIdx](x,y).value());
                  vImgSegment[k](x,y) = 0.0;
                  vImgPFig[k](x,y)    = 0.0;
                }
              vFigArea[k] = vImgSegment[k].getSum()/255.0;
              vSumPFig[k] = vImgPFig[k].getSum();
              
              cout << "  => " << setw(2) << k+1 << ". ";
              printHypothesis( vResultHypos[k] );
              cout << "             "
                   << "FigArea=" << vFigArea[k] 
                   << ", Sum(pfig)=" << vSumPFig[k] << endl;
            }
        }
      }
    }
    
    cout << "=======================================" << endl;
  }

  /* finish the "scene picture" */
  //imgAllSupport = imgAllSupport.div( (float) nCount );
  imgAllSegment = imgAllSegment.div( (float) nCount );
  //vPwResultImgs.push_back( imgAllSupport.getQtImage() ); 
  vPwSegmentImgs.push_back( imgAllSegment.getQtImage() );

  /* ...and display it */
  emit sigDisplayResultImg( imgAllSegment, imgAllSegment.getQtImage() );
  qApp->processEvents();
  
  m_vActiveVotes = vAllSupporting;

  if( bDisplayResults && (vPwResultImgs.size() > 0) ) {
    /* show the support area in one browser... */
    QtImgBrowser *qtPwResultBrowser = new QtImgBrowser( 0,"Reco Results (Refined)"); 
    qtPwResultBrowser->setGeometry( 950, 200, 300, 350 );
    qtPwResultBrowser->load( vPwResultImgs );
    qtPwResultBrowser->show();

    /* ...and the segmentation in another one */
    QtImgBrowser *qtPwSegmentBrowser = new QtImgBrowser( 0,"Segmentation Results (Refined)"); 
    qtPwSegmentBrowser->setGeometry( 950, 600, 300, 350 );
    qtPwSegmentBrowser->load( vPwSegmentImgs );
    qtPwSegmentBrowser->show();
  }
}


void Detector::refineHypothesesMultiScale( bool bDisplayResults )
  /*******************************************************************/
  /* Refine the current hypotheses (as defined by the votes contain- */
  /* ed in m_vActiveVotes) by uniform sampling (using scale votes).  */
  /*******************************************************************/
{
  unsigned nIdx=0;

  if( (m_vHyposSingle.size() == 0) ) {
    cerr << "  No hypotheses to refine!" << endl;
    return;
  } else
    cout << "  Refining " << m_vHyposSingle.size() 
         << " hypotheses (multi-scale)..." << endl;

  /***************************************/
  /*   Initialize the global variables   */
  /***************************************/
  m_vImagePatches.clear();
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    m_vvPointsInside[k].clear();
    m_vvFeatures[k].clear();
  }

  /***********************/
  /* Get the occurrences */
  /***********************/
  VecVecOccurrence vvOccurrences = m_vISMReco[nIdx].getOccurrences();

  /*************************************************/
  /*   Look up all possibly contributing patches   */
  /*************************************************/
  /* for all hypotheses */
  PointVector vPoints;
  for( int i=0; i<(int)m_vHyposSingle.size(); i++ ) {
    Hypothesis hypo = m_vHyposSingle[i];
    
    /* go through all occurrences */
    for( int j=0; j<(int)vvOccurrences.size(); j++ )
      for( int k=0; k<(int)vvOccurrences[j].size(); k++ ) {
        ClusterOccurrence occ = vvOccurrences[j][k];

        /* compute the corresponding patch location and scale */
        InterestPoint pt;
        pt.x     = (int) floor(hypo.x + (occ.dPosX * hypo.dScale) + 0.5);
        pt.y     = (int) floor(hypo.y + (occ.dPosY * hypo.dScale) + 0.5);
        pt.value = 1.0;
        pt.scale = hypo.dScale * occ.dScale;
        pt.angle = 0.0;
        pt.l1    = pt.scale;
        pt.l2    = pt.scale;
        
        /* add the point to the list */
        if( (pt.scale >= m_vCues[k].params()->m_dMinScale) && 
            (pt.scale <= m_vCues[k].params()->m_dMaxScale) )
          vPoints.push_back( pt );
      }   
  }

  if( vPoints.size() == 0 )
    return;

  /* sort the interest point list */
  stable_sort( vPoints.begin(), vPoints.end(), compInterestPtsX() );
  stable_sort( vPoints.begin(), vPoints.end(), compInterestPtsY() );
  stable_sort( vPoints.begin(), vPoints.end(), compInterestPtsScale() );

  /* transfer all unique points to a new list */
  m_vPoints.clear();
  int nLastX       = vPoints[0].x;
  int nLastY       = vPoints[0].y;
  float dLastScale = vPoints[0].scale;
  InterestPoint ptCurrent = vPoints[0];
  for( int i=1; i<(int)vPoints.size(); i++ )
    if( (vPoints[i].x != nLastX) || (vPoints[i].y != nLastY) ||
        (vPoints[i].scale != dLastScale) ) {
      m_vPoints.push_back( ptCurrent );

      nLastX     = vPoints[i].x;
      nLastY     = vPoints[i].y;
      dLastScale = vPoints[i].scale;
      ptCurrent  = vPoints[i];
    }
  m_vPoints.push_back( ptCurrent );

  /* extract the corresponding patches */
  //extractAllPatches( true );
  m_vCues[nIdx].processImage( m_sImgFullName, m_grayImg, m_grayImgMap, m_img, 
                              m_nFeatureType,
                              m_vPoints, m_vvPointsInside[nIdx], 
                              m_vImagePatches, m_vvFeatures[nIdx] );

  /***************************************************/
  /*   Match the extracted patches to the codebook   */
  /***************************************************/
  cout << "    Comparing image patches with all matches..." << endl;
  compareFeatures( false );
  
  /*******************************************************/
  /*   Apply the voting procedure to obtain hypotheses   */
  /*******************************************************/
  vector<Hypothesis> vResultHypos;

  /*==========================*/
  /* Apply patch-based voting */
  /*==========================*/
  //float dMSMESizeX    = m_parReco.params()->m_dMSMESizeX;
  //float dMSMESizeY    = m_parReco.params()->m_dMSMESizeY;
  float dMSMESizeS    = m_parReco.params()->m_dMSMESizeS;
  int   nObjWidth     = m_parReco.params()->m_nObjWidth;
  int   nObjHeight    = m_parReco.params()->m_nObjHeight;
  float dRecoScaleMin = m_parReco.params()->m_dRecoScaleMin;
  float dRecoScaleMax = m_parReco.params()->m_dRecoScaleMax;

  float dMaxScaleFactor = m_grayImg.width() / ((float) nObjWidth);
  float dMaxScaleLevel  = ( (floor(dMaxScaleFactor / dMSMESizeS)+1.0)*
                            dMSMESizeS );

  float dScoreThreshSingle = m_parReco.params()->m_dScoreThreshSingle;
  bool  bRejectOverlap     = m_parReco.params()->m_bRejectOverlap;
  float dMaxOverlap        = m_parReco.params()->m_dMaxOverlap;

  /*--------------------------------*/
  /*   Initialize the VotingSpace   */
  /*--------------------------------*/
  float dScaleMin = dRecoScaleMin - dMSMESizeS/2.0;
  float dScaleMax = min( dRecoScaleMax, dMaxScaleLevel) + dMSMESizeS/2.0;
  int   nScaleSteps = (int)floor((dScaleMax - dScaleMin)/
                                 dMSMESizeS) + 1;

  m_vISMReco[nIdx].setRecoParams    ( m_parReco );
  m_vISMReco[nIdx].createVotingSpace( m_grayImg.width(), m_grayImg.height(),
                                      SIZE_VOTINGBINS, 
                                      dScaleMin, dScaleMax, nScaleSteps, 
                                      true );

  /*==========================*/
  /* Apply patch-based voting */
  /*==========================*/
  cout << "    Applying voting procedure..." << endl;
  m_vISMReco[nIdx].doPatchVoting( m_vvPointsInside[nIdx], 
                                  m_vMatchResults[nIdx],
                                  m_vParMatching[nIdx].params()->m_dRejectionThresh, 
                                  true );
    
  cout << "================================" << endl;

  /*--------------------------*/
  /* Test single-patch voting */
  /*--------------------------*/
  cout << "    Refining hypotheses using the new votes..." << endl;
  vResultHypos = m_ismMultiCue.refinePatchHypotheses( m_vHyposSingle, 
                                                      m_vvPointsInside[nIdx],
                                                      bDisplayResults );

  /* adapt the hypotheses to fixed bounding boxes */
  for(unsigned i=0; i<vResultHypos.size(); i++ ) {
    int width  = (int) floor(nObjWidth*vResultHypos[i].dScale + 0.5);
    int height = (int) floor(nObjHeight*vResultHypos[i].dScale + 0.5);
    vResultHypos[i].nBoxX1 = (vResultHypos[i].x - width/2);
    vResultHypos[i].nBoxY1 = (vResultHypos[i].y - height/2);
    vResultHypos[i].nBoxWidth  = width;
    vResultHypos[i].nBoxHeight = height;
  }
  
  /* prepare for displaying the results */
  vector<QImage> vResultImgs;
  vector<QImage> vSegmentImgs;
  emit sigRefreshSrcImg();
  qApp->processEvents();

  
  /* initialize the results */
  m_vHyposSingle.clear();

  /* prepare a "scene image" */
  cout << "    Displaying the result..." << endl;
  FeatureVector fvWindowPos( 3 );
  //OpGrayImage imgAllSupport( m_grayImg.width(), m_grayImg.height() );
  OpGrayImage imgAllSegment( m_grayImg.width(), m_grayImg.height() );
  vector<HoughVote> vAllSupporting;
  int nCount = 0;
  for(unsigned i=0; i<vResultHypos.size(); i++ )
    if( vResultHypos[i].dScore >= dScoreThreshSingle ) {

      /* check if the hypothesis overlaps with higher-ranking hypos */
      bool bOverlaps = false;
      if( bRejectOverlap )
        for(unsigned j=0; j<i && !bOverlaps; j++ ) {
          float dOverlap = computeBoundingBoxOverlap( vResultHypos[j],
                                                      vResultHypos[i]);
          if( dOverlap >= dMaxOverlap )
            bOverlaps = true;
        }
          
      QColor qcol; 
      int width  = (int) floor(nObjWidth*vResultHypos[i].dScale + 0.5);
      int height = (int) floor(nObjHeight*vResultHypos[i].dScale + 0.5);
      if( !bOverlaps ) {
        m_vHyposSingle.push_back( vResultHypos[i] );

        /* draw a green rectangle around the detection */
        qcol = QColor::QColor(0, 255, 0);
      
        rsSourceImg->drawRect( vResultHypos[i].x - width/2,
                               vResultHypos[i].y - height/2,
                               width, height, qcol, true );
      
        /* extract the hypothesis support */
        fvWindowPos.setValue( 0, vResultHypos[i].x );
        fvWindowPos.setValue( 1, vResultHypos[i].y );
        fvWindowPos.setValue( 2, vResultHypos[i].dScale );
        
        vector<HoughVote> vSupporting;
        vSupporting = m_vISMReco[nIdx].getSupportingVotes( fvWindowPos );
        
        /* draw a support image */
//         OpGrayImage imgSupport = drawVotePatches( vSupporting, 
//                                                   m_bMapsOnManually, 
//                                                   m_bDrawConfidence );
        
        /* draw the segmentation */
        OpGrayImage imgSeg;
        OpGrayImage imgPFig;
        OpGrayImage imgPGnd;
        m_vISMReco[nIdx].drawSegmentation( vSupporting, 
                                           m_vvPointsInside[nIdx], 
                                           m_vCues[nIdx], 
                                           imgPFig, imgPGnd, imgSeg, true );

        //vResultImgs.push_back( imgSupport.getQtImage() ); 
        vSegmentImgs.push_back( imgSeg.getQtImage() );
        
        /* Prepare a "scene picture" */
        vAllSupporting.insert( vAllSupporting.end(), vSupporting.begin(),
                               vSupporting.end() );

        //imgAllSupport = imgAllSupport.add( imgSupport );
        imgAllSegment = imgAllSegment.add( imgSeg );
        nCount++;
      }
    }
  
        
  cout << "=======================================" << endl;
  
  /* finish the "scene picture" */
  //imgAllSupport = imgAllSupport.div( (float) nCount );
  imgAllSegment = imgAllSegment.div( (float) nCount );
  //vResultImgs.push_back( imgAllSupport.getQtImage() ); 
  vSegmentImgs.push_back( imgAllSegment.getQtImage() );

  /* ...and display it */
  emit sigDisplayResultImg( imgAllSegment, imgAllSegment.getQtImage() );
  qApp->processEvents();
  
  m_vActiveVotes = vAllSupporting;

  if( bDisplayResults && (vResultImgs.size() > 0) ) {
    /* show the support area in one browser... */
    QtImgBrowser *qtResultBrowser = new QtImgBrowser( 0,"Reco Results (Refined)"); 
    qtResultBrowser->setGeometry( 950, 200, 300, 350 );
    qtResultBrowser->load( vResultImgs );
    qtResultBrowser->show();

    /* ...and the segmentation in another one */
    QtImgBrowser *qtSegmentBrowser = new QtImgBrowser( 0,"Segmentation Results (Refined)"); 
    qtSegmentBrowser->setGeometry( 950, 600, 300, 350 );
    qtSegmentBrowser->load( vSegmentImgs );
    qtSegmentBrowser->show();
  }
}


void ISMReco::testMatching( unsigned nIdx )
{
  cout << endl
       << "===============================" << endl
       << "Testing the Matching run-times " << endl
       << "===============================" << endl;
     
  vector<FeatureVector> vFeatures = m_vCodebooks[nIdx].getFeatures();
  vector<FeatureVector> vClusters = m_vCodebooks[nIdx].getClusters();
  vector<ClStep>        vTrace    = m_vCodebooks[nIdx].getClusterTrace();

  // TIMING CODE
  time_t  tu0, tu1, tu2;
  double  tc0, tc1, tc2;
  
  /* matching radius for Euclidean distances */
  float dFeatureSimFact  = m_vParMatching[nIdx].params()->m_dFeatureSimFact;
  float dRejectionThresh = m_vParMatching[nIdx].params()->m_dRejectionThresh;
  float dDimFact         = (float)vFeatures.front().numDims();
  float dRadius          = sqrt( -dFeatureSimFact*dDimFact * 
                                 log(dRejectionThresh) );
  float dRadius2         = dRadius*dRadius;

  unsigned nNumFeatures  = vFeatures.size();

  long nNumCorrect  = 0;
  long nNumInserted = 0;
  long nNumMissed   = 0;

  // TIMING CODE
  time(&tu0);
  tc0 = CPUTIME();

  /**************************/
  /* Test the Linear Search */
  /**************************/
  cout << "  Testing the Linear Search: " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << endl;

  //for( int k=0; k<1; k++ ) {
  int   k=0;
  float dPercentMatched = 1.0 - k*0.01;
  
  vector<unsigned> vEmpty;
  vector<int>      vEmptyInt;
  //vector<int>          vNNCorrect( vFeatures.size(), -1 );
  vector<vector<unsigned> > vvMatchesCorrect( vFeatures.size(), vEmpty );

  cout << "    Matching to " << setw(3) 
       << (int)floor(dPercentMatched*100.0+0.5) 
       << "% of the clusters... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  unsigned nNumClusters = (unsigned)floor(vClusters.size()*dPercentMatched 
                                          + 0.5);
  long nNumFoundLinear = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    //float dMinDist = vFeatures[i].compSSD( vClusters[0] );
    //int   nMinIdx  = 0;
    vvMatchesCorrect[i].clear();
    for( unsigned j=1; j<nNumClusters; j++ ) {
      float dDist = vFeatures[i].compSSD( vClusters[j] );
      //if( dDist < dMinDist ) {
      //  dMinDist = dDist;
      //  nMinIdx = j;
      //}
      if( dDist <= dRadius2 )
        vvMatchesCorrect[i].push_back( j );
    }
    nNumFoundLinear += vvMatchesCorrect[i].size();
  }
  
  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

    cout << setw(12) << tc2-tc1 << "s (system), "
         << tu2-tu1 << "s (user)" << endl;

    cout << "      found " << nNumFoundLinear << " matches." << endl;
    //}

  cout << "-------------------------------" << endl;

  /*************************************/
  /* Test the Balltree(Anchors) Search */
  /*************************************/
  cout << "  Testing the Balltree Search (Anchors): " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    Matching radius: " << dRadius << endl
       << endl;

  /*-----------------------*/
  /* Build up the balltree */
  /*-----------------------*/
  int nMaxNodeSize = 400;
  cout << "    Building up an anchors balltree...  " << flush;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  BallTree btNNTree;
  btNNTree.build( vClusters, nMaxNodeSize );

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  cout << "\r    Building up an anchors balltree...   " << flush;
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;
  
  /*-------------------------*/
  /* Match the features (NN) */
  /*-------------------------*/
  dPercentMatched = 1.0;

  cout << "    Finding nearest neighbors (exact)... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    float    dMinDist = 999999999999999.9;
    unsigned nMinIdx = 0;

    btNNTree.findNN( vFeatures[i], dRadius2, nMinIdx, dMinDist );
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  /*---------------------------*/
  /* Match the features (AllN) */
  /*---------------------------*/
  dPercentMatched = 1.0;

  cout << "    Finding all neighbors (exact)...     " << flush;
  
  vector<vector<unsigned> > vvMatchesBallTree( vFeatures.size(), vEmpty );

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFoundBallTree = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vvMatchesBallTree[i] = btNNTree.findNeighbors( vFeatures[i], dRadius2 );
    nNumFoundBallTree += vvMatchesBallTree[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFoundBallTree << " matches." << endl
       << endl;

  cout << "    Verifying... " << flush;
  nNumCorrect  = 0;
  nNumInserted = 0;
  nNumMissed   = 0;
  for( unsigned i=0; i<nNumFeatures; i++ ) {
    /* get the two vectors */
    vector<unsigned> vCorrect = vvMatchesCorrect[i];
    vector<unsigned> vResult  = vvMatchesBallTree[i];
    
    /* sort the two vectors */
    sort( vCorrect.begin(), vCorrect.end() );
    sort( vResult.begin(), vResult.end() );

    cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
         << flush;

    /* verify the results */
    vector<unsigned>::iterator itCorrect = vCorrect.begin();
    vector<unsigned>::iterator itResult  = vResult.begin();
    while( !(itCorrect==vCorrect.end() && itResult==vResult.end()) ) {
      if( itCorrect==vCorrect.end() )
        while( itResult!=vResult.end() ) {
          nNumInserted++;
          itResult++;
        }
      else if( itResult==vResult.end() )
        while( itCorrect!=vCorrect.end() ) {
          nNumMissed++;
          itCorrect++;
        }
      else {
        if( *itCorrect < *itResult ) {
          nNumMissed++;
          itCorrect++;
        } else if( *itCorrect == *itResult ) {
          nNumCorrect++;
          itCorrect++;
          itResult++;
        } else {
          nNumInserted++;
          itResult++;
        }
      }
    }
  }
  cout << " done." << endl
       << "      #Correct : " << nNumCorrect << endl
       << "      #Inserted: " << nNumInserted << endl
       << "      #Missed  : " << nNumMissed << endl;

  cout << "-------------------------------" << endl;


//   /*************************************/
//   /* Test the Balltree(Trace) Search */
//   /*************************************/
//   cout << "  Testing the Balltree Search (Trace): " << endl
//        << "    #features: " << nNumFeatures << endl
//        << "    #clusters: " << vClusters.size() << endl
//        << "    #dims    : " << vFeatures[0].numDims() << endl
//        << "    Matching radius: " << dRadius << endl
//        << endl;

//   /*-----------------------*/
//   /* Build up the balltree */
//   /*-----------------------*/
//   cout << "    Building up a balltree from the trace...  " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   // for GLOH128
//   // Cluster:  7M
//   // Matching: 5M

//   KBallTree kbtNNTree;
//   float dMinThresh = dRadius2;
//   float dMaxThresh = 10.0*dMinThresh;
//   int   nLevels    = 10;
//   float dQuant     = 0.999;
//   kbtNNTree.build( vFeatures, vTrace, dMinThresh, dMaxThresh, nLevels, 
//                    dQuant );


//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Building up a balltree from the trace...  " << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;
  
//   /*---------------------------*/
//   /* Match the features (AllN) */
//   /*---------------------------*/
//   dPercentMatched = 1.0;

//   cout << "    Finding all neighbors (exact)...     " << flush;
  
//   vector<KM::CornerDescriptor*> vEmptyCD;
//   vector<vector<KM::CornerDescriptor*> > vvMatchesKBallTree( vFeatures.size(),
//                                                              vEmptyCD );

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   long nNumFoundKBallTree = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     vvMatchesKBallTree[i] = kbtNNTree.findNeighbors2( vFeatures[i], 
//                                                       dRadius2, 1 );
//     nNumFoundKBallTree += vvMatchesBallTree[i].size();
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      found " << nNumFoundKBallTree << " matches." << endl
//        << endl;

//   cout << "    Verifying... " << flush;
//   nNumCorrect  = 0;
//   nNumInserted = 0;
//   nNumMissed   = 0;
//   float dEps2  = dDimFact;
//   for( unsigned i=0; i<nNumFeatures; i++ ) {
//     /* get the two vectors */
//     vector<unsigned>      vCorrect = vvMatchesCorrect[i];
//     vector<bool>          vMatched( vCorrect.size(), false );
//     vector<FeatureVector> vResult; 
//     kbtNNTree.cnvDescriptorsToFeatureVecs( vvMatchesKBallTree[i],
//                                            vResult );
    
//     cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
//          << flush;

//     if( vCorrect.size()==0 )
//       continue;

//     /* verify the results */
//     for( unsigned j=0; j<vResult.size(); j++ ) {
//       int   nMinIdx  = -1;
//       float dMinDist = 9999999999999999999.9; 
//       for( unsigned k=0; k<vCorrect.size(); k++ )
//         if( !vMatched[k] ) {
//           float dDist = vResult[j].compSSD( vClusters[vCorrect[k]] );
//           if( dDist < dMinDist ) {
//             dMinDist = dDist;
//             nMinIdx = k;
//           }
//         }

//       if( nMinIdx>=0 )
//         if( dMinDist <= dEps2 ) { // found a match
//           vMatched[nMinIdx] = true;
//           nNumCorrect++;
//         } else                 // feature not in correct set
//           nNumInserted++;
//     }

//     for( unsigned k=0; k<vMatched.size(); k++ )
//       if( !vMatched[k] )
//         nNumMissed++;
//   }
//   cout << " done." << endl
//        << "      #Correct : " << nNumCorrect << endl
//        << "      #Inserted: " << nNumInserted << endl
//        << "      #Missed  : " << nNumMissed << endl;

//   cout << "-------------------------------" << endl;


//   /******************************/
//   /* Test the Nene/Nayar Search */
//   /******************************/
//   //float dEps = sqrt((dRadius*dRadius)/dDimFact);
//   float dEps = sqrt((dRadius*dRadius)/sqrt(dDimFact));
//   //float dEps = dRadius;
//   cout << "  Testing the Nene/Nayar search: " << endl
//        << "    #features: " << nNumFeatures << endl
//        << "    #clusters: " << vClusters.size() << endl
//        << "    #dims    : " << vFeatures[0].numDims() << endl
//        << "    Matching radius : " << dRadius << endl
//        << "    Radius/dimension: " << dEps << endl
//        << endl;

//   /*------------------------*/
//   /* Build up the hypercube */
//   /*------------------------*/
//   cout << "    Building up the hypercube structure...  " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   NNSearch nnNNCube;
//   nnNNCube.init( vFeatures );

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Building up the hypercube structure..." << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;
  
//   /*----------------------------------*/
//   /*  Verify the hypercube parameters */
//   /*----------------------------------*/
//   cout << "    Verifying the hypercube radius...     " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   unsigned nNumToTest = max( 100, (int)nNumFeatures/20 );
//   float    dMaxDimDist = 0.0;
//   for( unsigned i=0; i<nNumToTest; i++ )
//     for( unsigned j=0; j<vvMatchesCorrect[i].size(); j++ ) {
//       FeatureVector fvDist = ( vFeatures[i] - 
//                                vClusters[vvMatchesCorrect[i][j]] );
//       for( unsigned k=0; k<fvDist.numDims(); k++ )
//         if( fabs(fvDist.at(k)) > dMaxDimDist )
//           dMaxDimDist = fabs(fvDist.at(k));
//     }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Verifying the hypercube radius...     " << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      eps=" << dMaxDimDist << endl;
//   dEps = dMaxDimDist;

//   /*-------------------------*/
//   /* Match the features (NN) */
//   /*-------------------------*/
//   cout << "    Finding nearest neighbors (exact)... " << flush;
  
//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     float    dMinDist = 999999999999999.9;
//     unsigned nMinIdx = 0;

//     nMinIdx = nnNNCube.getNN( vFeatures[i], dRadius, dEps );
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   /*---------------------------*/
//   /* Match the features (AllN) */
//   /*---------------------------*/
//   cout << "    Finding all neighbors (exact)...     " << flush;
  
//   vector<vector<int> > vvMatchesNene( vFeatures.size(), vEmptyInt );

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   long nNumFoundNene = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     vvMatchesNene[i] = nnNNCube.getNNIdzs( vFeatures[i], dRadius, dEps );
//     nNumFoundNene += vvMatchesNene[i].size();
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      found " << nNumFoundNene << " matches." << endl
//        << endl;

//   cout << "    Verifying... " << flush;
//   nNumCorrect  = 0;
//   nNumInserted = 0;
//   nNumMissed   = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     /* get the two vectors */
//     vector<unsigned> vCorrect = vvMatchesCorrect[i];
//     vector<int>      vResult  = vvMatchesNene[i];
    
//     /* sort the two vectors */
//     sort( vCorrect.begin(), vCorrect.end() );
//     sort( vResult.begin(), vResult.end() );

//     cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
//          << flush;

//     /* verify the results */
//     vector<unsigned>::iterator itCorrect = vCorrect.begin();
//     vector<int>::iterator      itResult  = vResult.begin();
//     while( !(itCorrect==vCorrect.end() && itResult==vResult.end()) ) {
//       if( itCorrect==vCorrect.end() )
//         while( itResult!=vResult.end() ) {
//           nNumInserted++;
//           itResult++;
//         }
//       else if( itResult==vResult.end() )
//         while( itCorrect!=vCorrect.end() ) {
//           nNumMissed++;
//           itCorrect++;
//         }
//       else {
//         if( *itCorrect < *itResult ) {
//           nNumMissed++;
//           itCorrect++;
//         } else if( *itCorrect == *itResult ) {
//           nNumCorrect++;
//           itCorrect++;
//           itResult++;
//         } else {
//           nNumInserted++;
//           itResult++;
//         }
//       }
//     }
//   }
//   cout << " done." << endl
//        << "      #Correct : " << nNumCorrect << endl
//        << "      #Inserted: " << nNumInserted << endl
//        << "      #Missed  : " << nNumMissed << endl;

//   cout << "-------------------------------" << endl;

}
