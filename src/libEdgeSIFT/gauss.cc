/*********************************************************************/
/*                                                                   */
/* FILE         gauss.cc                                             */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for evaluating Gaussians of various sorts. */
/*                                                                   */
/* BEGIN        Fri Nov 16 2001                                      */
/* LAST CHANGE  Fri Nov 16 2001                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include "gauss.hh"


double evaluateUnnormalizedGauss( int nDim, double dDist, double dSigma )
  /* Evaluate the standard Normal distribution function with a uni-  */
  /* form sigma.                                                     */
  /*   p(x) = exp(-dist^2/2*sigma^2)                                 */
{
  double sigma2, exponent, normconst;

  sigma2    = dSigma*dSigma;
  if (sigma2 < EPSILON_SIGMA*EPSILON_SIGMA ) 
    sigma2 = EPSILON_SIGMA * EPSILON_SIGMA;
  exponent  = -( (dDist*dDist) / (2.0*sigma2) );
  //normconst = pow( 2.0 * PI, ((double) nDim)*0.5 ) * sqrt( sigma2 ); // ex. without sqrt()
  
  return exp(exponent);// / normconst;
}


double evaluateUnnormalizedGauss2( int nDim, double dDist, double dSigma2 )
  /* Evaluate the standard Normal distribution function with a uni-  */
  /* form sigma.                                                     */
  /*   p(x) = exp(-dist^2/2*sigma^2)                                 */
{
  double exponent, normconst;

  //sigma2    = dSigma*dSigma;
  if (dSigma2 < EPSILON_SIGMA*EPSILON_SIGMA ) 
    dSigma2 = EPSILON_SIGMA * EPSILON_SIGMA;
  exponent  = -( (dDist*dDist) / (2.0*dSigma2) );
  //normconst = pow( 2.0 * PI, ((double) nDim)*0.5 ) * sqrt( sigma2 ); // ex. without sqrt()
  
  return exp(exponent);// / normconst;
}


double evaluateGauss( int nDim, double dDist, double dSigma )
  /* Evaluate the standard Normal distribution function with a uni-  */
  /* form sigma.                                                     */
  /*   p(x) = (1/2*pi*sigma^2)*exp(-dist^2/2*sigma^2)                */
{
  double sigma2, exponent, normconst;

  sigma2    = dSigma*dSigma;
  if (sigma2 < EPSILON_SIGMA*EPSILON_SIGMA ) 
    sigma2 = EPSILON_SIGMA * EPSILON_SIGMA;
  exponent  = -( (dDist*dDist) / (2.0*sigma2) );
  normconst = pow( 2.0 * PI, ((double) nDim)*0.5 ) * sqrt( sigma2 ); // ex. without sqrt()
  
  return exp(exponent) / normconst;
}


double evaluateGauss( int nDim, double dDist, vector<double> vCov )
  /* Evaluate the standard Normal distribution function with a dia-  */
  /* gonal covariance matrix:                                        */
  /*   p(x) = (1/2*pi*sigma^2)*exp(-dist^2/2*sigma^2)                */
{
  double sigma2, exponent, normconst;
  double dist;

  sigma2 = 1.0;
  for ( int i=0; i<nDim; i++ )
    sigma2 *= vCov[i*nDim + i];
  sigma2 = sqrt(sigma2);
  if (sigma2 < EPSILON_SIGMA ) 
    sigma2 = EPSILON_SIGMA;

  exponent = 0.0;
  for ( int i=0; i<nDim; i++) {
    exponent += (dDist*dDist) / vCov[i*nDim + i];
  }
  normconst = pow( 2.0 * PI, ((double) nDim)*0.5 ) * sigma2;
  
  return exp(-0.5*exponent) / normconst;
}


