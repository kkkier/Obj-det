/**************************************************************************/
/* FILE      votingspace.hh                                               */
/* AUTHORS   Yuren Zhang                                                  */
/* EMAIL     kiekergaard@gmail.com; yuren.zhang@ia.ac.cn                  */
/* CONTENT   The voting space class is meant to be used with random forest. 
 *           It loads the trained random forest and generate voting space 
 *           for a test image. For the detail method please refer to :
 *           Class-Specific Hough Forests for Object Detection.
 * BEGIN     2013.2.15
 * CHANGE    2013.3.21
 **************************************************************************/
#ifndef VOTINGSPACE_HH
#define VOTINGSPACE_HH

/*******************/
/*     Includes    */
/*******************/ 
#include <randomforest.hh>
#include <featurevector.hh>
#include <opgrayimage.hh>
#include <vector>
#include <list>
#include <math.h>

/*******************/
/*   Defination    */
/*******************/
const float THRESHOLD_PROBABILITYVOTE = 0.1;
const float THRESHOLD_SCALE = 0.001;
const float EPS2 = 9;
const float EPS_MSME = 0.00005; //??
const float SCALE_FACTOR = sqrt(2);
const float PI = 3.1415926535;
const int   DEFAULTOBJECTWIDTH = 100;
const int   DEFAULTOBJECTHEIGHT = 100;
const int   DEFAULTXBINS        = 20;
const int   DEFAULTYBINS        = 20;
const int   DEFAULTSTEPSIZE     = 5;
const int   DEFAULTMINSCALE     = -2;
const int   DEFAULTMAXSCALE     = 3;
const int   DEFAULTDIM          = 3;
/*============================================================
 * houghvote represents the vote casted by each sampled patch 
  ===========================================================*/
class HoughVote
{
public:
	// x,y are store as the offset in the scale accordingly
	HoughVote(float x, float y, float s,
              float dValue, int nIndex,
			  const vector<uint32_t>& vnLeafNodes,
			  const vector<float>& vdConfidence);
	HoughVote(FeatureVector fvCoord,
	          float dValue, int nIndex,
			  const vector<uint32_t>& vnLeafNodes,
			  const vector<float>& vdConfidence);
	HoughVote(const HoughVote &other);
	HoughVote& operator=(const HoughVote& );
	~HoughVote();
private:
	HoughVote();
public:
	/*   content access  */
    inline	FeatureVector	getCoord(){return m_fvCoord;}
	inline  float			getValue(){return m_dValue;}
	inline  int				getIndex(){return m_nPatchIndex;}
	inline 	vector<uint32_t>		getLeafNodes(){return m_vnLeafNodes;}
	inline 	vector<float> 	getConfidences(){return m_vdConfidence;} 
protected:
	void createHoughVote(FeatureVector fvCoord,
						 float dValue, int nIndex, 
						 const vector<uint32_t>& vnLeafNodes, const vector<float>& vdConfidence );
	void copyFromOther( const HoughVote& other);
public:
	int m_nPatchIndex;
	FeatureVector m_fvCoord;
	float m_dValue;
	vector<uint32_t> m_vnLeafNodes;
	vector<float> m_vdConfidence;
};
/*============================================================
 * votingspace represents the voting space created by an image
 * with a randomforest.
 * ===========================================================*/
class VotingSpace
{
public:
	VotingSpace(int nWidth, int nHeight, int nBoxWidth, int nBoxHeight,
				int dMinScale, int dMaxScale, 
				int nDims, vector<int> vBins, 
				RandomForest* prfRandomForest);
	~VotingSpace();
public:
	/*  Random Forest  */
	void loadRandomForest(RandomForest*  prfForest);
	/*  Create Voting Space */
	void createVotingSpace();
	void createVotingSpace(vector<int> vBins);
	//void clearVotingSpace();
   	void voteWithPatch(const PatchSample& patch, int nIndex, int nClass);	
	void voteWithPatch(const PatchSample& patch, int nIndex); 
	/*	Get the result */
private:
	/* Functions that I dont want to use */
	VotingSpace(const VotingSpace&);
	VotingSpace& operator=(const VotingSpace&);	
private:
	/* utilities */
	int idx( const FeatureVector& fvBin );  // From offset in the image to index
	int idx( const vector<float>& vBin);
	int idx( int x, int y, int scaleIndex);
	int idx( int x, int y );
	FeatureVector getBinOffSet( int index );  // From index to the offset in image
	bool isInBox(int center_x, int center_y, float scale, float bin_x, float bin_y);
	void getMeanInWindow(	const FeatureVector& fvCurPos,
							const FeatureVector& fvWindow,
							int nClass,
							FeatureVector& fvNewPos, float& dScore);
	inline float getXStep(){ return float(m_nWidth)/m_vBins[0];}
	inline float getYStep(){ return float(m_nHeight)/m_vBins[1];}
	
public:
	/* results */
	vector<OpGrayImage> getConfMap(int nClass);
	float getBinSum( int x, int y, int scaleIndex, int nClass );
	float getBinSum( int index , int nClass);
	void displayConfMap( int nClass );
	void displayConfMap();
	vector<int> getSupportingPatches(int nClass, int x, int y, int nScaleIndex, vector<HoughVote>& vVotes);
	vector<int> getSupportingPatches(int nClass, int nVoteIndex, vector<HoughVote>& vVotes);
	vector<int> getSupportingLeafNodes(int bin_x, int bin_y);
    /* Hypothese generation */
	void applyMSME( float x, float y, int nClass,
					FeatureVector& fvResult, float& dScore,
					bool bAdaptiveScale = false, float dAdaptiveMinScale = 1);
	void applyMSME( float x, float y, float scale, int nClass,
					FeatureVector& fvResult, float& dScore,
					bool bAdaptiveScale = false, float dAdaptiveMinScale = 1);
    void applyMSME(	const FeatureVector& fvCoord, int nClass,
					FeatureVector& fvResult, float& dScore,
					bool bAdaptiveScale = false, float dAdaptiveMinScale = 1);
private:	
	/* Member variants */
    RandomForest*	m_prfRandomForest;
	vector<HoughVote> m_vVotes;
	int m_nDims;			// Number of dimensions
	vector<int> m_vBins;	// Number of Bins of each dimension
	int m_nWidth;
	int m_nHeight;
	int m_nBoxWidth;
	int m_nBoxHeight;
	int m_nMaxScale;	// Max and Min Scale index for sqrt(2)'s power
	int m_nMinScale;	
	vector< vector< list<HoughVote> > > m_vvlVotes;
	
};

#endif
