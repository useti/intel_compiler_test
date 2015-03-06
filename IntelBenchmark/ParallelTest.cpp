#include "ParallelTest.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

void ParallelTest::Describe()
{
#ifdef _WITH_OMP
	printf("\nOpenMP test\n");
#else
#ifdef _WITH_TBB
	printf("\nTBB test\n");
#else
#endif //_WITH_TBB
#endif //_WITH_OMP
}

float ParallelTest::RunTest()
{
	float n_aggregate = 0;
	START_TIMECHECK();

	//l_aggregate = 0;
	int ds_size = data->dataArray1.size();
	const int size = data->iterate_count;
#ifdef _WITH_OMP
#pragma omp parallel for schedule(static, data->chunk)
	for (int itr = 0; itr < size; itr++) {
#else
#ifdef _WITH_TBB
	tbb::parallel_for(0, size, 1, [&n_aggregate, ds_size, data](int itr) {
#else
	{
#endif //_WITH_TBB
#endif //_WITH_OMP
		int i;
#pragma novector
#pragma loop(no_vector)
		for (i = 0; i < ds_size; i++)
		{
			n_aggregate += data->dataArray1[i] + data->dataArray2[i] + data->dataArray3[i];
		}
	}
#ifdef _WITH_OMP
#else
#ifdef _WITH_TBB
	);
#endif
#endif
	STOP_TIMECHECK();
	return n_aggregate;
}

ParallelTest::~ParallelTest()
{
}
