#pragma once
#include "TestData.h"

class iCompilerTest
{
public:
	iCompilerTest(){ data = nullptr; };
	iCompilerTest(TestData& d){ data = &d; };

	/// \brief Printout class description
	virtual void Describe() = 0;

	/// \brief Perform test
	virtual float RunTest() = 0;
	virtual ~iCompilerTest(){};

	/// \brief Data to run test
	TestData* data;
};

