
/*******************************************************************/
/*                                                                 */
/*FILE         	randomforest.cc                                    */
/*AUTHORS     	Yuren Zhang                                        */
/*EMAIL       	kiekergaard@gmail.com                              */
/*                                                                 */
/*CONTENT	Class for  training and testing with random forest */
/*								   */
/*BEGIN		MON JAN 21 2013 				   */
/*LAST CHANGE	MON JAN 21 2013					   */
/*								   */
/*******************************************************************/

/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>

#include "randomforest.hh"

/*******************/
/*   Definitions   */
/*******************/
typedef std::pair<int, RandomNode> pairNode;
/********************************/
/*	Global Functions	*/
/********************************/
int BigRand();
void poolFromIndex(const vector<int>& vIndex, int percent, vector<int>& out_vIndex);


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

RandomNode::RandomNode(int nWeekClassifier,  int nMinSample,int key,  int parentKey, int level, int maxlevel):m_nWeekClassifier(nWeekClassifier),m_nMinSample(nMinSample), m_parentkey(parentKey), m_nLevel(level), m_nMaxLevel(maxlevel), m_key(key)
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
}

RandomNode::~RandomNode(){}

/************************/
/*	Access Patches	*/
/************************/
void RandomNode::loadPatches(const vector<int>& vIndex)
{
	vector<int>::const_iterator itr;
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
	
	// Do not copy the stored patches, parent and childer of the original RandomNode
	// Maybe it can be implemented such that when copying occurs, the original RandomNode
	// is wapped while the copying one get the full content.
}
/************************/
/*	Training	*/
/************************/

/*=======================================================================================*/
/* For training an internal node. The samples' indexes vIndex4Left and vIndex4Right are  */
/* returned for further training.If the node is successfully trained, it will return true*/
/*  If the depth of the leaf has reached the limit , or the  number of samples contained */
/*   in the node is less than the limit, this function will return false, meanning it is */ 
/*   a leaf node.									 */
/*=======================================================================================*/

bool RandomNode::trainNode(	const vector<PatchSample>& vFeatures,
				const vector<int>& vIndex,
				vector<int>& vIndex4Left,
				vector<int>& vIndex4Right)
{
	if(vIndex.size()<m_nMinSample || m_nLevel>=m_nMaxLevel)
	{
		m_rightkey = 0;
		m_leftkey = 0;
		// If the node is leaf node, the leftindex contains all the patches that fall in it.
		m_vLeftIndex = vIndex;
		return false;
	}
	else{
	srand(time(NULL));
	vector<int> 	channel;
	vector<int>	p;
	vector<int>	q;
	vector<int>	s;
	vector<int>	r;
	vector<int>	t;

	vector<float> vdEntropy;
	vector<vector<int> >	vIndex1;
	vector<vector<int> >	vIndex2;

	int imageSize = vFeatures[0].vChannels[0].width();
	int nChannels = vFeatures[0].nChannels;
	// Generate a series of week classifiers. The number of the classifiers is ?
	for(int i=0;i<m_nWeekClassifier;i++){
		vector<int> tmp_index_1;
		vector<int> tmp_index_2;
		// For test
	//	tmp_index_1.push_back(1);
		float entropy;

		channel.push_back(static_cast<int>((double)rand()/(RAND_MAX)*nChannels));
		p.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		q.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		r.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		s.push_back(static_cast<int>((double)rand()/(RAND_MAX)*imageSize));
		t.push_back(static_cast<int>((double)rand()/(RAND_MAX)*255));	

		calculateSplit(vFeatures, vIndex, channel.back(), p.back(), q.back(), r.back(), s.back(), t.back(), CLASS_ENTROPY, entropy, tmp_index_1,tmp_index_2);

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

void RandomNode::trainNode(const vector<PatchSample>& vFeatures, const vector<int>& vIndex)
{
	trainNode(vFeatures, vIndex, m_vLeftIndex, m_vRightIndex);

}

void RandomNode::trainLeafNode(	const vector<PatchSample>& vFeatures,
				const vector<int>& vIndex,
				RandomNode& rnLeftNode,
				RandomNode& rnRightNode)
{
	vector<int> vIndex4Left;
	vector<int> vIndex4Right;
	trainNode(vFeatures, vIndex, vIndex4Left, vIndex4Right);
	// Create the leaf nodes with vIndex4Left and vIndex4Right as their content m_cSampleIndex. To make sure they are the nodes, the leaves of these nodes are set to zero.
	rnLeftNode.loadPatches(vIndex4Left);
	rnLeftNode.setLeft(0);
	rnLeftNode.setRight(0);
	rnRightNode.loadPatches(vIndex4Right);
	rnRightNode.setLeft(0);
	rnRightNode.setRight(0);
		
}

void RandomNode::createLeafNode(const vector<int>& vIndex)
{
	m_vSampleIndex = vIndex;
	m_leftkey = 0;
	m_rightkey = 0;
}

void RandomNode::calculateSplit(const vector<PatchSample>& vFeatures,
				const vector<int>& vIndex,
				int c, int p, int q, int r, int s, int t,
				int entropyType,
				float& entropy,
				vector<int>& vIndex4Left,
				vector<int>& vIndex4Right)
{
	// Traverse all the samples assigned to this node
	for(vector<int>::const_iterator itr=vIndex.begin();itr!=vIndex.end();++itr){
		if(vFeatures[*itr].vChannels[c](p, q).value() - vFeatures[*itr].vChannels[c](r, s).value() > t)
			vIndex4Left.push_back(*itr);
		else
			vIndex4Right.push_back(*itr);

	}

	// Calculate the entropy of class or offset
	float tmp_entropy = 0;
	vector<int> classAccumulator(vFeatures[0].nTotalClasses, 0);
	float classPropotion;
	switch(entropyType){
	case CLASS_ENTROPY:
		// Accumulate the samples in each classes in the left leaf
		for(vector<int>::iterator  itr=vIndex4Left.begin();itr!=vIndex4Left.end();++itr){	
			classAccumulator[vFeatures[*itr].nClassLabel]+=1;
		}
		// Calculate the entropy for the left leaf
		for(int i = 0; i<vFeatures[0].nTotalClasses;++i){
			classPropotion = (double)classAccumulator[i]/vIndex4Left.size();
			tmp_entropy+= -classPropotion*log(classPropotion);
		}
		if(!(tmp_entropy==tmp_entropy)) tmp_entropy = 0;
		entropy = tmp_entropy * vIndex4Left.size();
		
		// Accumulate the samples in each classes in the right leaf
		classAccumulator.assign(vFeatures[0].nTotalClasses, 0);
		for(vector<int>::iterator  itr=vIndex4Right.begin();itr!=vIndex4Right.end();++itr){	
			classAccumulator[vFeatures[*itr].nClassLabel]+=1;
		}
		// Calculate the entropy for the right leaf
		tmp_entropy = 0;
		for(int i = 0; i<vFeatures[0].nTotalClasses;++i){
			classPropotion = (double)classAccumulator[i]/vIndex4Right.size();
			tmp_entropy+= -classPropotion*log(classPropotion);
		}
		if(!(tmp_entropy==tmp_entropy)) tmp_entropy = 0;
		entropy+= tmp_entropy * vIndex4Right.size();
		break;
	case POS_ENTROPY:
		// To be implemented
		break;
	default:
		break;
	}
}
/****************/
/*	Test	*/
/****************/

int RandomNode::testNode(const PatchSample& feature)
{
	// Maybe should check the size of the patch first. To be modified
	if(m_leftkey == 0&& m_rightkey ==0) return -1;
	if(feature.vChannels[m_channel](m_p, m_q).value() - feature.vChannels[m_channel](m_r, m_s).value()>m_t){
		return m_leftkey;
	}
	else return m_rightkey;
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
void RandomTree::trainTree(const vector<PatchSample>& vFeatures, const vector<int>& vIndex)
{
	// For the root node, all the samples are assigned to it without sampling
	RandomNode rdRoot( m_nWeekClassifier, m_nMinSample, 1, 0, 0, m_nTreeDepth);
	//createRootNode(vIndex);
	// Train the root node
	vector<int> vTmp_Index4Left;
	vector<int> vTmp_Index4Right;
	rdRoot.trainNode(vFeatures, vIndex,vTmp_Index4Left, vTmp_Index4Right);
	m_rnNodes.insert(make_pair<int, RandomNode>(rdRoot.getKey(), rdRoot));
	map<int, RandomNode>::iterator it;	
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
		vector<int> vLIndex;
		it->second.loadLeftPatches(vLIndex);
		rnNode.trainNode(vFeatures, vLIndex);
		m_rnNodes.insert(make_pair<int, RandomNode>(rnNode.getKey(), rnNode));
		}
		else if(it->second.getRight()!=0 && m_rnNodes.count(it->second.getRight()) == 0)
		{
		// The Right node has not been inserted
		RandomNode rnNode(m_nWeekClassifier, m_nMinSample, it->second.getRight(), it->second.getKey(), it->second.getLevel()+1, m_nTreeDepth);
		vector<int> vRIndex;
	 	it->second.loadRightPatches(vRIndex);
		rnNode.trainNode(vFeatures, vRIndex);
		m_rnNodes.insert(make_pair<int, RandomNode>(rnNode.getKey(), rnNode));	
		}
		else
		{
		// All the children  nodes have been inserted, or this node has no child.
		it++;
		if(it == m_rnNodes.end()) trainOver = true;
		}
		
	}
	
}
/*======================================================================*/
/* Function for testing a new sample with the trained tree. The returned
 *  value is the key of leaf node that the sample has reached.		*/
/*======================================================================*/
int RandomTree::testTree(const PatchSample& feature)			
{
	int result_key = 1;
	int tmp_key = 1;
	while(tmp_key!=-1)
	{
		result_key = tmp_key;
		tmp_key = m_rnNodes[tmp_key].testNode(feature); 
	
	}	
	return result_key;
	
}
/*--------------------------------------------------------------------------------------*/
/* Function to return the leaf nodes of this tree, in a map storing the key and the node*/
/*--------------------------------------------------------------------------------------*/
map<int, RandomNode> RandomTree::getLeafNode()
{
	map<int, RandomNode> mleafNodes;
	for(map<int, RandomNode>::iterator itr = m_rnNodes.begin();itr!= m_rnNodes.end();itr++){
		if(itr->second.getLeft()==0&&itr->second.getRight()==0){
			mleafNodes.insert(*itr);
		}
	}
	return mleafNodes;
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
}

RandomForest::RandomForest(int nTreeNumber, int nTreeDepth, int nWeekClassifiers, int nMinSample):m_nTreeNumber(nTreeNumber),m_nTreeDepth(nTreeDepth), m_nMinSample(nMinSample), m_nWeekClassifier(nWeekClassifiers)
{}

RandomForest::~RandomForest(){};

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


/********************/
/*  Training        */
/********************/

void RandomForest::trainRandomForest(const vector<PatchSample>& vFeatures)
{
	// Display all parameters
	cout<< "Starting random forest training..."<<endl;
	cout<< "m_nTreeNumber: "<<m_nTreeNumber<<endl;
	cout<< "m_nTreeDepth:"<<m_nTreeDepth<<endl;
	//cout<< "m_nMaxTrial:" << m_nMaxTrial<< endl;
	//Check all the parameters
	if(m_nTreeNumber<=0||m_nTreeDepth<=0||m_nWeekClassifier<=0){
		cerr<< "Errors in the parameter setting!"<<endl;
		return;
	}
	// Clean the original tree
	m_vTrees.clear();
	// Train each tree
	for(int i=0;i<m_nTreeNumber;i++){
	// Randomly select a set of training samples. The size of selected samlele is now set as 1/nTreeNumber. MIGHT NEED REVISE!
	srand(time(NULL));
	int nSampleperTree = vFeatures.size()/m_nTreeNumber;
	// When the size of all samples is smaller than 32767, use rand(), otherwise use bigrand()
	vector<int> vIndex;
	for(int j=0;j<nSampleperTree;j++){
		if(vFeatures.size()>RAND_MAX)
			vIndex.push_back(static_cast<int>((double)BigRand()/((double)RAND_MAX*RAND_MAX+RAND_MAX))*vFeatures.size());
		else
			vIndex.push_back(static_cast<int>((double)rand()/(RAND_MAX)*vFeatures.size()));	
	}	
	// Feed the samples to the tree training procedure	
	RandomTree rtRandomTree(m_nTreeDepth, m_nMinSample, m_nWeekClassifier);
	cout<< "Training tree "<<i<<"...";
	rtRandomTree.trainTree(vFeatures, vIndex);
	// Save the trained tree
	m_vTrees.push_back(rtRandomTree);
	// Get the leaf nodes in this tree
 	map<int, RandomNode> leafNodes = rtRandomTree.getLeafNode();
	m_vmClusters.push_back(leafNodes);
	cout<<"Done"<<endl;
	}
	
}

void RandomForest::testRandomForest(const PatchSample& psPatch, vector<int>& resultKeys)
{
	for(int i=0;i<m_nTreeNumber;i++){
		int key = m_vTrees[i].testTree(psPatch);
		resultKeys.push_back(key);	
	}
}	
/*-----------------------------------------------------------------------------------
 *  This function returns the assignment of each sample involved in the training.
 *  The first vector corresponds to the leaf nodes, the inner vector stores the patches' index
 *  It should be noted that NOT every sample involves in the training, hence the 
 *  sample left are assigned with the label of 0. The label can be 
 *  be derived with a full test on the training set, which may be more expensive.
 *-----------------------------------------------------------------------------------*/  
vector<vector<int> > RandomForest::getClusterAssignment()
{
	vector<vector<int> > vCluserAssignment;
	if(m_vmClusters.size() == 0)
	{	
		cout<<" 	Clusters are not trained yet!"<<endl;
		return vCluserAssignment;
	}
	for(vector<map<int, RandomNode> >::iterator itr = m_vmClusters.begin(); itr!=m_vmClusters.end();itr++){
		for(map<int, RandomNode>::iterator mitr = itr->begin(); mitr!=itr->end(); mitr++){
			if(mitr->second.getLeft()==0 && mitr->second.getRight()==0){
				// This is a leaf node
				vector<int> patchesInnode;
				mitr->second.loadLeftPatches(patchesInnode);
				vCluserAssignment.push_back(patchesInnode);
			}
		}
	}
	return vCluserAssignment;
}

/*==============================================================*/
/*		     Global Function to be used			*/	
/*==============================================================*/
int BigRand()
{
	assert((double)INT_MAX/(RAND_MAX) > RAND_MAX);
	return rand()*(RAND_MAX) + rand();
}

void poolFromIndex(const vector<int>& vIndex, int percent, vector<int>& out_vIndex)
{
	out_vIndex.clear();
	if(vIndex.size()>RAND_MAX){
	for(int j=0;j<vIndex.size()*percent/100;j++)
		out_vIndex.push_back(static_cast<int>((double)BigRand()/((double)RAND_MAX*RAND_MAX+RAND_MAX)*vIndex.size()));
	}
	else{
	for(int j=0;j<vIndex.size()*percent/100;j++)
		out_vIndex.push_back((double)rand()/(RAND_MAX)*vIndex.size());	
	}	
}

