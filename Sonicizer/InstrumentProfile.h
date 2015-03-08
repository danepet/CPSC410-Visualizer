#include <vector>
#include <map>

using namespace std;

class InstrumentProfile {
	std::string className;
	std::map<string, int> methodToNoteMap;
	int inheritanceFamily;
	int trackTemplate;
	int trackNumber;
	int registerBooster;
	int channel;

public:
	map<string, int>& getMethodToNoteMap();
	void putInMethodToNoteMap(string, int);

	string getClassName() const;
	int getInheritanceFamily();
	int getTrackTemplate();
	int getTrackNumber();
	int getRegisterBooster();
	int getChannel();

	void setClassName(string);
	void setInheritanceFamily(int);
	void setTrackTemplate(int);
	void setTrackNumber(int);
	void setRegisterBooster(int);
	void setChannel(int);

};