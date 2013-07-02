#ifndef IMAGEOPERATIONS_H
#define IMAGEOPERATIONS_H

using namespace std;

#include <qimage.h>
#include <qpoint.h>
#include <opgrayimage.hh>
#include <oprgbimage.hh>


class ImageOperations
{
public:
    static QImage stdGauss   ( const QImage &image );

    static void subsample    ( const QImage &image, QImage &ret,
                               bool resize = true );

    static void featureScale  ( const OpGrayImage& image, OpGrayImage& ret );
    static void featureScale  ( const QImage& image, QImage& ret );

    static void edgeSobel     ( const QImage& qimg, QImage& qimgRet,
                                int thresh );
    static void canny         ( const OpGrayImage& image, OpGrayImage& ret,
                                float lowThresh=0.5, float highThresh=0.8,
                                float gauss=1.5 );
    static void canny         ( const QImage& qimg, OpGrayImage& ret,
                                float lowThresh=0.5, float highThresh=0.8,
                                float gauss=1.5, bool color=false );
    static void edgeCanny     ( const OpGrayImage& image, OpGrayImage& ret,
                                float lowThresh=0.5, float highThresh=0.8,
                                float gauss=1.5 );
    static void edgeCanny     ( const QImage& image, QImage& ret,
                                float lowThresh=0.5, float highThresh=0.8,
                                float gauss=1.5 );

    static void colorCanny    ( const OpRGBImage& img, OpGrayImage& ret,
                                float lowThresh, float highThresh, float gauss );
    static void colorCanny    ( const QImage& img, QImage& ret,
                                float lowThresh, float highThresh, float gauss );

    static void distanceTransform  ( const OpGrayImage& image,
                                     OpGrayImage& ret );
    static void connectedComponents( const OpGrayImage&,
                                     OpGrayImage& ret,
                                     int minSize=1, int maxColors=10000 );

    static void drawMaskOnImage    ( const QImage& qimgTempl, QImage& qimg,
                                     int pos_x, int pos_y, QRgb color );
    static void drawMaskOnImage    ( const OpGrayImage &imgTempl, QImage& qimg,
                                     int pos_x, int pos_y, QRgb color );

    static void addBorder          ( OpGrayImage& img,
                                     int width=5, int height=5,
                                     float color=255.0 );

    static vector<OpGrayImage> getEdgesByOrientation( const OpGrayImage& img,
            float sigma,
            float tlow, float thigh );

    static void opgrayimages2qimages( const vector<OpGrayImage>& src,
                                      vector<QImage>& dst );

    static vector<OpGrayImage> split2x2(const OpGrayImage& img);
	static OpGrayImage join2x2(const vector<OpGrayImage>& vPatches);
    
    static vector<QPoint> sampleFromPoints(const vector<QPoint>& inPoints, int pixelDist);
};

#endif
