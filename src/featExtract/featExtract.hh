
/************************************************************************/
/*                                                                      */
/* FILE		rf_ismreco.hh                                               */
/* AUTHOR	Yuren Zhang                                                 */
/* EMAIL	kiekergaard@gmail.com;yuren.zhang@ia.ac.cn                  */
/*                                                                      */
/* CONTENT	Object detecton with Random forest and ISM.                 */
/*		The prototype program for object deteciont using Leibe's        */
/*		ISM library and home-made random forest algorithm.              */
/* BEGIN	2013.2.1                                                    */
/* LAST CHANGE	2013.2.1                                                */
/************************************************************************/
#ifndef HF_ISMRECO_H
#define HF_ISMRECO_H
using namespace std;

/************************/
/*	Include		*/
/************************/
#include <QMainWindow>
#include <qstring.h>
#include <qimage.h>
#include <string>

#include <qtresizeimg.hh>
#include <qtclusterview.hh>
#include <featurecue.hh>
#include <featuregui.hh>
#include <randomforest.hh>
#include <randomforestgui.hh>
#include <votingspace.hh>
#include <kmeans.hh>
/********************************/
/*	Const Defintions	*/
/********************************/
const string STR_NAME	= "Feature Extractor";
const string STR_VERSION= "0.1";
const QString IMG_DIR = "/root/Download/VOCdevkit/VOC2012";
const unsigned int SIFT_FEATURE_DIMENSION = 128;
const unsigned int DEFAULT_CENTER_SIZE = 1000;

typedef struct _Annotation
{
	int classno;
	int isOccluded;
	int x1;
	int y1;
	int x2;
	int y2;
}Annotation;

typedef struct _SpatialInfo
{
	int patchno;
	int classno;
	// the ratio of window size/offset to the patch's scale 
	float width_scale_ratio;
	float height_scale_ratio;
	float offset_x_scale_ratio;
	float offset_y_scale_ratio;
} SpatialInfo;
/************************/
/*	Defintions	*/
/************************/

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
QT_END_NAMESPACE

class FeatExtract : public QWidget
{
	Q_OBJECT
public:
	FeatExtract( QWidget *parent=0, const char *name=0);
	~FeatExtract();
public slots:
	void loadImageSet();
	void loadFeatureSet();
	void cluster();
	void loadCenters();
	void getAssignmentandSave();
private:
	void processImage(QString qstr,  uint32_t nImageindex);	
	void loadImage(QString qstr);
	void loadFeature(QString qstr);
	void clearFeature();
	void loadAnnotation(string filename);
	void extractFeature();
	void collectPatchSpatialInfo();
	bool collectPatchSpatialInfo_UnOccluded(int patchindex);
	void collectPatchSpatialInfo_Occluded(int patchindex);
	bool isInsideBBox(int patchindex, int annoindex);
	void pushbackSpatialInfo(int patchindex, int annoindex);
	void saveFeatures();
	void saveFeatures(const vector< vector<float> >& features, const string& filename);
	string makeFeatureFileName();
	void saveSpatialInfo();
	string makeSpatialInfoFileName();
	void saveKeyPoints();
	string makeKeyPointFileName();
	string makeAnnoFileName();
	void drawInterestPoints();
	void drawInterestPointsEllipse();
	vector<OpGrayImage> getImagePatches();
	void displayPatchesForBrowsing( vector<OpGrayImage>& vImagePatches);
	vector<QImage> grayImageVector2QImageVector(const vector<OpGrayImage>& grayImageVector);
	void cleanValidateVector();
	
	unsigned int getFeatureNum(const QStringList& featurefilenames);
	QStringList loadFileSet(const QString& filenameinstructor,const QString& dir);

	void assignFeaturestoCodeWordandSave(QString featurefilename);
	string replacesuffix(const string& input, const string& torepalce);
	void loadFeaturetoBuffer(const QString& filename, float* ptr, uint32_t valuenum);
	uint32_t getFeatureNum(const QString& filename);
	bool clustererExists();
protected:
	
private:

    QtResizeImg		*rsSourceImg;
	QtResizeImg		*rsResultImg;
	QtClusterView	*qClassView;

	FeatureCue		m_fcCue;	
	RandomForest	m_rfForest;
	PointVector		m_vPoints;
	PointVector		m_vPointsInside;

	int		m_nFeatureType;	
	// All the training Image Names and Class Assignment
	QStringList m_qslFileList;
	vector<int> m_vImageClass;

	QStringList m_featureFileList;
	
	// All the training patches
	vector<PatchSample> m_vpsPatch4Train;
	vector<PatchSample> m_vpsPatch4Test;
	
	//Violate variables for processing
	QString m_qsLastImage;
	OpGrayImage	m_grayImg;
	OpGrayImage m_grayImgMap;
	QImage		m_img;
	string		m_sImgNameShort;
	string		m_sImgName;
	string		m_sImgPath;
	string		m_sImgFullName;
	bool		m_bMapsOn;
	bool		m_bPicLoaded;
    int			m_nImgClass;
	vector<Annotation> m_vAnnotation;
	vector<FeatureVector> m_vFeature;
	vector< vector<float> > m_vFeature_raw;
	vector<OpGrayImage> m_vPatch;
	vector<SpatialInfo> m_vSpatialInfo;

	// Voting Space
	VotingSpace *vsHough;

	// Feature Raw Data
	//TODO: wraper the raw data with a class, such that the operations are safer
	float* m_pFeatures;
	unsigned int m_FeatureValueIndex; // Used to indicate the position where the feature should be writen to while loading the features
	KMeansClustering<float> *m_pClusterer;
	vector< vector<float> > m_featureCenters;
};

#endif	
