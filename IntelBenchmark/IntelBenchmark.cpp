#include "stdafx.h"
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>

#include "json/json.h"
#include "json/value.h"

#ifdef _WITH_OMP
#include <omp.h>
#else
#ifdef _WITH_TBB
#endif // _WITH_TBB
#include <tbb/tbb.h>
#endif // _WITH_OMP

#define START_TIMECHECK() ptime startt = boost::posix_time::microsec_clock::local_time();

#define STOP_TIMECHECK() ptime curentt = boost::posix_time::microsec_clock::local_time();\
    boost::posix_time::time_duration diff = curentt - startt;\
    char tf[255];\
    std::stringstream ssDuration;\
    ssDuration << diff;\
    std::string str = ssDuration.str();\
    sprintf(tf, "%s\n", str.c_str());\
    cout << tf ;

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

namespace po = boost::program_options;
float aggregate = 0;
long long int iterate_count = 100;
int run_count = 3;
int chunk = 10;
float l_aggregate = 0;

po::options_description desc("Usage:\n\nIntellBenchmark [options] dataset.json\n\n Allowed options");

std::vector<float> dataArray1, dataArray2, dataArray3;

////////////////////////////////////////////////////////////////
/// Tools
////////////////////////////////////////////////////////////////

int printHelp(){
	cout << desc << "\n";
	return 0;
}

int writeInputFile(const std::string &rewritePath, const Json::Value &root)
{
	Json::StyledWriter writer;
	std::string rewrite = writer.write(root);
#ifdef _WINDOWS                                                                        
	FILE *fout;
	errno_t errn = fopen_s(&fout, rewritePath.c_str(), "wt");
#else                                                                                  
	FILE *fout = fopen(rewritePath.c_str(), "wt");
#endif                                                                                 
	if (!fout)
	{
		return 1;
	}
	fprintf(fout, "%s\n", rewrite.c_str());
	fclose(fout);
	return 0;
}

int generateDataset(string fname, long long int ds_size)
{
	dataArray1.reserve(ds_size);
	dataArray2.reserve(ds_size);
	dataArray3.reserve(ds_size);

	string ds_text = (char*)"{ \"ds_size\":0,\"aggregate\":0,\"iterate_count\":100, \"dataArray1\":[], \"dataArray2\":[], \"dataArray3\":[] }";
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

			return writeInputFile(fname, ds_json); // We dont want to run tests after generate, so output inverted
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

std::string readInputFile(const char *path)
{
#ifdef _WINDOWS
	FILE *file;
	errno_t errn = fopen_s(&file, path, "rb");
#else
	FILE *file = fopen(path, "rb");
#endif
	if (!file)
		return std::string("");
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::string text;
	char *buffer = new char[size + 1];
	buffer[size] = 0;
	if (fread(buffer, 1, size, file) == (unsigned long)size)
		text = buffer;
	fclose(file);
	delete[] buffer;
	return text;
}

int loadDataset(string fname)
{

	try
	{
		string ds_text = readInputFile(fname.c_str());
		Json::Features ds_ftrs;
		Json::Value ds_json;
		Json::Reader reader(ds_ftrs);
		if (reader.parse(ds_text, ds_json)){
			Json::Value da1 = ds_json[(char*)"dataArray1"];
			Json::Value da2 = ds_json[(char*)"dataArray2"];
			Json::Value da3 = ds_json[(char*)"dataArray3"];
			size_t ds_size = ds_json[(char*)"ds_size"].asUInt();
			aggregate = (float)ds_json[(char*)"aggregate"].asDouble();
			iterate_count = (long long int)ds_json[(char*)"iterate_count"].asInt();
			chunk = ds_json[(char*)"chunk"].asInt();
			run_count = ds_json[(char*)"run_count"].asInt();
			dataArray1.reserve(ds_size);
			dataArray2.reserve(ds_size);
			dataArray3.reserve(ds_size);
			for (size_t i = 0; i < ds_size; i++)
			{
				float r1 = (float)da1[i].asDouble();
				dataArray1.push_back(r1);
				float r2 = (float)da2[i].asDouble();
				dataArray2.push_back(r2);
				float r3 = (float)da3[i].asDouble();
				dataArray3.push_back(r3);
			}
		}
		else{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
	return 1;
}

int init(int argc, char* argv[])
{
    START_TIMECHECK();

	desc.add_options()
		("help", "Show this message")
		("generate,G", po::value< vector<long long int> >(), "Generate dataset with provided size")
		("iterate-count,I", po::value< vector<long long int> >(), "Number of test repeats")
		("chunk,c", po::value< vector<int> >(), "Chunk size (for parallel tests)")
		("run-count,r", po::value< vector<int> >(), "Count of repeats for each test")
		("dataset,d", po::value< vector<string> >(), "Test dataset file")
		;

	po::variables_map vm;

	po::positional_options_description p;
	p.add("dataset", -1);

	po::store(po::command_line_parser(argc, argv).
		options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("dataset")){
		vector<string> ds_fnames = vm["dataset"].as< vector<string> >();
		size_t nTasks = ds_fnames.size();
		if (nTasks != 1){
			cerr <<"Only one dataset !\n\n";
			return printHelp();
		}
		if (vm.count("iterate-count")){
			iterate_count = vm["iterate-count"].as< vector<long long int> >()[0];
		}
		if (vm.count("chunk")){
			chunk = vm["chunk"].as< vector<int> >()[0];
		}
		if (vm.count("run-count")){
			run_count = vm["run-count"].as< vector<int> >()[0];
		}
		if (vm.count("generate")){
			printf("\nInit time (generate)\n");
            int r = generateDataset(ds_fnames[0], vm["generate"].as< vector<long long int> >()[0]);
            STOP_TIMECHECK();
            return r;
		}
		else{
			printf("\nInit time (load)\n");
            int r = loadDataset(ds_fnames[0]);
            STOP_TIMECHECK();
			return r;
		}	
	}

	if ((vm.count("help")) || (argc == 1)) {
		return printHelp();
	}
	printf("\nInit time\n");
    STOP_TIMECHECK();
	return 1;
}


////////////////////////////////////////////////////////////////
/// Tests
////////////////////////////////////////////////////////////////

void VECTest(){
    START_TIMECHECK();

	l_aggregate = 0;
	int ds_size = dataArray1.size();
	for (int itr = 0; itr < iterate_count; itr++)
	{	
#pragma vector always
#pragma ivdep
		for (int i = 0; i < ds_size; i++)
		{
			l_aggregate = dataArray1[i] + dataArray2[i] + dataArray3[i];
		}
	}
    STOP_TIMECHECK();
}

void OMPTest(){
    START_TIMECHECK();

	l_aggregate = 0;
	int ds_size = dataArray1.size();
	const int size = iterate_count;
#ifdef _WITH_OMP
#pragma omp parallel for schedule(static,chunk)
		for (int itr = 0; itr < size; itr++) {
#else
#ifdef _WITH_TBB
	tbb::parallel_for(0, size ,1, [=] (int itr) {
#else
	{
#endif //_WITH_TBB
#endif //_WITH_OMP
	
#pragma novector
			for (int i = 0; i < ds_size; i++)
			{
				l_aggregate = dataArray1[i] + dataArray2[i] + dataArray3[i];
			}
		}
#ifdef _WITH_OMP
#else
#ifdef _WITH_TBB
	);
#endif
#endif
    STOP_TIMECHECK();
}

void CMPTest(){
    START_TIMECHECK();

	l_aggregate = 0;
	int ds_size = dataArray1.size();
	const int size = iterate_count;
#ifdef _WITH_OMP
#pragma omp parallel for schedule(static,chunk)
		for (int itr = 0; itr < size; itr++) {
#else
#ifdef _WITH_TBB
	tbb::parallel_for(0, size ,1, [=] (int itr) {
#else
	{
#endif //_WITH_TBB
#endif //_WITH_OMP

#pragma vector always
#pragma ivdep
			for (int i = 0; i < ds_size; i++)
			{
				l_aggregate = dataArray1[i] + dataArray2[i] + dataArray3[i];
			}
		}
#ifdef _WITH_OMP
#else
#ifdef _WITH_TBB
	);
#endif
#endif
    STOP_TIMECHECK();
}

int main(int argc, char* argv[])
{
    START_TIMECHECK();

#ifdef _WITH_OMP
	printf("\nBenchmark OpenMP\n");
#else
#ifdef _WITH_TBB
	printf("\nBenchmark TBB\n");
#endif
#endif
#ifdef _WINDOWS
	printf("WIN\n");
#else
	printf("LIN\n");
#endif
	if (init(argc, argv)){

		printf("\nVectorisation test\n");
		for (int i = 0; i < run_count; i++){
			VECTest();
		}
#ifdef _WITH_OMP
		printf("\nOpenMP test\n");
#else
#ifdef _WITH_TBB
		printf("\nTBB test\n");
#else
#endif //_WITH_TBB
#endif //_WITH_OMP
		for (int i = 0; i < run_count; i++){
			OMPTest();
		}
		printf("\nComposite test\n");
		for (int i = 0; i < run_count; i++){
			CMPTest();
		}
		printf("\nOverall runtime\n");
        STOP_TIMECHECK();
	}
	return 0;
}

