#pragma once
#include <boost/date_time.hpp>
#include "Tools.h"
#include "iCompilerTest.h"
#include "TestData.h"
class CompositeTest :
	public iCompilerTest
{
public:
	CompositeTest() = default;
	CompositeTest(TestData& d){ data = &d; };
	virtual void Describe();
	virtual float RunTest();
	virtual ~CompositeTest();
};

