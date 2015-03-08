#include "DynamicParser.h"

bool DynamicParser::parseFile(std::string filePath) {
	std::ifstream file(filePath.c_str());
	std::string line;
	if (file.is_open()) {
		while(std::getline(file, line)) {
			DynClassInfo dynInfo = generateDynClassInfoFromLine(line);
			if (funcCallInfoMap.count(dynInfo.getFunctionName()) == 0) {
				std::vector<DynClassInfo> info;
				info.push_back(dynInfo);
					// create a new element in the map, with functionName as the key and info as the vector
				funcCallInfoMap.insert(std::pair<std::string, std::vector<DynClassInfo> >(dynInfo.getFunctionName(), info));
			}
			else {
				funcCallInfoMap.at(dynInfo.getFunctionName()).push_back(dynInfo);
			}
		}

		typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
		std::map<std::string, std::vector<DynClassInfo> > mapCopy(funcCallInfoMap);
		for (it_type iterator = funcCallInfoMap.begin(); iterator != funcCallInfoMap.end(); iterator++) {
			std::vector<DynClassInfo> info = iterator->second;
			std::vector<DynClassInfo> ons;
			//std::vector<DynClassInfo> offs;
			//boost::posix_time::ptime timestamp = first.getTimeStamp();
			for (int i = 0; i < info.size(); i++) {
				DynClassInfo thisInfo = info.at(i);
				if (thisInfo.getOn()) {
					ons.push_back(thisInfo);
				}
				else {
					//std::cout << "before pop\n";
					ons.pop_back();
					//std::cout << "after pop\n";
				}
				if (ons.size() == 0) {
					//std::cout << "How many times do we get here? Should be 4!\n";
					DynClassInfo nextInfo;
					// we know there has to be an on/off after this, so we can look ahead 2 
					// indices // NOTE --- this is assuming that the program ended normally.
					// if someone force closed the program, behaviour here is undefined.
					if ((i+1) < info.size()) {
						//std::cout << "How many times do we get here? Should be 3!\n";
						nextInfo = info.at(i+1);
						boost::posix_time::ptime this_ts = thisInfo.getTimeStamp();
						boost::posix_time::ptime next_ts = nextInfo.getTimeStamp();
						boost::posix_time::time_duration time_between = next_ts - this_ts;
						//std::cout << "time between hours: " << time_between.hours() << "\n";
						//std::cout << "time between minutes: " << time_between.minutes() << "\n";
						//std::cout << "time between seconds: " << time_between.seconds() << "\n";
						//std::cout << "time between fractional_seconds: " << time_between.fractional_seconds() << "\n";
						if (time_between.hours() == 0 && time_between.minutes() == 0 && time_between.seconds() == 0 &&
							time_between.fractional_seconds() < 1000000) {
							mapCopy.at(thisInfo.getFunctionName()).erase(mapCopy.at(thisInfo.getFunctionName()).begin()+i);
							info.erase(info.begin()+i);
							// keep i the same on the next iteration
							i = i-1;
					}
				}
				else {
					// at the end of the vector, we need to break out since this is the only
					// 'off' that is left
					break;
				}
			}

			
				//if (thisInfo)
		}
			//std::vector<DynClassInfo> ons;
		ons.clear();
			// get rid of ons that were left so that we left with distinct
			// pairs of on/offs
		for (int i = 0; i < info.size(); i++) {
			DynClassInfo thisInfo = info.at(i);
			if (thisInfo.getOn() && ons.size() != 1) {
				ons.push_back(thisInfo);
			}
			else if ((!thisInfo.getOn()) && ons.size() == 1) {
				ons.pop_back();
			}
			else {
				mapCopy.at(thisInfo.getFunctionName()).erase(mapCopy.at(thisInfo.getFunctionName()).begin()+i);
				info.erase(info.begin() + i);
				i = i-1;
			}
		}
	}
		// print all out to check output
	printOutput(mapCopy);
}
return true;
}

DynClassInfo DynamicParser::generateDynClassInfoFromLine(std::string line) {
	char *pch;
	unsigned int timestamp_delimiter = line.find(" - ");
	// save the timestamp into a string var
	std::string timestamp = line.substr(0, timestamp_delimiter);
	//printf("timestamp: %s - ", timestamp.c_str());
	std::string afterTimeStamp = line.substr(timestamp_delimiter+3, line.length());

	// do some parsing magic here to get each of class name, function name, and
	// on/off value
	unsigned int first_comma = afterTimeStamp.find(",");
	std::string className = afterTimeStamp.substr(0, first_comma);

	std::string afterClassName = afterTimeStamp.substr(first_comma+1, afterTimeStamp.length());

	unsigned int second_comma = afterClassName.find(",");
	std::string functionName = afterClassName.substr(0, second_comma);

	std::string on = afterClassName.substr(second_comma+1, afterClassName.length());

	const char *timeDeets = timestamp.c_str();
	// create a timestamp using the time string
	boost::posix_time::ptime ptimestamp = boost::posix_time::time_from_string(timeDeets);

	bool on_off;
	if (on == "on") {
		on_off = true;
	}
	else {
		on_off = false;
	}

	DynClassInfo dynInfo(ptimestamp, className, functionName, on_off);
	return dynInfo;
}

void DynamicParser::printOutput(std::map<std::string, std::vector<DynClassInfo> > dynMap) {
	typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
	for (it_type iterator = dynMap.begin(); iterator != dynMap.end(); iterator++) {
		std::vector<DynClassInfo> info = iterator->second;
		for (int i = 0; i < info.size(); i++) {
			DynClassInfo tinfo = info[i];
			tinfo.printClassInfo();
		}
	}
}

int main() {
	DynamicParser *parser = new DynamicParser();
	parser->parseFile("dynamicOutput.txt");
	return 1;
}