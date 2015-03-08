#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <sys/socket.h>
#include "../Tetraminos/static_lib/ClassProfile.h"
#include "InstrumentProfile.h"
#include <vector>
#include <map>
#include <set>
#include "ip/IpEndpointName.h"


using namespace std;

class OSCMessenger
{
	map<string, InstrumentProfile> instrumentMap;

private:
	int getNoteFromMap(string, string, map<string, InstrumentProfile>::iterator);
	void sendMIDINote(int, int, int);
	void sendAddInstrumentCommand(int, int);
public:
	OSCMessenger();
	bool createInstruments(map<string, ClassProfile>, vector<vector<string>>, vector<set<string>>);
	void playNote(string, string);
	void stopNote(string, string);
	map<string, InstrumentProfile>& getInstrumentMap();
};
