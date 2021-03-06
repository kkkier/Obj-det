/*==============================================================================================
 * Two ways of using KMeanClustering:
 * 1: First Cluster, then use
 * 		KMeansClustering kmeans(algorithm, distance, centerNum, itrNum, dataDim, dataNum, data);
 * 		vector<vector<float>> centers = kmeans.getCenters();
 * 		vecotr<DataPointAssignment> assignment = kmeans.getAssignment(newdata, newdataNum);
 * 2: First Load, then use
 * 		KMeansClustering kmeans(dataDim);
 * 		kmeans.setCenters(centers, centerNum);
 * 		vector<DataPointAssignment> assignment = kmeans.getAssignment(newdata, newdataNum);
 *=============================================================================================*/

#include <stdint.h>
#include <vector>
#include "kmeans.h"

#ifndef _KMEANS_HH_
#define _KMEANS_HH_

using namespace std;

typedef VlKMeansAlgorithm KMEANSALGORITHM;
const KMEANSALGORITHM KMEANS_Lloyd = VlKMeansLloyd;
const KMEANSALGORITHM KMEANS_ElKan = VlKMeansElkan;
const KMEANSALGORITHM KMEANS_ANN   = VlKMeansANN;
const KMEANSALGORITHM KMEANS_DEFAULT_ALGORITHM = KMEANS_Lloyd;

typedef VlVectorComparisonType KMEANSDISTANCETYPE;
const KMEANSDISTANCETYPE KMEANS_L2 = VlDistanceL2;
const KMEANSDISTANCETYPE KMEANS_DEFAULT_DISTANCE = KMEANS_L2;

enum KMEANS_SEEDMETHOD{RANDOM, PLUSPLUS};
typedef int KMEANSDATATYPE;
const KMEANSDATATYPE KMEANSDATATYPE_FLOAT = VL_TYPE_FLOAT;
const KMEANSDATATYPE KMEANSDATATYPE_DOUBLE = VL_TYPE_DOUBLE;

//typedef float VALUE_TYPE;
const uint32_t KMEANS_DEFAULT_CENTERNUM = 1000;
const uint32_t KMEANS_DEFAULT_ITRNUM = 100;

// currently restrict to float data type
template<class VALUE_TYPE = float>
struct DataPointAssignment
{
	uint32_t assignment;
	VALUE_TYPE distance;
};

template<class VALUE_TYPE = float>
class KMeansClustering{
	public:
		KMeansClustering(uint32_t dataDim);
		KMeansClustering(KMEANSALGORITHM algorithm, KMEANSDISTANCETYPE distance,  uint32_t centerNum, uint32_t itrNum, uint32_t dataDim, uint32_t dataNum, const void* data);
		~KMeansClustering();
		void setAlgorithm(KMEANSALGORITHM algorithm){m_algorithm = algorithm;}
		void setDistance(KMEANSDISTANCETYPE distance){m_distance = distance;}
		void setSeedMethod(KMEANS_SEEDMETHOD seedMethod){m_seedMethod = seedMethod;}
		KMEANSALGORITHM getAlgorithm(){return m_algorithm;}
		KMEANSDISTANCETYPE getDistance(){return m_distance;}

		void setCenters(const void* centers, uint32_t centerNum);
		vector< vector<VALUE_TYPE> > getCenters(); 

		vector< DataPointAssignment<VALUE_TYPE> > getAssignment(void const* data, uint32_t dataNum);
		VlKMeans* createEntity();
	private:
		void seedCenters(void const* data, uint32_t dataDim, uint32_t dataNum,  uint32_t centerNum );
	private:
		KMEANSALGORITHM m_algorithm ;
		KMEANSDISTANCETYPE m_distance ;
		uint32_t m_dataDim;
		uint32_t m_centerNum;
		uint32_t m_itrNum;

		KMEANS_SEEDMETHOD m_seedMethod;

		VlKMeans* m_pKmeans;
};

#endif
