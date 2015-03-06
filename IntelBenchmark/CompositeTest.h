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

/** \brief Composite loop test
*
* Trying to run external loop as parallel, and interntal loop as vectorised.
*
* Have for wariants - OpenMP/TBB and vector/simd .
*
* Switch by define _WITH_TBB / _WITH_OMP and _WITH_SIMD
*/
class CompositeTest :
	public iCompilerTest
{
public:
	CompositeTest() = default;
	CompositeTest(TestData& d){ data = &d; };

	/// \brief Printout class description
	virtual void Describe();

	/// \brief Perform test
	virtual float RunTest();
	virtual ~CompositeTest();
};

