
#ifndef GAUSSKERNEL_HH
#define GAUSSKERNEL_HH

#include <opgrayimage.hh>

#define CONST_PI        (float)3.14159265358979323846
#define CONST_2PI       (float)6.28318530717958647692

#define ZAP(x)          do { if (x) { delete(x); x = 0; } } while (0)
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define F2I(x)          ((x > 0.0f) ? (int)(x+0.5f) : (int)(x-0.5f))

#define IS_ODD(x)       ((x) % 2)     // only use these on ints
#define IS_EVEN(x)      (!IS_ODD(x))
#define ODDIFY(x)       if (IS_EVEN(x)) x++

class GaussKernel : public OpGrayImage
{
public:
  GaussKernel( const int& nSize, const float& fSigmaX, const float& fSigmaY,
               const int& nOrderY = 0, const float& fTheta = 0.0f, 
               const bool& bHilbert = false );

  GaussKernel& operator=( const GaussKernel& other );

protected:
  GaussKernel( const int& nSize );

  void draw( const float& fSigmaX, const float& fSigmaY, const int& nOrderY,
             const float& fTheta, const bool& bHilbert );

  void drawNoRot( const float& fSigmaX, const float& fSigmaY, 
                  const int& nOrderY, const bool& bHilbert );

  void drawRot( const float& fSigmaX, const float& fSigmaY, 
                const int& nOrderY, const bool& bHilbert , const float& fAngle );

  void extract( OpGrayImage& destImg, const int& left = 0, const int& top = 0 );

  float m_fSigmaX;
  float m_fSigmaY;
  int m_nOrderY;
};

#endif
