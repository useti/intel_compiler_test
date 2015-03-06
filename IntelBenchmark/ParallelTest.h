#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"
class ParallelTest :
	public iCompilerTest
{
public:
	ParallelTest()=default;
	ParallelTest(TestData& d){ data = &d; };
	virtual void Describe();
	virtual float RunTest();
	virtual ~ParallelTest();
};

