#include <iostream>
#include <vector>
using namespace std;

class ClassProfile {
    std::string profile;
    std::vector<std::string> methods;
    std::vector<std::string> fields; 
    std::vector<std::string> inheritance; 
    std::vector<std::string> dependencies;
    int size;
  public:
    void setProfile (std::string);
    std::string getProfile() const;
    vector<std::string> getMethods() const;
    vector<std::string> getFields() const;
    vector<std::string> getInheritance() const;
    vector<std::string>	 getDependency() const;
    int getSize();
    void setMethod(std::string);
    void setInheritance(std::string);
    void setSize(int);
    void setField(std::string);
    void setDependency(std::string);

};

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

std::vector<std::string> ClassProfile::getInheritance() const{
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
	inheritance.push_back(d);
}

void ClassProfile::setDependency (std::string d){
	methods.push_back(d);
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