#!/bin/tcsh

# Image libs
./make-tar.sh pccvImage
./make-tar.sh libGaussDeriv
./make-tar.sh libCanny
./make-tar.sh libMorphology
./make-tar.sh libGrayImage
./make-tar.sh libRGBImage
./make-tar.sh libHSIImage
./make-tar.sh libScaleSpace
./make-tar.sh libInterestPts
./make-tar.sh libGaussPyramid
./make-tar.sh libEdgeSIFT

# Feature libs
./make-tar.sh libChiSquare
./make-tar.sh libMatrix
./make-tar.sh libFeatures
./make-tar.sh libHistogram
./make-tar.sh libMath
./make-tar.sh libPCA
./make-tar.sh libNNSearch2

# Other libs
./make-tar.sh libCluster2
./make-tar.sh libIDL

# Qt tools
./make-tar.sh libQtTools

# Chamfer matching
./make-tar.sh libOrientationPlanes
./make-tar.sh libHelpers
./make-tar.sh libChamfer

# ISM libs
./make-tar.sh libPatchExtraction5
./make-tar.sh libContainer
./make-tar.sh libCodebook2
./make-tar.sh libVotingSpace2
./make-tar.sh libISM2
./make-tar.sh libDetector2
./make-tar.sh libCalibration

# Sample applications
#./make-tar.sh testviewer
./make-tar.sh clusterer9
./make-tar.sh scmatcher9
./make-tar.sh mcmatcher3
./make-tar.sh optimizer


mkdir -p tarfiles
mv *.tgz tarfiles
cp *.sh tarfiles
cp *.txt tarfiles


