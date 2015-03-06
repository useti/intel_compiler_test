#include "stdafx.h"
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>

#include "Tools.h"
#include "iCompilerTest.h"
#include "ParallelTest.h"
#include "VectorisationTest.h"
#include "CompositeTest.h"
#include "TestData.h"

#ifdef _WITH_OMP
#include <omp.h>
#else
#ifdef _WITH_TBB
#endif // _WITH_TBB
#include <tbb/tbb.h>
#endif // _WITH_OMP

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

std::vector<iCompilerTest*> tests;

TestData* dataset;

int printHelp(){
	cout << desc << "\n";
	return 0;
}

void initTests()
{
	tests.push_back(new VectorisationTest(*dataset));
	tests.push_back(new ParallelTest(*dataset));
	tests.push_back(new CompositeTest(*dataset));
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
			//int r = TestData::Generate(
			try{
				dataset = new TestData(
					ds_fnames[0],
					vm["generate"].as< vector<long long int> >()[0],
					chunk,
					iterate_count,
					run_count);

				initTests();

				STOP_TIMECHECK();
			}
			catch (...)
			{
				return 0;
			}
			
            return 1;
		}
		else{
			printf("\nInit time (load)\n");
			try{
				dataset = new TestData(ds_fnames[0]);

				initTests();

				STOP_TIMECHECK();
			}
			catch (...)
			{
				return 0;
			}
			return 1;
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

		for (std::vector<iCompilerTest*>::iterator itr = tests.begin(); itr != tests.end(); ++itr)
		{
			iCompilerTest* ii = *itr;
			ii->Describe();
			for (int i = 0; i < run_count; i++){
				float test_float = ii->RunTest();
			}
		}
	}
	printf("\nOverall runtime\n");
	STOP_TIMECHECK();
	return 0;
}


