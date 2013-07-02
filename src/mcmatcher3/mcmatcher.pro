######################################################################
# Automatically generated by qmake (1.07a) Fri Nov 5 10:09:33 2004
######################################################################

TEMPLATE = app
target = mcmatcher
CONFIG += release
#CONFIG += debug

QMAKE_CXXFLAGS_RELEASE = -O3 #-march=pentium4 -mfpmath=sse -mmmx

CODE = ${HOME}/code

INCLUDEPATH += . $${CODE}/include

# Input
HEADERS += mcmatcher.hh \
           unixtools.hh \
           veriparams.hh \
           verigui.hh \
           matwriter.hh

SOURCES += mcmatcher-interface.cc \
           mcmatcher-fileio.cc \
           mcmatcher-verification.cc \
           mcmatcher-experiments.cc \
           mcmatcher.cc \
           main.cc \
           veriparams.cc \
           verigui.cc \
           matwriter.cc \
           unixtools.cc 


QT_LIBS      = -lqt-mt -lQtTools
IMGDB_LIBS   = -lImgDB
#PCCV_LIBS    = -lrgbdisp 
IMAGE_LIBS   = -limage -lGrayImage -lRGBImage -lGaussDeriv -lCanny -lMorphology 
SCALE_LIBS   = -lScaleSpace -lInterestPts -lPatchExtraction5
FEATURE_LIBS = -lFeatures -lPCA -lMath -lMatrix -lChiSquare -lNNSearch2 -lCluster2 -lHistogram -lEdgeSIFT -lChamfer 
RECO_LIBS    = -lContainer -lCodebook2 -lVotingSpace2 -lISM2 -lDetector2 -lCalibration
OTHER_LIBS     = -lHelpers -lIDL -lOrientationPlanes 
KM_LIBS      = -L. -lkbt
STD_LIBS     = -lm -lstdc++
LIBS += -L$${CODE}/lib/i686 $${QT_LIBS} $${IMAGE_LIBS} $${SCALE_LIBS} $${FEATURE_LIBS} $${RECO_LIBS} $${OTHER_LIBS} #$${KM_LIBS}

