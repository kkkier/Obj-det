/*******************************************************************/
/*                                                                 */
/*FILE         	randomforest.hh                                    */
/*AUTHORS     	Yuren Zhang                                        */
/*EMAIL       	kiekergaard@gmail.com                              */
/*                                                                 */
/*CONTENT	Class for  training and testing with random forest */
/*								   */
/*BEGIN		MON JAN 21 2013 				   */
/*LAST CHANGE	MON JAN 21 2013					   */
/*								   */
/*******************************************************************/

#ifndef RANDOMFOREST_HH
#define RANDOMFOREST_HH

using namespace std;

/************************/
/*	Includes	*/
/************************/
#include <opgrayimage.hh>
#include <featurevector.hh>
#include <map>
#include <utility>
/********************************/
/*	Struct Defination	*/
/********************************/
typedef struct _PatchSample{
	vector<OpGrayImage> vChannels;
	int nChannels;
	FeatureVector fvOffset;
	int nTotalClasses;
	int nClassLabel;
	//int nPatchIndex;
}PatchSample;
/********************************/
/*	Param Defination	*/
/********************************/
const int CLASS_ENTROPY = 0;
const int POS_ENTROPY = 1;

/********************************/
/*	Class Defination	*/
/********************************/
/*======================================================================*/
/*				Class RandomNode			*/
/*======================================================================*/
class RandomNode
{
public: 
	RandomNode();
	RandomNode( int nWeekClassifier, int nMinSample,int key, int parentKey, int level, int maxlevel);
	RandomNode(const RandomNode& other);
	~RandomNode();
public:
	/********************************/
	/*	Accessing Patches	*/
	/********************************/
	void loadPatches(const vector<int>& vIndex);
	// Consider deleting the patches' index after loading to save memory.
	void loadLeftPatches(vector<int>& vIndex){vIndex = m_vLeftIndex;}
	void loadRightPatches(vector<int>& vIndex){vIndex = m_vRightIndex;}

public:
	/************************/
	/*	Operater	*/
	/************************/
	RandomNode& operator=(const RandomNode& rhs);

public:
	/****************************************/
	/*	Querry/Set Children/Parrent	*/
	/****************************************/
	int getLeft(){return m_leftkey;}
	int getRight(){return m_rightkey;}
	int getParent(){return m_parentkey;}
	int getKey(){return m_key;}
	int getLevel(){return m_nLevel;}
	void setLeft(int key){m_leftkey = key;}
	void setRight(int key){m_rightkey = key;}
	void setParent(int key){m_parentkey = key;} 
	void setKey(int key){m_key = key;}
	void setLevel(int level){m_nLevel = level;}
public:
	/****************/
	/*	Train	*/
	/****************/
	void 	trainNode(const vector<PatchSample>& vFeatures,
			const vector<int>& vIndex);
	bool 	trainNode(const vector<PatchSample>& vFeatures,
			const vector<int>& vIndex,
			vector<int>& vIndex4Left,
			vector<int>& vIndex4Right);
	void 	trainLeafNode(	const vector<PatchSample>& vFeatures,
				const vector<int>& vIndex,
				RandomNode& rnLeftNode,
				RandomNode& rnRightNode);
	void 	createLeafNode(	const vector<int>& vIndex);
	/****************/
	/*	Test	*/
	/****************/
	int 	testNode(const PatchSample& feature);
/*	bool 	testNode(const vector<PatchSample>& vFeatures,
			vector<PatchSample>& vFeatures4Left,
			vector<PatchSample>& vFeatures4Right);*/
private:
	/************************/
	/*	For Training  	*/
	/************************/
	void	calculateSplit(	const 	vector<PatchSample>& vFeatures, 
				const 	vector<int>& vIndex,
				int	c, int p, int q, int r, int s, int t,
				int 	entropyType,
				float& 	entropy,
				vector<int>& vIndex4Left,
				vector<int>& vIndex4Right );
	//void 	calculateFilter(const PatchSample)
private:
	/************************/
	/*	Params		*/
	/************************/
	int m_channel;
	int m_p;
	int m_q;
	int m_r;
	int m_s;
	int m_t;
	/********************************/
	/*	Params Given by tree	*/
	/********************************/
	int m_key;
	int m_nWeekClassifier;
	int m_nMinSample;
	int m_nLevel;
	int m_nMaxLevel;
private:
	/********************************/
	/*	Children and Parent	*/
	/********************************/
	int m_leftkey;
	int m_rightkey;
	int m_parentkey;
private:
	/********************************/
 	/*	Stored Patches		*/
	/********************************/
	vector<int> m_vSampleIndex;
	vector<int> m_vLeftIndex;
	vector<int> m_vRightIndex;
};
//typedef std::pair<int, RandomNode> pairNode;

/*======================================================================*/
/*				Class RandomTree			*/
/*======================================================================*/
class RandomTree
{
public:
	RandomTree();
	RandomTree( int nTreeDepth, int nMinSample, int nWeekClassifier);
	~RandomTree();
	//void createLeafNode(const vector<int>& vIndex);
	
public:
	/****************/
	/*	Train	*/
	/****************/
	void trainTree(const vector<PatchSample>& vFeatures, const vector<int>&
 vIndex);
	/****************/
	/*	Test	*/
	/****************/
	int testTree(const PatchSample& feature);
	/************************/
	/* 	Leaf Nodes	*/
	/************************/
	map<int, RandomNode> getLeafNode();
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
	std::map<int,RandomNode> m_rnNodes;
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
	/************************/
	/*	Parameters	*/
	/************************/
	void setParams(	int nTreeNumber, 
			int nTreeDepth, 
			int nWeekClassifier, 
			int nMinSample);
public:
	/************************/
	/*	Training	*/
	/************************/
	void trainRandomForest(const vector<PatchSample>& vFeatures);
	vector<vector<int> > getClusterAssignment();
public:
	/************************/
	/*	Testing		*/
	/************************/
	void testRandomForest(const PatchSample& psPatch, vector<int>& resultKeys);
private:
	/************************/
	/*	Parameters	*/
	/************************/
	int m_nTreeNumber;
	int m_nTreeDepth;
	int m_nWeekClassifier;
	int m_nMinSample;	
	/************************/
	/*	Nodes		*/
	/************************/
	vector<RandomTree> m_vTrees;
	// Stores the leaf nodes in all the trees, each tree in a vector element
	vector<map<int, RandomNode> > m_vmClusters;
};
#endif
