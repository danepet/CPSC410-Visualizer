#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include "DynClassInfo.h"
#include <iterator>

class DynamicParser {
public:
	bool parseFile(std::string filePath); 

private:
	std::map<std::string, std::vector<DynClassInfo> > funcCallInfoMap; // map holding function and vector of dynclassinfo
	void printOutput(std::map<std::string, std::vector<DynClassInfo> > dynMap);
	DynClassInfo generateDynClassInfoFromLine(std::string line);
};