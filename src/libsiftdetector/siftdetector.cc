/*=========================================================
 * TODO:
 *  Implement the internal interestpoint detection method
 *  =======================================================*/
#include <iostream>
#include "siftdetector.hh"

using namespace std;

SiftDetector::SiftDetector(const PGMImage& image, const PointVector& interestPointList)
{
	initializeRawData(image);
	loadAllInterestPoints(interestPointList);
	pSiftFilter = vl_sift_new(image.width(), image.height(), -1, 3, 0);
	bUseUprightOrientation = false;
}

SiftDetector::~SiftDetector()
{
	free(ptrRawImgData);
	vl_sift_delete(pSiftFilter);
}

void SiftDetector::loadAllInterestPoints(PointVector interestPointList)
{
	InterestPointList = interestPointList;
	bUseExternalInterestPoints = true;
}

void SiftDetector::initializeAllPreliminaryData()
{
	initializeOrientationList();
	initializeKeyPointList();
	initializeDescrList();
}

void SiftDetector::initializeKeyPointList()
{
	VlSiftKeypoint SiftKeypoint;
	KeyPointList.clear();
	for(int i=0; i<InterestPointList.size(); i++)
	{
		vl_sift_keypoint_init(pSiftFilter, &SiftKeypoint, InterestPointList[i].x, InterestPointList[i].y, InterestPointList[i].scale);
		KeyPointList.push_back(SiftKeypoint);
	}
}

void SiftDetector::initializeOrientationList()
{
	OrientationList.assign(InterestPointList.size(), 0);
}

void SiftDetector::initializeDescrList()
{
	vector<float> descr(128, 0);
	DescrList.assign(InterestPointList.size(), descr);
}

void SiftDetector::calculateAllSIFTDescriptor()
{
	int nOctave = 0;
	int result;
	result = vl_sift_process_first_octave(pSiftFilter, ptrRawImgData);
	while(result != VL_ERR_EOF)
	{
		cout<<"Calculating descriptor in octave "<<nOctave<<endl;
		updateAllKeyPointsOrientationsandSiftDescriptorsforCurrentOctave();
		result = vl_sift_process_next_octave(pSiftFilter);
		nOctave++;
	}
}

void SiftDetector::calculateAllSIFTDescriptorofUprightOrientation()
{
	bUseUprightOrientation = true;
	calculateAllSIFTDescriptor();
}

vector< vector<float> > SiftDetector::getDescr()
{
	return DescrList;
}

vector<VlSiftKeypoint> SiftDetector::getKeyPointList()
{
	return KeyPointList;
}

vector<double> SiftDetector::getOrientationList()
{
	return OrientationList;
}

double SiftDetector::getPatchScaleRatio()
{
	double rs = vl_sift_get_magnif(pSiftFilter)*vl_sift_get_window_size(pSiftFilter);
	return rs;
}

void SiftDetector::updateAllKeyPointsOrientationsandSiftDescriptorsforCurrentOctave()
{
	int result = 1;
	double orientation[4];
	float descr[128];
	for(int i=0; i<KeyPointList.size(); i++)
	{
		if(!bUseUprightOrientation)
			result = vl_sift_calc_keypoint_orientations(pSiftFilter, orientation, &KeyPointList[i]);
		else
		{
			if(vl_sift_get_octave_index(pSiftFilter) != KeyPointList[i].o)
				result = 0;
			else 
				result = 1;
		}
		if(result != 0)
		{
			if(!bUseUprightOrientation)
				OrientationList[i] = orientation[0];
			vl_sift_calc_keypoint_descriptor(pSiftFilter,(vl_sift_pix*)descr, &KeyPointList[i], OrientationList[i]);
			vector<float> descr_v(descr, descr+128);
			DescrList[i] = descr_v;
		}
	}
}

/*
void SiftDetector::initializeRawData()
{
	int width = TheImage.width();
	int height = TheImage.height();
	if(ptrRawImgData!=NULL)
	{
		delete[] ptrRawImgData;
		ptrRawImgData = NULL;
	}
	ptrRawImgData = new double[width*height];
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			ptrRawImgData[j*width + i] = qGray(QImage.pixel(j, i);
		}
	}
}
*/

void SiftDetector::initializeRawData(const PGMImage& image)
{
	size_t nbytes = image.width() * image.height() * sizeof(float);
	ptrRawImgData = (float*)malloc(nbytes);
	memcpy(ptrRawImgData, image.floatRawData(), nbytes);
}

double SiftDetector::calculateKeyPointOrientation(VlSiftKeypoint keypoint)
{
	double orientation[4];
	vl_sift_calc_keypoint_orientations(pSiftFilter, orientation, &keypoint);
	return orientation[0];
}

