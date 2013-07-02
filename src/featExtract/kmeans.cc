#include "kmeans.hh"
template<class VALUE_TYPE>
KMeansClustering<VALUE_TYPE>::KMeansClustering(uint32_t dataDim):
	m_dataDim(dataDim), 
	m_algorithm(KMEANS_DEFAULT_ALGORITHM),
	m_distance(KMEANS_DEFAULT_DISTANCE),
	m_centerNum(KMEANS_DEFAULT_CENTERNUM),
	m_itrNum(KMEANS_DEFAULT_ITRNUM),
	m_seedMethod(RANDOM)
{
	m_pKmeans = createEntity();
	vl_kmeans_set_algorithm(m_pKmeans, KMEANS_DEFAULT_ALGORITHM);
}
template<class VALUE_TYPE>
KMeansClustering<VALUE_TYPE>::KMeansClustering(KMEANSALGORITHM algorithm, KMEANSDISTANCETYPE distance, uint32_t centerNum, uint32_t itrNum, uint32_t dataDim, uint32_t dataNum, const void* data):
	m_algorithm(algorithm),
	m_distance(distance),
	m_centerNum(centerNum),
	m_itrNum(itrNum),
	m_dataDim(dataDim),
	m_seedMethod(RANDOM)
{
	m_pKmeans = createEntity();
	vl_kmeans_set_algorithm(m_pKmeans, m_algorithm);
	seedCenters(data, dataDim, dataNum, centerNum);
	vl_kmeans_set_max_num_iterations(m_pKmeans, m_itrNum);
	vl_kmeans_refine_centers(m_pKmeans, data, dataNum);
}

template<>
VlKMeans* KMeansClustering<float>::createEntity()
{
	return vl_kmeans_new(KMEANSDATATYPE_FLOAT, m_distance);
}

template<>
VlKMeans* KMeansClustering<double>::createEntity()
{
	return vl_kmeans_new(KMEANSDATATYPE_DOUBLE, m_distance);
}

template<class VALUE_TYPE>
KMeansClustering<VALUE_TYPE>::~KMeansClustering()
{
	if(m_pKmeans != NULL)
	{
		vl_kmeans_delete(m_pKmeans);
		m_pKmeans = NULL;
	}
}

template<class VALUE_TYPE>
void KMeansClustering<VALUE_TYPE>::setCenters(const void* centers, uint32_t centerNum)
{
	m_centerNum = centerNum;
	vl_kmeans_set_centers(m_pKmeans, centers, m_dataDim, centerNum);
}

template<class VALUE_TYPE>
vector< vector<VALUE_TYPE> >  KMeansClustering<VALUE_TYPE>::getCenters()
{
	vector< vector<VALUE_TYPE> > centerList;
	uint32_t memsize = m_centerNum * m_dataDim;
	VALUE_TYPE* rawData = new VALUE_TYPE[memsize];
	rawData = (VALUE_TYPE*)vl_kmeans_get_centers(m_pKmeans);
	for(int i=0; i<m_centerNum; i++)
	{	
		vector<VALUE_TYPE> oneFeature;
		for(int j=0; j< m_dataDim; j++)
		{
			VALUE_TYPE oneValue = rawData[ i*m_dataDim + j] ;
			oneFeature.push_back(oneValue);
		}
		centerList.push_back(oneFeature);
	}
	delete[] rawData;
	return centerList;
}

template<class VALUE_TYPE>
vector< DataPointAssignment<VALUE_TYPE> > KMeansClustering<VALUE_TYPE>::getAssignment(void const* data, uint32_t dataNum)
{	
	uint32_t* assignments = new uint32_t[dataNum];
	VALUE_TYPE* distances = new VALUE_TYPE[dataNum];

	vector< DataPointAssignment<VALUE_TYPE> > result;
	vl_kmeans_quantize(m_pKmeans, assignments, (void*)distances,data, dataNum);
	
	for(int i=0; i<dataNum; i++)
	{
		DataPointAssignment<VALUE_TYPE> dpa;
		dpa.assignment = assignments[i];
		dpa.distance = distances[i];
		result.push_back(dpa);
	}

	delete[] assignments;
	delete[] distances;
	return result;
}

template<class VALUE_TYPE>
void KMeansClustering<VALUE_TYPE>::seedCenters(void const* data, uint32_t dataDim, uint32_t dataNum, uint32_t centerNum)
{
	switch(m_seedMethod)
	{
		case RANDOM:
			vl_kmeans_seed_centers_with_rand_data(m_pKmeans, data, dataDim, dataNum, centerNum);
			break;
		case PLUSPLUS:
			vl_kmeans_seed_centers_plus_plus(m_pKmeans, data, dataDim, dataNum, centerNum);
			break;
		default:
			break;
	}
}

template class KMeansClustering<float>;
template class KMeansClustering<double>;
