#pragma once
#include "iCompilerTest.h"
#include "TestData.h"
class VectorisationTest :
	public iCompilerTest
{
public:
	VectorisationTest(TestData& d);

	virtual float RunTest();

	virtual ~VectorisationTest();
};

