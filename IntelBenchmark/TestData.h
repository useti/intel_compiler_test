#pragma once
#include <string>
#include <vector>
#include "json/json.h"
#include "json/value.h"
#include "Tools.h"


class TestData
{
public:
	TestData(
		std::string fname,
		long long int ds_size,
		int chunk,
		int iterate_count,
		int run_count);

	TestData(std::string fname);

	virtual ~TestData();

	std::vector<float> dataArray1;
	std::vector<float> dataArray2;
	std::vector<float> dataArray3;
	long long int iterate_count = 100;
	int chunk = 10;

private:

	int Generate(
		std::string fname, 
		long long int ds_size, 
		int chunk, 
		int iterate_count, 
		int run_count);

	void Clear( 
		long long int ds_size);

	int Load(
		std::string fname);

	float aggregate = 0;
	int run_count = 3;
	float l_aggregate = 0;
};

