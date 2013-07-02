/*********************************************************************/
/*                                                                   */
/* FILE         mcmatcher-verification.cc                            */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Interleaved Object Categorization and Segmentation   */
/*              with an Implicit Shape Model.                        */
/*                                                                   */
/*              This file contains experimental methods for hypothe- */
/*              sis verification.                                    */
/*                                                                   */
/* COPYRIGHT    Bastian Leibe, ETH Zurich, 2003.                     */
/*                                                                   */
/* BEGIN        Wed Feb 23 2005                                      */
/* LAST CHANGE  Tue Oct 11 2005                                      */
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

#include <qfiledialog.h>

#include <opgrayimage.hh>
#include <qtimgbrowser.hh>
#include <Candidate.h>
#include <chamfermatching.h>

#include "mcmatcher.hh"

/***********************************************************/
/*                          Slots                          */
/***********************************************************/

/*---------------------------------------------------------*/
/*                 Loading/Using Silhouettes               */
/*---------------------------------------------------------*/

void ISMReco::loadSilhouettes()
  /*******************************************************************/
  /* Load a new set of silhouettes from disk.                        */
  /*******************************************************************/
{ 
  m_bSilhouettesLoaded = false;
  QStringList qslImageList = getFileList( DIR_SILHOUETTES, 
                                          "Select Silhouettes" );

  if ( qslImageList.isEmpty() )
    return;
  
  
  cout << "  Loading silhouette images..." << endl;
  vector<OpGrayImage> vSilhouettes ( qslImageList.count() );
  vector<QImage>      vQSilhouettes( qslImageList.count() );
  vector<OpGrayImage> vSilhMasks   ( qslImageList.count() );
  vector<QImage>      vQSilhMasks  ( qslImageList.count() );
  m_vTemplates.clear();
  for( int i=0; i<(int)qslImageList.count(); i++ ) {
    QImage img;
    img.load( qslImageList[i] );

    OpGrayImage imgOriginal = img;

    OpGrayImage imgResized = img;
    int nNewWidth = (int)floor(imgResized.width()*
                               m_parVeri.params()->m_dTemplateScale+0.5);
    imgResized = imgResized.opRescaleToWidth( nNewWidth );

    vSilhouettes[i] = imgResized;
    //vSilhouettes[i].opThresholdAbove( 200.0, 0.0, 1.0 );
    vSilhouettes[i].opThresholdAbove( 213.0, 0.0, 1.0 );
    vQSilhouettes[i] = vSilhouettes[i].getQtImage();

    /* extract exact edges */
    OpGrayImage imgMask = imgOriginal;
    imgMask.opThresholdAbove( 254.0, 0.0, 1.0 );
    imgMask = imgMask.opAreaDistanceTransform(); // fill silhouette
    imgMask.opThresholdInside( 1.0, 2.0, 1.0, 0.0 );
    imgMask = imgMask.opErodeBlockMask( 1 );
    imgMask = imgMask.opRescaleToWidth( nNewWidth );
    //imgMask = imgMask.opCannyEdges( 0.7, 0.2, 0.5 ); // extract silhouette
    //imgMask.opThresholdAbove( 128.0, 0.0, 1.0 ); 
    vSilhMasks[i] = imgMask;
    vQSilhMasks[i] = vSilhMasks[i].getQtImage();

    /* prepare Templates */
    m_vTemplates.push_back( ChamferMatching::prepareTemplate(vSilhouettes[i]));

    imgOriginal.opThresholdAbove( 128.0, 0.0, 1.0 );
    m_vTemplatesOrig.push_back( ChamferMatching::prepareTemplate(imgOriginal));
  }
  m_bSilhouettesLoaded = true;
  m_vSilhouettes  = vSilhouettes;
  m_vQSilhouettes = vQSilhouettes;
  m_vSilhMasks    = vSilhMasks;
    
  /* display the silhouettes in a browser */
  QtImgBrowser *qtSilhBrowser = new QtImgBrowser( 0, "silhouettes" );
  qtSilhBrowser->setGeometry( 950, 0, 200, 300 );
  qtSilhBrowser->load( vQSilhouettes, vSilhouettes );
  qtSilhBrowser->show();

  /* display the silhouettes in a browser */
  QtImgBrowser *qtSilhMBrowser = new QtImgBrowser( 0, "silhouettes" );
  qtSilhMBrowser->setGeometry( 950, 0, 200, 300 );
  qtSilhMBrowser->load( vQSilhMasks, vSilhMasks );
  qtSilhMBrowser->show();

  /* prepare Templates */
  //m_vTemplates.clear();
  //for( int i=0; i<m_vSilhouettes.size(); i++ )
  //  m_vTemplates.push_back( prepareTemplate( m_vSilhouettes[i] ) );
}


/*---------------------------------------------------------*/
/*                 Hypothesis Verification                 */
/*---------------------------------------------------------*/
// void ISMReco::verifyHyposChamfer( const vector<Hypothesis> &vInitialHypos,
//                                     vector<Hypothesis>       &vVerifiedHypos,
//                                     vector<QImage>           &vResultQImgs )
// {
//   if( !m_bSilhouettesLoaded ) {
//     cerr << "Need to load verification silhouettes first!" << endl;
//     return;
//   }

//   /*---------------------------*/
//   /* Initialize some variables */
//   /*---------------------------*/
//   int   nObjWidth     = m_parReco.params()->m_nObjWidth;
//   int   nObjHeight    = m_parReco.params()->m_nObjHeight;

//   vector<Hypothesis>  vHyposSegmented;
//   vector<OpGrayImage> vImgSegment;
//   for( int k=0; k<(int)vInitialHypos.size(); k++ ) {
//     cout << "  Verifying hypothesis " << k+1 << ": " << endl;
//     Hypothesis hypoInit = vInitialHypos[k];

//     /*---------------------------------------------*/
//     /* create the segmentation for this hypothesis */
//     /*---------------------------------------------*/
//     FeatureVector fvWindowPos( 3 );
//     fvWindowPos.setValue( 0, vInitialHypos[k].x );
//     fvWindowPos.setValue( 1, vInitialHypos[k].y );
//     fvWindowPos.setValue( 2, vInitialHypos[k].dScale );

//     vector<HoughVote> vSupporting;
//     vSupporting = m_ismMultiCue.getSupportingVotes( fvWindowPos );
    
//     OpGrayImage imgSeg;
//     OpGrayImage imgPFig;
//     OpGrayImage imgPGnd;
    
//     /* draw segmentation */
//     unsigned nIdx=0;
//     m_vISMReco[nIdx].drawSegmentation( vSupporting, m_vvPointsInside[nIdx], 
//                                        m_vCues[nIdx], 
//                                        imgPFig, imgPGnd, imgSeg, true );

//     /*-----------------------------------------*/
//     /* extract the support region in the image */
//     /*-----------------------------------------*/
//     int x1 = hypoInit.nBoxX1 - nObjWidth/4;
//     int y1 = hypoInit.nBoxY1 - nObjHeight/4;
//     int x2 = ( hypoInit.nBoxX1 + hypoInit.nBoxWidth + nObjWidth/4 );
//     int y2 = ( hypoInit.nBoxY1 + hypoInit.nBoxHeight + nObjHeight/4 );
//     OpGrayImage imgCutOut = m_grayImg.extractRegion( x1, y1, x2, y2 );
//     //OpGrayImage imgCutOut = imgSeg.extractRegion( x1, y1, x2, y2 );
//     OpGrayImage imgSegCutOut = imgSeg.extractRegion( x1, y1, x2, y2 );
//     //float dHypoScale = hypoInit.dScale;
//     //int nOffsetX = x1;
//     //int nOffsetY = y1;
//     int nNewWidth = (int) floor( (x2-x1)/hypoInit.dScale + 0.5 );
//     imgCutOut    = imgCutOut.opRescaleToWidth( nNewWidth );
//     imgSegCutOut = imgSegCutOut.opRescaleToWidth( nNewWidth );
//     QImage qimgCutOut    = imgCutOut.getQtImage(); 
//     QImage qimgSegCutOut = imgSegCutOut.getQtImage(); 
//     vResultQImgs.push_back( qimgCutOut );
//     vResultQImgs.push_back( qimgSegCutOut );

//     /*------------------------------------------------------*/
//     /* compute the distance transform of the support region */
//     /*------------------------------------------------------*/
//     OpGrayImage imgDT  = ChamferMatching::getDTImage( imgCutOut );
//     QImage      qimgDT = ChamferMatching::drawDT( imgDT );
//     vResultQImgs.push_back( qimgDT );

//     /* perform Chamfer matching with all templates */
//     CombiCandidateSet vCandidates;
//     int nMaxCandidates = 10;
//     for( int i=0; i<(int)m_vTemplates.size(); i++ ) {
//       int minx = -(int)floor(m_vSilhouettes[i].width()*0.25 + 0.5);
//       int miny = -(int)floor(m_vSilhouettes[i].height()*0.25 + 0.5);
//       int maxx = ( imgDT.width() - 
//                    (int)floor(m_vSilhouettes[i].width()*0.75+0.5) );
//       int maxy = ( imgDT.height() - 
//                    (int)floor(m_vSilhouettes[i].height()*0.75+0.5) );
//       ChamferMatching:: matchTemplateOnWindow( vCandidates, imgDT, 
//                                                m_vSilhouettes[i], 
//                                                m_vTemplates[i], i,
//                                                minx, miny, maxx, maxy, 
//                                                nMaxCandidates );
//     }
    
//     /* display the best-matching templates */
//     QImage qimgResult = ChamferMatching:: drawImageResult( vCandidates, 
//                                                            m_vSilhouettes, 
//                                                            nMaxCandidates, 
//                                                            imgDT );
//     vResultQImgs.push_back( qimgResult );
    
//     /* print out the results */
//     cout << "    -----------------------------------" << endl;
//     cout << "    Results of Chamfer verification:" << endl;
//     CombiCandidateSet::iterator it;
//     int i;
//     for( i=0, it=vCandidates.begin(); 
//          i<nMaxCandidates && it!=vCandidates.end(); i++, it++ ) {
//       cout << "      " << i+1 << ". ";
//       it->print();
//     }
//     cout << "    -----------------------------------" << endl;
//   }
// }


void ISMReco::verifyHyposTemplate( const vector<Hypothesis> &vInitHypos,
                                     const vector<Hypothesis> &vInitHyposTight,
                                     const vector<OpGrayImage> &vInitImgSeg,
                                     const vector<OpGrayImage> &vInitImgPFig,
                                     const vector<OpGrayImage> &vInitImgPGnd,
                                     vector<Hypothesis>   &vVerifiedHypos,
                                     vector<Hypothesis>   &vVerifiedHyposTight,
                                     vector<EdgePtVec>    &vVerifiedTemplates,
                                     vector<int>          &vVerifiedTemplIds,
                                     vector<QImage>            &vResultQImgs )
{
  if( !m_bSilhouettesLoaded ) {
    cerr << "Need to load verification silhouettes first!" << endl;
    return;
  }

  /*---------------------------*/
  /* Initialize some variables */
  /*---------------------------*/
  int   nObjWidth      = m_parReco.params()->m_nObjWidth;
  int   nObjHeight     = m_parReco.params()->m_nObjHeight;
  float dTemplateScale = m_parVeri.params()->m_dTemplateScale;

  vVerifiedHypos.clear();
  vVerifiedHyposTight.clear();
  vVerifiedTemplates.clear();
  vVerifiedTemplIds.clear();

  vector<Hypothesis>  vHyposSegmented;
  vector<OpGrayImage> vImgSegment;
  for( int k=0; k<(int)vInitHypos.size(); k++ ) {
    cout << "  Verifying hypothesis " << k+1 << ": " << endl;
    Hypothesis hypoInit = vInitHypos[k];

    /*---------------------------------------------*/
    /* create the segmentation for this hypothesis */
    /*---------------------------------------------*/
//     FeatureVector fvWindowPos( 3 );
//     fvWindowPos.setValue( 0, vInitHypos[k].x );
//     fvWindowPos.setValue( 1, vInitHypos[k].y );
//     fvWindowPos.setValue( 2, vInitHypos[k].dScale );

//     /* set the window size */
//     m_vsHoughVotesScale.setWindowSize( getAdaptiveWinSize( fvWindowPos ) );
    
//     vector<HoughVote> vSupporting;
//     vSupporting = m_vsHoughVotesScale.getSupportingVotes( fvWindowPos );
    
//     OpGrayImage imgSeg;
//     OpGrayImage imgPFig;
//     OpGrayImage imgPGnd;
    
//     /* draw segmentation */
//     drawSegmentation( vSupporting, imgPFig, imgPGnd, imgSeg, true );
    OpGrayImage imgSeg = vInitImgSeg[k];

    /*-----------------------------------------*/
    /* extract the support region in the image */
    /*-----------------------------------------*/
    int x1 = hypoInit.nBoxX1 - nObjWidth/2;
    int y1 = hypoInit.nBoxY1 - nObjHeight/2;
    int x2 = ( hypoInit.nBoxX1 + hypoInit.nBoxWidth + nObjWidth/2 );
    int y2 = ( hypoInit.nBoxY1 + hypoInit.nBoxHeight + nObjHeight/2 );
    OpGrayImage imgCutOut = m_grayImg.extractRegion( x1, y1, x2, y2 );
    OpGrayImage imgSegCutOut = imgSeg.extractRegion( x1, y1, x2, y2 );
    float dHypoScale  = hypoInit.dScale;
    //float dTemplScale = 0.5;
    float dVerifyScale = dHypoScale/dTemplateScale;
    int nOffsetX = x1;
    int nOffsetY = y1;
    int nNewWidth = (int) floor( ((x2-x1)/hypoInit.dScale)*dTemplateScale + 
                                 0.5 );
    imgCutOut    = imgCutOut.opRescaleToWidth( nNewWidth );
    imgSegCutOut = imgSegCutOut.opRescaleToWidth( nNewWidth );
    QImage qimgCutOut    = imgCutOut.getQtImage(); 
    QImage qimgSegCutOut = imgSegCutOut.getQtImage(); 
    vResultQImgs.push_back( qimgCutOut );
    vResultQImgs.push_back( qimgSegCutOut );

    /*------------------------------------------------------*/
    /* compute the distance transform of the support region */
    /*------------------------------------------------------*/
    OpGrayImage imgDT  = ChamferMatching::getDTImage( imgCutOut, 0.3, 0.6 );
    QImage      qimgDT = ChamferMatching::drawDT( imgDT );
    vResultQImgs.push_back( qimgDT );

    /* perform Chamfer matching with all templates */
    CombiCandidateSet vCandidates;
    int nMaxCandidates = 150;
    int nNumScales = 7;
    float dTightScale = ( (float)vInitHyposTight[k].nBoxHeight /
                          (float)vInitHypos[k].nBoxHeight );
    float dScaleMin  = 0.80; //*hypoInit.dScale;
    float dScaleMax  = 1.20; //*hypoInit.dScale;
    dScaleMin = min( dScaleMin, dScaleMin*dTightScale );
    dScaleMax = max( dScaleMax, dScaleMax*dTightScale );
    float dScaleStep = (dScaleMax - dScaleMin)/((float)nNumScales-1.0);
    int nWinSize  = 9;
    int nStepSize = 2;
    float dCenterX = (float)imgCutOut.width()/2.0;
    float dCenterY = (float)imgCutOut.height()/2.0;
    for( int i=0; i<(int)m_vTemplates.size(); i++ ) {
      /* for all scales */
      for( int s=0; s<nNumScales; s++ ) {
        /* rescale template */
        float dScaleFact = dScaleMin + s*dScaleStep;

        float dTemplW2 = m_vSilhMasks[i].width()*0.5;
        float dTemplH2 = m_vSilhMasks[i].height()*0.5;
        int nStartX  = (int)floor(dCenterX-dTemplW2*dScaleFact + 0.5);
        int nStartY  = (int)floor(dCenterY-dTemplH2*dScaleFact + 0.5);

        /* define small window */
        //int minx = -(int)floor(m_vSilhMasks[i].width()*0.25 + 0.5);
        //int miny = -(int)floor(m_vSilhMasks[i].height()*0.25 + 0.5);
        //int maxx = imgDT.width()-(int)floor(m_vSilhMasks[i].width()*0.75+0.5);
        //int maxy = imgDT.height()-(int)floor(m_vSilhMasks[i].height()*0.75+0.5);
        int minx = nStartX - nWinSize;
        int miny = nStartY - nWinSize;
        int maxx = nStartX + nWinSize;
        int maxy = nStartY + nWinSize;
        ChamferMatching::matchTemplateOnWindow( vCandidates, imgDT, 
                                                m_vSilhMasks[i], 
                                                m_vTemplates[i], i,
                                                minx, miny, maxx, maxy, 
                                                dScaleFact, nStepSize,
                                                nMaxCandidates );
      }
    }
    
    /* compute the Bhattacharya coefficients of the best-matching candidates */
    /* print out the results */
    cout << "    -----------------------------------" << endl;
    cout << "    Results of Chamfer verification:" << endl;
    CombiCandidateSet::iterator it;
    int i;
    float     dMaxBhatta = 0.0;
    CombiCandidate cMaxCand;
    vector<CombiCandidate> vResultCands;
    //float dAlpha = 0.5;
    CombiCandidateSet vCombCands;
    for( i=0, it=vCandidates.begin(); 
         i<nMaxCandidates && it!=vCandidates.end(); i++, it++ ) {
      //cout << "      " << i+1 << ". ";

      /* Bhatta computation */
      CombiCandidate cCur = (*it);
      OpGrayImage imgMask = m_vSilhMasks[cCur.getTemplateId()];
      int nScaledW = (int)floor( imgMask.width()*cCur.getScale() + 0.5 );
      imgMask = imgMask.opRescaleToWidth( nScaledW );
      double dNormSeg  = sqrt( 1.0/imgSegCutOut.getSum() ); 
      double dNormMask = sqrt( 1.0/imgMask.getSum() );
      double dBhatta = compBhattaMask( imgSegCutOut, imgMask, 
                                       cCur.getPos().x(), cCur.getPos().y() );
      double dBhattaNorm = dBhatta*dNormSeg*dNormMask;
      //cout << "        (Bhatta coeff=" << dBhattaNorm << ")" << endl;

      cCur.getBhScore()   = dBhattaNorm;
      cCur.getCombScore() = ( m_parVeri.params()->m_dVerChAlpha*2.0*
                              (50.0-cCur.getDist()) + 
                              (1.0-m_parVeri.params()->m_dVerChAlpha)*100.0*
                              cCur.getBhScore() );
      //cCur.print();
      vResultCands.push_back( cCur );
      vCombCands.insert( cCur );
      
      if( dBhattaNorm > dMaxBhatta ) {
        dMaxBhatta = dBhattaNorm;
        cMaxCand = cCur;
      }

    }

    /* print the 5 best hypotheses */
    for( i=0, it=vCombCands.begin(); 
         i<5 && it!=vCombCands.end(); i++, it++ ) {
      cout << "      " << i+1 << ". ";
      it->print();
    }
    cout << "    -----------------------------------" << endl;

    /* display the best-matching templates */
    //QImage qimgResult = drawImageResult( vCandidates, m_vSilhouettes, 
    //                                     nMaxCandidates, imgDT );
    QImage qimgResult = ChamferMatching::drawImageResult( vResultCands, 
                                                          m_vTemplates, 
                                                          nMaxCandidates, 
                                                          imgDT );
    vResultQImgs.push_back( qimgResult );
    
    /* set the best-matching score for the hypothesis */
    //Candidate cBest = (*vCandidates.begin());
    //Candidate cBest = cMaxCand;
    CombiCandidate cBest = (*vCombCands.begin());
    hypoInit.dScore = cBest.getCombScore(); //-cBest.dDist;
    hypoInit.dScoreMDL = dMaxBhatta;

    Hypothesis hypo = vInitHypos[k];
    //hypo.x      = nOffsetX + (int)floor(cBest.qrPos.x()*dVerifyScale + 0.5);
    //hypo.y      = nOffsetY + (int)floor(cBest.qrPos.y()*dVerifyScale + 0.5);
    hypo.nBoxX1 = nOffsetX + (int)floor(cBest.getPos().x()*dVerifyScale + 0.5);
    hypo.nBoxY1 = nOffsetY + (int)floor(cBest.getPos().y()*dVerifyScale + 0.5);
    hypo.nBoxWidth  = (int)floor(cBest.getPos().width()*dVerifyScale + 0.5);
    hypo.nBoxHeight = (int)floor(cBest.getPos().height()*dVerifyScale + 0.5);
    hypo.dScale = cBest.getScale()*dHypoScale; //dVerifyScale;
    hypo.dScore = cBest.getCombScore(); //-cBest.dDist;
    //hypo.dScoreMDL = dMaxBhatta;

    /* compute MDL scores over template area */
    OpGrayImage imgScaledMask = m_vSilhMasks[cBest.getTemplateId()];
    int nMaskW = (int)floor(imgScaledMask.width()*hypo.dScale/dTemplateScale+
                            0.5);
    imgScaledMask = imgScaledMask.opRescaleToWidth( nMaskW );
    float dSumPFig = 0.0;
    float dFigArea = 0.0;
    OpGrayImage imgPFig = vInitImgPFig[k];
    OpGrayImage imgPGnd = vInitImgPGnd[k];
    int minx = max( 0, min( hypo.nBoxX1, imgPFig.width()-1 ) );
    int miny = max( 0, min( hypo.nBoxY1, imgPFig.height()-1 ) );
    int maxx = max( 0, min( hypo.nBoxX1+hypo.nBoxWidth, imgPFig.width()-1 ) );
    int maxy = max( 0, min( hypo.nBoxY1+hypo.nBoxHeight, imgPFig.height()-1 ));
    for( int y=miny, yy=0; y<maxy && yy<imgScaledMask.height(); y++, yy++ )
      for( int x=minx, xx=0; x<maxx && xx<imgScaledMask.width(); x++, xx++ )
        if( imgScaledMask(xx,yy).value() > 0.0 )
          if( imgPFig(x,y).value() > imgPGnd(x,y).value() ) {
            dSumPFig += imgPFig(x,y).value();
            dFigArea += 1.0;
          }

    //float dPFigScale = cBest.dScale*dHypoScale;
    float dMinMDLScale = m_parReco.params()->m_dAdaptMinMDLScale;
    if( m_parReco.params()->m_bNormScalePFig2 ) {
      if( hypo.dScale > dMinMDLScale ) {
        dSumPFig /= (hypo.dScale*hypo.dScale);
        dFigArea /= (hypo.dScale*hypo.dScale);
      } else {
        dSumPFig /= (dMinMDLScale*dMinMDLScale);
        dFigArea /= (dMinMDLScale*dMinMDLScale);
      }
    }
    float dScoreMDL = ( m_parVeri.params()->m_dVerWeightPFig*dSumPFig + 
                        (1.0-m_parVeri.params()->m_dVerWeightPFig)*dFigArea );
    hypo.dScoreMDL = dScoreMDL;
    hypoInit.dScoreMDL = dScoreMDL;
    hypo.nTemplateId     = cBest.getTemplateId();
    hypoInit.nTemplateId = cBest.getTemplateId();

    vVerifiedHypos.push_back( hypoInit );
    vVerifiedHyposTight.push_back( hypo );
    //vVerifiedTemplates.push_back( m_vTemplates[cBest.nTemplateId] );
    vVerifiedTemplates.push_back( m_vTemplatesOrig[cBest.getTemplateId()] );
    vVerifiedTemplIds.push_back( cBest.getTemplateId() );
  }


  cout << "---------------------------------------" << endl;
  cout << "Hypotheses after Verification:" << endl;
  for( int k=0; k<(int)vVerifiedHyposTight.size(); k++ ) {
    cout << "  " << setw(2) << k+1 << ". ";
    printHypothesisMDL( vVerifiedHyposTight[k] );
  }
  cout << "---------------------------------------" << endl;
 
  vector<Hypothesis> vVerHypos;
  vector<Hypothesis> vVerHyposTight;
  vector<EdgePtVec>  vVerTemplates;
  vector<int>        vVerTemplIds;
  for( int k=0; k<(int)vVerifiedHypos.size(); k++ )
    if( vVerifiedHypos[k].dScore >= m_parVeri.params()->m_dVerChThresh )
      if( vVerifiedHypos[k].dScoreMDL >= m_parVeri.params()->m_dVerMinPFig ) {
        vVerHypos.push_back( vVerifiedHypos[k] );
        vVerHyposTight.push_back( vVerifiedHyposTight[k] );
        vVerTemplates.push_back( vVerifiedTemplates[k] );
        vVerTemplIds.push_back( vVerifiedTemplIds[k] );
      }
  vVerifiedHypos      = vVerHypos;
  vVerifiedHyposTight = vVerHyposTight;
  vVerifiedTemplates  = vVerTemplates;
  vVerifiedTemplIds   = vVerTemplIds;
}


double ISMReco::compBhattaMask( const OpGrayImage &img, 
                                  const OpGrayImage &mask, 
                                  int nShiftX, int nShiftY )
{
  int minx = max( 0, min( nShiftX, img.width()-1 ));
  int miny = max( 0, min( nShiftY, img.height()-1 ));
  int maxx = max( 0, min( nShiftX+mask.width()-1, img.width()-1 ));
  int maxy = max( 0, min( nShiftY+mask.height()-1, img.height()-1 ));
  double dRes = 0.0;
  for( int y=miny, yy=0; y<maxy; y++, yy++ )
    for( int x=minx, xx=0; x<maxx; x++, xx++ ) {
      dRes += sqrt( img(x,y).value() * mask(xx,yy).value());
    }
  return dRes;
}


