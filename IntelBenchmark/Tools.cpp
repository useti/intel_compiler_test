#include "Tools.h"

/*
*/
int Tools::writeInputFile(const std::string &rewritePath, const Json::Value &root)
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

std::string Tools::readInputFile(const char *path)
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

Tools::Tools()
{
}


Tools::~Tools()
{
}
