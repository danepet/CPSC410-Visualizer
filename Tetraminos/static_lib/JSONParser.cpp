#include "JSONParser.h"

// Get PID of process running http://proswdev.blogspot.ca/2012/02/get-process-id-by-name-in-linux-using-c.html
int JSONParser::getProcIdByName(std::string procName)
    {
        int pid = -1;

    // Open the /proc directory
        DIR *dp = opendir("/proc");
        if (dp != NULL)
        {
            // Enumerate all entries in directory until process found
            struct dirent *dirp;
            while (pid < 0 && (dirp = readdir(dp)))
            {
                // Skip non-numeric entries
                int id = atoi(dirp->d_name);
                if (id > 0)
                {
                    // Read contents of virtual /proc/{pid}/cmdline file
                    std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                    ifstream cmdFile(cmdPath.c_str());
                    std::string cmdLine;
                    getline(cmdFile, cmdLine);
                    if (!cmdLine.empty())
                    {
                        // Keep first cmdline item which contains the program path
                        size_t pos = cmdLine.find('\0');
                        if (pos != std::string::npos)
                            cmdLine = cmdLine.substr(0, pos);
                        // Keep program name only, removing the path
                        pos = cmdLine.rfind('/');
                        if (pos != std::string::npos)
                            cmdLine = cmdLine.substr(pos + 1);
                        // Compare against requested process name
                        if (procName == cmdLine)
                            pid = id;
                    }
                }
            }
        }

        closedir(dp);

        return pid;
    }

// To determine if a vector contains a Matching std::string
int JSONParser::containsMatch(std::vector<std::vector<std::string> > block, std::string lookingFor, int& placeInt){

        // Iterate over the structurer looking for a match
        for(std::vector<std::vector<std::string> >::size_type f = 0; f != block.size(); f++){
            for(std::vector<std::string>::size_type m = 0; m != block[f].size(); m++){
                if(block[f][m].compare(lookingFor) == 0){
                    // return position in the outer vector of the inner vector where match is
                    placeInt = f;
                    return f;
                }
            }
        }
        return -1;
    }

// Determines whether any dependencies of a profile are already in the dependencyTree
void JSONParser::addDependencies(ClassProfile first, std::map<std::string, ClassProfile>& dmap, std::set<std::string>& tad, std::vector<std::string>& depvec, std::vector<std::string>& convec, std::vector<std::set<std::string> >& dtree, std::set<int>& pov){
        // Find out if the profile(Class) is already in the dependency tree
        if(find(convec.begin(), convec.end(), first.getProfile()) == convec.end()){
            // If it's not in the dependency tree recurse on it's dependencies
            tad.insert(first.getProfile());
            convec.push_back(first.getProfile());
            std::vector<std::string> deps = first.getDependency();
            for(std::vector<std::string>::size_type i = 0; i != deps.size(); i++){
                ClassProfile newFirst = dmap[deps[i]];
                JSONParser::addDependencies(newFirst, dmap, tad, depvec, convec, dtree, pov);

            }
        }else{
            // Loops to see if already in a different vector in the tree, if so we need to add all the positions so we can merge all of them into one later
            for(std::vector<std::set<std::string> >::size_type k = 0; k < dtree.size(); k++){
                if(find(dtree[k].begin(), dtree[k].end(), first.getProfile()) != dtree[k].end())
                    pov.insert(k);
            }
        }

    }

// Parse the JSON document passed in
void JSONParser::parseJSON(const rapidjson::Value& b,std::vector<ClassProfile>& listClasses,std::map<std::string,ClassProfile>& profilemap,std::map<int, std::vector<ClassProfile> >& inheritanceCounts,int& maxInheritances, std::vector<std::string>& dependencyVector, std::map<std::string,std::string>& classmap){
    for (rapidjson::SizeType i = 0; i < b.Size(); i++){ // rapidjson uses SizeType instead of size_t.

        //New cl object
        ClassProfile cl;
        const rapidjson::Value& temp = b[i];
        std::string className = temp["className"].GetString();
        std::string tempinheritance = temp["inheritance"].GetString();
        if (tempinheritance.compare("N/A") != 0){
            cl.setInheritance(tempinheritance);
        }
        std::cout << className << "\n";
        // Add classname key to profile name
        profilemap[className];
        // Create Profile
        cl.setProfile(className);
        dependencyVector.push_back(className);
        
        // Cycle over all methods in class
        const rapidjson::Value& tempMethod = temp["method"];
        for (rapidjson::SizeType j = 0; j < tempMethod.Size(); j++){
            printf("Method[%d] = %s\n", j, tempMethod[j].GetString());

            // add method to the ClassProfile
            cl.setMethod(tempMethod[j].GetString());

            // Add method to and class to hashmap
            classmap[tempMethod[j].GetString()] = temp["className"].GetString();

        }

        // Cycle over all fields in class
        const rapidjson::Value& tempField = temp["field"];
        for (rapidjson::SizeType h = 0; h < tempField.Size(); h++){
            printf("Field[%d] = %s\n", h, tempField[h].GetString());

            // add field to the ClassProfile
            cl.setField(tempField[h].GetString());

        }

        // Cycle over all dependencies in class
        const rapidjson::Value& tempDep = temp["dependency"];
        for (rapidjson::SizeType k = 0; k < tempDep.Size(); k++){
            printf("dependency[%d] = %s\n", k, tempDep[k].GetString());

            // add dependency to the ClassProfile
            cl.setDependency(tempDep[k].GetString());
        }


        // Add ClassProfile to the list of all classes
        listClasses.push_back(cl);

        // Add ClassProfile to the map of all classes, indexed by the className
        profilemap[className] = cl;

        // Used for inheritance groupings later on
        if (!cl.getInheritance().empty()){
            maxInheritances = 1;
            inheritanceCounts[1].push_back(cl);
        } else {
            inheritanceCounts[0].push_back(cl);
        }

    }
}

// Create groupings of all profiles that inherit from or are inherited from the same profiles
std::vector<std::vector<std::string> >* JSONParser::constructInheritance(int maxInheritances,std::map<int, std::vector<ClassProfile> > inheritanceCounts){
    std::vector<std::vector<std::string> > *inheritanceTree = new std::vector<std::vector<std::string> >;
    std::vector<ClassProfile> leftovers;
    JSONParser jp;
    for (int i = 0; i <= maxInheritances; i++){
        if (i==0){
            // Push all classes with 0 inheritance
            for(std::vector<ClassProfile>::size_type h = 0; h != inheritanceCounts[i].size(); h++){
                std::vector<std::string> tempClassName;
                tempClassName.push_back(inheritanceCounts[i][h].getProfile());
                inheritanceTree->push_back(tempClassName);
            }
        }else {
            // Push all classes that has their inheritance already in the array
            for(std::vector<ClassProfile>::size_type m = 0; m != inheritanceCounts[i].size(); m++){
                int placeInt2;
                // if an array exists where one of the inheritance of a profile matches
                if(0 < jp.containsMatch(*inheritanceTree, inheritanceCounts[i][m].getInheritance(), placeInt2)){
                    // push profile and all inheritance on to vector with match
                    inheritanceTree->at(placeInt2).push_back(inheritanceCounts[i][m].getProfile());
                } else {
                    // else we will have to cycle over these again so add it to leftovers
                    leftovers.push_back(inheritanceCounts[i][m]);
                }
                

            }
            int tracker = 0;

            // Iterate over the "leftovers", which are the remaining profiles that have not been added, until empty
            while(leftovers.size()>0) {
                int newTracker = leftovers.size();
                // determine wheter anything was added on the last cycle.
                if (tracker!=newTracker){
                    tracker = newTracker;
                    // Push all those that have corresponding inheritances into the inheritance tree
                    for(std::vector<ClassProfile>::size_type l = 0; l != leftovers.size(); l++){
                        int placeInt1;
                        if(0 < jp.containsMatch(*inheritanceTree, leftovers[l].getInheritance(), placeInt1)){
                            inheritanceTree->at(placeInt1).push_back(leftovers[l].getProfile());
                            leftovers.erase(leftovers.begin()+l);
                        }
                        
                    }
                } else {
                    // This means everything left is "N/A" so let's add it, and the while loop will be ended exited
                    for(std::vector<ClassProfile>::size_type j = 0; j != leftovers.size(); j++){
                        std::vector<std::string> tempAdd;
                        // Push onto temporary vector
                        tempAdd.push_back(leftovers[j].getProfile());
                        // erase profile from leftovers
                        leftovers.erase(leftovers.begin()+j);
                        // subtract one from iterator count since we removed an element
                        j = j-1;
                    }
                }

            }
        }
    }
    return inheritanceTree;
}

// get the earliest recorded timestamp from the map of DynClassInfos.
boost::posix_time::ptime JSONParser::getEarliestTimeStamp(std::map<std::string, std::vector<DynClassInfo> > *dynMap) {
    // initialize earliestTime variable using local_time. We are guaranteed that this is
    // later than the dynamic analysis timestamps, so it is safe to do this
    boost::posix_time::ptime earliestTime(boost::posix_time::second_clock::local_time());

    typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
    // for each key in the map
    for (it_type i = dynMap->begin(); i != dynMap->end(); i++){
        // extract the vector from the map
        std::vector<DynClassInfo> classVec = i->second;
        for (int s = 0; s < classVec.size(); s++) {
            DynClassInfo oninfo = classVec[s];
            boost::posix_time::ptime thisTime = oninfo.getTimeStamp();
            if (earliestTime > thisTime) {
                earliestTime = thisTime;
            }
        }
    }
    return earliestTime;
}

// return the difference between two ptimes
boost::posix_time::time_duration JSONParser::calcTimeDifference(boost::posix_time::ptime onTime, 
                                                    boost::posix_time::ptime offTime) {
    return offTime-onTime;
}

// extract the actual function name from a string - 
// eg. "class::subclass::function" returns "function"
std::string JSONParser::extractFunctionNameFromString(std::string fullFuncName) {
    int needle = fullFuncName.find("::");
    std::string ss = fullFuncName.substr(needle+2, fullFuncName.size());
    while (ss.find("::") != std::string::npos) {
        needle = ss.find("::");
        ss = ss.substr(needle+2, ss.size());
    }
    return ss;
}

// if the total time is less than 10 ms, set it to 10 ms
int JSONParser::checkTimeDurationForLessThanNSeconds(long totalTime) {
    if (totalTime < 10)
        return 10;
    else
        return totalTime;
}

void JSONParser::createInstrumentCommands(OSCMessenger *messenger){
    // initalize dynamic parser
    DynamicParser parser;
    // parse the dynamic output and return a map where:
    // key => function name
    // value => vector of DynClassInfo
    std::map<std::string, std::vector<DynClassInfo> > *dynMap = parser.parseFile("dynamicOutput.txt");
    // typedef for iterator
    typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
    // get the instrument map from the OSCMessenger
    std::map<std::string, InstrumentProfile> instrumentMap = messenger->getInstrumentMap();

    // get earliest time stamp in the dynamic analysis
    boost::posix_time::ptime earliestTime = getEarliestTimeStamp(dynMap);
    // open the final output file
    std::ofstream finalOutputFile;
    finalOutputFile.open("finalOutput.txt", std::ios::trunc);

    // for each element in the dynMap
    for (it_type i = dynMap->begin(); i != dynMap->end(); i++){
        std::vector<DynClassInfo> classVec = i->second;
        for (int s = 0; s < classVec.size(); s++) {
            // extract the on and off info
            DynClassInfo oninfo = classVec[s];
            DynClassInfo offinfo = classVec.at(s+1);
            // calculate the total duration that this note is on for
            boost::posix_time::time_duration timeDuration = calcTimeDifference(oninfo.getTimeStamp(), offinfo.getTimeStamp());
            std::string className = oninfo.getClassName();
            // use the class name to index into the instrument map to get an instrument profile
            InstrumentProfile iprof = instrumentMap.at(className);
            std::string fullFuncName = oninfo.getFunctionName();
            // extract the func name from the full func name
            // eg. "class::subclass::function" should return "function"
            std::string funcName = extractFunctionNameFromString(fullFuncName);
            // get the method to note map from the instrument profile
            std::map<std::string, int> mtnm = iprof.getMethodToNoteMap();
            // extract the note for this function
            int note = mtnm.at(funcName);
            long totalTime = checkTimeDurationForLessThanNSeconds(timeDuration.total_milliseconds());
            // get a relative time stamp so that we know when to play the note in relation to the earliest
            // time recorded
            boost::posix_time::time_duration relativeTimeStamp = oninfo.getTimeStamp() - earliestTime;

            // Finally, output all of this to the file.
            finalOutputFile << note << ",100," << iprof.getChannel() << "," << totalTime <<
            "," << iprof.getTrackNumber() << "," << relativeTimeStamp.total_milliseconds() <<
            "\n";

            s++;
        }
    }
    finalOutputFile.close();
}

// Returns a vector that has groupings (sets) of profiles that are dependent on one another
// All profiles that have some connection through dependencies will be in the same group
std::vector<std::set<std::string> >* JSONParser::constructDependency(std::map<std::string,ClassProfile>& profilemap, std::vector<std::string>& dependencyVector){
    std::vector<std::set<std::string> > *dependencyTree = new std::vector<std::set<std::string> >;
    // Keeps track of everything added to the dependencyTree thus far
    std::vector<std::string> containerVec;
    JSONParser jp;
    int count = 0;
    
    for (std::vector<std::string>::const_iterator i = dependencyVector.begin(); i != dependencyVector.end(); ++i){

        // Find place position of Class Name
        std::map<std::string, ClassProfile>::iterator searchit = profilemap.find(*i);
        // Set first as the ClassProfile found in the map
        ClassProfile first = profilemap[(*i)];
        // Local set of dependencies to add to the dependencyTree
        std::set<std::string> toAdd;

        // Make sure the profile has not been already added to the DependencyTree then continue
        if(find(containerVec.begin(), containerVec.end(), first.getProfile()) == containerVec.end()){
            
            std::set<int> posOfVecs;
            // Determine if any of it's dependencies are already added - returns vector of ints to positions of where they are
            jp.addDependencies(first, profilemap, toAdd, dependencyVector, containerVec, *dependencyTree, posOfVecs);
            /*std::cout << "Vector: " << count << std::endl;
            count = count + 1;
            for( std::set<std::string>::iterator p = toAdd.begin(); p != toAdd.end(); ++p){
                std::cout << *p << ' ' << std::endl;
            }*/
            
            // Merge Step
            // If there are cases of that need to be merged
            if(posOfVecs.size() > 0){
                std::set<int>::iterator setit = posOfVecs.begin();

                //add the current profile and all dependencies to first case of overlap
                dependencyTree->at(*setit).insert(toAdd.begin(), toAdd.end());
                // if more then one case of overlap
                if(posOfVecs.size() > 1){
                    // merge each additional overlap with the first occurance (Position 0 of posOfVecs)
                    for (std::set<int>::iterator k = std::next(posOfVecs.begin()); k != posOfVecs.end(); ++k){
                        dependencyTree->at(*setit).insert(dependencyTree->at(*k).begin(), dependencyTree->at(*k).end());
                        dependencyTree->at(*k).clear();
                    }
                }
            }else{
                // If no overlap, just push the set onto the dependencyTree
                dependencyTree->push_back(toAdd);
            }
        }
    }
    return dependencyTree;
}

int main(int argc, char * argv[]) {
    std::map<std::string,std::string> classmap;
    std::map<std::string,std::string>::iterator it;
    std::map<std::string,ClassProfile> profilemap;
    JSONParser jp;

    if (argc < 2){
        printf("Invalid number of arguments, please try again with 2 arguments");
        return -1;
    }
    
    // Find process id
    int pid = jp.getProcIdByName(argv[1]);
    std::cout << "pid: " << pid << std::endl;
    std::map<int, std::vector<ClassProfile> > inheritanceCounts;
    std::vector<ClassProfile>::iterator icit;
    int maxInheritances = 0;
    std::vector<std::string> dependencyVector;

    // Import JSON File
    // http://stackoverflow.com/questions/18107079/rapidjson-working-code-for-reading-document-from-file
    FILE * pFile = fopen(argv[2] , "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document d;
    d.ParseStream<0>(is);

    rapidjson::Document child;
    const rapidjson::Value& b = d["staticInfo"];
    assert(b.IsArray());
    std::vector<ClassProfile> listClasses;
    
    // Parse JSON file collecting groups of all Class Names, functions, inheritances, fields and dependencies.
    jp.parseJSON(b,listClasses, profilemap, inheritanceCounts, maxInheritances, dependencyVector, classmap);

    // Construct Inheritance Groupings to pass for Instrument Creation
    std::vector<std::vector<std::string> > *inheritanceTree = jp.constructInheritance(maxInheritances, inheritanceCounts);

    // Construct Dependency Groupings
    std::vector<std::set<std::string> > *dependencyTree = jp.constructDependency(profilemap, dependencyVector);



    OSCMessenger messenger;
    // Determine whether instruments were created successfully
    if (messenger.createInstruments(profilemap, *inheritanceTree, *dependencyTree)){
        printf("Successfully created Instruments\n");
    } else {
        printf("Failed Instrument Creation\n");
    }


    // Run the dynamic analysis
    DynamicRunner *dr = new DynamicRunner(pid, listClasses);
    dr->analyze();
    
    // Create the instrument commands after the dynamic analysis has finished
    jp.createInstrumentCommands(&messenger);


/* ~~~~~~~~ TESTING ~~~~~~~~~~~~ */

/*
// This is test data
    // const char myJson[] = "{\"staticInfo\":[{\"className\""cBlood\", \"method\":[\"cBlood\", 
    // \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\" \"bloodparts\",\"BLOODcount\"],
    // \"inheritance\":[],\"dependency\":[]},{\"className\""cBlood\", \"method\":[\"cBlood\", \"update\", 
    // \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\" \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],
    // \"dependency\":[]}]}";


// This is to check that all Inheritances are grouped together properly
std::cout << "This is the Inheritances" << std::endl;
for( std::vector<std::vector<std::string> >::const_iterator i = inheritanceTree.begin(); i != inheritanceTree.end(); ++i){
std::cout << "Vector: ";
std::cout << i - inheritanceTree.begin() << std::endl;
for( std::vector<std::string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j){
std::cout << *j << ' ' << std::endl;
}
}
*/

//This is for Dyninst to use for iterating over it's functions
/* std::cout << "This is printing from the list of Class" << std::endl;
for( std::vector<ClassProfile>::const_iterator i = listClasses.begin(); i != listClasses.end(); ++i){
std::cout << "This is profile: ";
std::cout << (*i).getProfile() << std::endl;
vector<std::string> methods = (*i).getMethods();
for( std::vector<std::string>::const_iterator j = methods.begin(); j != methods.end(); ++j){
std::cout << *j << ' ' << std::endl;
}
}*/




/*
std::cout << "This is the Dependencies" << std::endl;
for(std::vector<std::set<std::string> >::const_iterator k = dependencyTree.begin(); k != dependencyTree.end(); ++k){
std::cout << "Vector: " << k - dependencyTree.begin() << std::endl;
for( std::set<std::string>::const_iterator q = (*k).begin(); q != (*k).end(); ++k){
std::cout << *q << ' '<< std::endl;
}
}
*/

}

