/*********************************************************************/
/*                                                                   */
/* FILE         optimizer.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      Codebook optimizer, to be used with the clusterer/   */
/*              scmatcher/mcmatcher program series.                  */
/*              Compresses the occurrences and occurrence maps, thus */
/*              optimizing the codebook for speed.                   */
/*                                                                   */
/* BEGIN        Tue Jul 25 2006                                      */
/* LAST CHANGE  Thu Jul 27 2006                                      */
/*                                                                   */
/*********************************************************************/

#ifndef OPTIMIZER_HH
#define OPTIMIZER_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>

#include <opgrayimage.hh>
#include <featurevector.hh>
#include <cluster.hh>
#include <clstep.hh>
#include <clrnncagglo.hh>
#include <occurrences.hh>
#include <codebook.hh>
#include <ism.hh>

/*******************/
/*   Definitions   */
/*******************/
const string STR_APPNAME      = "optimizer";
const string STR_NAME         = "Codebook Optimizer";
const string STR_VERSION      = "0.4";

const string DIR_HOME         = "/home/bleibe/";
const string DIR_IMAGES       = DIR_HOME+"images";
const string DIR_EXPERIMENTS  = DIR_HOME+"projects/patches/experiments-cvpr07/";
const string DIR_CODEBOOKS    = DIR_EXPERIMENTS+"codebooks";
const string DIR_RESULTS      = DIR_EXPERIMENTS+"results";

#endif
