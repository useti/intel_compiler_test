#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"

/** \brief Loop vectorization test
*
* Trying to vectorize inner loop, exeternal loop - standart.
*
* Have two variants - vector/ivdep and simd.
*
* Switch by define _WITH_SIMD (microsoft compiler does not aware about vector pragma).
*/
class VectorizationTest :
	public iCompilerTest
{
public:
	VectorizationTest() = default;
	VectorizationTest(TestData& d){ data = &d; };
	
	/// \brief Printout class description
	virtual void Describe();
	
	/// \brief Perform test
	virtual float RunTest();
	virtual ~VectorizationTest();
};

