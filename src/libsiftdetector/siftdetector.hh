#include <vector>
#include "sift.h"
#include "opinterestimage.hh"

#include "mypgm.hh"
using namespace std;
/*======================================================
 * The stardard using method is:
 *  initializeAllPreliminaryData();
 *  calculateAllSIFTDescriptor(); or calculateAllSIFTDescriptorofUprightOrientation();
 *
 * The resulting descriptors can be retreived with
 *  getDescr();
 *
 * Note InterestPonits contains pure scale value, while
 * KeyPoints contain octave and scale/
 *
 * Author : Zhang Yuren
 * Date:    2013.6.16
 * =====================================================*/

class SiftDetector
{
public:
	SiftDetector(const PGMImage& image, const PointVector& interetPointList);
	~SiftDetector();
	
	void loadAllInterestPoints(PointVector interestPointList);
	/*****************************************************************/
	/*                  Preliminary Data Initialzation               */
	/*****************************************************************/
	void initializeAllPreliminaryData();
	// The meaning of KeyPoints follows the defination of VLFEAT 
	void initializeKeyPointList();
	void initializeOrientationList();
	void initializeDescrList();

	void calculateAllSIFTDescriptor();
	void calculateAllSIFTDescriptorofUprightOrientation();
private:
	void initializeRawData(const PGMImage& image);
	void updateAllKeyPointsOrientationsandSiftDescriptorsforCurrentOctave();
	double calculateKeyPointOrientation(VlSiftKeypoint keypoint);
public:
	/*****************************************************************/
	/*                            Querry                             */
	/*****************************************************************/
	vector< vector<float> > getDescr();
	vector<VlSiftKeypoint> getKeyPointList();
	vector<double> getOrientationList();
	double getPatchScaleRatio();
private:
	bool bUseExternalInterestPoints;
	bool bUseUprightOrientation;

    vl_sift_pix* ptrRawImgData;	
	
	PointVector InterestPointList;
	vector<VlSiftKeypoint> KeyPointList;
	vector<double> OrientationList;
	vector< vector<float> > DescrList;

	VlSiftFilt* pSiftFilter;
	
};
