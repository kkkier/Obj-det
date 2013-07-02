This package contains computer vision code, libraries, and demo
applications developed at ETH Zurich in 2001-2004, 2006-2008 and at TU Darmstadt in 2004-2005. Except where otherwise specified, all files are
copyrighted 2001-2005 by Bastian Leibe.

The programs, and all associated files or parts thereof, are made
available exclusively for non-commercial research purposes. If you are interested in commercialization, please contact the author under the email address given below.

Parts of the package may contain code that is copyrighted by other
parties. In particular, the directory "libPatchExtraction5" contains
interest point detectors and region descriptors made available by
Krystian Mikolajczyk (kma@robots.ox.ac.uk) for non-commercial research
use. Intellectual property for those parts must also be respected.

Disclaimer
----------
THIS CODE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Use at your own
risk.  

===============
1. Installation
===============
Some of the code libraries and all demo applications are based on Qt,
so a working installation of Qt is required for compilation
(preferrably version 3 or higher). NOTE: The code is not yet converted to Qt4. When using Qt4 on your machine, you need to perform this conversion manually!

I have prepared an installation script that automatically creates the
right directory structure, unzips the archives, and compiles the code
in the right order. For installation, please run the following script 
(or for better control, manually copy its contents to the command line):

install.sh

Please inform me if something doesn't work. I couldn't test the newest
version of the script on my own machine, since all the libraries are
already installed here, so there might still be some errors.

Bastian Leibe
leibe@umic.rwth-aachen.de


=================
2. Code Libraries
=================

Image Libraries
---------------
pccvImage.tgz       - The basic image class.
libGrayImage.tgz    - A package for grayvalue images.
libGaussDeriv.tgz   -   supplementary functions for the grayvalue image class.
libCanny.tgz        -   supplementary functions for the grayvalue image class.
libMorphology.tgz   -   supplementary functions for the grayvalue image class.
libHSIImage.tgz     - A package for RGB images.
libRGBImage.tgz     - A package for HSI images.
libScaleSpace.tgz   - A package for building scale spaces over images.
libInterestPts.tgz  - A package containing different interest point operators
                      (look at the files applydetectors.hh/cc for a demonstra-
                      tion of how these operators can be used).
libGaussPyramid.tgz - A package for Gaussian and Laplacian pyramids.
libEdgeSIFT.tgz     - A package for SIFT-like region descriptors based
                      on edge images.

Feature Libraries
-----------------
libFeatures.tgz     - The basic feature vector class.
libChiSquare.tgz    -   supplementary functions for the feature vector class.
libMatrix.tgz       -   supplementary functions for matrix operations.
libPCA.tgz          - A package for applying PCA on feature vectors.
libMath.tgz         -   supplementary math functions for the PCA.
libHistogram.tgz    - A histogram class derived from the feature vector class.
libNNSearch2.tgz    - A package for fast Nearest-Neighbor search.

Clustering Methods
------------------
libCluster2.tgz     - A package containing several clustering algorithms 
                      (currently, only kmeans and agglomerative clustering 
                       can be guaranteed to work).

Chamfer Libraries
-----------------
libChamfer.tgz      - A package containing basic Chamfer-matching functions.
libOrientationPlanes.tgz - supplementary functions for Chamfer matching 
                           with multiple orientation planes.
libHelpers.tgz           - supplementary functions for coarse-to-fine matching.

ISM Libraries
-------------
libPatchExtraction5 - Functionality for extracting local image features.
libCodebook2        - Functionality for creating and matching to codebooks.
libISM2             - Functionality for ISM-based object categorization.
libVotingSpace2     -   implementation of a continuous voting space.
libDetector2        - Functionality encapsulating entire ISM detectors
libContainer        -   helper class

Other Libraries
---------------
libIDL.tgz          - A package containing functions for reading/writing 
                      image annotation files.

GUI Tools
---------
libQtTools.tgz      - Some tool classes for Qt interfaces.


====================
3. Main Programs
====================
clusterer9.tgz      - A program for generating codebooks (see the README
                      file enclosed within this archive for a more detailed
                      documentation). 
scmatcher9.tgz      - A program for object recognition with a single detector.
                      This program can also be used to create occurrence files
                      for training a new detector (and for saving it for later
                      use with mcmatcher3).

                      categorization and segmentation using multiple pre-trained
                      detectors (see the README file enclosed within this archive for a more detailed 
                      documentation).
optimizer.tgz       - A program for optimizing the storage files of pre-trained
                      detector codebooks and occurrence files.
                      
