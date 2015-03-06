#include "VectorisationTest.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

void VectorisationTest::Describe()
{
	printf("\nVectorisation test\n");
}

float VectorisationTest::RunTest()
{
	int i;
	float n_aggregate = 0;
	START_TIMECHECK();


	int ds_size = data->dataArray1.size();
	for (int itr = 0; itr < data->iterate_count; itr++)
	{
#ifdef _WITH_SIMD
#pragma simd
#else
#pragma vector always
#pragma ivdep
#endif
		for (i = 0; i < ds_size; i++)
		{
			n_aggregate += data->dataArray1[i] + data->dataArray2[i] + data->dataArray3[i];
		}
	}
	STOP_TIMECHECK();
	return n_aggregate;
}

VectorisationTest::~VectorisationTest()
{
}
