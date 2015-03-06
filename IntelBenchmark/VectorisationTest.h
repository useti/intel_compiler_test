#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"
class VectorisationTest :
	public iCompilerTest
{
public:
	VectorisationTest() = default;
	VectorisationTest(TestData& d){ data = &d; };
	virtual void Describe();
	virtual float RunTest();
	virtual ~VectorisationTest();
};

