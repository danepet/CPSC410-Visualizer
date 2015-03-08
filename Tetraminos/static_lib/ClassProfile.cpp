#include "ClassProfile.h"

void ClassProfile::setProfile (std::string prof) {
  profile = prof;
}

std::string ClassProfile::getProfile () const {
  return profile;
}

std::vector<std::string> ClassProfile::getMethods() const{
	return methods; 
}

std::vector<std::string> ClassProfile::getFields() const{
	return methods; 
}

std::string ClassProfile::getInheritance() const{
	return inheritance; 
}

std::vector<std::string> ClassProfile::getDependency() const{
	return dependencies; 
}

int ClassProfile::getSize(){
	return size; 
}

void ClassProfile::setMethod (std::string d){
	methods.push_back(d);
}

void ClassProfile::setField (std::string d){
	fields.push_back(d);
}

void ClassProfile::setInheritance (std::string d){
	inheritance = d;
}

void ClassProfile::setDependency (std::string d){
	dependencies.push_back(d);
}

void ClassProfile::setSize(int i){
	size = i;
}

// int main () {
//   std::cout << "Hello world!" << std::endl;
//   ClassProfile pr;
//   pr.Profile("fish");
//   pr.setSize(1);
//   pr.setMethod("method1");
//   std::cout << pr.getSize() << std::endl;
//   for( std::vector<string>::const_iterator i = pr.getMethods().begin(); i != pr.getMethods().end(); ++i)
//     std::cout << *i << ' ' << std::endl;

//   return 0;
// }