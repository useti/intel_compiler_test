#pragma once
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>

#include "json/json.h"
#include "json/value.h"

#define START_TIMECHECK() ptime startt = boost::posix_time::microsec_clock::local_time();

#define STOP_TIMECHECK_W() ptime curentt = boost::posix_time::microsec_clock::local_time();\
    boost::posix_time::time_duration diff = curentt - startt;\
    char tf[255];\
    std::stringstream ssDuration;\
    ssDuration << diff;\
    std::string str = ssDuration.str();\
	sprintf_s(tf,sizeof(tf), "%s\n", str.c_str()); \
    cout << tf ;

#define STOP_TIMECHECK_L() ptime curentt = boost::posix_time::microsec_clock::local_time();\
    boost::posix_time::time_duration diff = curentt - startt;\
    char tf[255];\
    std::stringstream ssDuration;\
    ssDuration << diff;\
    std::string str = ssDuration.str();\
	sprintf(tf, "%s\n", str.c_str()); \
    cout << tf ;

#ifdef _WINDOWS
#define STOP_TIMECHECK() STOP_TIMECHECK_W()
#else
#define STOP_TIMECHECK() STOP_TIMECHECK_L()
#endif


/** \brief Common useful tools.

    \todo Добавить документацию
 */
class Tools
{
public:
	static int writeInputFile(const std::string &rewritePath, const Json::Value &root);
	static std::string readInputFile(const char *path);

	Tools();
	virtual ~Tools();
};

