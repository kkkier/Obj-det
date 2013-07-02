#!/bin/tcsh 

alias qmake qmake-3.3.4

#
# Image libs
#

setenv CC g++
setenv LN g++

pushd pccvImage 
qmake 
make uninstall 
make clean 
make 
make install 
popd 

pushd libGrayImage 
cp grayimage.hh ~/code/include/ 
popd 

pushd libGaussDeriv 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libCanny 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libMorphology 
qmake 
make uninstall 
make clean 
make 
make install 
popd 

pushd libGrayImage 
qmake 
make uninstall 
make clean 
make 
make install 
popd 

pushd libRGBImage 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libHSIImage 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libScaleSpace 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libInterestPts 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libGaussPyramid 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

#
# FeatureVector libs
#

pushd libChiSquare 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libFeatures 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libMatrix 
qmake 
make uninstall 
make clean 
make 
make install 
popd 

pushd libHistogram 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libMath 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libCluster2 
cp *.hh ~/code/include 
popd 

pushd libNNSearch2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libCluster2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libPCA 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

#
# Other libs 
#

pushd libIDL 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libEdgeSIFT 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

#
# Qt tools
#

pushd libQtTools 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

#
# Chamfer matching
#

pushd libOrientationPlanes 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libHelpers 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libChamfer 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

#
# ISM libs
#

pushd libPatchExtraction5 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libContainer 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libCodebook2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libVotingSpace2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libISM2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libCalibration 
qmake 
make uninstall 
make clean 
make  
make install 
popd 

pushd libDetector2 
qmake 
make uninstall 
make clean 
make  
make install 
popd 


#
# Sample applications
#

pushd clusterer9 
qmake 
make clean 
make  
popd 

#pushd scmatcher7
#qmake
#make clean
#make
#popd

pushd scmatcher9 
qmake 
make clean 
make 
popd 

pushd mcmatcher3 
qmake 
make clean 
make 
popd 

pushd optimizer 
qmake 
make clean 
make 
popd 

#pushd testviewer 
#make clean 
#make  
#popd
