
#include <opgrayimage.hh>
#include <gaussfilterbank.hh>
#include <orientationenergy.hh>
//#include <eimagebrowser.h>
#include <qtimgbrowser.hh>

#include "imageoperations.h"
#include <math.h>
#include <cassert>

//===========================================================================//
// void ImageOperations::distanceTransform(QImage image, QImage* ret)        //
//===========================================================================//
// INPUTS:    image: QImage to be transformed. Typically containing feature  //
//                   (greylevel=0) and non-feature (greylevel=255) points.   //
// OUTPUTS:   QImage: Chamfer distance transform of the input image.         //
// OPERATION: Creates a distance matrix which contains greylevel values      //
//            from the passed image. A one-pixel, non-feature border is      //
//            added. In a forward and backward pass masks are slided over    //
//            this matrix locally computing the chamfer distance of each     //
//            point to the nearest feature point. The values in the masks    //
//            are taken from Borgefors. The return image contains a visuali- //
//            sation of the distances.                                       //
//===========================================================================//
void ImageOperations::distanceTransform( const OpGrayImage& img,
    OpGrayImage& imgResult )
{
  double dist[img.width()+2][img.height()+2];           //
  double a = 0.95509;                                       //
  double b = 1.36930;                                       //
  double max_dist = 255;                                    //

  double fwd_mask[3][3] = {{ b,        a,        b       }, // Local distance
                           { a,        max_dist, max_dist}, //  maps for
                           { max_dist, max_dist, max_dist}};//  forward and
  double bwd_mask[3][3] = {{ max_dist, max_dist, max_dist}, //  backward pass.
                           { max_dist, max_dist, a       }, //
                           { b,        a,        b       }};//

  // init dist matrix //
  for (int x=0; x<img.width()+2; x++)
  {
    dist[x][0] = max_dist;
    dist[x][img.height()+1] = max_dist;
  }
  for (int y=0; y<img.height()+2; y++)
  {
    dist[0][y] = max_dist;
    dist[img.width()+1][y] = max_dist;
  }

  // init distance matrix with image features //
  for (int y=0; y<img.height(); y++)                  // Initialize
    for (int x=0; x<img.width(); x++)
    {             //  distance matrix
      dist[x+1][y+1] = img(x,y).value();          //  with features
    }                                                 //  from image.

  // Forward pass //
  for (int x=1; x<img.width()+1; x++)
    for (int y=1; y<img.height()+1; y++)
      for (int y_m=-1; y_m<=1; y_m++)
        for (int x_m=-1; x_m<=1; x_m++)
          dist[x][y] = min( dist[x][y],
                            dist[x+x_m][y+y_m] + fwd_mask[x_m+1][y_m+1] );

  // Backward pass //
  for (int x=img.width(); x>0; x--)
    for (int y=img.height(); y>0; y--)
      for (int y_m=-1; y_m<=1; y_m++)
        for (int x_m=-1; x_m<=1; x_m++)
          dist[x][y] = min( dist[x][y],
                            dist[x+x_m][y+y_m] + bwd_mask[x_m+1][y_m+1] );

  // create and fill return image //
  OpGrayImage imgDt(img.width(), img.height());
  for (int y=1; y<=img.height(); y++)                 // Determine
    for (int x=1; x<=img.width(); x++)
    {              //  color of each
      float col_r = int(15*dist[x][y]);                 //  pixel depending

      imgDt(x-1,y-1) = col_r;
    }

  imgResult = imgDt;
}


//===========================================================================//
// QImage ImageOperations::stdGauss(QImage image)                            //
//===========================================================================//
// INPUTS:    image: QImage to be filtered.                                  //
// OUTPUTS:   QImage: Filtered image.                                        //
// OPERATION: Passes a Gauss filter with mean 0 and variance 1 over the      //
//            image. Decompositions in x and y direction of a 5x5 matrix     //
//            approximating the Gaussian distribution are used during the    //
//            two passes of computing the weigthed average for the three     //
//            colour channels R, G and B.                                    //
//===========================================================================//
QImage ImageOperations::stdGauss( const QImage& qimg )
{
  QImage qimgFiltered = qimg.copy();
  double d_gauss[5] = {17, 66, 107, 66, 17};                // 1D Gauss weights
  for (int y=0; y<qimgFiltered.height(); y++)
  {
    for (int x=0; x<qimgFiltered.width(); x++)
    {
      double col_r = 0, col_b = 0, col_g = 0;
      double sum = 0;
      for (int i=-2; i<2; i++)
      {                            // Horizontal pass
        if ((x+i>=0) && (x+i<qimgFiltered.width()))
        {
          col_r += qRed  (qimg.pixel(x+i,y))*d_gauss[i+2];
          col_g += qGreen(qimg.pixel(x+i,y))*d_gauss[i+2];
          col_b += qBlue (qimg.pixel(x+i,y))*d_gauss[i+2];
          sum += d_gauss[i+2];
        }
      }
      col_r /= sum;                                         // Normalise to
      col_g /= sum;                                         //  consider
      col_b /= sum;                                         //  image borders
      qimgFiltered.setPixel(x, y, qRgb(int(col_r), int(col_g), int(col_b)));
    }
  }

  for (int x=0; x<qimgFiltered.width(); x++)
  {              // Vertical Pass
    for (int y=0; y<qimgFiltered.height(); y++)
    {
      double col_r = 0, col_b = 0, col_g = 0;
      double sum = 0;
      for (int i=-2; i<2; i++)
      {
        if ((y+i>=0) && (y+i<qimgFiltered.height()))
        {
          col_r += qRed  ( qimg.pixel(x,y+i))*d_gauss[i+2];
          col_g += qGreen( qimg.pixel(x,y+i))*d_gauss[i+2];
          col_b += qBlue ( qimg.pixel(x,y+i))*d_gauss[i+2];
          sum += d_gauss[i+2];
        }
      }
      col_r /= sum;
      col_g /= sum;
      col_b /= sum;
      int fil_r = qRed  ( qimgFiltered.pixel(x,y) );
      int fil_g = qGreen( qimgFiltered.pixel(x,y) );
      int fil_b = qBlue ( qimgFiltered.pixel(x,y) );
      qimgFiltered.setPixel(x, y,                            // Average with
                            qRgb(int(0.5*fil_r + 0.5*col_r),  //  horizontal
                                 int(0.5*fil_g + 0.5*col_g),  //  pass.
                                 int(0.5*fil_b + 0.5*col_b)));
    }
  }

  return qimgFiltered;
}


//===========================================================================//
// void ImageOperations::subsample(QImage image, QImage* ret,                //
//                                 bool resize = true)                       //
//===========================================================================//
// INPUTS:    image: QImage to be sub-sampled by factor 2.                   //
//            resize: flag to indicate whether the size of the image should  //
//                    be decreased by factor 2 as part of sub-sampling or    //
//                    pixel values duplicated to maintain the original size. //
// OUTPUTS:   QImage: Sub-sampled image of either equal or half the original //
//                    size.                                                  //
// OPERATION: The average of a pixel and its right, lower and lower-right    //
//            neighbours is computed and used as value for the pixel(s) in   //
//            the sub-sampled image.                                         //
//===========================================================================//
void ImageOperations::subsample( const QImage& qimg, QImage& qimgRet,
                                 bool bResize)
{
  int subs_w, subs_h;
  if( bResize )
  {                                          // Compute the
    subs_w = int(qimg.width()/2);                          //  size of the
    subs_h = int(qimg.height()/2);                         //  output image.

  }
  else
  {
    subs_w = qimg.width();
    subs_h = qimg.height();
  }
  qimgRet.create(subs_w, subs_h, 32);

  int col_r, col_g, col_b;
  for (int y=0; y<qimg.height(); y+=2)
  {                  // Traverse image
    for (int x=0; x<qimg.width(); x+=2)
    {                 //  and compute
      col_r = qRed(qimg.pixel(x,y));                      //  average of
      col_g = qGreen(qimg.pixel(x,y));                    //  the pixel and
      col_b = qBlue(qimg.pixel(x,y));                     //  its neighbours
      int sum = 1;                                        //  to the right,
      if (x < qimg.width()-1)
      {                           //  the bottom as
        col_r += qRed  (qimg.pixel(x+1, y));              //  well as the
        col_g += qGreen(qimg.pixel(x+1,y));               //  bottom-right.
        col_b += qBlue (qimg.pixel(x+1,y));               //  Consider image
        sum++;                                            //  borders.
      }

      if (y < qimg.height()-1)
      {
        col_r += qRed  (qimg.pixel(x,y+1));
        col_g += qGreen(qimg.pixel(x,y+1));
        col_b += qBlue (qimg.pixel(x,y+1));
        sum++;
      }

      if ((x<qimg.width()-1) && (y<qimg.height()-1))
      {
        col_r += qRed  (qimg.pixel(x+1, y+1));
        col_g += qGreen(qimg.pixel(x+1, y+1));
        col_b += qBlue (qimg.pixel(x+1, y+1));
        sum++;
      }
      col_r /= sum;
      col_g /= sum;
      col_b /= sum;
      if (bResize)
      {                                       // Set the output
        if ((int(x/2) < subs_w) && (int(y/2) < subs_h))
        {  //  image's pixels
          qimgRet.setPixel(int(x/2), int(y/2),             //  to the computed
                           qRgb(col_r, col_g, col_b));     //  values.
        }

      }
      else
      {                                             // In the case that
        qimgRet.setPixel(x,   y,   qRgb(col_r, col_g,      //  the image is
                                        col_b));           //  not being re-
        if (x < qimg.width()-1)
        {                          //  sized, four
          qimgRet.setPixel(x+1, y,   qRgb(col_r, col_g,    //  pixels hold
                                          col_b));         //  the same value
        }                                                  //  (with the
        if (y < qimg.height()-1)
        {                         //  exception of
          qimgRet.setPixel(x,   y+1, qRgb(col_r, col_g,    //  the borders).
                                          col_b));
        }
        if ((x<qimg.width()-1) && (y<qimg.height()-1))
        {
          qimgRet.setPixel(x+1, y+1, qRgb(col_r, col_g,
                                          col_b));
        }
      }
    }
  }
}


//===========================================================================//
// void ImageOperations::featureScale(QImage image, QImage* ret)             //
//===========================================================================//
// INPUTS:    image: QImage to be down-scaled by factor 2 while retaining    //
//            pixels denoting features (RGB = 000).                          //
// OUTPUTS:   QImage: down-scaled version of the input image.                //
// OPERATION: If in the input image for a given location either the pixel at //
//            that location or its right, bottom or bottom-right neighbour   //
//            contains a feature point, the corresponding output pixel will  //
//            also contain a feature point.                                  //
//===========================================================================//
void ImageOperations::featureScale( const QImage& qimg, QImage& qimgRet )
{
  OpGrayImage img   ( qimg );
  OpGrayImage imgRet( qimgRet );
  featureScale( img, imgRet );

  qimgRet = imgRet.getQtImage();
}


void ImageOperations::featureScale( const OpGrayImage& img,
                                    OpGrayImage& imgRet )
{

  QImage qimg = img.getQtImage();

  QImage qimgRet;
  qimgRet.create(qimg.width()/2, qimg.height()/2, 32);

  for( int y=0; y<qimg.height(); y+=2 )
  {
    for( int x=0; x<qimg.width(); x+=2 )
    {
      if( (int(x/2) < qimgRet.width()) && (int(y/2) < qimgRet.height()))
      {
        if( (qRed  (qimg.pixel(x,y)) == 0) &&      // If pixel in
            (qGreen(qimg.pixel(x,y)) == 0) &&      //  image contains feature
            (qBlue (qimg.pixel(x,y)) == 0))
        {      //  set in scaled
          qimgRet.setPixel(int(x/2), int(y/2), qRgb(0,0,0));

        }
        else
        {
          if ((x < qimg.width()-1) &&                // Else, if pixel
              ((qRed  (qimg.pixel(x+1,y)) == 0) &&   //  to the right
               (qGreen(qimg.pixel(x+1,y)) == 0) &&   //  contains feature
               (qBlue (qimg.pixel(x+1,y)) == 0)))
          {  //  set in scaled.
            qimgRet.setPixel(int(x/2), int(y/2), qRgb(0,0,0));

          }
          else
          {
            if( (y < qimg.height()-1) &&                    // Else if pixel
                ((qRed  (qimg.pixel(x,y+1)) == 0) &&        //  below contains
                 (qGreen(qimg.pixel(x,y+1)) == 0) &&        //  feature, set
                 (qBlue (qimg.pixel(x,y+1)) == 0)))
            {       //  in scaled.
              qimgRet.setPixel(int(x/2), int(y/2), qRgb(0,0,0));

            }
            else
            {                                      // Else if pixel
              if( ((x < qimg.width()-1) &&                //  to the right
                   (y < qimg.height()-1)) &&              //  and below
                  ((qRed  (qimg.pixel(x,y+1)) == 0) &&    //  contains
                   (qGreen(qimg.pixel(x,y+1)) == 0) &&    //  feature, set
                   (qBlue (qimg.pixel(x,y+1)) == 0)))
              {   //  it in scaled.
                qimgRet.setPixel(int(x/2), int(y/2), qRgb(0,0,0));

              }
              else
              {                                    // Else set
                qimgRet.setPixel(int(x/2), int(y/2),      // non-feature in
                                 qRgb(255,255,255));      //  scaled.
              }
            }
          }
        }
      }
    }
  }

  OpGrayImage tmp(qimgRet);
  imgRet = tmp;
}


//===========================================================================//
// void ImageOperations::edgeSobel(QImage image, QImage* ret, int thresh)    //
//===========================================================================//
// INPUTS:    image: QImage whose edges are to be detected.                  //
//            thresh: integer value above which a gradient difference will   //
//              be classified as an edge.                                    //
// OUTPUTS:   QImage: image containing the detected edges as feature points  //
//              which are marked by a value of RGB = 000. All other regions  //
//              of the output image have the value 255/255/255.              //
// OPERATION: The approximation to the two Sobel convolution matrixes is     //
//            used which computes the gradient value of a pixel in a single  //
//            pass. Then the passed threshold is applied to those values and //
//            the output image generated accordingly.                        //
//===========================================================================//
void ImageOperations::edgeSobel( const QImage& qimg, QImage& qimgRet,
                                 int nThresh )
{
  qimgRet = qimg.copy();
  for( int y=0; y < qimg.height(); y++ )
  {                 // Traverse image
    for( int x=0; x < qimg.width(); x++ )
    {                //  and compute
      //  grayscale
      int val = 0;                                         //  gradient value
      if( (x>0) && (y>0) )                                 //  taking into
        val += qGray(qimg.pixel(x-1, y-1));                //  consideration
      if( y>0 )                                            //  the edges.
        val += 2*qGray(qimg.pixel(x, y-1));                //  (later the
      if( (x<qimg.width()-1) && (y>0) )                    //  edge values
        val += qGray(qimg.pixel(x+1, y-1));                //  will be over-
      if( (x>0) && (y<qimg.height()-1) )                   //  written by non-
        val -= qGray(qimg.pixel(x-1, y+1));                //  features).
      if( y<qimg.height()-1 )                              //
        val -= 2*qGray(qimg.pixel(x, y+1));                //
      if( (x<qimg.width()-1) && (y<qimg.height()-1) )      //
        val -= qGray(qimg.pixel(x+1, y+1));                //
      val = abs(val);                                      //

      int val2 = 0;
      if( (x<qimg.width()-1) && (y>0) )
        val2 += qGray(qimg.pixel(x+1, y-1));
      if( x<qimg.width()-1 )
        val2 += 2*qGray(qimg.pixel(x+1, y));
      if( (x<qimg.width()-1) && (y<qimg.height()-1) )
        val2 += qGray(qimg.pixel(x+1, y+1));
      if( (x>0) && (y>0) )
        val2 -= qGray(qimg.pixel(x-1, y-1));
      if( x>0 )
        val2 -= 2*qGray(qimg.pixel(x-1, y));
      if( (x>0) && (y<qimg.height()-1) )
        val2 -= qGray(qimg.pixel(x-1, y+1));
      val2 = abs(val2);

      val += val2;

      if (val > 255)
        val = 255;
      if (nThresh == 0)
      {                                   // If thresh=0,
        qimgRet.setPixel(x, y, qRgb(val, val, val));        //  output value.

      }
      else
      {                                              // Normal case:
        if( (val >= nThresh) &&                             // If Threshold
            (x != 0) && (x != qimg.width()-1) &&            //  surpassed and
            (y != 0) && (y != qimg.height()-1))
        {           //  not on the
          qimgRet.setPixel(x, y, qRgb(0, 0, 0));          //  border, set
          //  feature.
        }
        else
        {                                            // Else set non-
          qimgRet.setPixel(x, y, qRgb(255, 255, 255));      //  feature.
        }
      }
    }
  }
}


void ImageOperations::canny( const OpGrayImage& img,
                             OpGrayImage& imgRet,
                             float dThreshLow, float dThreshHigh,
                             float dSigma )
{
  edgeCanny ( img, imgRet, dThreshLow, dThreshHigh, dSigma );
}


void ImageOperations::canny( const QImage& qimg,
                             OpGrayImage& imgRet,
                             float dThreshLow, float dThreshHigh,
                             float dSigma, bool bColor )
{
  if( bColor )
    colorCanny( qimg, imgRet, dThreshLow, dThreshHigh, dSigma );
  else
    edgeCanny ( qimg, imgRet, dThreshLow, dThreshHigh, dSigma );
}


void ImageOperations::edgeCanny( const QImage& qimg,
                                 QImage& qimgRet,
                                 float dThreshLow, float dThreshHigh,
                                 float dSigma )
{
  OpGrayImage img   ( qimg );
  OpGrayImage imgRet( qimgRet );
  edgeCanny( img, imgRet, dThreshLow, dThreshHigh, dSigma );
  qimgRet = imgRet.getQtImage();
}


void ImageOperations::edgeCanny( const OpGrayImage& img,
                                 OpGrayImage& imgRet,
                                 float dThreshLow, float dThreshHigh,
                                 float dSigma )
{
  //printf("ImageOperations::canny() ...\n");
  imgRet = img.opCannyEdges( dSigma, dThreshLow, dThreshHigh );
}


void ImageOperations::colorCanny( const QImage& qimg,
                                  QImage& qimgRet,
                                  float dThreshLow, float dThreshHigh,
                                  float dSigma )
{
  OpRGBImage  img   ( qimg );
  OpGrayImage imgRet( qimgRet );
  colorCanny( img, imgRet, dThreshLow, dThreshHigh, dSigma );
  qimgRet = imgRet.getQtImage();
}


void ImageOperations::colorCanny( const OpRGBImage& img,
                                  OpGrayImage& imgRet,
                                  float dThreshLow, float dThreshHigh,
                                  float dSigma )
{
  //printf("ImageOperations::colorCanny() ...\n");
  QImage qimg = img.getQtImage();

  int w = qimg.width();
  int h = qimg.height();

  //- create a YCC color-opponent image from the input image -//
  OpGrayImage imgY ( w, h );
  OpGrayImage imgC1( w, h );
  OpGrayImage imgC2( w, h );

  for (int y=0; y < h; y++)
  {
    for (int x=0; x < w; x++)
    {
      // img is the original QImage //
      uint *p = (uint *) qimg.scanLine(y) + x;

      //-- convert rgb to YCC --//
      imgY(x,y)  = (1.0/3.0)*(qRed(*p) + qGreen(*p) + qBlue(*p));
      imgC1(x,y) = 2.0*(0.5*qRed(*p) - 0.5*qGreen(*p));
      imgC2(x,y) = 2.0*(0.5*qRed(*p) + 0.5*qGreen(*p) - qBlue(*p));
    }
  }
  //-- apply the filter --//
  OpGrayImage imgDx( w, h );
  OpGrayImage imgDy( w, h );

  //-- fast gauss --//
  OpGrayImage imgYDx( w, h );
  OpGrayImage imgYDy( w, h );
  imgY.opFastGaussDxDy( dSigma, imgYDx, imgYDy );

  OpGrayImage imgC1Dx( w, h );
  OpGrayImage imgC1Dy( w, h );
  imgC1.opFastGaussDxDy( dSigma, imgC1Dx, imgC1Dy );

  OpGrayImage imgC2Dx( w, h );
  OpGrayImage imgC2Dy( w, h );
  imgC2.opFastGaussDxDy( dSigma, imgC2Dx, imgC2Dy );

  for (int y=0; y < h; y++)
  {
    for (int x=0; x < w; x++)
    {
      imgDx(x,y) = sqrt( imgYDx(x,y).value()*imgYDx(x,y).value() +
                         imgC1Dx(x,y).value()*imgC1Dx(x,y).value() +
                         imgC2Dx(x,y).value()*imgC2Dx(x,y).value() );
      imgDy(x,y) = sqrt( imgYDy(x,y).value()*imgYDy(x,y).value() +
                         imgC1Dy(x,y).value()*imgC1Dy(x,y).value() +
                         imgC2Dy(x,y).value()*imgC2Dy(x,y).value() );
    }
  }
  /*
  //-- slow gauss --//    
  OpGrayImage imgYDx  = imgY.opGaussDerivGx( m_dSigma );
  OpGrayImage imgYDy  = imgY.opGaussDerivGy( m_dSigma );
  OpGrayImage imgC1Dx = imgC1.opGaussDerivGx( m_dSigma );
  OpGrayImage imgC1Dy = imgC1.opGaussDerivGy( m_dSigma );
  OpGrayImage imgC2Dx = imgC2.opGaussDerivGx( m_dSigma );
  OpGrayImage imgC2Dy = imgC2.opGaussDerivGy( m_dSigma );

  for (int y=0; y < m_img.height(); y++)
  for (int x=0; x < m_img.width(); x++)
  {
  imgDx(x,y) = sqrt( imgYDx(x,y).value()*imgYDx(x,y).value() +
  imgC1Dx(x,y).value()*imgC1Dx(x,y).value() +
  imgC2Dx(x,y).value()*imgC2Dx(x,y).value() );
  imgDy(x,y) = sqrt( imgYDy(x,y).value()*imgYDy(x,y).value() +
  imgC1Dy(x,y).value()*imgC1Dy(x,y).value() +
  imgC2Dy(x,y).value()*imgC2Dy(x,y).value() );
  }
  */

  //-- apply the Canny operator --//
  OpGrayImage resultImg( w, h );
  imgRet = resultImg;
  imgRet = cannyEdgesDxDy( imgDx, imgDy, dThreshLow, dThreshHigh );
}


void ImageOperations::connectedComponents( const OpGrayImage& img,
    OpGrayImage&       imgRet,
    int minSize, int maxColors )
{
  imgRet = img.opConnectedComponents( minSize, maxColors );
}


void ImageOperations::drawMaskOnImage( const QImage& qimgTempl, QImage& qimg,
                                       int pos_x, int pos_y, QRgb color )
{
  int tpl_h = qimgTempl.height();
  int tpl_w = qimgTempl.width();
  int img_h = qimg.height();
  int img_w = qimg.width();

  // draw template on output //
  int minx = max(0, pos_x);
  int miny = max(0, pos_y);
  int maxx = min(pos_x + tpl_w, img_w);
  int maxy = min(pos_y + tpl_h, img_h);
  for (int y = miny; y < maxy; y++)
    for (int x = minx; x < maxx;  x++)
      if (qimgTempl.pixel(x-pos_x, y-pos_y) == qRgb(0,0,0))
        qimg.setPixel(x, y, color);
}


void ImageOperations::drawMaskOnImage( const OpGrayImage &imgTempl,
                                       QImage& qimg,
                                       int pos_x, int pos_y, QRgb color )
{
  int tpl_h = imgTempl.height();
  int tpl_w = imgTempl.width();
  int img_h = qimg.height();
  int img_w = qimg.width();

  // draw template on output //
  int minx = max(0, pos_x);
  int miny = max(0, pos_y);
  int maxx = min(pos_x + tpl_w, img_w);
  int maxy = min(pos_y + tpl_h, img_h);
  for( int y = miny; y < maxy; y++ )
    for( int x = minx; x < maxx;  x++ )
      if( imgTempl(x-pos_x, y-pos_y).value() != 0 )
        qimg.setPixel( x, y, color );
}


void ImageOperations::addBorder( OpGrayImage& img,
                                 int width, int height, float color )
{
  OpGrayImage tmp(img.width()+2*width, img.height()+2*height);
  for (int x=0; x<width; x++)
    for(int y=0; y<tmp.height(); y++)
      tmp(x,y)=color;

  for (int x=tmp.width()-width; x<tmp.width(); x++)
    for(int y=0; y<tmp.height(); y++)
      tmp(x,y)=color;

  for (int x=width; x<tmp.width()-width; x++)
    for(int y=0; y<height; y++)
      tmp(x,y)=color;

  for (int x=width; x<tmp.width()-width; x++)
    for(int y=tmp.height()-height; y<tmp.height(); y++)
      tmp(x,y)=color;

  img = tmp.add(img, width, height);
}


vector<OpGrayImage> ImageOperations::getEdgesByOrientation(
  const OpGrayImage& img, float dSigma, float dTLow, float dTHigh )
{
  GaussFilterbank gfbFilter;
  gfbFilter.setup(1, 4, 1.41, 3);

  return orientationCanny( img, dSigma, dTLow, dTHigh, gfbFilter);
}


void ImageOperations::opgrayimages2qimages( const vector<OpGrayImage>& vSrc,
    vector<QImage>& vDst )
{
  vDst.clear();

  vector<OpGrayImage>::const_iterator it;
  for(it=vSrc.begin(); it!=vSrc.end(); it++)
    vDst.push_back(it->getQtImage());
}

vector<OpGrayImage> ImageOperations::split2x2(const OpGrayImage& img)
{
  vector<OpGrayImage> vPatches;

  int x, y;
  int patchWidth=img.width()/2;
  int patchHeight=img.height()/2;

  for (int i=0; i<4; i++)
  {
    x = (i%2)*patchWidth;
    y = (i/2)*patchHeight;
    vPatches.push_back( img.extractRegion(x,y, x+patchWidth-1, y+patchHeight-1) );
  }
  return vPatches;
}


OpGrayImage ImageOperations::join2x2(const vector<OpGrayImage>& vPatches)
{
  //printf("ImageOperations::join2x2()\n");
  assert(vPatches.size()==4);
  for(int i=0; i<3; i++)
    assert(vPatches[i].width()==vPatches[i+1].width() && vPatches[i].height()==vPatches[i+1].height());

  int patchWidth= vPatches[0].width();
  int patchHeight = vPatches[0].height();

  OpGrayImage img(patchWidth*2, patchHeight*2);
  for (unsigned i=0; i<4; i++)
    img = img.copy(vPatches[i], patchWidth*(i%2), patchHeight*(i/2));

  //printf("ImageOperations::join2x2() finished!!!\n");
  return img;
}


vector<QPoint> ImageOperations::sampleFromPoints(const vector<QPoint>& inPoints, int pixelDist)
{
  vector<QPoint> points;
  int maxX=0;
  int maxY=0;
  for(unsigned i=0; i<inPoints.size(); i++)
  {
    maxX = std::max(maxX,inPoints[i].x());
    maxY = std::max(maxY,inPoints[i].y());
  }
  printf("MaxX: %d, MaxY: %d\n",  maxX, maxY);
  OpGrayImage imgCutOut(maxX+pixelDist,maxY+pixelDist);

  //--- adding contour points to return vector ---//
  for(unsigned j=0; j<inPoints.size(); j++)
  {
    QPoint tmp = inPoints[j];
    int x = tmp.x();
    int y = tmp.y();

    if (imgCutOut(x,y).value()==0)
    {
      points.push_back(tmp);
      //--- mark region as taken ---//
      for(int xx=x-pixelDist; xx<x+pixelDist; xx++)
        for(int yy=y-pixelDist; yy<y+pixelDist; yy++)
          if(xx>0 && yy>0)
            imgCutOut(xx, yy)=1.0f;
    }
    //else
    //  printf("Skipping point: %dx%d\n", x,y);
  }

  return points;
}


