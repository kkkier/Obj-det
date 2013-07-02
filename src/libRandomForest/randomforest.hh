/*******************************************************************/
/*                                                                 */
/*FILE         	randomforest.hh                                    */
/*AUTHORS     	Yuren Zhang                                        */
/*EMAIL       	kiekergaard@gmail.com                              */
/*                                                                 */
/*CONTENT	Class for  training and testing with random forest     */
/*                                                                 */
/*BEGIN		MON JAN 21 2013                                        */
/*LAST CHANGE	TUE 3 5 2013                                       */
/*                                                                 */
/*******************************************************************/

#ifndef RANDOMFOREST_HH
#define RANDOMFOREST_HH

using namespace std;

/****************/
/*	Includes	*/
/****************/
#include <stdint.h>
#include <libxml/xmlreader.h>

#include <map>
#include <utility>

#include <opgrayimage.hh>
#include <featurevector.hh>
#include <qtclusterview.hh>
#include <opinterestimage.hh>

#include "randomforestgui.hh"
/************************/
/*	Struct Defination	*/
/************************/
typedef struct _PatchSample{
	vector<OpGrayImage> vChannels;
	int nChannels;
	InterestPoint fvOffset;
	int nTotalClasses;
	int nClassLabel;
	uint32_t nImageIndex;
	//int nPatchIndex;
}PatchSample;

typedef struct _LeafNodeIndex{
	uint32_t tree;
	uint32_t key;
}LeafNodeIndex;

typedef struct _LeafNodeScore{
	LeafNodeIndex index;
	float score;
}LeafNodeScore;

typedef struct _Tuple_Sample_LeafNodeIndex{
	LeafNodeIndex leafnodeindex;
	uint32_t sampleindex;
	float discriminativeScore;
}Tuple_Sample_LeafNodeIndex;
/************************/
/*	Param Defination	*/
/************************/
const int CLASS_ENTROPY = 0;
const int POS_ENTROPY = 1;
const int OBJ_BKG_ENTROPY = 2;
const float THRESHOLD_ETA = 0.01;
const int MAXITERATION_ETA = 5;
const float PROPOTION_REJECT = 1;
// TODO modify this
const float LAMBDA_PURITY_DISCRIMINATIVE = 1;
const float SMALL_ENOUGH_PROPORTION = 0.0001;
//const char* MY_ENCODING_ISO = "ISO-8859-1";
/************************/
/*	Class Defination	*/
/************************/
/*==========================================*/
/*				Class RandomNode			*/
/*==========================================*/
class RandomNode
{
public: 
	RandomNode();
	RandomNode( int nWeekClassifier, int nMinSample,uint32_t key, uint32_t parentKey, int level, int maxlevel);
	RandomNode(const RandomNode& other);
	~RandomNode();
public:
	/********************************/
	/*	Accessing Patches	*/
	/********************************/
	void loadPatches(const vector<uint32_t>& vIndex);
	// Consider deleting the patches' index after loading to save memory.
	void loadLeftPatches(vector<uint32_t>& vIndex){vIndex = m_vLeftIndex;}
	void loadRightPatches(vector<uint32_t>& vIndex){vIndex = m_vRightIndex;}

public:
	/************************/
	/*	Operater	*/
	/************************/
	RandomNode& operator=(const RandomNode& rhs);

public:
	/****************************************/
	/*	Querry/Set Children/Parrent	*/
	/****************************************/
	uint32_t getLeft(){return m_leftkey;}
	uint32_t getRight(){return m_rightkey;}
	uint32_t getParent(){return m_parentkey;}
	uint32_t getKey(){return m_key;}
	int getLevel(){return m_nLevel;}
	void setLeft(uint32_t key){m_leftkey = key;}
	void setRight(uint32_t key){m_rightkey = key;}
	void setParent(uint32_t key){m_parentkey = key;} 
	void setKey(uint32_t key){m_key = key;}
	void setLevel(int level){m_nLevel = level;}
	vector<uint32_t> getReachedSample(){return m_vSampleIndex;}
	void appendReachedSample(uint32_t sampleindex){m_vSampleIndex.push_back(sampleindex);}
public:
	/**********************/
	/* Get the parameters */
	/**********************/
	int getChannel(){return m_channel;}
	int getp(){return m_p;}
	int getq(){return m_q;}
	int getr(){return m_r;}
	int gets(){return m_s;}
	int gett(){return m_t;}
	void setChannel(int channel)	{m_channel = channel;}
	void setp(int p)				{m_p = p;}
	void setq(int q)				{m_q = q;}
	void setr(int r)				{m_r = r;}
	void sets(int s)				{m_s = s;}
	void sett(int t)				{m_t = t;}
public:
	/****************/
	/*	Train	*/
	/****************/
	void 	trainNode(const vector<PatchSample>& vFeatures,
			const vector<uint32_t>& vIndex);
	bool 	trainNode(const vector<PatchSample>& vFeatures,
			const vector<uint32_t>& vIndex,
			vector<uint32_t>& vIndex4Left,
			vector<uint32_t>& vIndex4Right);
	void 	trainLeafNode(	const vector<PatchSample>& vFeatures,
				const vector<uint32_t>& vIndex,
				RandomNode& rnLeftNode,
				RandomNode& rnRightNode);
	void 	createLeafNode(	const vector<uint32_t>& vIndex);
	/****************/
	/*	Test	*/
	/****************/
	uint32_t 	testNode(const PatchSample& feature);
/*	bool 	testNode(const vector<PatchSample>& vFeatures,
			vector<PatchSample>& vFeatures4Left,
			vector<PatchSample>& vFeatures4Right);*/
public: 
	void calculateGeneralizedOOBError();
	void calculateDiscriminativeScore();
private:
	float entropyofObjectClasses();
	float proportionofObjectClasses();
private:
	/************************/
	/*	For Training  	*/
	/************************/
	void	calculateSplit(	const 	vector<PatchSample>& vFeatures, 
				const 	vector<uint32_t>& vIndex,
				int	c, int p, int q, int r, int s, int t,
				int 	entropyType,
				float& 	entropy,
				vector<uint32_t>& vIndex4Left,
				vector<uint32_t>& vIndex4Right );
	uint32_t getnumberofOBJPatch(const vector<PatchSample>& vFeatures, const vector<uint32_t>& index);
	uint32_t getnumberofBKGPatch(const vector<PatchSample>& vFeatures, const vector<uint32_t>& index);
	//void 	calculateFilter(const PatchSample)
private:
	/************/
	/*	Params	*/
	/************/
	int m_channel;
	int m_p;
	int m_q;
	int m_r;
	int m_s;
	int m_t;
	/****************************/
	/*	Params Given by tree	*/
	/****************************/
	uint32_t m_key;
	int m_nWeekClassifier;
	int m_nMinSample;
	int m_nLevel;
	int m_nMaxLevel;
private:
	/************************/
	/*	Children and Parent	*/
	/************************/
	uint32_t m_leftkey;
	uint32_t m_rightkey;
	uint32_t m_parentkey;
private:
	/********************/
 	/*	Stored Patches	*/
	/********************/
	vector<uint32_t> m_vSampleIndex;
	vector<uint32_t> m_vLeftIndex;
	vector<uint32_t> m_vRightIndex;
public:
	/********************/
	/* Training results */
	/********************/
	// The valid patches, propotion and cooresponding offset vectors 
	// of each class
	vector<float> m_dProportion;
	vector<uint32_t> m_nValidPatches;
	vector< vector<FeatureVector> > m_vCoords;

	vector<float> m_dProportion_Validation;
	vector<uint32_t> m_nValidPatches_Validation;
	vector< vector<FeatureVector> > m_vCoords_Validation;

	float generalizedOOBError;
	vector<float> offsetVariance_train;
	
	float discriminativeScore;
};
//typedef std::pair<int, RandomNode> pairNode;

/*==========================================*/
/*				Class RandomTree			*/
/*==========================================*/
class RandomTree
{
public:
	RandomTree();
	RandomTree( int nTreeDepth, int nMinSample, int nWeekClassifier);
	~RandomTree();
	//void createLeafNode(const vector<int>& vIndex);
	
public:
	/************/
	/*	Train	*/
	/************/
	void trainTree(const vector<PatchSample>& vFeatures, const vector<uint32_t>& vIndex);
	void validateTree(const vector<PatchSample>& vFeatures, const vector<uint32_t>& validationSampleIndex);
	void clearandinitiateValidationData(int numofclasses);
	void validateWithValidationSamples(const vector<PatchSample>& vFeatures, const vector<uint32_t>& vIndex);
	void seiriValidationData();
	/************/
	/*	Test	*/
	/************/
	uint32_t testTree(const PatchSample& feature);
	/****************/
	/* 	Leaf Nodes	*/
	/****************/
	map<uint32_t, RandomNode> getLeafNode();
	void clearNodes();
	void insertNode(uint32_t key, RandomNode node);
private:
	/************************/
	/*	Parameters	*/
	/************************/
	int m_nTreeDepth;
	/********************************/
	/*	Params for the nodes	*/
	/********************************/
	int m_nWeekClassifier;
	int m_nMinSample;
	/************************/
	/*	RandomNodes	*/
	/************************/
public:
	// Reconstruct this from public to private
	std::map<uint32_t,RandomNode> m_rnNodes;
};
/*======================================================================*/
/*				Class RandomForest			*/
/*======================================================================*/

class RandomForest
{
public:
	RandomForest();
	RandomForest(int nTreeNumber, int nTreeDepth, int nWeekClassifier, int nMinSample);
	~RandomForest();
public:
	/********************/
	/*	GUI Function	*/
	/********************/
	RandomForestGUI* createGUI(QWidget *parent);
public:
	/****************/
	/*	Parameters	*/
	/****************/
	void setParams(	int nTreeNumber, 
			int nTreeDepth, 
			int nWeekClassifier, 
			int nMinSample);
	int  getTotalClasses();
public:
	/****************/
	/*	Training	*/
	/****************/
	void trainRandomForest(const vector<PatchSample>& vFeatures);
	void trainRandomForest();
	vector<uint32_t> bootstrapSamplesandTheonesleft(uint32_t numofbase, float proportion);
	vector<uint32_t> bootstrapSamplesandTheonesleft(uint32_t numofbase, uint32_t numofsample);
	vector<uint32_t> bootstrapSamples(uint32_t numofbase, long numofsample);
	void purifyTrainingSet(vector<uint32_t> &removedIndex, vector<uint32_t> &top100DiscriminantIndex);
	vector<LeafNodeScore> rankLeafNodes();
	void saveRankedLeafNodeandScore(const vector<LeafNodeScore>& rankedLeafNodeScore);
	void saveSampleandImageCoverage2LeafNodeNumber(const vector<LeafNodeScore>& rankedLeafNodeScore);
	vector< vector<Tuple_Sample_LeafNodeIndex> > getTwoLeafNodeCoveringofImages(uint32_t totalimagenumber);
	vector< vector<Tuple_Sample_LeafNodeIndex> > getTwoLeafNodeCoveringofImagesofPossitive(uint32_t totalimagenumber);
	vector< vector<Tuple_Sample_LeafNodeIndex> > getAverage5LeafNodePerImageCoverage(uint32_t totalimagenumber);
private:
	vector<LeafNodeScore> calculateLeafNodeScore();
public:
	/*******************/
	/*  Acquire Result */
	/*******************/
	RandomNode 				getNode(int nTree, uint32_t nKey);
	PatchSample 			getPatchSample(uint32_t nIndex);
	OpGrayImage				getMeanPatchOfLeafNode(int tree, uint32_t key);
	void 					writeLeafNodetoFile( QString fileName );
	void 					computeClusterCenters_HF();
	vector<vector<int> > 	getClusterAssignment();
	void 					drawClusters( QtClusterView *qClassView );
public:
	/****************/
	/*	Testing		*/
	/****************/
	void testRandomForest(	const PatchSample&	psPatch, 
							vector<uint32_t>&		resultKeys	);
	void testRandomForestSingleClass(	const PatchSample&	psPatch,
							const float 					dThres,
							vector<FeatureVector>&			vfvCoords,
							vector<float>&					vValue ,
							vector<uint32_t>&					vnLeafNodes,
							vector<float>&					vdConfidence,
							int								nClass = 1);
	void testRandomForestMultiClass(	const PatchSample&		psPatch,
							const float 						dTres,
							vector< vector<FeatureVector> >&	vvfvCoords,
							vector< vector<float> >&			vvValues,
							vector<uint32_t>&						vnLeafNodes,
							vector< vector<float> >&			vvdConfidence);
public:
	/***************/
	/* Save & Load */
	/***************/
	void saveRandomForest(const char* uri);
	void loadRandomForest(const char* uri);
	void streamXMLFile(const char* uri);
	void processXMLNode(xmlTextReaderPtr reader);
private:
	/****************/
	/*	Parameters	*/
	/****************/
	// Parameters are now stored in RandomForestGUI, these are copies. They are
	// updated when trainRandomForest is called
	int m_nTreeNumber;
	int m_nTreeDepth;
	int m_nWeekClassifier;
	int m_nMinSample;
	int m_nTotalClasses;
	/***********************/
	/*  Original Resource  */
	/***********************/
    vector<PatchSample> m_vPatchSample;	
	vector<OpGrayImage> m_vImagePatches;
	/****************/
	/*	Cluster		*/
	/****************/
	vector<OpGrayImage>	m_vClusterPatches;
	vector<int>			m_vClusterAssignment;
	bool				m_bClustersValid;	
	vector<string>		m_vClusterInfo;
private:
	/********************************/
	/*	Trees ,Nodes and Clusters	*/
	/********************************/
	vector<RandomTree> m_vTrees;
	// Stores the leaf nodes in all the trees, each tree in a vector element
	vector<map<uint32_t, RandomNode> > m_vmClusters;
	// Mapping from each key in each tree to the cluster index, though very foolish...
	vector<map<uint32_t, uint32_t> >		  m_vmKey2Index;
	vector<vector<int> >		  m_vvClusterAssignment;
	vector<LeafNodeScore> m_rankedLeafNodeScore;
	/********************************/
	/*      Full testing result     */
	/********************************/
	vector< vector<uint32_t> > 		m_vvFullResult; // each training sample's testing result {each sample{each tree}}
	/********************************/
	/*      For sample puration     */
	/********************************/
	float m_eta ;	// The estimated proportion of the inliers in the possitive training set

public:
	RandomForestGUI* m_gui;
};
#endif
