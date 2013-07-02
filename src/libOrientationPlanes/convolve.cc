#include "convolve.hh"

void convolve( const GrayImage &source, const GrayImage &mask, GrayImage &result )
{
  int rw = source.width();
  int rh = source.height();

  GrayImage resImg( rw, rh );

  int mw = mask.width();
  int mh = mask.height();
  int mwh = mw / 2;
  int mhh = mh / 2;

  // convolve with mask
  for ( int x=0; x < rw; x++ )
    for ( int y=0; y < rh; y++ )
    {
      resImg( x, y ) = 0.0;
      for ( int xm=0; xm < mw; xm++ )
        for ( int ym=0; ym < mh; ym++ )
        {
          int posx = x - mwh + xm;
          int posy = y - mhh + ym;
          
          posx = (posx < 0) ? 0 : posx;
          posx = (posx >= rw) ? rw-1 : posx;
          
          posy = (posy < 0) ? 0 : posy;
          posy = (posy >= rh) ? rh-1 : posy;
          
          resImg( x, y ) = resImg( x, y ).value()
            + source( posx, posy ).value() * mask( xm, ym ).value();
        }
      //resImg( x, y ) = resImg( x, y).value() / (mw*mh);
    }

  result = resImg;
}
