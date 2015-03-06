#pragma once
#include "TestData.h"

class iCompilerTest
{
public:
	//iCompilerTest() = default;
	iCompilerTest(){ data = nullptr; };
	iCompilerTest(TestData& d){ data = &d; };
	virtual void Describe() = 0;
	virtual float RunTest() = 0;
	virtual ~iCompilerTest(){};
	TestData* data;
};

