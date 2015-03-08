#include <iostream>
#include <vector>
#include <string>

class ClassProfile {
    std::string profile;
    std::vector<std::string> methods;
    std::vector<std::string> fields; 
    std::string inheritance; 
    std::vector<std::string> dependencies;
    int size;
  public:
    void setProfile (std::string);
    std::string getProfile() const;
    std::vector<std::string> getMethods() const;
    std::vector<std::string> getFields() const;
    std::string getInheritance() const;
    std::vector<std::string>	 getDependency() const;
    int getSize();
    void setMethod(std::string);
    void setInheritance(std::string);
    void setSize(int);
    void setField(std::string);
    void setDependency(std::string);

};