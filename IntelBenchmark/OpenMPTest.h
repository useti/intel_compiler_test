#pragma once
#include "iCompilerTest.h"
#include "TestData.h"
class OpenMPTest :
	public iCompilerTest
{
public:
	OpenMPTest(TestData& d);
	virtual float RunTest();
	virtual ~OpenMPTest();
};

