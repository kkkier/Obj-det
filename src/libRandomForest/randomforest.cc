
/********************************************************************/
/*																	*/
/*FILE         	randomforest.cc										*/
/*AUTHORS     	Yuren Zhang											*/
/*EMAIL       	kiekergaard@gmail.com								*/
/*																	*/
/*CONTENT		Class for  training and testing with random forest	*/
/*																	*/
/*BEGIN			MON JAN 21	 2013									*/
/*LAST CHANGE	Tue April 2	 2013									*/
/*																	*/
/********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "randomforest.hh"

/*******************/
/*   Definitions   */
/*******************/
typedef std::pair<uint32_t, RandomNode> pairNode;
typedef struct _floatint{
  float dvalue;
  uint32_t   nvalue;
} FloatInt;

/********************************/
/*	Global Functions	*/
/********************************/
uint32_t BigRand();
void poolFromIndex(const vector<uint32_t>& vIndex, uint32_t percent, vector<uint32_t>& out_vIndex);
bool cmpFloatInt(FloatInt a, FloatInt b);
bool cmpLeafNodeScore(LeafNodeScore first, LeafNodeScore second);
bool cmpTuple_Sample_LeafNode_WithScore(Tuple_Sample_LeafNodeIndex first, Tuple_Sample_LeafNodeIndex second);
xmlChar *ConvertInput(const char *in, const char *encoding);
float extractValue(xmlTextReaderPtr reader);
uint32_t sumIntVector(vector<uint32_t> thevector);
float sumFloatVector(const vector<float>& thevector);
vector<float> num2percent(const vector<uint32_t>& numvector);
float calculateEucideanDistanceWithWeight(const vector<float>& distribution1, const vector<float>& distribution2, const vector<float>& weight);
float calculateEucideanDistance(const vector<float>& distribution1, const vector<float>& distribution2);
float calculateHellingerDistance(const vector<float>& distribution1, const vector<float>& distribution2);
vector<float> normalizeProportions(const vector<float>& original_proportions);
float calculateEntropy(const vector<float>& proportions);
bool IsInVector(uint32_t thevalue, vector<uint32_t> thevector);
/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                          Class RandomNode                         */
/*===================================================================*/
/********************/
/*   Constructor    */
/********************/

RandomNode::RandomNode()
{
	m_channel = 0;
	m_p = 0;
	m_q = 0;
	m_r = 0;	
	m_t = 0;
	m_key = 1;
	m_leftkey = 0;
	m_rightkey = 0;
	m_parentkey = 0;
	m_nWeekClassifier = 100;
	m_nMinSample = 20;
	m_nMaxLevel = 10;
	m_nLevel = 0;
}

RandomNode::RandomNode(int nWeekClassifier,  int nMinSample,uint32_t key,  uint32_t parentKey, int level, int maxlevel):m_nWeekClassifier(nWeekClassifier),m_nMinSample(nMinSample), m_parentkey(parentKey), m_nLevel(level), m_nMaxLevel(maxlevel), m_key(key)
{
	m_channel = 0;
	m_p = 0;
	m_q = 0;
	m_r = 0;	
	m_t = 0;
	m_leftkey = 0;
	m_rightkey = 0;
}
// 
RandomNode::RandomNode(const RandomNode& other): m_nWeekClassifier(other.m_nWeekClassifier), m_nMinSample(other.m_nMinSample), m_nLevel(other.m_nLevel), m_key(other.m_key)
{
	m_channel = other.m_channel;
	m_p = other.m_p;
	m_q = other.m_q;
	m_r = other.m_r;
	m_s = other.m_s;
	m_t = other.m_t;
	m_leftkey = other.m_leftkey;
	m_rightkey = other.m_rightkey;
	m_parentkey = other.m_parentkey;
	m_vSampleIndex = other.m_vSampleIndex;
	m_vLeftIndex = other.m_vLeftIndex;
	m_vRightIndex = other.m_vRightIndex;
	m_nMaxLevel = other.m_nMaxLevel;
	m_dProportion = other.m_dProportion;
	m_nValidPatches = other.m_nValidPatches;
	m_vCoords = other.m_vCoords;
	m_dProportion_Validation = other.m_dProportion_Validation;
	m_nValidPatches_Validation = other.m_nValidPatches_Validation;
	m_vCoords_Validation = other.m_vCoords_Validation;
}

RandomNode::~RandomNode(){}

/************************/
/*	Access Patches	*/
/************************/
void RandomNode::loadPatches(const vector<uint32_t>& vIndex)
{
	vector<uint32_t>::const_iterator itr;
	for(itr=vIndex.begin();itr!=vIndex.end();itr++){
		m_vSampleIndex.push_back(*itr);
	}
}

/************************/
/*	Operator	*/
/************************/

RandomNode& RandomNode::operator=(const RandomNode& rhs)
{
	m_channel = rhs.m_channel;
	m_p = rhs.m_p;
	m_q = rhs.m_q;
	m_r = rhs.m_r;
	m_s = rhs.m_s;
	m_t = rhs.m_t;
	m_nWeekClassifier = rhs.m_nWeekClassifier;
    m_dProportion = rhs.m_dProportion;
	m_nValidPatches = rhs.m_nValidPatches;
	m_vCoords = rhs.m_vCoords;	
	// Do not copy the stored patches, parent and childer of the original RandomNode
	// Maybe it can be implemented such that when copying occurs, the original RandomNode
	// is wapped while the copying one get the full content.
}
/****************/
/*	Training	*/
/****************/

/*=======================================================================================*/
/* For training an internal node. The samples' indexes vIndex4Left and vIndex4Right are  */
/* returned for further training.If the node is successfully trained, it will return true*/
/*  If the depth of the leaf has reached the limit , or the  number of samples contained */
/*   in the node is less than the limit, this function will return false, meanning it is */ 
/*   a leaf node.                                                                        */
/*=======================================================================================*/

bool RandomNode::trainNode(	const vector<PatchSample>& vFeatures,
				const vector<uint32_t>& vIndex,
				vector<uint32_t>& vIndex4Left,
				vector<uint32_t>& vIndex4Right)
{
	if(vIndex.size()<m_nMinSample || m_nLevel>=m_nMaxLevel)
	{
		m_rightkey = 0;
		m_leftkey = 0;
		// If the node is leaf node, the leftindex contains all the patches that fall in it.
		m_vLeftIndex = vIndex;
		m_vSampleIndex = vIndex;
		// Gather the training result: CL(Proportion) and DL(valid patches)
		m_dProportion.clear();
		m_dProportion.assign(vFeatures[0].nTotalClasses,0);
		vector<uint32_t> DL(vFeatures[0].nTotalClasses,0);
		vector<FeatureVector> tmp;
		vector< vector<FeatureVector> > offsets(vFeatures[0].nTotalClasses, tmp);
		FeatureVector coord(2);
		m_vCoords.clear();
		for(uint32_t i=0;i<vIndex.size();i++)
		{
			if( vFeatures[vIndex[i]].nClassLabel != 0 )
			{
				DL[vFeatures[vIndex[i]].nClassLabel]++;
				coord.setValue(0, vFeatures[vIndex[i]].fvOffset.x);
				coord.setValue(1, vFeatures[vIndex[i]].fvOffset.y);
				offsets[vFeatures[vIndex[i]].nClassLabel].push_back(coord);
			}
			else
			{
				DL[vFeatures[vIndex[i]].nClassLabel]++;
			}
		}
		m_nValidPatches = DL;
		for(uint32_t i=0;i<DL.size();i++)
		{
			m_dProportion[i] = (float)DL[i] / vIndex.size();
		}
		m_vCoords = offsets;
		return false;
	}
	else{
	//srand(time(NULL));
	vector<int> 	channel;
	vector<int>	p;
	vector<int>	q;
	vector<int>	s;
	vector<int>	r;
	vector<int>	t;

	vector<float> vdEntropy;
	vector<vector<uint32_t> >	vIndex1;
	vector<vector<uint32_t> >	vIndex2;

	int imageSize = vFeatures[0].vChannels[0].width();
	int nChannels = vFeatures[0].nChannels;
	
	int nEntropy = OBJ_BKG_ENTROPY;
	//int nEntropy = CLASS_ENTROPY;
	//if(rand()>RAND_MAX/2)
	//	nEntropy = POS_ENTROPY;
	
	// Generate a series of week classifiers. The number of the classifiers is ?
	for(int i=0;i<m_nWeekClassifier;i++){
		vector<uint32_t> tmp_index_1;
		vector<uint32_t> tmp_index_2;
		// For test
	//	tmp_index_1.push_back(1);
		float entropy;
		// Generate the random split
		channel.push_back(static_cast<int>((double)rand()/(RAND_MAX)*nChannels));
		p.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		q.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		r.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		s.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		t.push_back(static_cast<int>((double)rand()/(RAND_MAX)*255*2-255));	

		calculateSplit(vFeatures, vIndex, channel.back(), p.back(), q.back(), r.back(), s.back(), t.back(), nEntropy, entropy, tmp_index_1,tmp_index_2);

		vdEntropy.push_back(entropy);
		vIndex1.push_back(tmp_index_1);
		vIndex2.push_back(tmp_index_2);
	}
	// Select the split with the largest information gain.
	int minEtpIndex = (int)(min_element(vdEntropy.begin(), vdEntropy.end())-vdEntropy.begin());
	m_channel = channel[minEtpIndex];
	m_p = p[minEtpIndex];
	m_q = q[minEtpIndex];
	m_s = s[minEtpIndex];
	m_t = t[minEtpIndex];
	m_r = r[minEtpIndex];
	// If the samples contained in each node is less than the minimal threhold, claim it to be a leaf node by assigning the children as 0.
	m_leftkey = m_key * 2;
	m_rightkey = m_key * 2 + 1;
	vIndex4Left = vIndex1[minEtpIndex];
	vIndex4Right = vIndex2[minEtpIndex];	
	m_vLeftIndex = vIndex4Left;
	m_vRightIndex = vIndex4Right;
	return true;
	}
}

void RandomNode::trainNode(const vector<PatchSample>& vFeatures, const vector<uint32_t>& vIndex)
{
	trainNode(vFeatures, vIndex, m_vLeftIndex, m_vRightIndex);

}

void RandomNode::trainLeafNode(	const vector<PatchSample>& vFeatures,
				const vector<uint32_t>& vIndex,
				RandomNode& rnLeftNode,
				RandomNode& rnRightNode)
{
	vector<uint32_t> vIndex4Left;
	vector<uint32_t> vIndex4Right;
	trainNode(vFeatures, vIndex, vIndex4Left, vIndex4Right);
	// Create the leaf nodes with vIndex4Left and vIndex4Right as their content m_cSampleIndex. To make sure they are the nodes, the leaves of these nodes are set to zero.
	rnLeftNode.loadPatches(vIndex4Left);
	rnLeftNode.setLeft(0);
	rnLeftNode.setRight(0);
	rnRightNode.loadPatches(vIndex4Right);
	rnRightNode.setLeft(0);
	rnRightNode.setRight(0);
		
}

void RandomNode::createLeafNode(const vector<uint32_t>& vIndex)
{
	m_vSampleIndex = vIndex;
	m_leftkey = 0;
	m_rightkey = 0;
}

void RandomNode::calculateSplit(const vector<PatchSample>& vFeatures,
				const vector<uint32_t>& vIndex,
				int c, int p, int q, int r, int s, int t,
				int entropyType,
				float& entropy,
				vector<uint32_t>& vIndex4Left,
				vector<uint32_t>& vIndex4Right)
{
	// Traverse all the samples assigned to this node
	for(vector<uint32_t>::const_iterator itr=vIndex.begin();itr!=vIndex.end();++itr){
		if(vFeatures[*itr].vChannels[c](p, q).value() - vFeatures[*itr].vChannels[c](r, s).value() > t)
			vIndex4Left.push_back(*itr);
		else
			vIndex4Right.push_back(*itr);

	}

	// Calculate the entropy of class or offset
	float tmp_entropy = 0;
	vector<uint32_t> classAccumulator(vFeatures[0].nTotalClasses, 0);
	uint32_t numberofOBJPatch, numberofBKGPatch;
	float classPropotion;
	vector<FeatureVector> vfvRightCentor(vFeatures[0].nTotalClasses, FeatureVector(2));
	vector<FeatureVector> vfvLeftCentor(vFeatures[0].nTotalClasses, FeatureVector(2));
	vector<uint32_t> vSampleperClass(vFeatures[0].nTotalClasses, 0);
	float dDistances = 0;
	switch(entropyType){
	/*----------------------------------------*/
	/*      Calculate the class entropy       */
	/*----------------------------------------*/
	case OBJ_BKG_ENTROPY:
		tmp_entropy = 0;
		numberofOBJPatch = getnumberofOBJPatch(vFeatures, vIndex4Left);
		numberofBKGPatch = getnumberofBKGPatch(vFeatures, vIndex4Left);
		
		classPropotion = static_cast<float>(numberofOBJPatch)/vIndex4Left.size();
		if((classPropotion==classPropotion) && (classPropotion!=0)) 
			tmp_entropy = -classPropotion*log(classPropotion);

		classPropotion = static_cast<float>(numberofBKGPatch)/vIndex4Left.size();
		if((classPropotion==classPropotion) && (classPropotion!=0)) 
			tmp_entropy += -classPropotion*log(classPropotion);
		entropy = tmp_entropy * vIndex4Left.size();

		tmp_entropy = 0;
		numberofOBJPatch = getnumberofBKGPatch(vFeatures, vIndex4Right);
		numberofBKGPatch = getnumberofOBJPatch(vFeatures, vIndex4Right);
		
		classPropotion = static_cast<float>(numberofOBJPatch)/vIndex4Right.size();
		if((classPropotion==classPropotion) && (classPropotion!=0)) 
			tmp_entropy = -classPropotion*log(classPropotion);

		classPropotion = static_cast<float>(numberofBKGPatch)/vIndex4Right.size();
		if((classPropotion==classPropotion) && (classPropotion!=0)) 
			tmp_entropy += -classPropotion*log(classPropotion);
		entropy = entropy + tmp_entropy * vIndex4Right.size();
		break;

	case CLASS_ENTROPY:
		// Accumulate the samples in each classes in the left leaf
		for(vector<uint32_t>::iterator  itr=vIndex4Left.begin();itr!=vIndex4Left.end();++itr){	
			classAccumulator[vFeatures[*itr].nClassLabel]+=1;
		}
		// Calculate the entropy for the left leaf
		for(int i = 0; i<vFeatures[0].nTotalClasses;++i){
			classPropotion = (double)classAccumulator[i]/vIndex4Left.size();
			if((classPropotion==classPropotion) && (classPropotion!=0)) 
				tmp_entropy+= -classPropotion*log(classPropotion);
		}
		if(!(tmp_entropy==tmp_entropy)) tmp_entropy = 0;
		entropy = tmp_entropy * vIndex4Left.size();
		
		// Accumulate the samples in each classes in the right leaf
		classAccumulator.assign(vFeatures[0].nTotalClasses, 0);
		for(vector<uint32_t>::iterator  itr=vIndex4Right.begin();itr!=vIndex4Right.end();++itr){	
			classAccumulator[vFeatures[*itr].nClassLabel]+=1;
		}
		// Calculate the entropy for the right leaf
		tmp_entropy = 0;
		for(int i = 0; i<vFeatures[0].nTotalClasses;++i){
			classPropotion = (double)classAccumulator[i]/vIndex4Right.size();
			if((classPropotion==classPropotion) && (classPropotion!=0))
				tmp_entropy+= -classPropotion*log(classPropotion);
		}
		if(!(tmp_entropy==tmp_entropy)) tmp_entropy = 0;
		entropy+= tmp_entropy * vIndex4Right.size();
		break;
	/*---------------------------------------------------------------*/
	/*     Calculate the offset entropy and select the best split    */
	/*---------------------------------------------------------------*/
	case POS_ENTROPY:
		// To be implemented
		// Calculate the offset centor of samples in the left leaf, for each class except for class 0
		for(vector<uint32_t>::iterator itr = vIndex4Left.begin();itr != vIndex4Left.end();itr++)
		{
			int nClass =  vFeatures[*itr].nClassLabel;
			vfvLeftCentor[nClass].at(0) = vfvLeftCentor[nClass].at(0) + vFeatures[*itr].fvOffset.x;
			vfvLeftCentor[nClass].at(1) = vfvLeftCentor[nClass].at(1) + vFeatures[*itr].fvOffset.y;
			vSampleperClass[nClass]++;
		}
		for(int i = 1; i<vFeatures[0].nTotalClasses; i++)
		{
			if(vSampleperClass[i]!=0)
				vfvLeftCentor[i] = vfvLeftCentor[i]/vSampleperClass[i];
		}
		// Calculate the offset entropy of the left leaf, which is the sum of the offset distance among each 
		// classes
		for(vector<uint32_t>::iterator itr = vIndex4Left.begin(); itr != vIndex4Left.end(); itr++)
		{
			int nClass =  vFeatures[*itr].nClassLabel;
			if(nClass!=0)
				dDistances = dDistances + pow((vfvLeftCentor[nClass].at(0) - vFeatures[*itr].fvOffset.x),2)
				 					+ pow((vfvLeftCentor[nClass].at(1) - vFeatures[*itr].fvOffset.y),2);
		}
		// Calculate the offset centor of samples in the right leaf, for each class except for class 0		
		vSampleperClass.clear();
		vSampleperClass.assign(vFeatures[0].nTotalClasses, 0);
		for(vector<uint32_t>::iterator itr = vIndex4Right.begin();itr != vIndex4Right.end();itr++)
		{
			int nClass =  vFeatures[*itr].nClassLabel;
			vfvRightCentor[nClass].at(0) = vfvRightCentor[nClass].at(0) + vFeatures[*itr].fvOffset.x;
			vfvRightCentor[nClass].at(1) = vfvRightCentor[nClass].at(1) + vFeatures[*itr].fvOffset.y;
			vSampleperClass[nClass]++;
		}
		for(int i=1; i<vFeatures[0].nTotalClasses; i++)
			if(vSampleperClass[i]!=0)
				vfvRightCentor[i] = vfvRightCentor[i]/vSampleperClass[i];
		// Calculate the offset entropy of the right leaf, which is the sum of the offset distance among each
		// classes
		for(vector<uint32_t>::iterator itr = vIndex4Right.begin(); itr != vIndex4Right.end(); itr++)
		{
			int nClass =  vFeatures[*itr].nClassLabel;
			if(nClass!=0)
				dDistances = dDistances + pow((vfvRightCentor[nClass].at(0) - vFeatures[*itr].fvOffset.x),2)
				 					+ pow((vfvRightCentor[nClass].at(1) - vFeatures[*itr].fvOffset.y),2);
		}
		entropy = dDistances;
		break;
	default:
		break;
	}
}

uint32_t RandomNode::getnumberofOBJPatch(const vector<PatchSample>& vFeatures, const vector<uint32_t>& index)
{
	uint32_t numberofOBJPatch = 0;
	for(vector<uint32_t>::const_iterator itr = index.begin(); itr != index.end(); itr++)
	{
		if(vFeatures[*itr].nClassLabel != 0)
			numberofOBJPatch++;
	}
	return numberofOBJPatch;
}
uint32_t RandomNode::getnumberofBKGPatch(const vector<PatchSample>& vFeatures, const vector<uint32_t>& index)
{
	uint32_t numberofBKGPatch = 0;
	for(vector<uint32_t>::const_iterator itr = index.begin(); itr != index.end(); itr++)
	{
		if(vFeatures[*itr].nClassLabel == 0)
			numberofBKGPatch++;
	}
	return numberofBKGPatch;
}
/************/
/*	Test	*/
/************/
uint32_t RandomNode::testNode(const PatchSample& feature)
{
	// Maybe should check the size of the patch first. To be modified
	if(m_leftkey == 0&& m_rightkey ==0) return -1;
	if(feature.vChannels[m_channel](m_p, m_q).value() - feature.vChannels[m_channel](m_r, m_s).value()>m_t){
		return m_leftkey;
	}
	else return m_rightkey;
}

void RandomNode::calculateGeneralizedOOBError()
{
	if(fabs(sumFloatVector(m_dProportion) - 1) > SMALL_ENOUGH_PROPORTION || fabs(sumFloatVector(m_dProportion_Validation) - 1) > SMALL_ENOUGH_PROPORTION)
	{
		cout<<" Warning in calculateGeneralizedOOBError: invalid distributions."<<endl;
		cout<<" sumFloatVector(m_dProportion)="<<sumFloatVector(m_dProportion)<<endl;
		cout<<" sumFloatVector(m_dProportion_Validation)="<<sumFloatVector(m_dProportion_Validation)<<endl;
		generalizedOOBError = 1;
		return;
	}
	generalizedOOBError = calculateHellingerDistance(m_dProportion, m_dProportion_Validation);
}
void RandomNode::calculateDiscriminativeScore()
{
	//TODO: this is a tentary form of the disriminative score
	float maxentropy;
	vector<float> uniformproportions(m_dProportion.size(), 1);
	uniformproportions = normalizeProportions(uniformproportions);
	maxentropy = calculateEntropy(uniformproportions);
//	discriminativeScore = (0 - proportionofObjectClasses())*(maxentropy - entropyofObjectClasses()); 
	float denominator = 1 - proportionofObjectClasses();
	if(denominator == 0) denominator = 0.0001;
	discriminativeScore = 0-log(proportionofObjectClasses()/denominator) / log(2);
}
float RandomNode::proportionofObjectClasses()
{
	return (1-m_dProportion[0]);
}
float RandomNode::entropyofObjectClasses()
{
	vector<float> original_proportions(m_dProportion.begin()+1, m_dProportion.end());
	vector<float> normalized_proprotions = normalizeProportions(original_proportions);
	return calculateEntropy(normalized_proprotions);
}
/*===================================================================*/
/*                          Class RandomTree                         */
/*===================================================================*/
/********************/
/*   Constructor    */
/********************/
RandomTree::RandomTree():m_nTreeDepth(10), m_nMinSample(20), m_nWeekClassifier(50){}

RandomTree::RandomTree(int nTreeDepth, int nMinSample, int nWeekClassifier): m_nTreeDepth(nTreeDepth), m_nMinSample(nMinSample), m_nWeekClassifier(nWeekClassifier)
{}

RandomTree::~RandomTree(){}

/*void RandomTree::createRootNode(const vector<int>& vIndex)
{
	RandomNode tmp_node(m_nTreeNumber, m_nTreeDepth, m_nMaxTrial, m_nMinSample,1) ;
	tmp_node.loadPatches(vIndex);
	// Firstly clean the RandomNodes
	m_rnNodes.clear();
	// Then insert the prepared node
	m_rnNodes.insert(std::pair<int, RandonNode>(tmp_node.m_key, tmp_node));
	
}*/
/********************/
/*   Training 	    */
/********************/
void RandomTree::trainTree(const vector<PatchSample>& vFeatures, const vector<uint32_t>& vIndex)
{
	// For the root node, all the samples are assigned to it without sampling
	RandomNode rdRoot( m_nWeekClassifier, m_nMinSample, 1, 0, 0, m_nTreeDepth);
	//createRootNode(vIndex);
	// Train the root node
	vector<uint32_t> vTmp_Index4Left;
	vector<uint32_t> vTmp_Index4Right;
	rdRoot.trainNode(vFeatures, vIndex,vTmp_Index4Left, vTmp_Index4Right);
	m_rnNodes.insert(make_pair<uint32_t, RandomNode>(rdRoot.getKey(), rdRoot));
	map<uint32_t, RandomNode>::iterator it;	
	it = m_rnNodes.find(1);
	// Traverse and train all the nodes' children untill all nodes either have no child 
	// , or its child has been trained. Note that the std::map stores the element accord-
	// ing to the keys' order. The following traverse method can ensure that traversing 
	// stops only when all the leaf nodes are reached. The nodes on the higher level are
	// always stored at the front of the map.
	bool trainOver = false;
	while(!trainOver)
	{
		if(it->second.getLeft()!=0 && m_rnNodes.count(it->second.getLeft())==0)
		// The  Left node has not been inserted
		{
		RandomNode rnNode(m_nWeekClassifier, m_nMinSample, it->second.getLeft(), it->second.getKey(), it->second.getLevel()+1, m_nTreeDepth);
		vector<uint32_t> vLIndex;
		it->second.loadLeftPatches(vLIndex);
		rnNode.trainNode(vFeatures, vLIndex);
		m_rnNodes.insert(make_pair<uint32_t, RandomNode>(rnNode.getKey(), rnNode));
		}
		else if(it->second.getRight()!=0 && m_rnNodes.count(it->second.getRight()) == 0)
		{
		// The Right node has not been inserted
		RandomNode rnNode(m_nWeekClassifier, m_nMinSample, it->second.getRight(), it->second.getKey(), it->second.getLevel()+1, m_nTreeDepth);
		vector<uint32_t> vRIndex;
	 	it->second.loadRightPatches(vRIndex);
		rnNode.trainNode(vFeatures, vRIndex);
		m_rnNodes.insert(make_pair<uint32_t, RandomNode>(rnNode.getKey(), rnNode));	
		}
		else
		{
		// All the children  nodes have been inserted, or this node has no child.
		it++;
		if(it == m_rnNodes.end()) trainOver = true;
		}
		
	}
	
}

void RandomTree::validateTree(const vector<PatchSample>& vFeatures, const vector<uint32_t>& validationSampleIndex)
{
	clearandinitiateValidationData(vFeatures[0].nTotalClasses);
	validateWithValidationSamples(vFeatures, validationSampleIndex);
	seiriValidationData();
}

void RandomTree::clearandinitiateValidationData(int numofclasses)
{
	for(map<uint32_t, RandomNode>::iterator itr = m_rnNodes.begin(); itr!=m_rnNodes.end(); itr++)
	{
		itr->second.m_dProportion_Validation.assign(numofclasses, 0);
		itr->second.m_nValidPatches_Validation.assign(numofclasses, 0);
		vector<FeatureVector> vfv;
		itr->second.m_vCoords_Validation.assign(numofclasses, vfv);
	}
}

void RandomTree::validateWithValidationSamples(const vector<PatchSample>& vFeatures, const vector<uint32_t>& vIndex)
{
	for(vector<uint32_t>::const_iterator itr = vIndex.begin(); itr != vIndex.end(); itr++)
	{
		uint32_t reachedNodeKey = testTree(vFeatures[*itr]);
		map<uint32_t, RandomNode>::iterator leafnodeitr= m_rnNodes.find(reachedNodeKey);
		int classlabel = vFeatures[*itr].nClassLabel;
		leafnodeitr->second.m_nValidPatches_Validation[classlabel]++;
		FeatureVector coord(2);
		coord.setValue(0, vFeatures[*itr].fvOffset.x);
		coord.setValue(1, vFeatures[*itr].fvOffset.y);
		leafnodeitr->second.m_vCoords_Validation[classlabel].push_back(coord);
		leafnodeitr->second.appendReachedSample(*itr);
	}
}

void RandomTree::seiriValidationData()
{
	for(map<uint32_t, RandomNode>::iterator leafnodeitr = m_rnNodes.begin(); 
		leafnodeitr != m_rnNodes.end();
		leafnodeitr++)
	{
		leafnodeitr->second.m_dProportion_Validation = num2percent(leafnodeitr->second.m_nValidPatches_Validation);
	}
}


/*======================================================================*/
/* Function for testing a new sample with the trained tree. The returned
 *  value is the key of leaf node that the sample has reached.	    	*/
/*======================================================================*/
uint32_t RandomTree::testTree(const PatchSample& feature)			
{
	uint32_t result_key = 1;
	uint32_t tmp_key = 1;
	while(tmp_key!=-1)
	{
		result_key = tmp_key;
		tmp_key = m_rnNodes[tmp_key].testNode(feature); 
	
	}	
	return result_key;
	
}

map<uint32_t, RandomNode> RandomTree::getLeafNode()
/*---------------------------------------------------------------------------------------*/
/* Function to return the leaf nodes of this tree, in a map storing the key and the node */
/*---------------------------------------------------------------------------------------*/
{
	map<uint32_t, RandomNode> mleafNodes;
	for(map<uint32_t, RandomNode>::iterator itr = m_rnNodes.begin();itr!= m_rnNodes.end();itr++){
		if(itr->second.getLeft()==0&&itr->second.getRight()==0){
			mleafNodes.insert(*itr);
		}
	}
	return mleafNodes;
}

void RandomTree::clearNodes()
{
	m_rnNodes.clear();
}

void RandomTree::insertNode(uint32_t key, RandomNode node)
{
	m_rnNodes.insert(pair<uint32_t, RandomNode>(key, node));
}
/*===================================================================*/
/*                          Class RandomForest                       */
/*===================================================================*/
/********************/
/*   Constructor    */
/********************/
RandomForest::RandomForest()
{
	m_nTreeNumber = 5;
	m_nTreeDepth = 10;
	m_nMinSample = 20;
	m_nWeekClassifier = 50;
	m_gui = 0;
	m_bClustersValid = false;
	m_eta = 0.8;
}

RandomForest::RandomForest(int nTreeNumber, int nTreeDepth, int nWeekClassifiers, int nMinSample):m_nTreeNumber(nTreeNumber),m_nTreeDepth(nTreeDepth), m_nMinSample(nMinSample), m_nWeekClassifier(nWeekClassifiers), m_gui(0)
{
	m_eta = 0.8;
}

RandomForest::~RandomForest(){};

/************/
/*	GUI		*/
/************/
RandomForestGUI* RandomForest::createGUI( QWidget *parent)
{
	if( m_gui != 0){
		cerr<< "Error in RandomForest::createGUI:"<<
			 "Tried to initialize the GUI twice"<< endl;
		return 0;
	}
	else{
		cout<< "Initializing random forest GUI "<< endl;
		m_gui = new RandomForestGUI( parent );
	}
	
  /* seed the random number generator */
  timeval time;
  gettimeofday( &time, NULL );
  srandom( time.tv_usec ); 
  srand(time.tv_usec);

  return m_gui;
}
/********************/
/*  Parameters      */
/********************/

void RandomForest::setParams(	int nTreeNumber,
				int nTreeDepth,
				int nWeekClassifier,
				int nMinSample)
{
	m_nTreeNumber = nTreeNumber;
	m_nTreeDepth = nTreeDepth;
	m_nWeekClassifier = nWeekClassifier;
	m_nMinSample = nMinSample;
}

int RandomForest::getTotalClasses()
{
	if(m_vPatchSample.size()>0)
		return m_vPatchSample[0].nTotalClasses;
	else
		return m_nTotalClasses;
}

/********************/
/*  Training        */
/********************/
void RandomForest::trainRandomForest()
{
	if(m_vPatchSample.size()!=0)
	{
		trainRandomForest(m_vPatchSample);
	}
}
void RandomForest::trainRandomForest(const vector<PatchSample>& vFeatures)
{
	// Update all params
	m_nTreeNumber = m_gui->m_nTreeNum;
	m_nTreeDepth  = m_gui->m_nTreeDepth;
	m_nWeekClassifier = m_gui->m_nWeekClas;
	m_nMinSample = m_gui->m_nMinSample;
	// Display all parameters
	cout<< "Starting random forest training..."<<endl;
	cout<< "m_nTreeNumber: "<<m_nTreeNumber<<endl;
	cout<< "m_nTreeDepth: "<<m_nTreeDepth<<endl;
	cout<< "m_nWeakClassifier: "<<m_nWeekClassifier<<endl;
	cout<< "m_nMinSample:" << m_nMinSample<<endl;
	//cout<< "m_nMaxTrial:" << m_nMaxTrial<< endl;
	//Check all the parameters
	if(m_nTreeNumber<=0||m_nTreeDepth<=0||m_nWeekClassifier<=0){
		cerr<< "Errors in the parameter setting!"<<endl;
		return;
	}
	// Clean the original tree
	m_vTrees.clear();
	// Derive m_vImagePatches from vFeatures
	// If the forest is trained for the first time, assign the samples to the members
	m_vImagePatches.clear();
	if(m_vPatchSample.size() == 0)
	{
		m_vPatchSample = vFeatures;
	}
	if(m_vImagePatches.size() == 0)
	{
		for(vector<PatchSample>::const_iterator itr = vFeatures.begin(); itr!=vFeatures.end(); itr++){
			m_vImagePatches.push_back(itr->vChannels[0]);
		}

	}
	/*----------------------*/
	/*  Bootstrap sampling  */
	/*----------------------*/
	for(int i=0;i<m_nTreeNumber;i++){
	// Randomly select a set of training samples. The size of selected samlele is now set as 1/nTreeNumber. MIGHT NEED REVISE!
	//int nSampleperTree = vFeatures.size()/m_nTreeNumber;
	//int nSampleperTree = static_cast<uint32_t>(vFeatures.size() * 0.67);
	uint32_t numoftrainigsample = static_cast<uint32_t>(vFeatures.size() * 0.67);
	vector<uint32_t> trainSampleIndex;
	vector<uint32_t> validationSampleIndex;
	vector<uint32_t> train_validationSampleIndex;
	clock_t start, end;
	start = clock();
	train_validationSampleIndex = bootstrapSamplesandTheonesleft(vFeatures.size(), numoftrainigsample);
	trainSampleIndex.assign(train_validationSampleIndex.begin(), train_validationSampleIndex.begin() + numoftrainigsample);	
	validationSampleIndex.assign(train_validationSampleIndex.begin() + numoftrainigsample + 1, train_validationSampleIndex.end());
	end = clock();
	cout<<"time for bootstraping samples: "<<static_cast<float>(end - start)/CLOCKS_PER_SEC<<endl;
	/*------------------------*/
	/*     Train each tree    */
	/*------------------------*/	
	start = clock();
	RandomTree rtRandomTree(m_nTreeDepth, m_nMinSample, m_nWeekClassifier);
	cout<< "Training tree "<<i<<"..."<<endl;
	rtRandomTree.trainTree(vFeatures, trainSampleIndex);
	end = clock();
	cout<<"time for training tree: "<<static_cast<float>(end - start)/CLOCKS_PER_SEC<<endl;
	start = clock();
	rtRandomTree.validateTree(vFeatures, validationSampleIndex);
	end = clock();
	cout<<"time for validating trees: "<<static_cast<float>(end - start)/CLOCKS_PER_SEC<<endl;
	// Save the trained tree
	m_vTrees.push_back(rtRandomTree);

	/*----------------------------*/
	/*     Save the leaf nodes    */
	/*----------------------------*/
 	map<uint32_t, RandomNode> leafNodes = rtRandomTree.getLeafNode();
	m_vmClusters.push_back(leafNodes);
	cout<<"Done"<<endl;
	}
}
vector<uint32_t> RandomForest::bootstrapSamplesandTheonesleft(uint32_t numofbase, float proportion)
{
	uint32_t numofsample = numofbase * proportion;
	return bootstrapSamplesandTheonesleft(numofbase, numofsample);
}

vector<uint32_t> RandomForest::bootstrapSamplesandTheonesleft(uint32_t numofbase, uint32_t numofsample)
/*============================================================*/
/* returns [training indexes; validation indexes] as a vector */
/*============================================================*/
{
	vector<uint32_t> train_validationSampleIndex; 
	train_validationSampleIndex = bootstrapSamples(numofbase, numofsample);
	sort(train_validationSampleIndex.begin(), train_validationSampleIndex.end());
	uint32_t currentSampleNumber = 0;
	uint32_t nextSampleNumber = 0;
	for(uint32_t j = 0; j < numofsample; j++)
	{
		currentSampleNumber = nextSampleNumber;
		nextSampleNumber = train_validationSampleIndex[j];
		for(int offset = 1; (currentSampleNumber+offset)< nextSampleNumber; offset++)
		{
			train_validationSampleIndex.push_back(currentSampleNumber + offset);
		}
	}
	return  train_validationSampleIndex;
}
vector<uint32_t> RandomForest::bootstrapSamples(uint32_t numofbase, long numofsample)
{
	vector<uint32_t> sampledIndex;
	for(uint32_t j=0;j<numofsample;j++){
		if(numofbase > RAND_MAX)
			sampledIndex.push_back(static_cast<uint32_t>((double)BigRand()
							/((double)RAND_MAX*RAND_MAX+RAND_MAX)
							*numofbase));
		else
			sampledIndex.push_back(static_cast<uint32_t>((double)rand()
							/(RAND_MAX)
							*numofbase));	
	}
	return sampledIndex;
}



void RandomForest::purifyTrainingSet(vector<uint32_t> &removedIndex ,vector<uint32_t> &top100DiscriminantIndex)
/*------------------------------------------------------------------------------*/
/* Estimate the probability of each POSITIVE sample patch being an inlier,      */
/* then estimate new mixing factor eta. Do the EM procedure until eta converges.*/
/* This function should be used after getClusterAssignment().					*/
/* After the purification, \alpha(1-\eta) of the possitive samples in           */
/* m_vPatchSample will be changed to be negative samples						*/
/*------------------------------------------------------------------------------*/
{
	float eta = m_eta;
	float lasteta = 0;
	int nIteration = 0;
	uint32_t   nPostive = 0;
	uint32_t   nLastPostive = 0;
	float dPr = 0;
	vector<FloatInt> vfiPrInlier;
	vector<FloatInt> vfiLastPrInlier;
	cout<<"******************************************"<<endl;
	cout<<"*         Start the purification         *"<<endl;
	cout<<"******************************************"<<endl;
	uint32_t rnd = 0;
	while(((lasteta - eta)>THRESHOLD_ETA||(lasteta - eta)<-THRESHOLD_ETA) && nIteration<MAXITERATION_ETA)
	{
		rnd++;
		cout<<"The "<<rnd<<"th round"<<endl; 	
		lasteta = eta;
		clock_t start, end;
		start = clock();
		/*------------------------------------------------------*/
		/*  Estimate each POSITIVE sample's inlier probability  */
		/*-----------------------------------------------------*/
		for(uint32_t s=0; s<m_vPatchSample.size(); s++)// for each training sample
		{
			end = clock();
			if((end - start)>1000)
			{
				cout<<"Completed : "<<s*100/m_vPatchSample.size()<<"%\r";
				start = clock();
			}
			if(m_vPatchSample[s].nClassLabel!=0)
			{
				float dPrInlier = 0;
				for(int i=0; i<m_nTreeNumber; i++)
				{
					RandomNode node = getNode(i, m_vvFullResult[s][i]);
					float dPosi = 1 - node.m_dProportion[0];
					dPrInlier+= eta*dPosi/(eta*dPosi + (1-eta)*(1-dPosi));	
				}
				if(dPrInlier == dPrInlier)
				{
					dPrInlier = dPrInlier / static_cast<float>(m_nTreeNumber);
					FloatInt fiPrIndex;
					fiPrIndex.dvalue = dPrInlier;
					fiPrIndex.nvalue = s;
					vfiPrInlier.push_back(fiPrIndex);
					nPostive++;
					dPr+=dPrInlier;
					//cout<<" Calculating the "<<s<<"th patch's possibility of being an inlier: "<<dPrInlier<<endl;
					// TODO: delete this
					//cout<<" Pr"<<dPr<<endl;
				}
			}
		}// end for each training sample
			
		/*----------------*/
		/*  Estimate eta  */
		/*----------------*/
		eta = dPr / static_cast<float>(nPostive);
		cout<<endl;
		cout<<"eta(mixing factor): "<<eta<<endl;
		nIteration++;
		nLastPostive = nPostive;
		vfiLastPrInlier = vfiPrInlier;
		nPostive = 0;
		dPr = 0;
		vfiPrInlier.clear();
	}
	m_eta = eta;
	cout<<" Currently estimated mixing factor:"<<eta<<endl;
	/*--------------------------------------*/
	/* Rank the patches' inlier probability */
	/*--------------------------------------*/
	sort(vfiLastPrInlier.begin(), vfiLastPrInlier.end(), cmpFloatInt);
	/*------------------------------------------------------*/
	/* Change the labels of the top \alpha(1-\eta) outliers */
	/*------------------------------------------------------*/
	for(uint32_t i=0; i<PROPOTION_REJECT*(1-eta)*nLastPostive; i++)
	{
		m_vPatchSample[vfiLastPrInlier[i].nvalue].nClassLabel = 0;
		removedIndex.push_back(vfiLastPrInlier[i].nvalue);
	}
	for(uint32_t i=vfiLastPrInlier.size()-100; i<vfiLastPrInlier.size(); i++)
	{
		top100DiscriminantIndex.push_back(vfiLastPrInlier[i].nvalue);
	}
}
vector<LeafNodeScore> RandomForest::rankLeafNodes()
//TODO
{
	vector<LeafNodeScore> discriminativeScores = calculateLeafNodeScore();
	sort(discriminativeScores.begin(), discriminativeScores.end(), cmpLeafNodeScore);
	m_rankedLeafNodeScore = discriminativeScores;
	saveRankedLeafNodeandScore(discriminativeScores);
	saveSampleandImageCoverage2LeafNodeNumber(discriminativeScores);

	return discriminativeScores;

}

void RandomForest::saveRankedLeafNodeandScore(const vector<LeafNodeScore>& rankedLeafNodeScore)
{
	fstream filerankedleafnodes;
	filerankedleafnodes.open("rankedleafnode.txt", fstream::in | fstream::out | fstream::trunc);
	for(vector<LeafNodeScore>::const_iterator itr = rankedLeafNodeScore.begin(); itr != rankedLeafNodeScore.end(); itr++)
	{
		filerankedleafnodes << itr->index.tree <<" "<< itr->index.key <<" "<< itr->score << endl;
	}
	filerankedleafnodes.close();
}

void RandomForest::saveSampleandImageCoverage2LeafNodeNumber(const vector<LeafNodeScore>& rankedLeafNodeScore)
{
	vector<uint32_t> sampleCoveredFlag(m_vPatchSample.size(), 0);
	vector<uint32_t> imageCoveredFlag(m_vPatchSample.size(), 0);
	vector<uint32_t> imageCoveredFlagofPossitive(m_vPatchSample.size(), 0);
	int leafnodecount = 0;
	fstream filesamplecovered2leafnodenumber;
	fstream fileimagecovered2leafnodenumber;
	fstream fileimagecoveredofpossitive2leafnodenumber;
	filesamplecovered2leafnodenumber.open("samplecoverage.txt", fstream::in | fstream::out | fstream::trunc);
	fileimagecovered2leafnodenumber.open("imagecoverage.txt", fstream::in | fstream::out | fstream::trunc);
	fileimagecoveredofpossitive2leafnodenumber.open("possitiveimagecoverage.txt", fstream::in | fstream::out | fstream::trunc);
	for(leafnodecount = 0; leafnodecount < rankedLeafNodeScore.size(); leafnodecount++)
	{
		int tree = rankedLeafNodeScore[leafnodecount].index.tree;
		uint32_t key = rankedLeafNodeScore[leafnodecount].index.key;
		vector<uint32_t> reachedSample = m_vmClusters[tree][key].getReachedSample();
		for(vector<uint32_t>::iterator itr = reachedSample.begin(); itr != reachedSample.end(); itr++)
		{
			sampleCoveredFlag[*itr] = 1;
			imageCoveredFlag[m_vPatchSample[*itr].nImageIndex] = 1;
			if(m_vPatchSample[*itr].nClassLabel != 0)
				imageCoveredFlagofPossitive[m_vPatchSample[*itr].nImageIndex] = 1; 
		}
		filesamplecovered2leafnodenumber <<leafnodecount <<" "<<sumIntVector(sampleCoveredFlag) << endl;
		fileimagecovered2leafnodenumber << leafnodecount <<" "<<sumIntVector(imageCoveredFlag) << endl; 
		fileimagecoveredofpossitive2leafnodenumber<< leafnodecount <<" "<<sumIntVector(imageCoveredFlagofPossitive) << endl;
	}
	filesamplecovered2leafnodenumber.close();
	fileimagecovered2leafnodenumber.close();
	fileimagecoveredofpossitive2leafnodenumber.close();
}

vector< vector<Tuple_Sample_LeafNodeIndex> > RandomForest::getTwoLeafNodeCoveringofImages(uint32_t totalimagenumber)
{
	vector< vector<Tuple_Sample_LeafNodeIndex> > sampleinleafnodeperimage(totalimagenumber);
	vector<int> imageCoveredCount(totalimagenumber, 0);
	vector<LeafNodeScore> rankedLeafNodeScore = m_rankedLeafNodeScore;
	for(uint32_t leafnodecount = 0; leafnodecount < rankedLeafNodeScore.size(); leafnodecount++)
	{
		int tree = rankedLeafNodeScore[leafnodecount].index.tree;
		uint32_t key = rankedLeafNodeScore[leafnodecount].index.key;
		vector<uint32_t> reachedSample = m_vmClusters[tree][key].getReachedSample();
		for(vector<uint32_t>::iterator itr = reachedSample.begin(); itr != reachedSample.end(); itr++)
		{
			++imageCoveredCount[m_vPatchSample[*itr].nImageIndex];
			Tuple_Sample_LeafNodeIndex sample_leafnode;
			sample_leafnode.leafnodeindex.tree = tree;
			sample_leafnode.leafnodeindex.key = key;
			sample_leafnode.sampleindex = *itr;
			sampleinleafnodeperimage[m_vPatchSample[*itr].nImageIndex].push_back(sample_leafnode);
			 
		}
		if(*min_element(imageCoveredCount.begin(), imageCoveredCount.end()) == 2)
			break;
	}
	fstream fileleafnodeinImage;
	fileleafnodeinImage.open("twoleafnodesinimage.txt", fstream::in | fstream::out | fstream::trunc);
	for(int i = 0; i < sampleinleafnodeperimage.size(); i++)
	{
		fileleafnodeinImage << i << " ";
		for(int j=0; j < sampleinleafnodeperimage[i].size(); j++)
		{
			fileleafnodeinImage << sampleinleafnodeperimage[i][j].leafnodeindex.tree << " "
								<< sampleinleafnodeperimage[i][j].leafnodeindex.key << " ";
		}
		fileleafnodeinImage << endl;
	}
	fileleafnodeinImage.close();
	return sampleinleafnodeperimage;
}

vector< vector<Tuple_Sample_LeafNodeIndex> > RandomForest::getTwoLeafNodeCoveringofImagesofPossitive(uint32_t totalimagenumber)
{
	vector< vector<Tuple_Sample_LeafNodeIndex> > positivesampleinleafnodeperimage(totalimagenumber);
	vector<int> imageCoveredCount(totalimagenumber, 0);
	vector<LeafNodeScore> rankedLeafNodeScore = m_rankedLeafNodeScore;
	for(uint32_t leafnodecount = 0; leafnodecount < rankedLeafNodeScore.size(); leafnodecount++)
	{
		int tree = rankedLeafNodeScore[leafnodecount].index.tree;
		uint32_t key = rankedLeafNodeScore[leafnodecount].index.key;
		vector<uint32_t> reachedSample = m_vmClusters[tree][key].getReachedSample();
		for(vector<uint32_t>::iterator itr = reachedSample.begin(); itr != reachedSample.end(); itr++)
		{
			if(m_vPatchSample[*itr].nClassLabel !=0 )
			{
				++imageCoveredCount[m_vPatchSample[*itr].nImageIndex];
				
			}
			Tuple_Sample_LeafNodeIndex sample_leafnode;
			sample_leafnode.leafnodeindex.tree = tree;
			sample_leafnode.leafnodeindex.key = key;
			sample_leafnode.sampleindex = *itr;
			positivesampleinleafnodeperimage[m_vPatchSample[*itr].nImageIndex].push_back(sample_leafnode);
		}
		if(*min_element(imageCoveredCount.begin(), imageCoveredCount.end()) == 2)
			break;
	}
	
	fstream fileleafnodeinImage;
	fileleafnodeinImage.open("twopositiveleafnodesinimage.txt", fstream::in | fstream::out | fstream::trunc);
	for(int i = 0; i < positivesampleinleafnodeperimage.size(); i++)
	{
		fileleafnodeinImage << i << " ";
		for(int j=0; j < positivesampleinleafnodeperimage[i].size(); j++)
		{
			fileleafnodeinImage << positivesampleinleafnodeperimage[i][j].leafnodeindex.tree << " "
								<< positivesampleinleafnodeperimage[i][j].leafnodeindex.key << " ";
		}
		fileleafnodeinImage << endl;
	}
	fileleafnodeinImage.close();
	
	return positivesampleinleafnodeperimage;

}

vector< vector<Tuple_Sample_LeafNodeIndex> > RandomForest::getAverage5LeafNodePerImageCoverage(uint32_t totalimagenumber)
{
	uint32_t leafnodeperImage = 10;
	vector< vector<Tuple_Sample_LeafNodeIndex> > sampleinleafnodeperimage(totalimagenumber);
	vector<uint32_t> imageCoveredCount(totalimagenumber, 0);
	vector<LeafNodeScore> rankedLeafNodeScore = m_rankedLeafNodeScore;
	for(uint32_t leafnodecount = 0; leafnodecount < rankedLeafNodeScore.size()/20; leafnodecount++)
	{
		int tree = rankedLeafNodeScore[leafnodecount].index.tree;
		uint32_t key = rankedLeafNodeScore[leafnodecount].index.key;
		vector<uint32_t> reachedSample = m_vmClusters[tree][key].getReachedSample();
	    	
		for(vector<uint32_t>::iterator itr = reachedSample.begin(); itr != reachedSample.end(); itr++)
		{
			if(m_vPatchSample[*itr].nClassLabel != 0)
			{
				Tuple_Sample_LeafNodeIndex sample_leafnode;
				sample_leafnode.leafnodeindex.tree = tree;
				sample_leafnode.leafnodeindex.key = key;
				sample_leafnode.sampleindex = *itr;
				sample_leafnode.discriminativeScore = m_vmClusters[tree][key].discriminativeScore;
				sampleinleafnodeperimage[m_vPatchSample[*itr].nImageIndex].push_back(sample_leafnode);	
			}
		}
	}

	fstream fileleafnodeinImage;
	fileleafnodeinImage.open("average5leafnodesinimage.txt", fstream::in | fstream::out | fstream::trunc);
	for(int i = 0; i < sampleinleafnodeperimage.size(); i++)
	{
		sort(sampleinleafnodeperimage[i].begin(), sampleinleafnodeperimage[i].end(), cmpTuple_Sample_LeafNode_WithScore);
		fileleafnodeinImage << i << " ";
		for(int j=0; j < sampleinleafnodeperimage[i].size(); j++)
		{
			fileleafnodeinImage << sampleinleafnodeperimage[i][j].leafnodeindex.tree << " "
								<< sampleinleafnodeperimage[i][j].leafnodeindex.key << " "
								<< sampleinleafnodeperimage[i][j].discriminativeScore;
		}
		fileleafnodeinImage << endl;
	}
	fileleafnodeinImage.close();
	return sampleinleafnodeperimage;
}

vector<LeafNodeScore>  RandomForest::calculateLeafNodeScore()
{
	fstream fileleafnodescore;
	fileleafnodescore.open("leafnodescore.txt", fstream::in | fstream::out | fstream::trunc);
	vector<LeafNodeScore> discriminativeScores;
	for(int i = 0; i < m_vmClusters.size(); i++)
	{
		for(map<uint32_t, RandomNode>::iterator itr = m_vmClusters[i].begin(); itr !=  m_vmClusters[i].end(); itr++)
		{
			itr->second.calculateGeneralizedOOBError();
			itr->second.calculateDiscriminativeScore();
			//TODO  delete after test
			LeafNodeScore leafnodescore;
			leafnodescore.index.tree = i;
			leafnodescore.index.key = itr->first;
			//float score = itr->second.discriminativeScore + LAMBDA_PURITY_DISCRIMINATIVE*itr->second.generalizedOOBError;
			float score = itr->second.generalizedOOBError;
			leafnodescore.score = score;
			fileleafnodescore << i << " " << itr->first <<" "<<itr->second.discriminativeScore<<" "<<
				itr->second.generalizedOOBError<<" "<<score<<endl;
			discriminativeScores.push_back(leafnodescore);
		}
	}
	fileleafnodescore.close();
	return discriminativeScores;
}
/*======================================================================================*/
/*                                  Testing                                             */
/*======================================================================================*/

void RandomForest::testRandomForest(const PatchSample& psPatch, vector<uint32_t>& resultKeys )
/*----------------------------------------------------------------------------*/
/* Test psPatch with the trained random forset. The result is stored in resultK
 * eys, which is the key of the leaf nodes in each random tree.
 * ---------------------------------------------------------------------------*/
{
	resultKeys.clear();
	for(int i=0;i<m_nTreeNumber;i++){
		uint32_t key = m_vTrees[i].testTree(psPatch);
		resultKeys.push_back(key);	
	}
}
void RandomForest::testRandomForestSingleClass( const PatchSample&		patch,
												const float 			dThres,
												vector<FeatureVector>&	vfvCoords,
												vector<float>&			vValue,
												vector<uint32_t>&			vnLeafNodes,
												vector<float>&			vdConfidence,
												int						nClass) 
/*----------------------------------------------------------------------------*/
/* Test psPatch with the trained random forset. The params for estimating the 
 * probability votes are stored in vfvCoords and vValue. vfvCoords stores the 
 * offset vectors of each trainning patch in the resulting leaf nodes, vValue 
 * stores the coeffience.
 * ---------------------------------------------------------------------------*/
{
	vector<uint32_t> resultKeys;
	testRandomForest(patch, resultKeys);
	vfvCoords.clear();
	vValue.clear();
	vnLeafNodes.clear();
	vdConfidence.clear();
	vnLeafNodes = resultKeys;
	// Each tree's leaf node
	for(int i=0;i<resultKeys.size();i++)
	{
		RandomNode node= m_vmClusters[i].at(resultKeys[i]);
		vdConfidence.push_back(node.m_dProportion[nClass]);
		if(node.m_nValidPatches[nClass] == 0)
		{	
			cout<<" No valid patches in the leaf node in Tree:"<< i 
				<<" Key:"<<resultKeys[i]<<endl;	
		}
		else if(node.m_dProportion[nClass]>dThres)
		{
			for(uint32_t j=0;j<node.m_vCoords[nClass].size();j++)
			{
				// Coef = 1/T * Proportion / Valid Patches
				vValue.push_back(node.m_dProportion[nClass]/node.m_nValidPatches[nClass]/m_nTreeNumber);
				vfvCoords.push_back(node.m_vCoords[nClass][j]);
			}
		}
	}
}

void RandomForest::testRandomForestMultiClass(	const PatchSample&					patch,
												const float 						dThres,
												vector< vector<FeatureVector> >&	vvfvCoords,
												vector< vector<float> >&			vvValues,
												vector<uint32_t>&						vnLeafNodes,
												vector< vector<float> >&			vvdConfidence)
/*----------------------------------------------------------------------------------------------*/
/* Test the patch with the trained random forest,
 * inputs incluse:
 * 	patch: 			the patch to be tested
 * 	dThres:			the threshold for each leaf node's confidence					
 * outputs include:																				*/
/* 	vvfvCoords: 	<each class <each training offset> >										*/
/* 	vvValues:		<each class <each factor to be multiplied>>									*/
/*	vnLeafNodes: 	<key of the leaf node in each tree>											*/
/* 	vvdConfidence: 	<ecah class <confidence(propotion) in each tree's leaf node>>				*/
/*----------------------------------------------------------------------------------------------*/
{
	vector<uint32_t> resultKeys;
	testRandomForest(patch, resultKeys);
	vvfvCoords.clear();
	vvValues.clear();
	vnLeafNodes.clear();
	vvdConfidence.clear();
	vnLeafNodes = resultKeys;
	// Volatile varibles
	vector<FeatureVector> vfvCoords;
	vector<float> vValues;
	vector<float> vConfidence;
	
	vvfvCoords.push_back(vfvCoords);
	vvValues.push_back(vValues);
	vvdConfidence.push_back(vConfidence);

	//each class
	for(int c=1; c<m_vPatchSample[0].nTotalClasses;c++)
	{
		//each tree(leaf node)
		for(uint32_t i=0;i<resultKeys.size();i++)
		{
			RandomNode node = m_vmClusters[i].at(resultKeys[i]);
			if(node.m_nValidPatches[c] == 0)
			{	
				//cout<<" No valid patches in the leaf node in Tree:"<< i 
				//	<<" Key:"<<resultKeys[i]<<endl;	
			}
			else if(node.m_dProportion[c]>dThres)
			{
				for(uint32_t j=0;j<node.m_vCoords[c].size();j++)
				{
					// Coef = 1/T * Proportion / Valid Patches
					vValues.push_back(node.m_dProportion[c]/node.m_nValidPatches[c]/m_nTreeNumber);
					vfvCoords.push_back(node.m_vCoords[c][j]);
				}
			}
			vConfidence.push_back(node.m_dProportion[c]);
		}//end each tree
		vvfvCoords.push_back(vfvCoords);
		vvValues.push_back(vValues);
		vvdConfidence.push_back(vConfidence);
		vfvCoords.clear();
		vValues.clear();
		vConfidence.clear();
	}//end each class
}	

/*=====================================================================================*/
/*                                 Acquire Result                                      */
/*=====================================================================================*/

vector<vector<int> > RandomForest::getClusterAssignment()
{
/*------------------------------------------------------------------------
 *  This function returns the assignment of each sample loaded in the train
 *  ing. The first vector corresponds to the leaf nodes, the inner vector 
 *  stores the patches' index. The cluster assignment is derived  with a 
 *  full test on the training set.
 *------------------------------------------------------------------------*/
	m_vvFullResult.clear();
	vector<vector<int> > vCluserAssignment;
	if(m_vmClusters.size() == 0)
	{	
		cout<<" 	Clusters are not trained yet!"<<endl;
		return vCluserAssignment;
	}
	// Clear the cluster infomation	
	m_vClusterInfo.clear();
	//  Preassign the size of the cluster
	m_vmKey2Index.clear();
	map<uint32_t, uint32_t> mKey2Index;
	int nTreeIndex = 0;
	int nClusterIndex = 0;
	for(vector<map<uint32_t, RandomNode> >::iterator itr = m_vmClusters.begin();
		itr!= m_vmClusters.end();
		itr++){
		m_vmKey2Index.push_back( mKey2Index );
		for(map<uint32_t, RandomNode>::iterator mitr = itr->begin(); 
			mitr!=itr->end(); 
			mitr++){
				ostringstream os;
				os	<< "# Tree: "	<< nTreeIndex 
					<<", Level:"	<< mitr->second.getLevel()
					<< ", Key:"		<< mitr->second.getKey();
				m_vClusterInfo.push_back(os.str());
				vector<int> vTmp;
				vCluserAssignment.push_back( vTmp );
				m_vmKey2Index[nTreeIndex].insert(make_pair<uint32_t, uint32_t>(mitr->second.getKey(), nClusterIndex));
				nClusterIndex++;
		}
		nTreeIndex++;
	}

	int nImgIndex = 0;
	// Test each patch
	for(vector<OpGrayImage>::iterator itr = m_vImagePatches.begin();
		itr != m_vImagePatches.end();
		itr++){
		vector<uint32_t> resultKeys;
		PatchSample psTest;
		psTest.vChannels.push_back(*itr);
		testRandomForest( psTest, resultKeys);
	    m_vvFullResult.push_back(resultKeys);
		for(uint32_t i = 0; i < resultKeys.size(); i++){
		vCluserAssignment[m_vmKey2Index[i][resultKeys[i]]].push_back(nImgIndex);
		}
		nImgIndex++;
	}
/*
	int nTreeIndex = 0;
	for(vector<map<int, RandomNode> >::iterator itr = m_vmClusters.begin(); itr!=m_vmClusters.end();itr++){
		for(map<int, RandomNode>::iterator mitr = itr->begin(); mitr!=itr->end(); mitr++){
			if(mitr->second.getLeft()==0 && mitr->second.getRight()==0){
				// This is a leaf node
				vector<int> patchesInnode;
				mitr->second.loadLeftPatches(patchesInnode);
				vCluserAssignment.push_back(patchesInnode);
				ostringstream os;
				os	<< "# Tree: "	<< nTreeIndex 
					<<", Level:"	<< mitr->second.getLevel()
					<< ", Key:"		<< mitr->second.getKey();
				
				m_vClusterInfo.push_back(os.str());
			}
		}
		nTreeIndex++;
	}
*/
	m_vvClusterAssignment = vCluserAssignment;
	return vCluserAssignment;
}
void RandomForest::writeLeafNodetoFile( QString fileName )
/*-------------------------------------------------------*/
/* Write the leaf node info to file.                     */
/*-------------------------------------------------------*/
{
    fstream fsLeafNodeFile;
	fsLeafNodeFile.open(fileName.toStdString().data(), fstream::in | fstream::out | fstream::trunc); 
	int nTreeIndex = 0;
	for(vector<map<uint32_t, RandomNode> >::iterator itr = m_vmClusters.begin();
		itr!= m_vmClusters.end();
		itr++){
		for(map<uint32_t, RandomNode>::iterator mitr = itr->begin(); 
			mitr!=itr->end(); 
			mitr++){
						fsLeafNodeFile<< nTreeIndex <<" "
						<< mitr->second.getLevel()<<" "
						<< " "<< mitr->second.getKey()<<endl;
						vector<float> dProportion = mitr->second.m_dProportion;
						for(vector<float>::iterator ditr=dProportion.begin();
							ditr!=dProportion.end();
							ditr++)
						{
							fsLeafNodeFile<<*ditr<<" ";
						}
						fsLeafNodeFile<<endl;
					}
		nTreeIndex++;
	}
    fsLeafNodeFile.close();
}
void RandomForest::computeClusterCenters_HF()
{
	// Get the cluster in random forest format
	vector<vector<int> > vvAssignment = getClusterAssignment();
	// Fake a m_vClusterAssignment
	m_vClusterAssignment.clear();
	m_vClusterAssignment.assign(m_vImagePatches.size(),0);
	
	// Calculate average Patch in every leaf node and store in m_vClusterPatches
	OpGrayImage imgTmp( m_vImagePatches.front().width(),
                            m_vImagePatches.front().height() );
   	vector<OpGrayImage> vTmpImg( vvAssignment.size(), imgTmp );
   	m_vClusterPatches = vTmpImg;
	int currentIndex = 1;
	for(vector<vector<int> >::iterator itr_1 = vvAssignment.begin(); itr_1!=vvAssignment.end();itr_1++){
		
		for(vector<int>::iterator itr_2 = itr_1->begin(); itr_2!=itr_1->end();itr_2++){
			m_vClusterAssignment[*itr_2] = currentIndex;
			m_vClusterPatches[currentIndex - 1] = m_vClusterPatches[currentIndex-1].add(m_vImagePatches[*itr_2].div(itr_1->size()));
		}
		m_vClusterPatches[currentIndex-1] = m_vClusterPatches[currentIndex-1];
		currentIndex++;
	}
	m_bClustersValid = true;
}

void RandomForest::drawClusters(QtClusterView *qClassView)
{
	if(!m_bClustersValid)
		computeClusterCenters_HF();
	qClassView->loadImageSets( m_vClusterPatches, "cl",
							   m_vImagePatches, m_vvClusterAssignment,
							   m_vClusterInfo);
}

RandomNode RandomForest::getNode(int nTree, uint32_t nKey)
/*----------------------------------------------------------*/
/* Get the leaf node in tree nTree and key nKey.			*/
/*----------------------------------------------------------*/
{
	return m_vmClusters[nTree].at(nKey);
}

PatchSample RandomForest::getPatchSample(uint32_t nIndex)
{
	return m_vPatchSample[nIndex];
}

OpGrayImage RandomForest::getMeanPatchOfLeafNode(int tree, uint32_t key)
{
	uint32_t indexofleafnode = m_vmKey2Index[tree][key];
	return m_vClusterPatches[indexofleafnode];
}
/*========================================================================*/
/*                      Save & Load Random Forest                         */
/*========================================================================*/
void RandomForest::saveRandomForest(const char* uri)
/*========================================================================*/
/* Save the part of the randomforest that are essential for the testing,  */
/* see the code to figure out what are saved and what are not.            */
/*========================================================================*/
{
	int 				rc;
	xmlTextWriterPtr 	writer;
	xmlChar				*tmp;
	stringstream 		ss;				
	/*--------------------------------*/
	/* Create a new xmlWriter for uri */
    /*--------------------------------*/
	writer = xmlNewTextWriterFilename(uri, 0);
	if(writer == NULL)
	{
		cout<<"saveRandomForest:  cannot create XmlWriter!"<<endl;
		return;
	}
	
	/*-------------------------------------------*/	
	/* Start the document with standard xml head */
	/*-------------------------------------------*/
	rc = xmlTextWriterStartDocument(writer, NULL, "ISO-8859-1", NULL);
	if(rc < 0)
	{
		cout<<"saveRandomForest:   Error at xmlTextWriterStartDocument"<<endl;
		return;
	}

	/*--------------------------------------*/	
	/* Start with the first element: Forest */
	/*--------------------------------------*/
	rc = xmlTextWriterStartElement(writer , BAD_CAST"Forest");
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterStartElement"<<endl;
		return;
	}
	
	/* Write the tree number */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"ntree", "%d", m_nTreeNumber);
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriteFormatElement of #tree"<<endl;
		return;
	}
	/* Write the tree depth */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"treedepth", "%d", m_nTreeDepth);
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of treedeoth"<<endl;
		return;
	}
	/* Write the number of weakclassifier */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"weakclassifier", "%d", m_nWeekClassifier);
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of weakclassifier"<<endl;
		return;
	}

	/* Write the number of minsample */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"minsample","%d", m_nMinSample);
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of #minsample"<<endl;
		return;
	}

	/* Write the number of totalclasses */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"totalclass","%d", getTotalClasses());
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of totalclasses"<<endl;
		return;
	}
	/*---------------------*/
	/* Add trees and nodes */
	/*---------------------*/
	for(int t = 0; t < m_nTreeNumber; t++)
	// for each tree
	{
		/* Add a tree as the child of the forest */
		rc = xmlTextWriterStartElement(writer, BAD_CAST"Tree");
		if(rc < 0)
		{
			cout<<"saveRandomForest:	Error at xmlTextWriterStartElement of tree "<<t<<endl;
			return;
		}
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"treeindex", "%d", t);
		if(rc < 0)
		{
			cout<<"saveRandomForest:	Error at xmlTextWriterStartElement of "<<endl;
			return;
		}
		for(map<uint32_t, RandomNode>::iterator nitr = m_vTrees[t].m_rnNodes.begin();
			nitr!=m_vTrees[t].m_rnNodes.end();
			nitr++)
		// for each element Node
		{
			/* Add a node as the child of the tree */
			rc = xmlTextWriterStartElement(writer, BAD_CAST"Node");
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterStartElement of Node"<<endl;
				return;
			}
			/* Write the key of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"key", "%d", nitr->second.getKey());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the parent of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"parent", "%d", nitr->second.getParent());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of parent of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the left child of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"left", "%d", nitr->second.getLeft());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of leftchild of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the right child of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"right", "%d", nitr->second.getRight());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of rightchild of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the level of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"level", "%d", nitr->second.getLevel());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of level of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the channel of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"channel", "%d", nitr->second.getChannel());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of channel of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the p of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"p", "%d", nitr->second.getp());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of p of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the q of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"q","%d", nitr->second.getq());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of q of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the r of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"r","%d", nitr->second.getr());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of r of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the s of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"s","%d", nitr->second.gets());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of s of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the t of the node */
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"t","%d", nitr->second.gett());
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of t of node "<<nitr->first<<" in tree "<<t<<endl;
				return;
			}
			/* Write the propotions, valid patches and offset vectors of each class */ 
			for(int c = 0; c < nitr->second.m_dProportion.size(); c++) // for each element Class
			{
				rc = xmlTextWriterStartElement(writer, BAD_CAST"Class");
				if(rc < 0)
				{
					cout<<"saveRandomForest:	Error at xmlTextWriterWriteStartElement of class of node "<<nitr->first<<" in tree "<<t<<endl;
					return;
				}
				/* Write the class label */
				rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"classlabel", "%d", c);
				if(rc < 0)
				{
					cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of classlabel of node "<<nitr->first<<" in tree "<<t<<endl;
					return;
				}
				/* Write the propotion */
				rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"proportion", "%f", nitr->second.m_dProportion[c]);
				if(rc < 0)
				{
					cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of propotion of node "<<nitr->first<<" in tree "<<t<<endl;
					return;
				}
				/* Write the validpatches */
				ss<<nitr->second.m_nValidPatches[c];
				rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"validpatches", "%d", nitr->second.m_nValidPatches[c]);
				if(rc < 0)
				{
					cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of propotion of node "<<nitr->first<<" in tree "<<t<<endl;
					return;
				}
				/* Write the offset coord. */
				for(uint32_t coo = 0; coo < nitr->second.m_vCoords[c].size(); coo++)
				// for each stored coordiante
				{
					// TODO Delete this  
					rc = xmlTextWriterStartElement(writer, BAD_CAST"Coord");
					if(rc < 0)
					{
						cout<<"saveRandomForest:	Error at xmlTextWriterStartElement of coord of node "<<nitr->first<<" in tree "<<t<<endl;
						return;
					}
					rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"x", "%f", nitr->second.m_vCoords[c][coo].at(0));
					if(rc < 0)
					{
						cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of coord of node "<<nitr->first<<" in tree "<<t<<endl;
						return;
					}
					rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"y", "%f", nitr->second.m_vCoords[c][coo].at(1));
					if(rc < 0)
					{
						cout<<"saveRandomForest:	Error at xmlTextWriterWriteFormatElement of coord of node "<<nitr->first<<" in tree "<<t<<endl;
						return;
					}
					rc = xmlTextWriterEndElement(writer); // end of element Coord
					if(rc < 0)
					{
						cout<<"saveRandomForest:	Error at xmlTextWriterEndElement of coord of node "<<nitr->first<<" in tree "<<t<<endl;
						return;
					}
				}// end for each stored coordinate
				rc = xmlTextWriterEndElement(writer); // end of element Class
				if(rc < 0)
				{
					cout<<"saveRandomForest:	Error at xmlTextWriterEndElement of class of node "<<nitr->first<<" in tree "<<t<<endl;
					return;
				}
			}// end for each element Class
			rc = xmlTextWriterEndElement(writer); // end of element Node
			if(rc < 0)
			{
				cout<<"saveRandomForest:	Error at xmlTextWriterEndElement of Node "<<nitr->first<<endl;
				return;
			}
		}// end for each element Node 
		rc = xmlTextWriterEndElement(writer); // end of element Tree
		if(rc < 0)
		{
			cout<<"saveRandomForest:	Error at xmlTextWriterEndElement of Tree "<<t<<endl;
			return;
		}
	}
	rc = xmlTextWriterEndDocument(writer); // end of root element Forest
	if(rc < 0)
	{
		cout<<"saveRandomForest:	Error at xmlTextWriterEndDocument"<<endl;
		return;
	}
	cout<<"=============End Saving RandomForest=============="<<endl;
	xmlFreeTextWriter(writer);
}

void RandomForest::loadRandomForest(const char* uri)
/*=============================================================*/
/* Load the RandomForest from xml file. Note that not all parts*/
/* of the RandomForest are loaded. Only the test function is   */
/* guranteed to work, other parts may or may not work.         */
/*=============================================================*/
{
	/*---------------------------------*/
	/* Clear the original RandomForest */
	/*---------------------------------*/
	m_vPatchSample.clear();
	m_vImagePatches.clear();
	m_vClusterPatches.clear();
	m_vClusterAssignment.clear();

	m_vTrees.clear();
	m_vmClusters.clear();
	m_vmKey2Index.clear();
	m_vvClusterAssignment.clear();

	/*----------------*/
	/* Read from file */
	/*----------------*/
	streamXMLFile(uri);
	/*------------------------------*/
	/* Verify the RandomForest class*/
	/*------------------------------*/
	//TODO: delete this part when the program is verified
	/*string ss(uri);
	unsigned int pos = ss.find(".xml");
	if(pos == string::npos)
	{
		pos = ss.find(".XML");
	}
	if(pos == string::npos)
	{
		pos = ss.find(".Xml");
	}
	if(pos == string::npos)
	{
		ss.append("_regn.xml");
	}
	else
	{
		ss.replace(pos, 4, "_regn.xml");
	}
	saveRandomForest(ss.c_str());*/
}

void RandomForest::streamXMLFile(const char* uri)
/*===============================================================================*/
/* Traverse through the xml file and read the information into this RandomForest */
/*===============================================================================*/
{
	xmlTextReaderPtr reader;
	int ret;

	reader = xmlReaderForFile(uri, NULL, 0);
	if(reader != NULL)
	{
		ret = xmlTextReaderRead(reader);
		while(ret == 1)
		{
			processXMLNode(reader);
			ret = xmlTextReaderRead(reader);
		}
		// When the file is finished, inform the process
		processXMLNode(NULL);
		xmlFreeTextReader(reader);
		if(ret != 0)
		{
			cout<<"streamXMLFile: Error "<<uri<<"can not be closed"<<endl;
		}
	}
	else
	{
		cout<<"streamXMLFile: Error "<<uri<<"can not be open"<<endl;
	}
}

void RandomForest::processXMLNode(xmlTextReaderPtr reader)
/*================================================================================*/
/* Parse each node.                                                               */
/*================================================================================*/
{
	static int treeindex, nodeindex, classindex;
	static RandomTree tree;
	static RandomNode node;
	static FeatureVector coord(2);
	static bool nodeStarted, treeStarted;
	xmlChar *name, *xmlvalue;
	char value[256];
	stringstream ss;
	if(reader == NULL)
	{
		// Insert the last node and insert the last tree
		tree.insertNode(node.getKey(),node);
		m_vTrees.push_back(tree);
	}
	else
	{
		name = xmlTextReaderName(reader);
		xmlvalue = xmlTextReaderValue(reader);
		if(xmlvalue!=NULL)
		{
			strcpy(value, (char *)xmlvalue);
		}
		if(xmlStrEqual(name, BAD_CAST"Forest"))
		{
			treeindex = 0;
			nodeindex = 0;
			nodeStarted = false;
			treeStarted = false;
		}
		else if(xmlStrEqual(name, BAD_CAST"ntree"))
		{
			 m_nTreeNumber = extractValue(reader);
			 xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"treedepth"))
		{
			 m_nTreeDepth = extractValue(reader);
			 xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"weakclassifier"))
		{
			 m_nWeekClassifier = extractValue(reader);
			 xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"minsample"))
		{
			 m_nMinSample = extractValue(reader);
			 xmlTextReaderRead(reader);ss.str(value);
		}
		else if(xmlStrEqual(name, BAD_CAST"totalclass"))
		{
			m_nTotalClasses = extractValue(reader);
			 xmlTextReaderRead(reader);
			
		}
		else if(xmlStrEqual(name, BAD_CAST"Tree"))
		{
			if(!treeStarted)
			{
				/*------------------------------------------------*/
				/* Push back the last Node and Push back the tree */
				/*------------------------------------------------*/
				if(treeindex != 0)
				{
					tree.insertNode(node.getKey(), node);
					m_vTrees.push_back(tree);
				}
				/*----------------*/
				/* Clear the tree */
				/*----------------*/
				tree.clearNodes();
				nodeindex = 0;
				treeindex++;

				treeStarted = true; 
			}
			else
				treeStarted = false;

		}
		else if(xmlStrEqual(name, BAD_CAST"treeindex"))
		{
			ss.str(value);
		//	ss >> treeindex;
		}
		else if(xmlStrEqual(name, BAD_CAST"Node"))
		{
			if(!nodeStarted)
			{
				if(nodeindex != 0)
				{
					tree.insertNode(node.getKey(), node);
				}	
				/*----------------*/
				/* Clear the node */
				/*----------------*/
				node.m_dProportion.clear();
				node.m_nValidPatches.clear();
				node.m_vCoords.clear();
				vector<FeatureVector> tmpv;
				node.m_vCoords.assign(m_nTotalClasses, tmpv);

				nodeindex++;
				nodeStarted = true;
			}
			else
				nodeStarted = false;
		}
		/*------------------------------------*/
		/* The content below will form a node */
		/*------------------------------------*/
		else if(xmlStrEqual(name, BAD_CAST"key"))
		{
			uint32_t key  = extractValue(reader);
			 xmlTextReaderRead(reader);;
			node.setKey(key);
		}
		else if(xmlStrEqual(name, BAD_CAST"parent"))
		{
			uint32_t parent = extractValue(reader);
			 xmlTextReaderRead(reader);
			node.setParent(parent);
		}
		else if(xmlStrEqual(name, BAD_CAST"left"))
		{

			uint32_t left = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setLeft(left);
		}
		else if(xmlStrEqual(name, BAD_CAST"right"))
		{
		
			uint32_t right = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setRight(right);
		}
		else if(xmlStrEqual(name, BAD_CAST"level"))
		{
			int level = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setLevel(level);
		}
		else if(xmlStrEqual(name, BAD_CAST"channel"))
		{
			int channel = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setChannel(channel);
		}
		else if(xmlStrEqual(name, BAD_CAST"p"))
		{
			int p = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setp(p);
		}
		else if(xmlStrEqual(name, BAD_CAST"q"))
		{
			int q = extractValue(reader);
     		xmlTextReaderRead(reader);
			node.setq(q);
		}
		else if(xmlStrEqual(name, BAD_CAST"r"))
		{
			int r = extractValue(reader);
			xmlTextReaderRead(reader);
			node.setr(r);
		}
		else if(xmlStrEqual(name,BAD_CAST"s"))
		{
			int s= extractValue(reader);
			 xmlTextReaderRead(reader);
			node.sets(s);
		}
		else if(xmlStrEqual(name, BAD_CAST"t"))
		{
			int t = extractValue(reader);
			xmlTextReaderRead(reader);
			node.sett(t);
		}
		else if(xmlStrEqual(name, BAD_CAST"Class"))
		{
		}
		else if(xmlStrEqual(name, BAD_CAST"classlabel"))
		{
			classindex = extractValue(reader);
			xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"proportion"))
		{
			float propotion = extractValue(reader);
			node.m_dProportion.push_back(propotion);
			xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"validpatches"))
		{
			int validpatches = extractValue(reader);
			node.m_nValidPatches.push_back(validpatches);
			xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"Coord"))
		{}
		else if(xmlStrEqual(name, BAD_CAST"x"))
		{
			double x = extractValue(reader);
			coord.setValue(0, x);
			xmlTextReaderRead(reader);
		}
		else if(xmlStrEqual(name, BAD_CAST"y"))
		{
			double y = extractValue(reader);
			coord.setValue(1, y);
			xmlTextReaderRead(reader); 
			node.m_vCoords[classindex].push_back(coord);
		}
	}// end else
}


/*==============================================*/
/*		     Global Function to be used			*/	
/*==============================================*/
uint32_t BigRand()
{
	cout<<"BigRand() called!"<<endl;
	assert((double)INT_MAX/(RAND_MAX) > RAND_MAX);
	return rand()*(RAND_MAX) + rand();
}

void poolFromIndex(const vector<uint32_t>& vIndex, uint32_t percent, vector<uint32_t>& out_vIndex)
{
	out_vIndex.clear();
	if(vIndex.size()>RAND_MAX){
	for(uint32_t j=0;j<vIndex.size()*percent/100;j++)
		out_vIndex.push_back(static_cast<int>((double)BigRand()/((double)RAND_MAX*RAND_MAX+RAND_MAX)*vIndex.size()));
	}
	else{
	for(uint32_t j=0;j<vIndex.size()*percent/100;j++)
		out_vIndex.push_back((double)rand()/(RAND_MAX)*vIndex.size());	
	}	
}

bool cmpFloatInt(FloatInt a, FloatInt b)
{
  return (a.dvalue < b.dvalue);
}

bool cmpLeafNodeScore(LeafNodeScore first, LeafNodeScore second)
{
	return (first.score < second.score);
}


bool cmpTuple_Sample_LeafNode_WithScore(Tuple_Sample_LeafNodeIndex first, Tuple_Sample_LeafNodeIndex second)
{
	return (first.discriminativeScore < second.discriminativeScore);
}

xmlChar *ConvertInput(const char *in, const char *encoding)
{
	    xmlChar *out;
	    int ret;
	    int size;
	    int out_size;
	    int temp;
	    xmlCharEncodingHandlerPtr handler;
	    if (in == 0)
	        return 0;
	    handler = xmlFindCharEncodingHandler(encoding);
	    if (!handler) {
	        printf("ConvertInput: no encoding handler found for '%s'\n", encoding ? encoding : "");
			return 0;
		 }
	    size = (int) strlen(in) + 1;
	    out_size = size * 2 - 1;
	    out = (unsigned char *) xmlMalloc((size_t) out_size);
	    if (out != 0) {
	        temp = size - 1;
	        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
	        if ((ret < 0) || (temp - size + 1)) {
	            if (ret < 0) {
	                printf("ConvertInput: conversion wasn't successful.\n");
	            } else {
	                printf("ConvertInput: conversion wasn't successful. converted: %i octets.\n", temp);
                }
             xmlFree(out);
	         out = 0;
	        } else {
	         out = (unsigned char *) xmlRealloc(out, out_size + 1);
			 out[out_size] = 0;  /*null terminating out */
		    }
		} else {
			printf("ConvertInput: no mem\n");
		}
        return out;
}

float extractValue(xmlTextReaderPtr reader)
{
	int ret = xmlTextReaderRead(reader);
	if(ret != 1)
	{
		cout<<"extractValue : Error in xmlTextReaderRead()"<<endl;
		return 0;
	}
	xmlChar *xmlstring = xmlTextReaderValue(reader);
	char charstring[256];
	if(xmlstring!=NULL)
	{
		strcpy(charstring, (char *)xmlstring);
	}
	stringstream ss;
	ss.str(charstring);
	float value;
	ss >> value;
	return value;
}

uint32_t sumIntVector(vector<uint32_t> thevector)
{
	uint32_t rc = 0;
	for(vector<uint32_t>::iterator itr = thevector.begin(); itr != thevector.end(); itr++)
	{
		rc = rc + *itr;
	}
	return rc;
}

float sumFloatVector(const vector<float>& thevector)
{
	float result = 0;
	for(vector<float>::const_iterator itr = thevector.begin(); itr != thevector.end(); itr++)
		result += *itr;
	return result;
}
vector<float> num2percent(const vector<uint32_t>& numvector)
{
	vector<float> percentvector;
	uint32_t sumofnum = sumIntVector(numvector);
	if(sumofnum != 0)
	{
		for(vector<uint32_t>::const_iterator itr = numvector.begin(); itr != numvector.end(); itr++)
		{
			percentvector.push_back(static_cast<float>(*itr)/sumofnum);
		}
	}
	return percentvector;
}

float calculateEucideanDistanceWithWeight(const vector<float>& distribution1, const vector<float>& distribution2, const vector<float>& weight)
{
	float result =  0;
	if(!(distribution1.size() == distribution2.size() && distribution1.size() == weight.size()))
	{
		cout<< "Erorr in calculateEucideanDistanceWithWeight: mismatch vector size."<<endl;
		return -1;
	}
	for(uint32_t i=0;  i < distribution1.size(); i++)
	{
		result += weight[i]*(distribution1[i]-distribution2[i])*(distribution1[i]-distribution2[i]);
	}
	result = sqrt(result);
	return result;
}
float calculateHellingerDistance(const vector<float>& distribution1, const vector<float>& distribution2)
{
	float result =  0;
	if(!(distribution1.size() == distribution2.size() ))
	{
		cout<< "Erorr in calculateHellingerDistance: mismatch vector size."<<endl;
		return -1;
	}
	for(uint32_t i=0;  i < distribution1.size(); i++)
	{
		result += (sqrt(distribution1[i])-sqrt(distribution2[i]))*(sqrt(distribution1[i])-sqrt(distribution2[i]));
	}
	result = sqrt(result)/sqrt(2);
	return result;
}
float calculateEucideanDistance(const vector<float>& distribution1, const vector<float>& distribution2)
{
	vector<float> weight(distribution1.size(), 1);
	return calculateEucideanDistanceWithWeight(distribution1, distribution2, weight);
}

vector<float> normalizeProportions(const vector<float>& original_proportions)
{
	float sum = sumFloatVector(original_proportions);
	vector<float> result;
	for(vector<float>::const_iterator itr = original_proportions.begin(); itr != original_proportions.end(); itr++)
	{
		result.push_back(*itr/sum);
	}
	return result;
}


float calculateEntropy(const vector<float>& proportions)
{
	float entropy = 0;
	for(vector<float>::const_iterator itr = proportions.begin(); itr != proportions.end(); itr++)
	{
		if(*itr < SMALL_ENOUGH_PROPORTION)
			entropy += -SMALL_ENOUGH_PROPORTION*log(SMALL_ENOUGH_PROPORTION);
		else
			entropy += -(*itr)*log(*itr);
	}
	return entropy;
}

bool IsInVector(uint32_t thevalue, vector<uint32_t> thevector)
{
	for(vector<uint32_t>::iterator itr=thevector.begin();itr!=thevector.end();itr++)
	{
		if(*itr == thevalue)
			return true;
	}
	return false;
}
