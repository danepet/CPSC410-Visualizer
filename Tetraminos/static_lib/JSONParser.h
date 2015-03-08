#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include <iostream>
#include <map>
#include <cstdio>
#include <algorithm>
#include <set>	
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iterator> 
#include "DynamicParser.h"
#include "../../Sonicizer/OSCMessenger.h"
#include "../../Fuser/DynamicAnalyser/DynamicRunner.h"
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;
class JSONParser
{
public:
	int containsMatch(std::vector<std::vector<std::string> >, std::string, int&);
	void addDependencies(ClassProfile, std::map<std::string, ClassProfile>&, std::set<std::string>&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::set<std::string> >&, std::set<int>&);
	int getProcIdByName(std::string);
	void parseJSON(const rapidjson::Value&, std::vector<ClassProfile>&, std::map<std::string,ClassProfile>&, std::map<int, std::vector<ClassProfile> >&, int&, std::vector<std::string>&, std::map<std::string,std::string>&);
	void createInstrumentCommands(OSCMessenger *messenger);
	std::vector<std::set<std::string> >* constructDependency(std::map<std::string,ClassProfile>&, std::vector<std::string>&);
	std::vector<std::vector<std::string> >* constructInheritance(int, std::map<int, std::vector<ClassProfile> >);
private:
	boost::posix_time::ptime getEarliestTimeStamp(std::map<std::string, std::vector<DynClassInfo> > *dynMap);
	boost::posix_time::time_duration calcTimeDifference(boost::posix_time::ptime onTime, boost::posix_time::ptime offTime);
	std::string extractFunctionNameFromString(std::string fullFuncName);
	int checkTimeDurationForLessThanNSeconds(long totalTime);
};