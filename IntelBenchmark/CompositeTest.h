#pragma once
#include "iCompilerTest.h"
#include "TestData.h"
class CompositeTest :
	public iCompilerTest
{
public:
	CompositeTest(TestData& d);
	virtual float RunTest();
	virtual ~CompositeTest();
};

