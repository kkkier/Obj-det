#ifndef CHAMFERMATCHING_H
#define CHAMFERMATCHING_H

//#include <eimagebrowser.h>
#include <opgrayimage.hh>
#include <qimage.h>
#include <vector>
#include <set>
#include <list>
#include <imgdescrlist.hh>
#include <integralimage.h>

#include "Candidate.h"
#include "chamferimage.h"

#define HIER_ELMT_SEPARATOR ":"
#define MAX_MATCHING_DIST 1e10

using namespace std;

const int NO_PLANES =4 ;

///////////////////////////////////////////////////////////////////////////////
//
// ChamferMatching class
//
//
//

class ChamferMatching
{
public:

    ///////////////////////////////////////////////////////////////////////////
    //
    // static
    //

    //-- feature related --//
    static double distanceOnFeatures     ( const OpGrayImage& dt,
                                           const EdgePtVec& vPoints,
                                           int shift_x, int shift_y,
                                           int& nCount );
    static double distanceOnFeatures     ( const OpGrayImage &imgDt,
                                           const EdgePtVec   &vPoints,
                                           int shift_x, int shift_y,
                                           float dScaleFact,
                                           int &nCount);

    static double calcDistanceAtPosition ( int x, int y,
                                           const OpGrayImage &dt,
                                           const EdgePtVec   &vPoints );
    static double calcDistanceAtPosition ( int x, int y, float dScaleFact,
                                           const OpGrayImage &dt,
                                           const EdgePtVec   &vPoints );
    double        calcDistanceAtPosition ( int x, int y, const QSize& dim,
                                           const IntegralImage& iimg,
                                           const OpGrayImage&   imgDt,
                                           const EdgePtVec&     vPoints );
    static double normalizedDistance    ( ChamferImage& patch1, ChamferImage& patch2);
    static double normalizedSymDistance ( ChamferImage& patch1, ChamferImage& patch2);
    static void   matchTemplateOnWindow ( CombiCandidateSet &vCandidates,
                                          const OpGrayImage &imgDt,
                                          const OpGrayImage &imgTempl,
                                          const EdgePtVec   &vPoints,
                                          int nTemplateId,
                                          int x, int y, int w, int h,
                                          int nMaxCandidates );
    static void   matchTemplateOnWindow ( CombiCandidateSet &vCandidates,
                                          const OpGrayImage &imgDt,
                                          const OpGrayImage &imgTempl,
                                          const EdgePtVec   &vPoints,
                                          int nTemplateId,
                                          int x, int y, int w, int h,
                                          float dScaleFact,
                                          int nStepSize,
                                          int nMaxCandidates );
    void          matchTemplateOnWindow ( CandidateSet&        vCandidates,
                                          const OpGrayImage&   imgEdges,
                                          const OpGrayImage&   imgDt,
                                          const IntegralImage& iimg,
                                          const OpGrayImage&   imgTempl,
                                          const EdgePtVec&     vPoints,
                                          int x, int y, int w, int h );


    static void   getFeaturePoints       ( const OpGrayImage& templ,
                                           EdgePtVec& vPoints );
    static void   getFeaturePoints       ( const OpGrayImage& templ,
                                           const OpGrayImage& mask,
                                           EdgePtVec& vPoints );

    static void   getBlackPoints         ( const OpGrayImage& img,
                                           EdgePtVec& vPoints,
                                           const OpGrayImage* mask=0 );
    static void   getDarkPoints          ( const OpGrayImage& img,
                                           EdgePtVec& vPoints,
                                           const OpGrayImage* mask=0 );

 public:
    static EdgePtVec   prepareTemplate   ( const OpGrayImage &imgTempl );
    static OpGrayImage getDTImage        ( const OpGrayImage &img,
                                           float dThreshLo=0.4,
                                           float dThreshHi=0.8 );

 public:
    //-- drawing --//
    static void   drawDT                    ( const OpGrayImage& imgDT,
            QImage& ret );
    static QImage drawDT                    ( const OpGrayImage& imgDt );

    static void   drawEdgesOnDT             ( const OpGrayImage& edges,
            QImage& dt );

    static void   drawTemplateOnImage       ( const OpGrayImage& templ,
            QImage& image,
            int x, int y, QRgb color );
    static void   drawTemplateOnImage       ( const OpGrayImage& imgTempl,
            QImage& qimg,
            int pos_x, int pos_y,
            float dScale, QRgb color );
    static void   drawTemplateOnImage       ( const EdgePtVec &vTemplPoints,
            QImage& qimg,
            int pos_x, int pos_y,
            float dScale, QRgb color );

    QImage         drawImageResult  ( unsigned int imgNo,
                                      unsigned int hypothesisNo ) const;
    static QImage  drawImageResult  ( CombiCandidateSet         &vCandidates,
                                      const vector<OpGrayImage> &vSilhouettes,
                                      int nNumHypos,
                                      const OpGrayImage imgDt );
    static QImage  drawImageResult  ( CombiCandidateSet       &vCandidates,
                                      const vector<EdgePtVec> &vvTemplPoints,
                                      int nNumHypos,
                                      const OpGrayImage imgDt );
    static QImage drawImageResult   ( vector<CombiCandidate>  &vCandidates,
                                      const vector<EdgePtVec> &vvTemplPoints,
                                      int nNumHypos,
                                      const OpGrayImage imgDT );


    //-- other --//
    static void     cleanCandidates         ( CandidateSet& candidates,
            float thresh, float cover);

    ///////////////////////////////////////////////////////////////////////////
    //
    // non-static
    //
    void   clear();

    //-- Matching --//
    void   doCoarseToFineMatching( CandidateSet& candidates,
                                   ChamferImage& image );
    void   coarseToFineMatching  ( CandidateSet& candidates,
                                   ChamferImage& img,
                                   ChamferImage& tpl,
                                   int levels, int x, int y, int w, int h );

    //void   hierarchicalMatching ( CandidateSet& candidates,
    //                              const QImage& image,
    //                              const QString baseDir,
    //                              const QStringList& templateHierarchy );

    //-- Verification --//
    double symmetryCheck        ( const OpGrayImage& edges,
                                  unsigned int templateIndex,
                                  int x, int y );
    void   symmetryCheck        ( const CandidateSet& cand,
                                  CandidateSet& ret,
                                  const OpGrayImage& edges );




public:
    ChamferMatching();

    //-- preparation --//
    void   setOptions           ( int levels=4,
                                  bool symmetryCheck=false,
                                  bool edgeNormalization=false,
                                  float candidateThresh=100,
                                  float coverThresh=0.8,
                                  float thresh1=0.7,
                                  float thresh2=0.8,
                                  float gauss=1.5,
                                  bool colorCanny=false,
                                  bool planes=false );
    void   setTemplates         ( const QStringList& tplNames );
    void   setImages            ( const QStringList& tplNames );
    void   setCodebook          ( const vector<OpGrayImage> codebook, bool orientationPlanes=false );


    //-- Matching --//
    void   matchIDLFile           ( const QString& idlFile );
    void   matchImage(const QImage& img);
    void   matchImages();
    void   matchImagesByEdgePlanes();

    double matchTemplateAtPosition( int x, int y );

    vector< vector<float> > getDistanceMatrixOfTemplates();
    static void          getDistanceMatrix(vector<float>& sims, 
                                           const vector<OpGrayImage>& vPatches,
                                           bool bOriPlanes=false);
    static vector<float> getDistanceMatrix(const vector<OpGrayImage>& vTempl,
                                           bool bOriPlanes=false);
    vector<float>        getDistancesToCodebook( const OpGrayImage& img, 
                                                 bool bOriPlanes=false, 
                                                 bool bSymDist=true );
    static float         getDistance      ( const OpGrayImage& img1,
                                            const OpGrayImage& img2 );

    //void doHierarchyMatching( const QImage& image, QImage& ret,
    //                          QStringList tempHier,
    //                          QString stDir, double dist[4],
    //                          int boundBox[3][2], Rect* Result,
    //                          QStringList *activated);

    //-- Results --//
    void           saveResults    ( const QString& filename ) const;
    Candidate      getCandidate   ( unsigned int imgNo,
                                    unsigned int hypothesisNo ) const;

    vector<QImage> drawResults      ( unsigned int noOfHypotheses );

    vector<QPoint> getContourPointsOfCandidates(int pixelDist, unsigned noOfHypotheses);
    
    //-- Old Stuff --//
    //static QRgb   getColumnColor            ( int col );
    //static void   addOccurenceToOccurenceBox( int x, int y, QRect& box );

private:
    //EImageBrowser* browser;

    //-- member varibles --//
    int   _levels;
    float _cannyLow;
    float _cannyHigh;
    float _sigma;
    bool  _colorCanny;
    float _candidateThresh;
    float _coverThresh;
    bool  _edgeNormalization;
    bool  _symmetryCheck;
    bool  _planes;

    //-- images --//
    vector<ChamferImage> _images;
    vector<ChamferImage> _templates;

    vector<ChamferImage> m_vCodebook;
    vector<OpGrayImage>  _codebookDTs;//should be removed
    vector<EdgePtVec>    _codebookFeatures;//should be removed

    //-- names --//
    QStringList  _imageNames;
    QStringList  _templateNames;
    ImgDescrList _descrList;

    //-- results --//
    vector<CandidateSet> _candidateSets;

    QString      _currentTemplateName;
    unsigned int _currentTemplateIndex;
};

#endif //CHAMFERMATCHING_H
