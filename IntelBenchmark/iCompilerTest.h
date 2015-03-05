#pragma once
#include "TestData.h"

class iCompilerTest
{
public:
	virtual float RunTest();

	virtual ~iCompilerTest();

	TestData& data;
};

