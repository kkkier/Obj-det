                       Clusterer Documentation
                       =======================


General Information
===================

This program, and all associated files or parts thereof, are made
available for research purposes only. Any commercial use or resale of
this software requires a license agreement with the author and/or ETH
Zurich.

Copyright Bastian Leibe, ETH Zurich, 2003
(except where designated otherwise).


An explanation of the employed algorithms can be found in the
following papers:
                               
Bastian Leibe and Bernt Schiele,                    
Interleaved Object Categorization and Segmentation.
In Proc. British Machine Vision Conference (BMVC'03),
Norwich, GB, Sept. 9-11, 2003.                    

Bastian Leibe, Ales Leonardis and Bernt Schiele,                    
Combined Object Categorization and Segmentation with an Implicit Shape
Model. submitted to ECCV'04 Workshop on Statistical Learning in Computer
Vision, Prague, May, 2004.



Description of the Interface
============================

The main purpose of this program is to generate codebooks for use with
the "scmatcher" program. 

Convention: If segmentation masks are provided with the training
images, they should be located in a subdirectory "maps" of the
directory containing the corresponding image, and they should have the
same name as the training image (apart from the suffix), plus the
extension "-map.png"

Example:
image: ~/projects/images/test-pic.png
mask : ~/projects/images/maps/test-pic-map.png


I.) Quick-Start Instructions
----------------------------

I.0.) After the program has compiled, it can be started by typing
"./clusterer &" from the command line interface.

I.1.) Load a set of images from which the codebook shall be generated
by clicking on "Load Images for Processing" and selecting the images
in the subsequent file dialog. If the images shall be taken from
multiple directories then answer the following dialogue window with
"More"; if all images have been selected, choose "Stop" in this
dialogue. The system will now load all images in sequence and extract
image patches from them using the chosen interest point detector (by
default the Harris detector). If segmentation masks are available for
each training image, then only those patches are kept that share a
sufficient overlap with the object (~5 pixels). The extracted patches
and their patch segmentation masks are then displayed in separate
windows.

I.2.) Choose a suitable clustering method. Only the options
"Agglomerative (post)", "Agglomerative (fast)", "Agglomerative
(RNNC)", and "Div'n'Conq PostAgglo" are supported by this version of
the program. In most cases, the RNNC option is favorable, since its memory
requirements are only O(N), while all other methods require O(N^2)
memory (and are thus not applicable with more than 15-20.000
patches). 

I.3.) Press the "Cluster Patches" button to start the clustering
process. Depending on the number of patches and the available
computing resources, this may take some time (typically ~1-2 hrs for
7.000 patches, up to 4-7 hrs for 15.000 patches), as the runtime grows
with O(N^2) or O(N^2 log N) depending on the chosen clustering
method. When the clustering process has finished, the final codebook
clusters are displayed in the bottom part of the main window. The
number underneath each cluster center shows the number of training
patches that were grouped in this cluster. If the user clicks on a
cluster, a separate window is opened to display those grouped patches.

I.4.) Save the finished codebook by clicking on the "Save" button.

I.5.) Click on the "Remove 1P Cls" button to remove those clusters
that only contain one patch. Usually, this reduces the codebook size
by a factor of 30-50% while discarding only information that was not
typical for the object category. (Note: this part of the code may
still contain a bug. If the program crashes during the removal
operation, just restart the program, load the previously saved
codebook again, and repeat the operation).

I.6.) Save the reduced codebook by clicking on the "Save" button and
choosing a different file name.



II.) In-detail Description of the Interface Elements
----------------------------------------------------

Left-Hand Side:
---------------

II.1.) Load Image
loads a test image and displays it in the main window.

II.2.) Load Images for Processing
loads a set of images that is the basis for the codebook generation
process (as described in I.1.).

II.3.) Collect Patches 
extracts image patches from the current image and displays them in a
separate window. The patch extraction can be done using either a
uniform sampling scheme or an interest point detector (option
"Patch Extraction" on the "Detect." tab). For the Harris detector, the
"Harris" tab reveals a set of more detailed options ("Sigma1",
"Sigma2", "Alpha", "Thresh"). In practice, they can be left
unchanged. The "Patch" tab contains two options: "Patch Size"
determines the size of the extracted image patches (size=2*k+1); the
"Patch Resolution" option is antiquated and should be left unchanged.

II.4.) Cluster Patches 
starts the clustering process as described in I.3. Note: this may take
a long time (typically ~2-3 hrs for 7.000 patches, up to 10 hrs for
15.000 patches).

II.5.) Continue
This option is antiquated and should not be used.

II.6.) Load / Save 
loads and saves a finished codebook. A codebook consists of 3 separate
files: a collection of the original patches (suffix ".flz" or "fls"),
a collection of the patch segmentation masks extracted from the
training images (suffix ".seg.flz" or ".seg.fls"), and an assignment
file relating the original patches to the clusters (suffix ".ass"). In
practice, only the original patches and the assignment file are used;
the patch segmentations are just saved for compatibility reasons.

II.7.) [Clustering Options]

II.7.a) Method tab
allows to select different clustering methods. In this version of the
program, only the options 3 and 4 ("Agglomerative (post)" and
"Div'n'Conq PostAgglo") are supported. For details, please see I.2.

II.7.b,c,d) PostAgglo/PreAgglo/KMeans tabs
Those tabs contain internal parameters of the clustering methods and
shouldn't be changed.

II.8.) Checkbox "Only use figure area"
This option allows to select if the full range of the training images
shall be used for patch extraction, or only the segmented region. In
practice, this option should always be selected.


Right-Hand Side:
----------------

II.9.) Remove 1P Cls removes all clusters that contain only one patch
(see I.5. for details). (Note: this part of the code may still contain
a bug. If the program crashes during the removal operation, just
restart the program, load the previously saved codebook again, and
repeat the operation).

II.10.) Compute FV Eigenspace
computes an eigenspace from the original training patches to be used
for clustering.

II.11.) Load Eigenspace Matlab
loads a precomputed eigenspace from disk in an ASCII file format.

II.12.) checkbox "Use PCA with"
allows to use the projections of the patches into the truncated
eigenspace for clustering, instead of the original patches. The
number specifies the eigenvalue energy which shall be preserved
(default: 90%), which determines how many eigenvectors are used.

II.13.) Save FV Matlab 
save the extracted image patches from the codebook in a plain ASCII
file format, so that they can be imported into Matlab.

II.14.) Save Images
saves a collection of images for each original training patch and each
cluster center in the current codebook. The user needs to specify only
a directory in which the result images shall be saved. 

II.15.) Quit
terminates the program.