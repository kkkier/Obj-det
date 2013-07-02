//
// C++ Interface: chamferimage
//
// Description: 
//
//
// Author: Edgar Seemann <edi@i5100>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHAMFERIMAGE_H
#define CHAMFERIMAGE_H

//#include <eimagebrowser.h>
#include <integralimage.h>
#include <opgrayimage.hh>
#include <imageoperations.h>

typedef vector<QPoint> EdgePtVec;

class ChamferImage
{

private:
    QString _name;
    bool _isTemplate;
    
    // -- canny params -- //
    float _tlow;
    float _thigh;
    float _sigma;
    float _colorCanny;
    
    OpGrayImage _mask;
    
    // -- Pyramids -- //
    vector<QSize>       _dimensionPyramid;
    vector<OpGrayImage> _imagePyramid;
    
    //vector<OpGrayImage>   _edgesPyramid;
    //vector<OpGrayImage>   _dtPyramid;
    //vector<IntegralImage> _iiPyramid;
    //vector<Features>      _featuresPyramid;
    
    vector<vector<OpGrayImage> >   _edgePlanesPyramid;
    vector<vector<OpGrayImage> >   _dtPlanesPyramid;
    vector<vector<IntegralImage> > _iiPlanesPyramid;
    vector<vector<EdgePtVec> >     _featurePlanesPyramid;
    
private:
    EdgePtVec calcFeaturePoints( unsigned int level, unsigned int plane );
    void calcMask();
    
public:
    ChamferImage( const QString& name, 
                  float tlow=0.6, float thigh=0.7, 
                  float sigma=2.0, bool colorCanny=false, bool tpl=false);
    ChamferImage( const QImage& img, float tlow=0.6, float thigh=0.7, 
                  float sigma=2.0, bool colorCanny=false, bool tpl=false);
    ChamferImage( const OpGrayImage& img, bool tpl=true );
	
    inline bool isTemplate() const { return _isTemplate; };
    
    OpGrayImage&   getMask();
    QSize&         getDimensions   ( unsigned int level );
    
    OpGrayImage&   getEdges        ( unsigned int level, unsigned int plane=0);
    OpGrayImage&   getDT           ( unsigned int level, unsigned int plane=0);
    IntegralImage& getIntegralImage( unsigned int level, unsigned int plane=0);
    EdgePtVec&     getFeatures     ( unsigned int level, unsigned int plane=0);
    
    inline int     width()   const { return _imagePyramid[0].width(); }
    inline int     height()  const { return _imagePyramid[0].height(); }
    inline QString getName() const { return _name; }
    inline OpGrayImage& getImage() { return _imagePyramid.front(); }
};
#endif
