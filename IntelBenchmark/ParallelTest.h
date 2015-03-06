#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"

#ifdef _WITH_OMP
#include <omp.h>
#else
#ifdef _WITH_TBB
#endif // _WITH_TBB
#include <tbb/tbb.h>
#endif // _WITH_OMP

/** \brief Parallel loop test
*
* Trying to run external loop as parallel, interntal loop - standart.
*
* Have two wariants - OpenMP and TBB.
*
* Switch by define _WITH_TBB / _WITH_OMP
*/
class ParallelTest :
	public iCompilerTest
{
public:
	ParallelTest()=default;
	ParallelTest(TestData& d){ data = &d; };
	
	/// \brief Printout class description
	virtual void Describe();
	
	/// \brief Perform test
	virtual float RunTest();
	virtual ~ParallelTest();
};

