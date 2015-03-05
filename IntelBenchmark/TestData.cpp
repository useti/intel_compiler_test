#include "TestData.h"


TestData::TestData()
{
}

int TestData::Generate(std::string fname, long long int ds_size, int chunk, int iterate_count, int run_count)
{
	float aggregate = 0;

	std::vector<float> dataArray1;
	std::vector<float> dataArray2;
	std::vector<float> dataArray3;

	dataArray1.reserve(ds_size);
	dataArray2.reserve(ds_size);
	dataArray3.reserve(ds_size);

	std::string ds_text = (char*)"{ \"ds_size\":0,\"aggregate\":0,\"iterate_count\":100, \"dataArray1\":[], \"dataArray2\":[], \"dataArray3\":[] }";
	Json::Features ds_ftrs;
	Json::Value ds_json;
	Json::Reader reader(ds_ftrs);

	srand(static_cast <unsigned> (time(0)));
	try
	{
		if (reader.parse(ds_text, ds_json)){
			long long int ds_sz = ds_size;
			ds_json[(char*)"ds_size"] = Json::Value((int)ds_size);
			Json::Value da1 = ds_json[(char*)"dataArray1"];
			Json::Value da2 = ds_json[(char*)"dataArray2"];
			Json::Value da3 = ds_json[(char*)"dataArray3"];
			for (long long int i = 0; i < ds_sz; i++)
			{
				float r1 = static_cast <float> (rand());
				dataArray1.push_back(r1);
				da1.append(Json::Value(r1));
				aggregate += r1;
				float r2 = static_cast <float> (rand());
				dataArray2.push_back(r2);
				da2.append(Json::Value(r2));
				aggregate += r2;
				float r3 = static_cast <float> (rand());
				dataArray3.push_back(r3);
				da3.append(Json::Value(r3));
				aggregate += r3;
			}
			ds_json[(char*)"dataArray1"] = da1;
			ds_json[(char*)"dataArray2"] = da2;
			ds_json[(char*)"dataArray3"] = da3;
			ds_json[(char*)"aggregate"] = Json::Value(aggregate);
			ds_json[(char*)"iterate_count"] = Json::Value((int)iterate_count);
			ds_json[(char*)"chunk"] = Json::Value(chunk);
			ds_json[(char*)"run_count"] = Json::Value(run_count);

			return Tools::writeInputFile(fname, ds_json); // We dont want to run tests after generate, so output inverted
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}

	return 1;
}

TestData::~TestData()
{
}
