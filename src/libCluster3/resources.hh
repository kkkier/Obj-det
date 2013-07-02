/*********************************************************************/
/*                                                                   */
/* FILE         resources.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Useful macros for timing code and determining memory */
/*              usage.                                               */
/*                                                                   */
/* BEGIN        Fri Jun 04 2004                                      */
/* LAST CHANGE  Fri Jun 04 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef RESOURCES_HH
#define RESOURCES_HH

/****************/
/*   Includes   */
/****************/
#include <sys/time.h>      // for timing the clustering code
#include <sys/resource.h>
#include <sys/types.h>
#include <time.h>

/*******************/
/*   Definitions   */
/*******************/
extern int getrusage();

/*****************************/
/*   Function Declarations   */
/*****************************/
/* for timing the clustering code */
inline float CPUTIME()
{ 
  struct rusage ruse;
  getrusage(RUSAGE_SELF,&ruse);

  return ( ruse.ru_utime.tv_sec + ruse.ru_stime.tv_sec + 
           1e-6 * (ruse.ru_utime.tv_usec + ruse.ru_stime.tv_usec) );
}

inline float MEMUSAGERSS()
{
  struct rusage ruse;
 getrusage(RUSAGE_SELF,&ruse);
 
 return ruse.ru_maxrss;
}

inline float MEMUSAGE()
{
  struct rusage ruse;
  getrusage(RUSAGE_SELF,&ruse);

  return ruse.ru_ixrss + ruse.ru_idrss + ruse.ru_isrss;
}


#endif
