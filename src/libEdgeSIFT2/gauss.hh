/*********************************************************************/
/*                                                                   */
/* FILE         gauss.hh                                             */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for evaluating Gaussians of various sorts. */
/*                                                                   */
/* BEGIN        Fri Nov 16 2001                                      */
/* LAST CHANGE  Fri Nov 16 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_GAUSS_HH
#define LEIBE_GAUSS_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

  
/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <vector>

using namespace std;

/*******************/
/*   Definitions   */
/*******************/
#ifndef PI
#define PI 3.141592654
#endif

#define EPSILON_SIGMA 0.001


/***************************/
/*   Function Prototypes   */
/***************************/
double evaluateUnnormalizedGauss( int nDim, double dDist, double dSigma );
/* Evaluate the standard Normal distribution function with a uni-  */
/* form sigma.                                                     */

double evaluateUnnormalizedGauss2( int nDim, double dDist, double dSigma2 );
/* Evaluate the standard Normal distribution function with a uni-  */
/* form sigma.                                                     */

double evaluateGauss( int nDim, double dDist, double dSigma );
/* Evaluate the standard Normal distribution function with a uni-  */
/* form sigma.                                                     */

double evaluateGauss( int nDim, double dDist, vector<double> vCov, int idx );
/* Evaluate the standard Normal distribution function with a dia-  */
/* gonal covariance matrix:                                        */


#endif // LEIBE_GAUSS_HH
