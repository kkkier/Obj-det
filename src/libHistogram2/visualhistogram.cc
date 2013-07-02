/*********************************************************************/
/*                                                                   */
/* FILE         visualhistogram.cc                                   */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implement some histogram visualization routines for  */
/*              1D and 2D histograms.                                */
/*                                                                   */
/* BEGIN        MON Aug 27 2001                                      */
/* LAST CHANGE  MON Aug 27 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>

#include <qimage.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <q3pointarray.h>
#include <qstring.h>

#include <opgrayimage.hh>
#include "visualhistogram.hh"

/*===================================================================*/
/*                         Class Histogram                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

VisualHistogram& VisualHistogram::operator=( Histogram other )
  /* assignment operator */
{
  copyFromOther( other );
  return *this;
}


VisualHistogram& VisualHistogram::operator=( VisualHistogram other )
  /* assignment operator */
{
  copyFromOther( other );
  return *this;
}


/***********************************************************/
/*                  Visualization Functions                */
/***********************************************************/

void VisualHistogram::drawHistogram( QImage &image, 
				     float normalizeMaxValue )
  /* draw the histogram into a QPixmap structure */
{
  assert( isValid() );
  assert( (m_nDims==1) || (m_nDims==2) );

  if( getSumAbs() == 0.0 )
    // empty image
    return;

  if( m_nDims == 1 )
    draw1DHistogram( image, normalizeMaxValue );

  else if( m_nDims == 2 )
    draw2DHistogram( image, normalizeMaxValue );

  else
    cerr << "Error in VisualHistogram::drawHistogram(): " 
	 << "Don't know how to draw " << m_nDims << "D histogram!" << endl;
}


void VisualHistogram::draw1DHistogram( QImage &image, 
				       float normalizeMaxValue,
				       bool ScaleXAxisBins )
  /* Draw a 1D histogram as a bar chart */
{
  QPixmap pixmap = draw1DHistogram( normalizeMaxValue, ScaleXAxisBins );

  image = pixmap.convertToImage();
}


QPixmap VisualHistogram::draw1DHistogram( float normalizeMaxValue, 
					  bool ScaleXAxisBins )
  /* Draw a 1D histogram as a bar chart */
{
  //cout << "    VisualHistogram::draw1DHistogram(float) called." << endl;
  assert( isValid() );
  assert( m_nDims == 1 );

  //const int HISTOGRAM_WIDTH  = 220;
  int HISTOGRAM_WIDTH  = 220;
//   if ( m_vNumBins[0] > 2*HISTOGRAM_WIDTH )
//     HISTOGRAM_WIDTH = 440;
  const int HISTOGRAM_HEIGHT = 200;
  const int BAR_WIDTH = 7;
  const int BAR_DIST  = 2;
  const int MAX_BAR_HEIGHT  = HISTOGRAM_HEIGHT;

  const int LINE_WIDTH    = 1;
  const int OFFSET_LEFT   = 43; //35;
  const int OFFSET_BOTTOM = 32;
  const int OFFSET_TOP    = 10;
  const int OFFSET_RIGHT  = 10;
  const int SCALE_WIDTH   =  6;
  const int TEXT_LEFT     =  2;
  const int TEXT_BOTTOM   = 10;
  const int TEXT_CHARWIDTH=  8;
  const int ARROW_TOP     = 10;
  const int ARROW_RIGHT   = 10;
  const int ARROW_LEN     = 10;
  const int ARROW_WIDTH   =  8;

  int   width;
  int   height;

  width  = ( OFFSET_LEFT + LINE_WIDTH + 
             //m_vNumBins[0]*(BAR_WIDTH + BAR_DIST) +
             HISTOGRAM_WIDTH + 
             ARROW_RIGHT + ARROW_LEN + OFFSET_RIGHT );
  
  height = ( OFFSET_BOTTOM + LINE_WIDTH + 
             //MAX_BAR_HEIGHT + 
             HISTOGRAM_HEIGHT + 
             ARROW_TOP + ARROW_LEN + OFFSET_TOP );
  
  QPixmap pixmap( width, height );
  //cout << "      width : " << width << endl;
  //cout << "      height: " << height << endl;
  
  float bar_width;
  float bar_dist;
  
  bar_width = ( ( ((float)BAR_WIDTH) / ((float)BAR_WIDTH+BAR_DIST) ) * 
                ( ((float)HISTOGRAM_WIDTH) / ((float)m_vNumBins[0]) ) );
  bar_dist  = ( ( ((float)BAR_DIST) / ((float)BAR_WIDTH+BAR_DIST) ) * 
                ( ((float)HISTOGRAM_WIDTH) / ((float)m_vNumBins[0]) ) );
  
  //cout << "      bar_width: " << bar_width << endl;
  //cout << "      bar_dist : " << bar_dist << endl;
  
  float height_factor = 1.0;
  float min_content;
  float max_content;
  float max_value;
  getMinMax( min_content, max_content );
  if( normalizeMaxValue > 0 ) {
    /* compute a height factor s.t. the MAX_BAR_HEIGHT corresponds   */
    /* to the limit given in the procedure call.                     */
    height_factor = MAX_BAR_HEIGHT / normalizeMaxValue;
    max_value = normalizeMaxValue;

  } else {
    /* set the height factor s.t. the MAX_BAR_HEIGHT corresponds to  */
    /* the maximum cell content.                                     */
    height_factor = MAX_BAR_HEIGHT / max_content;
    max_value = max_content;
  }
  //cout << "      max_value    : " << max_value << endl;
  //cout << "      height_factor: " << height_factor << endl;
  
  /*************************/
  /*   Set up the pixmap   */
  /*************************/
  /* clear the background */
  pixmap.fill( Qt::white );

  /* set the coordinate system s.t. the origin is in the bottom left */
  /* corner and the y axis points up.                                */
  QPainter p( &pixmap );
  p.translate( 0, height-1 );
  p.scale( 1.0, -1.0 );

  /*************************/
  /*   Draw all the bars   */
  /*************************/
  p.setPen( Qt::red );
  float startpos_x = OFFSET_LEFT + LINE_WIDTH + bar_dist; //+ BAR_DIST;
  int startpos_y = OFFSET_BOTTOM + LINE_WIDTH;
  for ( int i=0; i<m_vNumBins[0]; i++ ) {
    int bar_height = ((int) floor(m_vBins[idx(i)] * height_factor));
    if ( bar_height > MAX_BAR_HEIGHT )
      bar_height = MAX_BAR_HEIGHT;
    p.fillRect( qRound(startpos_x), startpos_y, 
		qRound(bar_width+0.5), //BAR_WIDTH, 
		bar_height,
		Qt::red );

    startpos_x += bar_width + bar_dist; //BAR_WIDTH + BAR_DIST;
  } 
  
  /******************************************/
  /*   Draw the coordinate system's lines   */
  /******************************************/
  p.setPen( QPen( Qt::black, LINE_WIDTH ) );
  p.drawLine( OFFSET_LEFT, OFFSET_BOTTOM, 
	      width-OFFSET_RIGHT-ARROW_LEN, OFFSET_BOTTOM );
  p.drawLine( OFFSET_LEFT, OFFSET_BOTTOM, 
	      OFFSET_LEFT, height-OFFSET_TOP-ARROW_LEN );

  /**************************************/
  /*   Draw the scales in y-direction   */
  /**************************************/
  /* search for the greatest decimal power <= max_value */
  int power_y = ((int) floor( log10(max_value) ) );
  //cout << "      power_y: " << power_y << endl;
  
  /* draw a short line at every occurrence of half this power */
  p.setPen( QPen( Qt::black, 1 ) );
  //for ( int next_at=qRound(pow(10.0, power_y)/2.0); next_at <= max_value; 
  //next_at += qRound(pow(10.0, power_y)/2.0) ) {
  //cout << "      Drawing short lines..." << endl;
  
  if(max_value!=0){
	  for ( double next_at = pow(10.0, power_y)/2.0; next_at <= max_value*1.01; 
		next_at += pow(10.0, power_y)/2.0 ) {
	    int pos_y = OFFSET_BOTTOM+LINE_WIDTH + ((int) floor(next_at*height_factor));
	    p.drawLine( OFFSET_LEFT-SCALE_WIDTH/2-LINE_WIDTH, pos_y, OFFSET_LEFT, pos_y );
	  }
  }
  /* draw a longer line at every occurrence of this power */
  p.setPen( QPen( Qt::black, 1 ) );
  //  for ( int next_at=qRound(pow(10.0, power_y)); next_at <= max_value; 
  //next_at += qRound(pow(10.0, power_y)) ) {
  //cout << "      Drawing long lines..." << endl;
  if(max_value!=0){
	  for ( double next_at = pow(10.0, power_y); next_at <= max_value*1.01; 
		next_at += pow(10.0, power_y) ) {
	    int pos_y = OFFSET_BOTTOM+LINE_WIDTH + ((int) floor(next_at*height_factor));
	    p.drawLine( OFFSET_LEFT-SCALE_WIDTH-LINE_WIDTH, pos_y, OFFSET_LEFT, pos_y );

	    /* print a caption for this line */
	    QString text;
	    if ( power_y <= 4 )
	      text.setNum(next_at);
	    else
	      text.setNum( (float)next_at, 'g', 0 );
	    p.scale( 1.0, -1.0 );
	    p.drawText( TEXT_LEFT, -pos_y-1, text );
	    p.scale( 1.0, -1.0 );
	  }
  }
  //cout << "      Drawing done." << endl;
  
  /**************************************/
  /*   Draw the scales in x-direction   */
  /**************************************/

  if ( ScaleXAxisBins ) {
    /* Draw the x-axis scale with the bin numbers */
    
    /* search for the greatest decimal power <= max_value */
    int power_x = ((int) floor( log10( (float) m_vNumBins[0]) ) );
    //cout << "      power_x: " << power_x << endl;
    
    /* draw a short line at every occurrence of half this power */
    p.setPen( QPen( Qt::black, 1 ) );
    if ( power_x > 0 )
      for ( int next_at=(int)pow(10.0, power_x)/2; next_at <= m_vNumBins[0]; 
            next_at += (int)pow(10.0, power_x)/2 ) {
        int pos_x = qRound( OFFSET_LEFT+LINE_WIDTH+bar_dist + //BAR_DIST + 
                            (floor((next_at-0.5)*(bar_width+bar_dist))) ); 
        //(BAR_WIDTH+BAR_DIST))) );
        p.drawLine( pos_x, OFFSET_BOTTOM-SCALE_WIDTH/2-LINE_WIDTH, 
                    pos_x, OFFSET_BOTTOM );
      }
    /* draw a longer line at every occurrence of this power */
    p.setPen( QPen( Qt::black, 1 ) );
    if ( power_x > 0 )
      for ( int next_at=(int)pow(10.0, power_x); next_at <= m_vNumBins[0]; 
            next_at += (int)pow(10.0, power_x) ) {
        int pos_x = qRound( OFFSET_LEFT+LINE_WIDTH+bar_dist + //BAR_DIST + 
                            (floor((next_at-0.5)*(bar_width+bar_dist))) ); 
        //(BAR_WIDTH+BAR_DIST))) );
        p.drawLine( pos_x, OFFSET_BOTTOM-SCALE_WIDTH-LINE_WIDTH, 
                    pos_x, OFFSET_BOTTOM );
        
        /* print a caption for this line */
        QString text;
        text.setNum(next_at);
        p.scale( 1.0, -1.0 );
        p.drawText( pos_x - TEXT_CHARWIDTH*(1+power_x)/2, -TEXT_BOTTOM, text );
        p.scale( 1.0, -1.0 );
      }
    
  } else {
    /* Draw the x-axis scale with the histogram values */
    float first;
    float increment;
    
    /* search for the greatest decimal power <= max_value */
    int power_x;
    if ( fabs(m_vMaxValues[0]) >= fabs(m_vMinValues[0]) )
      power_x = ((int) floor( log10(fabs(m_vMaxValues[0])) ) );
    else
      power_x = ((int) floor( log10(fabs(m_vMinValues[0])) ) );
    //cout << "      power_x: " << power_x << endl;
    
    /* draw a short line at every occurrence of half this power */
    p.setPen( QPen( Qt::black, 1 ) );
    /* find first occurrence of this power */
    increment = pow(10.0, power_x)/2.0;
    first = ((float)((int)(m_vMinValues[0]/increment))*increment);
    for ( float next_at=first; next_at <= m_vMaxValues[0]*1.01; 
          next_at += increment ) {
      int pos_x = qRound( OFFSET_LEFT+LINE_WIDTH+bar_dist +
                          ( (next_at-m_vMinValues[0])/
                            (m_vMaxValues[0]-m_vMinValues[0]) ) * 
                          HISTOGRAM_WIDTH );
      p.drawLine( pos_x, OFFSET_BOTTOM-SCALE_WIDTH/2-LINE_WIDTH, 
                  pos_x, OFFSET_BOTTOM );
    }
    /* draw a longer line at every occurrence of this power */
    p.setPen( QPen( Qt::black, 1 ) );
    increment = pow(10.0, power_x);
    first = ((float)((int)(m_vMinValues[0]/increment))*increment);
    for ( float next_at=first; next_at <= m_vMaxValues[0]*1.01; 
          next_at += increment ) {
      int pos_x = qRound( OFFSET_LEFT+LINE_WIDTH+bar_dist +
                          ( (next_at-m_vMinValues[0])/
                            (m_vMaxValues[0]-m_vMinValues[0]) ) * 
                          HISTOGRAM_WIDTH );
      p.drawLine( pos_x, OFFSET_BOTTOM-SCALE_WIDTH-LINE_WIDTH, 
                  pos_x, OFFSET_BOTTOM );
      
      /* print a caption for this line */
      QString text;
      text.setNum(next_at);
      p.scale( 1.0, -1.0 );
      p.drawText( pos_x - TEXT_CHARWIDTH*(text.length())/2, -TEXT_BOTTOM, text );
      //p.drawText( pos_x - TEXT_CHARWIDTH*(1+power_x)/2, -TEXT_BOTTOM, text );
      p.scale( 1.0, -1.0 );
    }
  }

  /****************************/
  /*   Draw the arrow heads   */
  /****************************/
  p.setPen( QPen(Qt::black, 1, //LINE_WIDTH, 
		 Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
  p.setBrush( Qt::black );
  Q3PointArray pa(3);
  pa.setPoint( 0, OFFSET_LEFT, height-OFFSET_TOP );
  pa.setPoint( 1, OFFSET_LEFT-ARROW_WIDTH/2, height-OFFSET_TOP-ARROW_LEN );
  pa.setPoint( 2, OFFSET_LEFT+ARROW_WIDTH/2, height-OFFSET_TOP-ARROW_LEN );
  p.drawPolygon( pa );

  pa.setPoint( 0, width-OFFSET_RIGHT, OFFSET_BOTTOM );
  pa.setPoint( 1, width-OFFSET_RIGHT-ARROW_LEN, OFFSET_BOTTOM-ARROW_WIDTH/2 );
  pa.setPoint( 2, width-OFFSET_RIGHT-ARROW_LEN, OFFSET_BOTTOM+ARROW_WIDTH/2 );
  p.drawPolygon( pa );
  
  /* finish drawing */
  p.end();

  //cout << "    done." << endl;
  return pixmap;
}


void VisualHistogram::draw2DHistogram( QImage &image, 
				       float normalizeMaxValue )
{
  assert( isValid() );
  assert( m_nDims == 2 );

  OpGrayImage grayImg = draw2DHistogram();
  if( normalizeMaxValue > 0.0 )
    grayImg.opClipAbove( normalizeMaxValue );

  image = grayImg.getQtImage();
}


OpGrayImage VisualHistogram::draw2DHistogram()
  /* Draw a 2D histogram as a grayvalue image */
{
  assert( isValid() );
  assert( m_nDims == 2 );

  /* create a grayvalue image of the right size */
  OpGrayImage histoimg( m_vNumBins[0], m_vNumBins[1] );

  /* draw the histogram content into the grayvalue image */
  for ( int x=0; x<m_vNumBins[0]; x++ )
    for ( int y=0; y<m_vNumBins[1]; y++ ) {
      int index = idx(x,y);

      histoimg(x,y) = m_vBins[index];
    }

  return histoimg;
}



#ifdef _USE_PERSONAL_NAMESPACES
}
#endif
