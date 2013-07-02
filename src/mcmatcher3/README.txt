                       Multi-Cue ISM Documentation
                       ===========================


General Information
===================

This program, and all associated files or parts thereof, are made
available exclusively for non-commercial research purposes. Any
commercial use or resale of this software requires a license agreement
with the author, the Multimodal Interactive Systems Group at TU
Darmstadt, and the Computer Vision Laboratory at ETH Zurich. The code
and binaries are under copyright protection. If you are interested in
commercialization or more optimized versions, please contact the
author under the following email address: leibe@vision.ee.ethz.ch.

Copyright Bastian Leibe, 
Multimodal Interactive Systems Group, TU Darmstadt, 2004-2005.
Computer Vision Laboratory, ETH Zurich, 2006-2008.

Parts of the package may contain code that is copyrighted by other
parties. In particular, the subdirectory "code" contains interest
point detectors and region descriptors made available by Krystian
Mikolajczyk (kma@robots.ox.ac.uk) for non-commercial research
use. Intellectual property for those parts has to be respected, as
well.

Disclaimer
----------
THIS CODE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Use at your own
risk.  

Further Information
-------------------
An explanation of the employed algorithms can be found in the
following papers:
                               
Bastian Leibe, Ales Leonardis and Bernt Schiele,                    
Robust Object Detection with Interleaved Categorization and Segmentation
In International Journal of Computer Vision, Vol.77, No. 1-3, May 2008.

Bastian Leibe, Krystian Mikolajczyk, and Bernt Schiele,
Segmentation-Based Multi-Cue Integration for Object Detection
In British Machine Vision Conference (BMVC'06), 2006.



Contents:
=========
I.     Quick-Start Instructions
  I.1  Using the Provided Detectors
  I.2  Important Detector Parameters
  I.3  Using a Ground Plane Calibration
  I.4  Caveats
II.    In-Detail Description of the GUI
  II.1 Description of the Main Window Interface Elements
  II.2 Description of the Detector Window GUI
  II.3 Description of the Feature Extraction Window GUI
III.   Description of the Command Line Interface
IV.    Description of the File Formats Used
  IV.1 IDL Format
  IV.2 Calibration File Format
  IV.3 Matlab-Readable Recognition Result Format
  IV.4 Matlab Workspace Format for Storing Result Segmentations



I. Quick-Start Instructions
===========================

I.1.) Using the Provided Detectors
----------------------------------

I.1.0.) After the program has been installed, it can be started by
typing "./start.sh &" from the command line interface.

I.1.1.) Before anything can be recognized, the system first needs to
load a detector file. Several pre-trained detectors are already
available with the installed code. Others can be downloaded from our
webpage (http://www.vision.ee.ethz.ch/bleibe/code/). In order to load
a detector, click on the button "Add Detector" on the left side of the
program window. A new window with the title "Detector 1" will open
up. Click on the "Load" button in the bottom row of this window and
select a detector file for loading (detector files can be recognized
by the suffix ".det"). Once the detector is fully loaded, it will be
displayed in the detector table on the left side of the program
window. 

Note that when loading a detector, all GUI parameters will be
automatically set to the saved default values for this
detector. If the imaging conditions of the test data vastly differ
from those used during training, it may be beneficial to
adapt some of those parameters. How that is done is explained in
Section I.2 below.

I.1.2.) Now the system is ready to recognize objects of the trained
category. Click on "Process Test Image" to load an image and start the
recognition process. The system will automatically compute interest
points with the selected detector, extract local features from the
detected regions, match them to the codebook, and generate
probabilistic votes in a Hough voting space. The maxima in this space
are then extracted using Mean-Shift-Mode-Estimation and form, if they
surpass a certain threshold ("Thresh Single" in the detector
parameters), the initial object hypotheses. If the MDL hypothesis
verification stage is activated (check boxes "Do MDL Selection" and
"Rej. Savings<" in the "Hypothesis Section"), the system then verifies
the hypotheses and only accepts those that surpass a second threshold
("Rej. Savings<"). Details of the underlying algorithm can be found in
the paper mentioned at the beginning of this document. The system
draws a rectangle around every detection and opens a window to display
the results in detail. This window shows one automatically computed
top-down segmentation for each of the accepted hypotheses. Additional
outputs can be displayed by selecting the corresponding visualization
options in the "Output" and "Display" tabs (see Section II.1.7).


I.2.) Important Detector Parameters
-----------------------------------

When loading one of the provided detectors, all of its parameters will
automatically be set to its default settings (optimized for our
training/test data sets). When imaging conditions differ considerably
from the training conditions, it may be beneficial to adapt some
parameters. This section introduces the most important parameters and
describes their effects. All other parameters can (and should) in
general be left at their default settings.

I.2.1) Final Hypothesis Verification Threshold
The most important recognition parameter is the final hypothesis
verification threshold in the field "Reject Savings<" on the right
hand side of the main program window, This threshold defines the
minimum MDL score for a valid hypothesis such that it is still
accepted (see II.1.14.b for a detailed description). By lowering this
score, more detection hypotheses will be returned. When applying the
detectors on new test data, it is therefore often useful to lower this
threshold in order to get a feeling for the approach's limits, i.e. to
see which objects are still detected with a lower score and which ones
are missed entirely.

Note that when lowering this threshold, it is usually necessary to
lower the Detector thresholds, too, in order for this change to have
an effect! This is described in the following section.

I.2.2) Individual Detector Thresholds (Voting, MDL)
Before passing hypotheses to the final verification procedure, each
detector on its own already makes a pre-selection in order to filter
out low-scoring detections that will not survive the verification
anyway. This is done in two steps. Both steps can be influenced by
adapting the corresponding parameters in the Detector window.

First, an initial recognition threshold ("Reco --> Thresh Single",
see II.2.12) specifies which maxima in the Hough voting space are kept
as initial hypotheses. For each such hypothesis, the program then
computes a top-down segmentation and derives the hypothesis's MDL
score (before interactions are taken into account). Secondly, this
computed MDL score is checked against another threshold ("MDL -->
Rej. Savings<", see II.2.13) in order to filter out too weak
hypotheses. It is important to keep in mind that when the final
verification threshold from I.2.1 is lowered, this second threshold
also needs to be adapted.

In addition, when imaging conditions differ considerably from the
training conditions, the initial "Thresh Single" threshold can be
relaxed to a lower setting in order to permit more initial hypotheses
to survive until the MDL verification stage. However, a lower value of
"Thresh Single" will increase computational load and thus slow down
recognition.

I.2.3) Area Factor
Different object types may take up different image areas (e.g. a car's
side view is much larger than its rear view). Since the final MDL
verification integrates per-pixel likelihoods over the hypothesized
object area, this may bias recognition scores in favor of larger
categories. In addition, the absolute range of a detector's scores may
also depend on the size and quality of its training set. When working
with several different detectors, both of those influences need to be
balanced out. This can be done by specifying a different "Area Factor"
for each detector (in the Detector window, see II.2.3), which is taken
as a normalization constant during the hypothesis combination
procedure. Each detector's recognition scores will be divided by this
area factor.

I.2.4) Detector Scale Range
The ISM detector automatically performs multi-scale image
analysis. That is, it tries to detect objects of the target category
regardless of their size in the image. However, it can intuitively be
seen that this search over scales becomes more expensive the larger
the scale search range is. The detection procedure can therefore be
sped up if this range can be restricted by application-specific
information.

This can be done by adapting the fields "Reco --> min Scale" and "Reco
--> max Scale" in the Detector window (see II.2.12). Those fields
determine the search scale range for recognition (relative to the
training scale 1.0). The default range is [0.3,1.5]. If larger objects
shall be recognized, the "max Scale" value needs to be increased. When
doing that, the following two points have to be kept in mind.
- The scale search range needs to be slightly larger than the
  effective object range, so that the corresponding object location and
  scale can be identified as a local maximum in Hough space. I would
  therefore recommend keeping a safety margin of about 0.5 for the "max
  Scale" value.
- When the upper scale limit exceeds 2.0, the interest point scale
  range should also be adapted, as explained in the following Section
  I.2.5).

I.2.5) Feature Extractor Scale Range
For similar reasons as detailed in I.2.4, the employed interest point
detectors are also applied with a limited scale search range. When
trying to recognize objects at very large scales (e.g. in
high-resolution images), those interest point detectors need to be
adapted to extract features at larger scales. This can be done using
the "Scale --> min Scale" and "Scale --> max Scale" fields in the
Feature window (see II.3 and II.3.4 for details). The default setting
for those values is [1.0,32.0] (allowing recognition under scale
changes of up to a factor of 2, relative to the training scale). If
larger scale changes are to be tolerated during recognition, the "Max
Scale" value needs to be increased accordingly (to 48.0 or 60.0 for
scale factors up to 4 or 5, respectively).


I.3.) Using a Ground Plane Calibration
--------------------------------------

In general, detection performance can be improved considerably if
scene geometry information in the form of a ground plane calibration
is available. In order for the detector to use this information, three
conditions have to be fulfilled. First, the ground plane information
has to be available in a text file according to the specifications
described in IV.2. Second, the detector options need to be set such
that the detector makes use of the available 3D information (see
II.2.5, II.2.7). Finally, the main GUI options should be set such that
the calibration scale is correctly converted to meters (see II.1.15). 

We strongly advise using ground plane information wherever it is
available, as this can really bring a significant performance increase
both in detection accuracy and in run-time efficiency.


I.4.) Caveats
-------------

When experimenting with the provided detectors, it is important to
keep in mind the approach's limitations and adjust one's expectations
accordingly.

- The ISM approach has been designed with the goal to detect and
localize novel instances of a given visual category that are seen from
the same viewpoint or aspect. This means that a detector trained on
side views of cars will typically not be able to recognize rear or
front views of cars. Thus, for many real-world categories, it will be
necessary to combine several different detectors. On the ISM webpage,
we provide example detectors for different viewpoints of cars
(http://www.vision.ee.ethz.ch/bleibe/ism). For some other categories,
different viewpoints may be sufficiently similar that the same
detector reacts to all of them. For example, the provided pedestrian
detector was trained mainly on side views, but it will often react
also to front or rear views although typically with a lower
confidence.

- In addition, the provided detectors are sensitive to contrast. They
typically work better in images where contrast is good. This is mainly
a limitation of the underlying local feature extractors. If the input
images are low in contrast, those detectors typically find fewer
features, rendering object detection more difficult. Similarly, very
strong contrast regions (e.g. due to bright lighting and hard shadows
in the image) often yield a large number of local features, which
might bias detection to create more hypotheses there. If the contrast
settings of a test scenario are known, it is therefore advisable to
either adjust the gamma factor of the input images or the detector
parameters accordingly.

- One major advantage of the ISM approach, compared to many monolithic
detectors, is that it can recognize objects under significant partial
occlusion. However, this also means that the approach may return false
positives due to partial object structures. This may happen for
example for pedestrian detection, where certain road markings contain
similar shapes as a pedestrian's legs. We are currently working on an
extension of the approach to reduce those effects.

- In general, detection performance can be improved considerably if
scene geometry information in the form of a ground plane calibration
is available, as described in I.3. If such information can be obtained
(even if it's not that accurate), we strongly advise using it in the
detector.



II. In-Detail Description of the GUI
====================================

II.1.) Description of the Main Window Interface Elements
--------------------------------------------------------

Left-Hand Side:
---------------

II.1.1.) Load Test Image
loads a new image and displays it in the main window.

II.1.2.) Better RGB->Gray Conversion 
This option is just provided for compatibility reasons. Some older
experiments were performed using the suboptimal RGB->Gray conversion
according to the formula I = (R+G+B)/3. The newer version uses the
more exact conversion I = (0.3*R+0.59*G+0.11*B). In order to replicate
those older experiments, the older formula can be selected by
unchecking this checkbox.

II.1.3.) Perform Gamma Normalization
Dalal & Triggs reported a performance improvement in their detection
system when performing a Gamma normalization on their test images
prior to feature extraction (Dalal & Triggs, CVPR'05). This option
allows to perform a similar gamma normalization, where each pixel
grayvalue is replaced by its square root prior to feature extraction.
In my experiments, this option did however not yield a consistent
improvement.

II.1.4.) Add Detector
This button adds a new detector to the system. It opens a new
"Detector" window, where the detector parameters can be set manually,
or where a pre-defined detector can be loaded. See Section II.2 for
details about the detector parameters.

II.1.5.) Table of Detectors.  
This table contains an entry for each currently loaded detector. The
table columns summarize the following information:
- The detector's target category (e.g. "car", "motorbike"),
- its target pose (e.g. "side", "rear"), 
- whether or not the detector is also applied to a mirrored version of
  the image (see II.2.8),
- the detector's initial voting threshold (see II.2.12 below)
- its assigned bounding box color.

In general, several detectors can be loaded and executed in
parallel. Each detector has his own parameter window, which can be
opened by double-clicking the corresponding list entry (see II.2 for
information about the detector GUI). The program is written with the
goal to reuse as much existing information as possible. Thus, if two
detectors are based on the same features, they will share the outputs
of the feature detector. Also, if two detectors are based on the same
codebook, they will share this codebook, so that the extracted
features have to be matched to it only once.

II.1.6.) Table of Cues.
This table contains an entry for each currently loaded feature
extractor. The table columns summarize the following information:
- The employed interest point detector,
- the feature descriptor,
- whether or not the feature extractor is also applied to a mirrored
  version of the image,
- the minimum feature scale,
- the maximum feature scale.

This table is updated automatically when new detectors are loaded. As
stated above under II.1.5, the program tries to reuse as much
information as possible. Thus, if two detectors are based on the same
features, they will automatically share the underlying feature
extractor. Each feature extractor also has its own parameter window,
which can be opened by double-clicking on the corresponding table
entry (see II.3 for information about the feature extractor GUI).

II.1.7.) GUI Options 
The following options can be used to display additional outputs of the
object detector. They do not affect the algorithm's results, but they
may affect its runtime.

II.1.7.a) Drawing tab
The first 2 options specify what should be drawn as the support of
a hypothesis (see II.1.7.c)). If "Draw Maps" is checked, the hypothesis
segmentation will be shown. If "Draw Confidence" is checked, the
p(figure) probability map is shown instead. If "Draw Tight BBoxes" is
checked, the result image not only displays the regular bounding box
for each hypothesis, but also draws the bounding box of the
segmentation (which is sometimes more accurate). The last option
"Eval. Unique Contrib." is only experimental and should be left
unchanged.

II.1.7.b) Output tab
Those options determine what output should be displayed on the command
line. The first four options specify different levels of detail for
this output: just the algorithm's "Main Steps", additional "Details",
the intermediate "Voting Results", and the final "MDL Results". The
final option "Show Timings" displays detailed timing results for the
individual steps of the algorihtm.

II.1.7.c) Display tab
Those options allow to display additional graphical output. The first
option selects whether interest points should be drawn into the input
image. The following three options open result windows displaying the
Hough "Voting Space", each hypothesis's "Support" (see also
II.1.7.a)), and the result "Segmentations". The last option
additionally displays each accepted hypothesis's result segmentation
in the bottom part of the program window.

WARNING: If the input images are very large (such as those from the
MIT LabelMe Database), the additional outputs may take up a lot of
space and may eventually crash the computer when its memory limit is
reached.

II.1.8.) Save Images
saves the currently displayed images under a given file name.

II.1.9.) Save Segmentations 
saves the segmentation for the current result image in 3 separate
files: one for the p(figure) probability map, one for the p(ground)
probability map, and one for the final segmentation. Only one file
name needs to be specified -- the others are generated automatically.


Right-Hand Side:
----------------

II.1.10.) Process Test Image
start the recognition process. The system asks for a test image and
applies each of the loaded detectors in sequence. For each detector,
it extracts and matches image features to the detector's codebook,
then generates probabilistic votes for the position of the object
center in a Hough voting space, extracts maxima from this space as
initial hypotheses, and computes a top-down segmentation for each
generated hypothesis. The resulting hypotheses are then pooled and
combined in an MDL hypothesis verification procedure (see II.1.14). 

The basic idea behind this hypothesis combination and verification
procedure is that each pixel can belong to at most one object. Thus,
all hypothesized detections compete for pixels, which results in
interaction costs. The algorithm tries to find the optimal combination
of hypotheses, such that the total sum of their scores (their
"savings" in the terminology of the algorithm) minus their interaction
cost is maximized. The final acceptance decision is made based on the
threshold in II.1.14.

As objects occurring at different scales take up different portions of
the image, an automatic scale normalization is performed as part of
the algorithm. However, different object types may also take up
different image areas (e.g. a car's side view is much larger than its
rear view). In addition, the absolute range of a detector's scores may
also depend on the size and quality of its training set. When working
with several different detectors, both of those influences need to be
balanced out by hand. This can be done by specifying a different "Area
Factor" for each detector (see Section II.2.3), which is taken as a
normalization constant during the hypothesis combination procedure.

II.1.11.) Perform IDL Test 
starts a test series on a whole set of images, where the image set is
specified by an annotation file in a special file format (suffix
".idl", details to this format are described in Section IV.1). The
function by default generates one result file containing the detection
bounding boxes for each image, together with the final hypothesis
scores. Optionally, two additional kinds of output can be written to
disk using the following checkboxes.

II.1.12.) Write Result Images 
This option stores the result images with detection bounding boxes in
png format. The program asks for a result directory in which to store
those images. In addition, the program writes out a Matlab-readable
text file containing more detailed information about each detection
(the corresponding file format is described in Section III.2).

II.1.13.) Write Segmentations
This options writes out the result segmentations as a matlab workspace
(suffix ".mat", see Section IV.4 for details) for each image. As
also in II.1.12, the program additionally writes out a
Matlab-readable text file containing more detailed information about
each detection (the corresponding file format is described in Section
IV.2).

II.1.14.) Hypothesis Selection tab 
This tab contains options and parameters for the final MDL hypothesis
verification. If only a single detector is used, this stage is
identical to the one described in our IJCV paper. If multiple
detectors are run in parallel, the same stage can also be used to
combine their detection results. In that case, it however becomes
important to balance out their outputs. This is necessary, as
different object types may take up different image areas (e.g. a car's
side view is much larger than its rear view). In addition, the
absolute range of a detector's scores may also depend on the size and
quality of its training set. When working with several different
detectors, both of those influences need to be balanced out by
specifying a different "area factor" for each detector (see Section
II.2.3), which is taken as a normalization constant during the
hypothesis combination procedure.

II.1.14.a) Do MDL Selection
This option determines whether the outputs of the individual detectors
should be combined in the final MDL hypothesis selection stage, or if
all candidate hypotheses should simply be displayed without MDL
verification. This option is useful for visualizing the effect of the
MDL stage. It can also be useful if the detector results shall be read
in to Matlab and combined with additional information there (in that
case, Option I.2.13 should be checked).

II.1.14.b) Hypothesis Selection options
determines which method for hypothesis verification is used. Two
options are possible: the MDL criterion (check boxes "Do MDL
Selection" and "Rej. Savings<" selected) and/or the Bounding Box
criterion (check box "Rej. Overl>" selected). The text fields allow to
specify the percentage of overlap for the bounding box criterion, and
the minimum MDL score for a valid hypothesis, respectively. The
parameter "K2/K0" determines how much the MDL criterion should trust
the size of a segmentation as opposed to its supporting p(figure)
score. It can be varied between about 0.90-0.95 (both count equally)
and 1.0 (only the p(figure) score counts).

II.1.15.) Ground Plane tab
This tab contains two parameter fields that are used when working with
a ground plane calibration. Note that the use of a ground plane
requires a calibration file to be available (see IV.2 for details to
the calibration file). In addition, the detectors need to be
explicitly set to use the ground plane (see II.2.5, II.2.7).

For many test datasets we used in our experiments, we rescaled all
images to twice their original size for object detection. In those
cases, the calibration files however still refer to the original image
size. Therefore, all image coordinates need to be divided by a factor
of 2 prior to applying the calibration. This can be accomodated for by
setting the "Image Scale" parameter to 2. The second field "World
Scale" is used to convert the calibrated world coordinates to
meters. Depending on the test set, it may be necesary to adjust this
parameter, so that the object size prior can be properly expressed
during recognition.

II.1.16.) Verification tab
The options on this tab can be used to activate the Chamfer
verification described in our CVPR'05 paper (if a set of silhouettes
has been loaded, see II.1.17). However, the Chamfer verification code
is not optimized and runs very slowly. In addition, advances on the
feature detection side have in the meantime improved detector
performance to a level such that the Chamfer verification brings no
further advantage. This option therefore cannot be recommended and is
only left for compatibility purposes.

II.1.17.) Load Silhouettes
Loads a set of silhouettes for the Chamfer verification stage. This
functionality is left in the program for compatibility purposes, but
should not be used in regular experiments. (The silhouettes needed for
this are not included in the regular code distribution, but may be
provided upon request).

II.1.18.) Display Scale Footprint
displays a histogram of the interest point scales detected in the
current image. Useful for debugging purposes.

II.1.19.) Quit
terminates the program.


II.2.) Description of the Detector Window Interface Elements
------------------------------------------------------------

This window contains the parameter settings for a single detector. It
can be accessed by double-clicking on the corresponding entry in the
Table of Detectors (see II.1.5). The optimal settings for the provided
codebooks are set automatically upon loading the corresponding
detectors.

II.2.1.) Category
specifies a name for the detector's target category (e.g. "cars",
"cows,...)

II.2.2.) Pose
specifies a name for the detector's target pose (e.g. "side",
"rear",...)

II.2.3.) Area Factor
Different object types may take up different image areas (e.g. a car's
side view is much larger than its rear view). Since the final MDL
verification integrates per-pixel likelihoods over the hypothesized
object area, this may bias recognition scores in favor of larger
categories. In addition, the absolute range of a detector's scores may
also depend on the size and quality of its training set. When working
with several different detectors, both of those influences need to be
balanced out. This can be done by specifying a different "Area Factor"
for each detector, which is taken as a normalization constant during
the hypothesis combination procedure. Each detector's recognition
scores will be divided by this area factor.

II.2.4.) Size(m)
specifies the target category's mean size (in meters). This parameter
only has an effect when working with a ground plane calibration.

II.2.5.) Size Variance
specifies the target category's size variance (in m^2). The adjacent
check box determines if this variance shall be used to weight the
detection scores (or if just a hard ground plane corridor shall be
used, see also II.2.7). This parameter only has an effect when working
with a ground plane calibration and when the "Ground Plane Filter"
option from II.2.7 is selected (see also II.1.15 and IV.2).

II.2.6.) Dist. from Center
specifies the distance from the object's bounding box footpoint to its
3D center point (in meters). This can be used in order to let several
single-view detectors (e.g. "frontal car" and "semi-profile car")
agree on a common object center. This parameter only has an effect
when working with a ground plane calibration.

II.2.7.) Use Ground Plane Filter
specifies whether the ground plane shall be used in order to limit
object detection to a corridor in the (x,y,scale) volume. This
parameter only has an effect when a ground plane calibration is
available. If such a calibration is available, the "Size Variance"
from II.2.5 can additionally be used in order to weight the detection
scores accordingly (see also II.1.15 and IV.2).

II.2.8.) Mirror Image
When this option is selected, the detector will additionally be
applied to a mirrored version of the input image. This can be used for
convenience, since the detectors then need to be trained only for a
single direction (e.g. for cows walking left).

II.2.9.) Add Cue
This button can be used to build up a custom detector by adding
another cue to it. Here, a "cue" means the combination of a
pre-trained codebook and occurrence file based on the same basic
features (i.e. the same interest point extractor and feature
descriptor). There is no restriction to the number or type of cues to
be added. However, in practice, a detector usually consists of between
1 and 3 cues. The way how cues are combined is described in our
BMVC'06 paper.

II.2.10.) Table of Cues
This table contains an entry for each cue assigned to this
detector. The table columns summarize the following information:
- The employed interest point detector,
- the feature descriptor,
- whether or not the feature extractor is also applied to a mirrored
  version of the image,
- the codebook size (number of stored cluster centers)
- the number of stored occurrences.

This table is updated automatically when new cues are added. As
stated above under II.1.5, the program tries to reuse as much
information as possible. Thus, if two detectors are based on the same
features, they will automatically share the underlying feature
extractor. Each feature extractor also has its own parameter window,
which can be opened by double-clicking on the corresponding table
entry (see II.3 for information about the feature extractor GUI).

In addition, this table is linked to the Table of Cues in the main GUI
(see II.1.6). When changing the min or max scale of a feature
extractor in any of the loaded detectors, the corresponding values are
automatically updated in the main GUI table (note that the "Return"
key may have to be pressed for this to happen). However, the opposite
is not true.

II.2.11.) MSME Tab
The first 5 fields determine the radius of the MSME kernel window in
x-, y-, scale-, aspect-, and rotation-direction. This window size
corresponds to a tolerance to small alignment changes. The current
settings are optimized for a training object size of ~200 pixels
(maximum of width and height). For regular scale-invariant
recognition, only the first 3 size parameters are used. The aspect and
rotation window sizes are only used for research purposes and are not
fully functional. The subsequent check boxes are responsible for
performing the correct scale normalization. In general, the parameters
on this tab should be left at their default settings.

II.2.12.) Reco Tab
This tab contains various recognition parameters. The initial
recognition threshold ("Thresh Single") specifies which maxima in the
Hough voting space are kept as initial hypotheses. The optimal value
for the provided codebooks is set automatically upon loading the
corresponding detectors. When imaging conditions differ considerably
from the training conditions, this value can be relaxed to a lower
setting in order to permit more initial hypotheses to survive until
the MDL verification stage. When the MDL verification is selected, the
exact value of this threshold is not as important anymore, since the
MDL stage is powerful enough to reject additional false
positives. However, a higher value of "Thresh Single" will reduce
computational load and thus speed up recognition.

The two fields "Obj. Width" and "Obj. Height" determine the size of
the detection bounding box that is drawn for each detection. When
using one of the provided detectors, these parameters should be left
unchanged. "Extend Rg." can be ignored. The fields "min Scale" and
"max Scale" determine the search scale range for recognition (relative
to the training scale). The default range is [0.3,1.5]. If larger
objects shall be recognized, the "max Scale" value must be increased
(Note that when the upper scale limit exceeds 2.0, the interest point
scale range must also be adapted, see II.3.4). The "min Vote Wt." and
"max Vote Wt" fields, finally, should be left unchanged.

II.2.13.) MDL Tab
This tab originally specified which method for hypothesis verification
was used. However, since the newest version of our code permits to
execute several detectors in parallel, this functionality has been
transferred to the main GUI (see II.1.14). Most options on this tab
can therefore be ignored. The only important remaining parameter is
the "Rej. Savings<" field. This parameter defines a threshold
restricting which hypotheses are passed on to the main program for
hypothesis verification, already using the same score as in the later
MDL stage. When loading a predefined detector, this value is
initialized to its optimal setting for our test data sets. However,
when imaging conditions differ considerably from the training
conditions, this threshold can be adapted to a lower setting in order
to allow more hypotheses to survive until the final MDL verification
stage.

The field "adapt for scales>" governs the scale normalization and
should be left unchanged. The final option determines if a cubical or
spherical/ellipsoidal MSME kernel shall be used. In our experience,
both perform equally well, but the cubical kernel can be evaluated
faster.

II.2.14.) Misc Tab
This tab only contains one relevant parameter: the checkbox "Use fast
MSME" determines whether a fast approximation shall be used instead of
the exact values in order to select the initial MSME starting
locations. This option can be generally recommended, as it brings a
considerable speedup without affecting recognition performance too
much. The other parameters on this tab belong to experimental options
which should be left at their default settings.

II.2.15.) Misc2 Tab
This tab contains several experimental options which should be left at their default settings.

II.2.16.) Load Button
This button can be used to load a predefined detector (detector files
can be recognized by the suffix ".det"). Once the detector is fully
loaded, it will be displayed in the detector table on the left side of
the main program window.

Note that when loading a detector, all GUI parameters will be
automatically set to the saved default values for this detector. If
the imaging conditions of the test data vastly differ from those used
during training, it may be beneficial to adapt some of those
parameters. How that is done is explained in Section I.2.

II.2.17.) Save Button
This button can be used to save the current detector parameters,
including all codebooks.

II.2.18.) Clear Button
This button removes all cues loaded for this detector and clears the
parameter fields.


II.3.) Description of the Feature Window Interface Elements
-----------------------------------------------------------

This window contains the parameters for a single feature extractor. It
can be accessed by double-clicking the corresponding entry in the
"Table of Cues" of either the main GUI (see II.1.6) or a detector
window (see II.2.10). Note that since the loaded detectors are based
on specific interest point extractors and feature descriptors,
changing those settings will result in unpredictable behavior and may
crash the program. The only fields which sometimes need to be adapted
in this window are the "Min Scale" and "Max Scale" fields (see
II.3.4).

II.3.1.) Detector tab
Local feature extraction can be done using either a uniform sampling
scheme or different interest point detectors. For the Harris,
Exact-DoG, and SURF detectors, the "Param." tab reveals a set of more
detailed options ({"Sigma1", "Sigma2", "Alpha", "Thresh"}, {"Scale
Octaves", "Levels/Octave", "Sigma0", "Threshold"}). In practice, they
can be left unchanged. All other interest point detectors are operated
at their default settings. If segmentation masks are available for the
training images, the option "Use only figure area" can be used to keep
only interest points which sufficiently overlap with the object (where
"sufficiently" is defined by the value of "Min. Figure Pixels" in the
"Params" tab. This option specifies the minimum number of object
pixels relative to a 25*25 pixel patch).

II.3.2.) Features tab
These options determine the type of features that are computed for
each interest region. Many options are available here, from simple
25*25 image patches, to SIFT, GLOH, or Shape Context features. The
option "Make Rotation Invariant" can be used for all features (except
"Patches") to use a rotation-invariant representation.

II.3.3.) Params tab
contains parameters for certain interest point operators (as explained
in II.3.1.a).

II.3.4.) Scale tab
The "Min Scale" and "Max Scale" fields can be used to determine the
scale range of the interest point detector. When training a new
detector, it is often useful to restrict the scale range, e.g. to
[1.9,16.0]. For later recognition, these values should however be set
to a larger scale range of e.g. [1.0,32.0] (allowing recognition under
scale changes of up to a factor of 2, relative to the training
scale). If larger scale changes are to be tolerated during
recognition, the "Max Scale" value needs to be increased accordingly
(to 48.0 or 60.0 for scale factors up to 4 or 5, respectively).



III. Description of the Command-Line Interface
==============================================

In addition to the interaction possibilities via the GUI, the program
parameters can also directly be set via the command line options. This
even makes it possible to run the program entirely without GUI.

USAGE: mcmatcher [OPTIONS]
  -nw       : no gui
  -t   T    : set final recognition threshold to T (see I.2.1)
  -minsc S  : set min detection scale to S (rel. to training scale,
              see I.2.4)
  -maxsc S  : set max detection scale to S (rel. to training scale,
              see I.2.4)
  -nomdl    : disable MDL hypothesis selection stage (see I.1.14a)
  -imagesc S: image scale factor for ground plane calculation (see II.1.15)
  -worldsc S: world scale factor for ground plane calculation (see II.1.15)
  -det FILE : load detector from FILE
              (can occur several times to add more detectors, see II.1.4)
  -img FILE : process single image from FILE (see II.1.10)
  -idl FILE : process a set of test images from IDL FILE (see II.1.11)
  -out FILE : result IDL file for output
  -odir DIR : result directory for detailed output
  -timings F: enable (F=1) or disable (F=0) timing output
  -q        : quiet mode (no text output)
  -v        : verbose output
  -vv       : very verbose output

Additional parameters can be made available via command line options
upon request.



IV. Description of the File Formats Used
========================================

In the following, we describe the different file formats that are used
in our program.

IV.1) IDL Format
----------------
The IDL files are used for three purposes: for specifying a list of
test images to process in sequence (see II.1.11), for storing the
recognition results of such a test run, and for storing the ground
truth annotations of the sequence (in a different file). For each
image, the file format lists a set of bounding boxes + recognition
scores, separated by commas. The boxes contain upper-left and
lower-right corner, but are not necessarily sorted according to
this. A semicolon ends the list of bounding boxes for a single file, a
period ends the file.

"filename": (x1, y1, x2, y2):score, (x1, y1, x2, y2):score, ...;

If there are no annotations (as in the case when just a batch list of
test images shall be specified, each line just contains the file name,
followed by a semicolon.

"filename";

The ground truth annotation, finally, do not contain score values, so
the corresponding lines boil down to the following format.

"filename": (x1, y1, x2, y2), (x1, y1, x2, y2), ...;

A simple Matlab reader for the IDL format is available here:
http://www.vision.ee.ethz.ch/~aess/iccv2007/readIDL.m


IV.2) Calibration File Format
-----------------------------
Many test sets that are available from our website
(http://www.vision.ee.ethz.ch/~bleibe/data/datasets.html) come with a
subdirectory "maps" containing either a single calibration file
"camera.default" (in the case of a static camera), or a separate
calibration file "camera.XXXXX" for every frame (in the case of a
moving camera). In the latter case, those calibrations were
automatically obtained using the Structure-from-Motion approach by
Cornelis et al., CVPR'06.

Calibration files contain the calibration for one image at a time (K
[3x3], rad [1x3], R [3x3], t [1x3], GP[1x4]), with K the internal
calibration, rad the radial distortion coefficients, R/t external
calibration, world -> camera (i.e. X_cam = R X_world + t), and GP the
ground plane coordinates (in the form GP(1:3)x - GP(4)=0). For your
convenience, we provide the Matlab function read_camera.m (available
in the subdirectory "matlab" or from
http://www.vision.ee.ethz.ch/~bleibe/data/read_camera.m), which
demonstrates how to read in the camera parameters.

Please note that for many datasets, we rescaled all images to twice
their original size for object detection. In those cases, the
calibration files still refer to this original size. Therefore, all
image coordinates need to be divided by a factor of 2 prior to
applying the calibration, which can be done by setting the "Image
Scale" parameter to 2 (see II.1.15). For the static sequences, the
world scale is already expressed in meters.


IV.3) Matlab-Readable Recognition Result Format
-----------------------------------------------
This kind of result file is written out to disk when processing an
image list (see II.1.11) and either option II.1.12 or II.1.13 is
selected. The file format is a tab-separated text matrix containing
the following information, where each line corresponds to one object
hypothesis. An example Matlab script for loading in result files in
this format is available in the file "load_detections.m" in the
"matlab/" subdirectory distributed with this archive.

Column   Content
   1   - Image number
   2   - Hypothesis number
   3   - Object center, x coordinate
   4   - Object center, y coordinate
   5   - Object scale
   6   - Object category (the first loaded detector has the label "0", etc.)
   7   - Object bounding box, top left, x coordinate
   8   - Object bounding box, top left, y coordinate
   9   - Object bounding box, bottom right, x coordinate
  10   - Object bounding box, bottom right, y coordinate
  11   - Initial voting score
  12   - Final MDL score
  13   - Real-world distance to object footpoint (requires calibration)
  14   - Real-world object height (requires calibration)
  15   - Real-world object top point, x coordinate
  16   - Real-world object top point, y coordinate
  17   - Real-world object top point, z coordinate
  18   - Real-world object footpoint, x coordinate
  19   - Real-world object footpoint, y coordinate
  20   - Real-world object footpoint, z coordinate
  21   - Real-world object main axis direction, x coordinate
  22   - Real-world object main axis direction, y coordinate
  23   - Real-world object main axis direction, z coordinate
  

IV.4) Matlab Workspace Format for Storing Result Segmentations
--------------------------------------------------------------

When performing a test run over an entire list of input images, option
II.1.13 can be selected to store a result segmentation for each
obtained detection. This will write out a Matlab workspace for each
detection, containing the following data fields:

pfig     - Figure probability map "pfig"
pfig_xmn - x offset of pfig
pfig_ymn - y offset of pfig
pgnd     - Ground probability map "pgnd"
pgnd_xmn - x offset of pgnd
pgnd_ymn - y offset of pgnd

In order to save space, only the rectangular part of the pfig and pgnd
maps that actually contains non-zero entries is stored in the
workspace. In order to reconstruct the full maps, one therefore has to
copy the stored content into an image-sized array, e.g. as follows:

(imh, imw) = size(image);

(h,w) = size(pfig);
pfig_full = zeros(imh, imw);
pfig_full(pfig_ymn:pfig_ymn+h-1, pfig_xmn:pfig_ymn+w-1) = pfig;




If any of the above steps doesn't work or produces errors, please
contact me by email (leibe@vision.ee.ethz.ch)

Feb 29, 2008
Bastian Leibe
