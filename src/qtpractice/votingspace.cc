/****************************************************************************/
/* FILE        votingspace.cc                                               */
/* AUTHORS     Yuren Zhang                                                  */
/* EMAIL       kiekergaard@gmail.com; yuren.zhang@ia.ac.cn                  */
/* CONTNET     Implement of the voting space class, which gathers the voting
 *             information from the patches of a test image, and find the ma-
 *             ximum position as the final hypothesis.
 * BEGIN       2013.2.16
 * CHANGE      2013.3.21
 ****************************************************************************/
/********************/
/*     Include      */
/********************/
#include "votingspace.hh"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <qtimgbrowser.hh>

/*********************/
/*    HoughVote      */
/*********************/
HoughVote::HoughVote(float x, float y, float s, float dValue, int nIndex, 
					const vector<uint32_t>& vnLeafNodes, const vector<float>& vdConfidence)
{
	FeatureVector fvCoord( 3 );
	fvCoord.setValue( 0, x );
	fvCoord.setValue( 1, y );
	fvCoord.setValue( 2, s);
	createHoughVote(fvCoord, dValue, nIndex, vnLeafNodes, vdConfidence);
}

HoughVote::HoughVote( FeatureVector fvCoord, float dValue, int nIndex,
					const vector<uint32_t>& vnLeafNodes, const vector<float>& vdConfidence)
{
	createHoughVote(fvCoord, dValue, nIndex, vnLeafNodes, vdConfidence);
}


HoughVote::HoughVote( const HoughVote& other)
{
	copyFromOther( other );
}

HoughVote::~HoughVote()
{
}
void HoughVote::createHoughVote(FeatureVector fvCoord, float dValue, int nIndex, 
				const vector<uint32_t>& vnLeafNodes, const vector<float>& vdConfidence)
{
	m_fvCoord = fvCoord;
	m_dValue = dValue;
	m_nPatchIndex = nIndex;
	m_vnLeafNodes = vnLeafNodes;
	m_vdConfidence = vdConfidence;
}

HoughVote& HoughVote::operator=(const HoughVote& other)
{
	copyFromOther(other);
}
void HoughVote::copyFromOther(const HoughVote& other)
{
	m_fvCoord = other.m_fvCoord;
	m_dValue = other.m_dValue;
	m_nPatchIndex = other.m_nPatchIndex;
	m_vnLeafNodes = other.m_vnLeafNodes;
	m_vdConfidence = other.m_vdConfidence;
}


/**********************/
/*     VotingSpace    */
/**********************/
VotingSpace::VotingSpace(int nWidth, int nHeight, int nBoxWidth, int nBoxHeight, 
			int nMinScale, int nMaxScale,
			int nDims, vector<int> vBins, 
			RandomForest*  prfRandomForest)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBoxWidth = nBoxWidth;
	m_nBoxHeight = nBoxHeight;
	m_nMinScale = nMinScale;
	m_nMaxScale = nMaxScale;
	m_nDims = nDims;
	m_vBins.assign(nDims, 0);
	if(vBins.size()<nDims)
	{
		cout<<"error: vBins' dimension is less than"<< nDims<<endl;
	}
	for(int i=0; i<m_nDims;i++)
	{
		m_vBins[i] = vBins[i];
	}
    // The scale dimension is determined by min and max.
    m_vBins[2] = m_nMaxScale - m_nMinScale + 1;
	if(prfRandomForest!=NULL)
		m_prfRandomForest = prfRandomForest;
	else
	{
		cout<<" error: randomforest is not available."<<endl;
		exit(0);
	}
	createVotingSpace();
}

VotingSpace::~VotingSpace()
{	
	m_prfRandomForest = NULL;
}

void VotingSpace::loadRandomForest(RandomForest*  prfForest)
{
	m_prfRandomForest = prfForest;
}
void VotingSpace::createVotingSpace()
/*========================================================================*/
/* Reserve the space to store HoughVotes.                                 */
/*========================================================================*/
{
	createVotingSpace( m_vBins );
}
void VotingSpace::createVotingSpace(vector<int> vBins)
/*========================================================================*/
/* Reserve the space to store HoughVotes.                                 */
/*========================================================================*/
{
	m_vvlVotes.clear();
	long nBins = 1;
	for(int i=0;i<m_nDims;i++)
	{
		nBins*=m_vBins[i];
	}
	list<HoughVote> tmpHoughVote;
	vector< list<HoughVote> > tmpvHoughVote(nBins, tmpHoughVote);
	// !! The voting space for class 0 is also stored, but DO NOT use it !
	m_vvlVotes.assign(m_prfRandomForest->getTotalClasses(), tmpvHoughVote);
}

void VotingSpace::voteWithPatch(const PatchSample& patch, int nIndex, int nClass)
/*============================================================================*/
/* Test with a patch and fill the corresponding vote to the voting space.     */
/* This function only fill in the voting space of class nClass.               */
/*============================================================================*/
{
	// Center and Scale
	int x = patch.fvOffset.x;
	int y = patch.fvOffset.y;
	float dScale = patch.fvOffset.scale;
	// Test the patch in the randomforest
	float dThres = 1.0/m_vvlVotes.size();
	vector<FeatureVector> vfvCoords;
	vector<float> vCoeff;
	vector<uint32_t> vnLeafNodes;
	vector<float> vdConfidence;
	m_prfRandomForest->testRandomForestSingleClass( patch,dThres, vfvCoords, vCoeff, vnLeafNodes, vdConfidence, nClass );
	// Traverse the nearby region of the test patch in the Patch's scale.
	float dTmp = 100;
	int nTmpIndex;
	for(int i=m_nMinScale;i<=m_nMaxScale;i++)
	{
		if(fabs(dScale - pow(sqrt(2),i))<dTmp)
		{
			dTmp = fabs(dScale - pow(sqrt(2),i));
			nTmpIndex = i;
		}
	}
	if(dTmp>=THRESHOLD_SCALE)
	{
		cout<<"error in voteWithPatch: scale unidentified!"<<endl;
		exit(0);
	}
	// The start index and end index in the vector
	int nStartIndex = (nTmpIndex - m_nMinScale) * m_vBins[0] * m_vBins[1];
	int nEndIndex = nStartIndex + m_vBins[0]*m_vBins[1] - 1;
	FeatureVector fvBinOffsetScaled(2);
	FeatureVector fvDiff(2);
	for( int i=nStartIndex;i<=nEndIndex;i++)
	{
		FeatureVector fvBinPos = getBinOffSet(i);
		fvBinOffsetScaled.setValue(0, (fvBinPos.at(0) - x)*dScale);
		fvBinOffsetScaled.setValue(1, (fvBinPos.at(1) - y)*dScale);
		if(isInBox(x, y, dScale, fvBinPos.at(0), fvBinPos.at(1)))
		{
		// This bin is in the nearby box, calculate the votes.
			double value = 0; 
			for(int j=0;j<vCoeff.size();j++)
			{
				fvDiff = fvBinOffsetScaled - vfvCoords[j];
				/*cout<<"Voting for bin at("<<fvBinPos.at(0)<<","<<fvBinPos.at(1)
                    <<")"<<endl;
                cout<<"Diff = ("<<fvDiff.at(0)<<","<<fvDiff.at(1)<<")"<<endl;
                cout<<"Voting Score = "<<vCoeff[j]*exp(-fvDiff.dot(fvDiff)/2/EPS2)/2/PI/EPS2<<endl;*/
				value+=vCoeff[j]*exp(-fvDiff.dot(fvDiff)/2/EPS2)/2/PI/EPS2;
			}
		// generate the vote with this patch
			HoughVote vote(fvBinOffsetScaled.at(0), fvBinOffsetScaled.at(1), dScale,value, 
							nIndex, vnLeafNodes, vdConfidence);
		// Insert the vote in m_vvlVotes
			m_vvlVotes[nClass][i].push_back(vote);
		}
	}
	
}

void VotingSpace::voteWithPatch(const PatchSample& patch, int nIndex)
/*========================================================================*/
/* Test the patch and fill in the voting space. All classes are voted for */
/*========================================================================*/
{
	// Center and Scale
	int x = patch.fvOffset.x;
	int y = patch.fvOffset.y;
	float dScale = patch.fvOffset.scale;
	// Test the patch in the randomforest
	float dThres = 1.0/m_vvlVotes.size();
	vector< vector<FeatureVector> > vvfvCoords;
	vector< vector<float> > vvCoeff;
	vector<uint32_t> vnLeafNodes;
	vector< vector<float> > vvdConfidence;
	m_prfRandomForest->testRandomForestMultiClass( patch, dThres, vvfvCoords, vvCoeff, vnLeafNodes, vvdConfidence);
	// Traverse the nearby region of the test patch in the Patch's scale.
	float dTmp = 100;
	int nTmpIndex;
	for(int i=m_nMinScale;i<=m_nMaxScale;i++)
	{
		if(fabs(dScale - pow(sqrt(2),i))<dTmp)
		{
			dTmp = fabs(dScale - pow(sqrt(2),i));
			nTmpIndex = i;
		}
	}
	if(dTmp>=THRESHOLD_SCALE)
	{
		cout<<"error in voteWithPatch: scale unidentified!"<<endl;
		exit(0);
	}
	// Fill in the nearby bins in current scale
	int nStartIndex = (nTmpIndex - m_nMinScale) * m_vBins[0] * m_vBins[1];
	int nEndIndex = nStartIndex + m_vBins[0]*m_vBins[1] - 1;
	FeatureVector fvBinOffsetScaled(2);
	FeatureVector fvDiff(2);
	for( int i=nStartIndex;i<=nEndIndex;i++)
	{
		FeatureVector fvBinPos = getBinOffSet(i);
		fvBinOffsetScaled.setValue(0, (fvBinPos.at(0) - x)*dScale);
		fvBinOffsetScaled.setValue(1, (fvBinPos.at(1) - y)*dScale);
		if(isInBox(x, y, dScale, fvBinPos.at(0), fvBinPos.at(1)))
		{
			for(int nClass=1;nClass<m_vvlVotes.size();nClass++)
			{
				// This bin is in the nearby box, calculate the votes.
				double value = 0; 
				for(int j=0;j<vvCoeff[nClass].size();j++)
				{
					fvDiff = fvBinOffsetScaled - vvfvCoords[nClass][j];
					/*cout<<"Voting for bin at("<<fvBinPos.at(0)<<","<<fvBinPos.at(1)
						<<")"<<endl;
					cout<<"Diff = ("<<fvDiff.at(0)<<","<<fvDiff.at(1)<<")"<<endl;
					cout<<"Voting Score = "<<vCoeff[j]*exp(-fvDiff.dot(fvDiff)/2/EPS2)/2/PI/EPS2<<endl;*/
					value+=vvCoeff[nClass][j]*exp(-fvDiff.dot(fvDiff)/2/EPS2)/2/PI/EPS2;
				}
				// generate the vote with this patch, if the average coffidence of this patch is above 
				// dThres
			
				HoughVote vote(fvBinOffsetScaled.at(0), fvBinOffsetScaled.at(1), dScale,value, 
								nIndex, vnLeafNodes, vvdConfidence[nClass]);
				// Insert the vote in m_vvlVotes
				m_vvlVotes[nClass][i].push_back(vote);
			}
		}
	}
	
}
FeatureVector  VotingSpace::getBinOffSet( int index )
{
  // The bins are arranges from the first dimension to the last, such as:
  // {(x,y,s)} = (1,1,1), (2,1,1),(3,1,1)...
	if(m_nDims != 3)
	{
		cout<<" error: in getBinOffSet, currently only Dim = 3 is supported!"<<endl;
		exit(0);
	}
	int nScaleIndex = index/m_vBins[0]/m_vBins[1];
	int tmpIndex = index%(m_vBins[0]*m_vBins[1]);
	int nYIndex = tmpIndex/m_vBins[0];
	tmpIndex = tmpIndex%m_vBins[0];
	int nXIndex = tmpIndex;

	FeatureVector fvBinOffSet(3);
	fvBinOffSet.setValue(0, float(m_nWidth)/m_vBins[0]*( nXIndex+0.5 ) );
	fvBinOffSet.setValue(1, float(m_nHeight)/m_vBins[1]*( nYIndex+0.5 ) );
	fvBinOffSet.setValue(2, pow(sqrt(2),m_nMinScale + nScaleIndex));
	return fvBinOffSet;	
}

bool VotingSpace::isInBox( int center_x, int center_y, float scale, float bin_x, float bin_y)
{
	if(	bin_x>center_x - 0.5*m_nBoxWidth/scale	&&
		bin_x<center_x + 0.5*m_nBoxWidth/scale	&&
		bin_y>center_y - 0.5*m_nBoxHeight/scale	&&
		bin_y<center_y + 0.5*m_nBoxHeight/scale	)
	return true;
	else return false;
}

void VotingSpace::displayConfMap( int nClass )
/*==========================================================================*/
/* Display the confidence map for class nClass								*/
/*==========================================================================*/
{
	vector<OpGrayImage> vHistoValues= getConfMap(nClass);
	float max,min;
	vHistoValues[2].getMinMax(min, max);
	cout<<"max = "<<max<<", min ="<<min<<endl;
	vector<QImage>		vImgs;
	for(int i=0; i<(int)vHistoValues.size(); i++)
	{
		vImgs.push_back(vHistoValues[i].getQtImage() );
	}
	QtImgBrowser *qtHoughBrowser = new QtImgBrowser( 0, "Hough Results"	);
	qtHoughBrowser->setGeometry( 950, 200, 300, 350);
	qtHoughBrowser->load( vImgs, vHistoValues);
	qtHoughBrowser->show();
}

void VotingSpace::displayConfMap()
/*===========================================================================*/
/* Display the confidence map for all classes.                               */
/*===========================================================================*/
{
	for(int i=1; i<m_vvlVotes.size(); i++)
		displayConfMap(i);
}

vector<OpGrayImage> VotingSpace::getConfMap( int nClass )
/*============================================================================*/
/* Get the confidence map of each scale.                                      */
/*============================================================================*/
{	
	vector<OpGrayImage> vVoteMap;
	for(int scale=0; scale<m_vBins[2]; scale++)
	{
	// TODO: add some range in order to find half-visible object?
	OpGrayImage imgVote( m_vBins[0], m_vBins[1] );
		for(int y=0; y<m_vBins[1]; y++)
		{
			for(int x=0; x<m_vBins[0]; x++)
			{
				imgVote(x, y) = getBinSum(x, y, scale, nClass);
			}
		}
	vVoteMap.push_back(imgVote);
	}
	return vVoteMap;
}
vector<int> VotingSpace::getSupportingPatches(int nClass, int x, int y, int nScaleIndex, vector<HoughVote>& vVotes)
/*==============================================================================*/
/* Get the supporting patches that voted for the bin. Returns the indexes of    */
/* these patches in the patch pool.												*/
/*==============================================================================*/
{
	vector<int> vResult = getSupportingPatches(nClass, idx(x/getXStep(), y/getYStep(), nScaleIndex), vVotes);
	return vResult;
}

vector<int> VotingSpace::getSupportingPatches(int nClass, int nVoteIndex, vector<HoughVote>& vVote)
/*==============================================================================*/
/* Get the supporting patches that voted for the bin. Returns the indexes of    */
/* these patches in the patch pool.												*/
/*==============================================================================*/
{ 
	list<HoughVote> lHoughVotes = m_vvlVotes[nClass][nVoteIndex];
	vector<int> vResult;
	vVote.clear();
	fstream fsSupportingVoteFile;
	ostringstream osFileName;
	osFileName << "SupporingInfo_Class" << nClass;
	//fsSupportingVoteFile.open(osFileName.str().data(), fstream::in | fstream::out | fstream::trunc);
	for(list<HoughVote>::iterator litr = lHoughVotes.begin() ; litr!=lHoughVotes.end(); litr++)
	{
		// Find the average coffidence of the vote
		float mean = 0;
		for(int i=0; i<litr->m_vdConfidence.size(); i++)
		{
			mean += litr->m_vdConfidence[i];
		}
		mean = mean/litr->m_vdConfidence.size();
		//float dThres = 1.0/m_vvlVotes.size();
		float dThres = 0.3;
		if(mean > dThres)
		{
			vResult.push_back(litr->getIndex());
			vVote.push_back(*litr);
			/*
			// Store the supporting patches' information to the file(only those above the threshold)
			// The stored information includes:
			// 		Patch's position
			// 		The leaf nodes' voting offset(of all the random trees)
			
			// The patch's position
			fsSupportingVoteFile<< "Patch"
								<<endl;
			PatchSample psPatch = m_prfRandomForest->getPatchSample(litr->m_nPatchIndex);
			fsSupportingVoteFile<<psPatch.fvOffset.x
								<<","
								<< psPatch.fvOffset.y
								<<endl;
			fsSupportingVoteFile<<"Offset"<<endl;
			// All the offset coordinates of the current patch
			for(int i = 0; i < litr->m_vnLeafNodes.size(); i++)
			{
				RandomNode node = m_prfRandomForest->getNode(i,litr->m_vnLeafNodes[i]);
				for(int j=0; j<node.m_vCoords[nClass].size();j++)
				{
					fsSupportingVoteFile 	<< node.m_vCoords[nClass][j].at(0)
											<< ","
											<< node.m_vCoords[nClass][j].at(1)
											<<endl;
				}
			}
			*/
		}
		
	}
	//fsSupportingVoteFile.close();
	return vResult;
}
float VotingSpace::getBinSum( int x, int y, int scaleIndex, int nClass )
{
	if( x>=m_vBins[0] || x<0 ||
		y>=m_vBins[1]|| y<0 ||
		scaleIndex<0||
		scaleIndex>m_vBins[2])
		return 0;
	int index = idx(x, y, scaleIndex);
	float sum = 0;
	for(list<HoughVote>::iterator itr = m_vvlVotes[nClass][index].begin();itr!=m_vvlVotes[nClass][index].end();itr++)
	{
		sum += itr->m_dValue;
	}
	return sum;
}
float VotingSpace::getBinSum( int index, int nClass)
{
	float sum = 0;
	for(list<HoughVote>::iterator itr = m_vvlVotes[nClass][index].begin();itr!=m_vvlVotes[nClass][index].end();itr++)
	{
		sum += itr->m_dValue;
	}
	return sum;
}

/*****************/
/*   Indexings   */
/*****************/

int VotingSpace::idx( const FeatureVector& fvBin)
/*=============================================================*/
/* Inputs are the BIN INDEX in each dimension.(NOTE it is not  */
/* the coordinate or the scale index).                         */
/*=============================================================*/
{
	vector<int> vBin(3,0);
	vBin[2] = max(0-m_nMinScale, 0);
	for(int i=0;i<fvBin.numDims();i++)
	{
		vBin[i] = (int)fvBin.at(i);	
	}
	if( vBin[0]>=m_vBins[0] || vBin[0]<0 ||
		vBin[1]>=m_vBins[1]|| vBin[1]<0 ||
		vBin[2]<0||
		vBin[2]>m_vBins[2])
	{
		cout<<"Error in VotingSpace::idx: out of index"<<endl;
		cout<<"Indexes=("<<vBin[0]<<","<<vBin[1]<<","<<vBin[2]<<")"<<endl;
		exit(0);
	}
	return (vBin[0]+vBin[1]*m_vBins[0]+vBin[2]*m_vBins[1]*m_vBins[0]);
}

int VotingSpace::idx(const vector<float>& vBin)
{
	FeatureVector fvBin(vBin);
	return idx(fvBin);
}

int VotingSpace::idx( int x, int y, int scaleIndex )
{
	FeatureVector fvBin(3);
	fvBin.setValue( 0, x );
	fvBin.setValue( 1, y );
	fvBin.setValue( 2, scaleIndex);
	return idx(fvBin);
}
int VotingSpace::idx( int x, int y )
/*===================================================================*/
/* Returns the index of the bin at (x,y) at scale 0, if scale 0 does */
/* not exist, return the index at minScale.                          */
/*===================================================================*/
{
	FeatureVector fvBin(3);
	fvBin.setValue( 0, x );
	fvBin.setValue( 1, y );
	fvBin.setValue( 2, max(0-m_nMinScale, 0));
	return idx(fvBin);
}


/************************/
/* Hypothese Generation */
/************************/
void VotingSpace::applyMSME( float x, float y, int nClass,
				FeatureVector& fvResult, float& dScore,
				bool bAdaptiveScale, float dAdaptiveMinScale)
/*========================================================================*/
/* Find local maxima from initial point fvCoord, final pos stored in      */
/* fvResult, final score in dScore. AdaptveScale means adjusting the      */
/* size of window acoording to current scale.                             */
/*========================================================================*/
{
	FeatureVector fvCoord(2);
	fvCoord.setValue( 0, x );
	fvCoord.setValue( 1, y );
	applyMSME( fvCoord, nClass, fvResult, dScore );
}
void VotingSpace::applyMSME( float x, float y, float scale, int nClass,
				FeatureVector& fvResult, float& dScore,
				bool bAdaptiveScale, float dAdaptiveMinScale)
/*========================================================================*/
/* Find local maxima from initial point fvCoord, final pos stored in      */
/* fvResult, final score in dScore. AdaptveScale means adjusting the      */
/* size of window acoording to current scale.                             */
/*========================================================================*/
{
	FeatureVector fvCoord(3);
	fvCoord.setValue( 0, x );
	fvCoord.setValue( 1, y );
	fvCoord.setValue( 2, scale);
	applyMSME( fvCoord, nClass, fvResult, dScore, bAdaptiveScale, dAdaptiveMinScale);
}

void VotingSpace::applyMSME( const FeatureVector& fvCoord, int nClass,
				FeatureVector& fvResult, float& dScore,
				bool bAdaptiveScale, float dAdaptiveMinScale)
/*========================================================================*/
/* Find local maxima from initial point fvCoord, final pos stored in      */
/* fvResult, final score in dScore. AdaptveScale means adjusting the      */
/* size of window acoording to current scale.                             */
/*========================================================================*/
{
	// Initialize Window
	FeatureVector fvWindow(3);
	fvWindow.setValue( 0, (float)m_nBoxWidth/2 );
	fvWindow.setValue( 1, (float)m_nBoxHeight/2 );
	fvWindow.setValue( 2, 2.0 ); // !!Scale stored in scale index
	
	// Initialize current posiztion
    FeatureVector fvCurPos = fvCoord;
	FeatureVector fvNewPos = fvCurPos;

    dScore = 0;
	float dLastScore = 0;
	// Start the iteration
	bool bStop = false;
	while(!bStop)
	{
		dLastScore = dScore;
		// Adjust the window size if bAdaptiveScale
		if(bAdaptiveScale && fvCurPos.numDims()>=3)
		{
			fvWindow.setValue( 0, fvWindow.at(0)*pow(SCALE_FACTOR,fvCurPos.at(2) ));
			fvWindow.setValue( 1, fvWindow.at(1)*pow(SCALE_FACTOR,fvCurPos.at(2) ));
			// do not scale the scale
		}
		// collect the votes and Get the mean
		getMeanInWindow(fvCurPos, fvWindow, nClass, fvNewPos, dScore);
		
		// Update the position
		if(fabs(dScore - dLastScore) < EPS_MSME)
			bStop = true;
		if(dScore > dLastScore)
		{
			cout<<" Moving from("<<fvCurPos.at(0)/getXStep()<<", "<<fvCurPos.at(1)/getYStep()<<") to ("
				<<fvNewPos.at(0)/getXStep()<<", "<<fvNewPos.at(1)/getYStep()<<")"<<endl;
			fvCurPos = fvNewPos;

		}
		else 
			bStop = true;
	}
	fvResult = fvCurPos;
	cout<<"====== MSME output======"<<endl;
	cout<<" Class:"<<nClass<<endl;
	cout<<" Local maxima position :("<<fvCurPos.at(0)<<","<<fvCurPos.at(1)<<")"<<endl;
	cout<<" Local maxima bin :("<<fvCurPos.at(0)/getXStep()<<","<<fvCurPos.at(1)/getYStep()<<","<<endl;
	cout<<" Maximal value:\t"<<dScore<<endl;
	cout<<"========================"<<endl;
}

void VotingSpace::getMeanInWindow(	const FeatureVector& fvCurPos,
						const FeatureVector& fvWindow,
						int nClass,
						FeatureVector& fvNewPos, float& dScore)
/*=====================================================================*/
/* Get the mean pos in the window fvWindow centered at fvCurPos.       */
/*=====================================================================*/
{
	int nDims = fvCurPos.numDims();
	float scale = 1.0 * fvWindow.at(0) / m_nBoxWidth;
	// Bounds of the window
	FeatureVector fvMinIndexes(nDims);
	FeatureVector fvMaxIndexes(nDims);
	cout<<"\t\tMin.\tMax."<<endl;
	for(int i=0; i<nDims; i++)
	{
		// TODO: The window may exceed out of the image, the extension is really needed
		if(i==0)
		{
			fvMaxIndexes.setValue( i, min(1.0*m_vBins[i]-1, floor(( fvCurPos.at(i) + fvWindow.at(i)/2)/getXStep())) );
			fvMinIndexes.setValue( i, max(0.0, floor(( fvCurPos.at(i) - fvWindow.at(i)/2)/getXStep())));
		}
		else if(i==1)
		{
			fvMaxIndexes.setValue( i, min(1.0*m_vBins[i]-1, floor(( fvCurPos.at(i) + fvWindow.at(i)/2)/getYStep())) );
			fvMinIndexes.setValue( i, max(0.0, floor(( fvCurPos.at(i) - fvWindow.at(i)/2)/getYStep())));
		}
		else
		{ 
		// TODO: The window may exceed out of the image, the extension is really needed
			fvMinIndexes.setValue( i, max(0.0, floor( fvCurPos.at(i) - fvWindow.at(i)/2)));
			fvMaxIndexes.setValue( i, min(1.0*m_vBins[i]-1,floor( fvCurPos.at(i) + fvWindow.at(i)/2) ));
		}

		cout<<"dim"<<i<<":\t\t"<<fvMinIndexes.at(i)<<"\t"<<fvMaxIndexes.at(i)<<endl;
	}
	cout<<endl;
	
	// Storations for probabilities and coords
	dScore = 0;
	int nCount = 0;
	FeatureVector coord(nDims);
	// Traverse in the window
	bool bStop = false;
	vector<float> vIndexes = fvMinIndexes.getData();
	while(!bStop)
	{
		//vector<float> vTmpIndex = vIndexes;
		//vTmpIndex[0] = floor(vIndexes[0]/getXStep());
		//vTmpIndex[1] = floor(vIndexes[1]/getYStep()); 
		int index = idx(vIndexes);
		float value = getBinSum(index, nClass);
		dScore+=value;
		FeatureVector fvTmp(vIndexes);
		coord.addVector(fvTmp.mul(value));
		nCount++;
		bStop = true;
	 	for(int i=0;i<nDims;i++)
		{
			if(vIndexes[i] < fvMaxIndexes.at(i))
			{
				vIndexes[i]++;
				bStop = false;
				break;
			}
			else
				vIndexes[i] = fvMinIndexes.at(i);
		}
	}
    coord.setValue(0, coord.at(0)*getXStep());
	coord.setValue(1, coord.at(1)*getYStep());
	fvNewPos = coord.div(dScore);
}
