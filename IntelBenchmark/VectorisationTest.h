#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"

/** \brief Loop vectorisation test
*
* Trying to vectorise inner loop, exeternal loop - standart.
*
* Have two wariants - vector/ivdep and simd.
*
* Switch by define _WITH_SIMD (microsoft compiler does not aware about vector pragma).
*/
class VectorisationTest :
	public iCompilerTest
{
public:
	VectorisationTest() = default;
	VectorisationTest(TestData& d){ data = &d; };
	
	/// \brief Printout class description
	virtual void Describe();
	
	/// \brief Perform test
	virtual float RunTest();
	virtual ~VectorisationTest();
};

