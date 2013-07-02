//
// C++ Implementation: chamferimage
//
// Description:
//
//
// Author: Edgar Seemann <edi@i5100>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "chamferimage.h"
#include <fileoperations.h>
#include <stringoperations.h>
#include <imageoperations.h>


ChamferImage::ChamferImage( const QString& name,
                            float tlow, float thigh, float sigma,
                            bool colorCanny, bool tpl )
{
  assert(FileOperations::fileExists(name));
  OpGrayImage img(name);

  _imagePyramid.push_back(img);

  for(int i=0; i<5; i++)
  {
    _edgePlanesPyramid.push_back(vector<OpGrayImage>());
    _dtPlanesPyramid.push_back(vector<OpGrayImage>());
    _iiPlanesPyramid.push_back(vector<IntegralImage>());
    _featurePlanesPyramid.push_back(vector<EdgePtVec>());
  }

  _isTemplate = tpl;
  
  _name=name;
  _tlow=tlow;
  _thigh=thigh;
  _sigma=sigma;
  _colorCanny=colorCanny;
}

ChamferImage::ChamferImage( const QImage& qimg, float tlow, float thigh,
                            float sigma, bool colorCanny, bool tpl)
{
  OpGrayImage img(qimg);

  _imagePyramid.push_back(img);

  for(int i=0; i<5; i++)
  {
    _edgePlanesPyramid.push_back(vector<OpGrayImage>());
    _dtPlanesPyramid.push_back(vector<OpGrayImage>());
    _iiPlanesPyramid.push_back(vector<IntegralImage>());
    _featurePlanesPyramid.push_back(vector<EdgePtVec>());
  }

  _isTemplate = tpl;
  
  _name="noName";
  _tlow=tlow;
  _thigh=thigh;
  _sigma=sigma;
  _colorCanny=colorCanny;

}

ChamferImage::ChamferImage(const OpGrayImage& img, bool tpl)
{
  _imagePyramid.push_back(img);

  for(int i=0; i<5; i++)
  {
    _edgePlanesPyramid.push_back(vector<OpGrayImage>());
    _dtPlanesPyramid.push_back(vector<OpGrayImage>());
    _iiPlanesPyramid.push_back(vector<IntegralImage>());
    _featurePlanesPyramid.push_back(vector<EdgePtVec>());
  }

  _isTemplate = tpl;
  _name = "nix";
  _tlow=0.6;
  _thigh=0.7;
  _sigma=2.0;
  _colorCanny=false;
}


void ChamferImage::calcMask()
{
  OpGrayImage& img = _imagePyramid[0];
  ImageOperations::addBorder(img, 5, 5);
  _mask = img.opAreaDistanceTransform();
  _mask.opThresholdAbove(1, 0, 255.0);
  _mask = _mask.opErodeBlockMask(3,255.0, 0.0);
}


OpGrayImage& ChamferImage::getEdges(unsigned int level, unsigned int plane)
{
  //-- how many pyramid levels are there for plane? --//
  unsigned int size=_edgePlanesPyramid[plane].size();

  //-- no pyramid levels at all? --//
  if (size==0)
  {
    if (plane==0 && _isTemplate)
    { //edge features already known
      _edgePlanesPyramid[0].push_back(_imagePyramid[0]);
      _dimensionPyramid.push_back(QSize( _imagePyramid[0].width(),
                                         _imagePyramid[0].height()) );

    }
    else if (plane==0)
    { //compute edge features
      OpGrayImage edges;
      ImageOperations::canny(_imagePyramid[0], edges, _tlow, _thigh, _sigma);
      _edgePlanesPyramid[0].push_back(edges);
      _dimensionPyramid.push_back(QSize(edges.width(), edges.height()));

    }
    else
    { //compute edge planes
      vector<OpGrayImage> edgePlanes =
        ImageOperations::getEdgesByOrientation( _imagePyramid[0], _sigma,
                                                _tlow, _thigh);
      _edgePlanesPyramid[1].push_back(edgePlanes[0]);
      _edgePlanesPyramid[2].push_back(edgePlanes[1]);
      _edgePlanesPyramid[3].push_back(edgePlanes[2]);
      _edgePlanesPyramid[4].push_back(edgePlanes[3]);
      //_dimensionPyramid.push_back(QSize( edgePlanes[0].width(),
      //                                   edgePlanes[0].height()) );
    }
    size++;
  }

  //-- get the missing pyramid levels --//
  if (size<=level)
  {
    OpGrayImage tmp;
    for (unsigned int i= size; i<=level; i++)
    {
      ImageOperations::featureScale(_edgePlanesPyramid[plane][i-1], tmp);
      _edgePlanesPyramid[plane].push_back(tmp);
      if (_dimensionPyramid.size()<_edgePlanesPyramid[plane].size())
        _dimensionPyramid.push_back(QSize(tmp.width(), tmp.height()));
    }
  }
  return _edgePlanesPyramid[plane][level];
}


QSize& ChamferImage::getDimensions(unsigned int level)
{
  unsigned int size=_dimensionPyramid.size();
  if (size<=level)
  {
    getEdges(level);
  }
  return _dimensionPyramid[level];
}


OpGrayImage& ChamferImage::getDT(unsigned int level, unsigned int plane)
{
  unsigned int size=_dtPlanesPyramid[plane].size();
  if (size<=level)
  {
    OpGrayImage tmp;
    for (unsigned int i= size; i<=level; i++)
    {
      ImageOperations::distanceTransform(getEdges(i, plane), tmp);
      _dtPlanesPyramid[plane].push_back(tmp);
    }
  }
  return _dtPlanesPyramid[plane][level];
}


IntegralImage& ChamferImage::getIntegralImage( unsigned int level,
    unsigned int plane )
{
  unsigned int size=_iiPlanesPyramid[plane].size();
  if (size<=level)
  {
    IntegralImage tmp;
    for (unsigned int i= size; i<=level; i++)
    {
      tmp.computeFromQImage(getDT(i,plane));
      _iiPlanesPyramid[plane].push_back(tmp);
    }
  }
  return _iiPlanesPyramid[plane][level];
}


EdgePtVec& ChamferImage::getFeatures(unsigned int level, unsigned int plane)
{
  unsigned int size=_featurePlanesPyramid[plane].size();
  if (size<=level)
  {
    EdgePtVec tmp;
    for (unsigned int i= size; i<=level; i++)
    {
      _featurePlanesPyramid[plane].push_back(calcFeaturePoints(i,plane));
    }
  }
  return _featurePlanesPyramid[plane][level];
}


EdgePtVec ChamferImage::calcFeaturePoints( unsigned int level,
    unsigned int plane)
{
  OpGrayImage& edges = getEdges(level,plane);
  EdgePtVec f;
  for (int x=0; x<edges.width(); x++)
    for (int y=0; y<edges.height(); y++)
      if ( edges(x, y).value()==0 )
        f.push_back(QPoint(x,y));
  return f;
}


OpGrayImage& ChamferImage::getMask()
{
  calcMask();
  return _mask;
}

/*
void ChamferMatching::getFeaturePoints(const OpGrayImage& templ, const OpGrayImage& mask, vector<QPoint>& points)
{
    for (int x=0; x<templ.width(); x++)
        for (int y=0; y<templ.height(); y++)
            if ( templ(x, y).value()==0 && mask(x,y).value()==0 )
                points.push_back(QPoint(x,y));
}
*/
