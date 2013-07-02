
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
/********************************/
/*	Const Defintions	*/
/********************************/
const string STR_NAME	= "Multi-Class Object Detector";
const string STR_VERSION= "0.1";
const QString IMG_DIR = "/root/Download/VOCdevkit";
/************************/
/*	Defintions	*/
/************************/

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
QT_END_NAMESPACE

class RFISMReco : public QWidget
{
	Q_OBJECT
public:
	RFISMReco( QWidget *parent=0, const char *name=0);
	~RFISMReco();
public slots:
	void loadImageSet4Training();
	void loadImage4Testing();
	void loadImageSet4Testing();
	void train();
	void trainwithpuri();
	void rankLeafNodes();
	void showDiscriminateLeafNodeCoverage();
	void saveRandomForest();
	void loadRandomForest();
	void test();
private:
	void processImage(QString qstr, int nImageClass , uint32_t nImageindex);	
	void processImage4Test(QString qstr);
	void loadImage(QString qstr);
	void drawInterestPoints();
	void drawInterestPointsEllipse();
	vector<OpGrayImage> getImagePatches();
	void displayPatchesForBrowsing( vector<OpGrayImage>& vImagePatches);
	void test(	vector<FeatureVector>&				vfvCoord,
				vector<float>&						vdValue );
	void showRankedLeafNodes(const vector<LeafNodeScore>& leafnodescore);
	vector<OpGrayImage> drawLeafNodesOnImages(vector< vector<Tuple_Sample_LeafNodeIndex> > leafnodeperimage);
	OpGrayImage drawSeveralLeafNodesOnOneImage(OpGrayImage inputImage, vector<Tuple_Sample_LeafNodeIndex> leafnodesonimage);
	OpGrayImage drawOneLeafNodeOnOneImage(OpGrayImage inputImage, Tuple_Sample_LeafNodeIndex leafnodeonimage);
	OpGrayImage drawImagePatchOnImage(OpGrayImage imgPatch, OpGrayImage pallete, InterestPoint offset);
	vector<QImage> grayImageVector2QImageVector(const vector<OpGrayImage>& grayImageVector);
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

	// Voting Space
	VotingSpace *vsHough;
};

#endif	
