#include "DynClassInfo.h"
// Default constructor, does nothing
DynClassInfo::DynClassInfo() {}

// Constructor that sets tha private vars
DynClassInfo::DynClassInfo(boost::posix_time::ptime ts, std::string cn, std::string funcName, bool on_off) {
	timestamp = ts;
	className = cn;
	functionName = funcName;
	on = on_off;
}

// returns the function name
std::string DynClassInfo::getFunctionName() {
	return functionName;
}

// returns the timestamp
boost::posix_time::ptime DynClassInfo::getTimeStamp() {
	return timestamp;
}

// returns the classname
std::string DynClassInfo::getClassName() {
	return className;
}

// returns whether this was an on or an off
bool DynClassInfo::getOn() {
	return on;
}

// debug function to print private vars
void DynClassInfo::printClassInfo() {
	std::string timestampAsString = boost::posix_time::to_simple_string(timestamp);
	std::cout << "Timestamp: " << timestampAsString << " - Class: " << className <<
	" - Function: " << functionName << " - on: " << on << "\n";
}