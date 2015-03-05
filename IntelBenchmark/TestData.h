#pragma once
#include <string>
#include <vector>
#include "json/json.h"
#include "json/value.h"
#include "Tools.h"


class TestData
{
public:
	TestData();

	static int Generate(std::string fname, long long int ds_size, int chunk, int iterate_count, int run_count);

	virtual ~TestData();

	std::vector<float> dataArray1;
	std::vector<float> dataArray2;
	std::vector<float> dataArray3;


	float aggregate = 0;
	long long int iterate_count = 100;
	int run_count = 3;
	int chunk = 10;
	float l_aggregate = 0;
};

