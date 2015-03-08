#include "DynamicRunner.h"

// DynamicRunner initializer
// sets up the app image and gets the functions from the app
DynamicRunner::DynamicRunner(int pid, std::vector<ClassProfile>& c) {
	std::vector<BPatch_process*> *getProcesses();

	bpatch = new BPatch();
	// Attach the bpatch to the process given by pid
    appProc = bpatch->processAttach("exe", pid);
    // Get the process image
    appImage = appProc->getImage();
    classes = c;
}

// The workhorse function
void DynamicRunner::analyze() {
	//std::cout << "size is: " << classes.size() << "\n";

	// clear out the output file before we start
	FILE *pfile;
    pfile = fopen("/../../Tetraminos/static_lib/dynamicOutput.txt", "w");
    fclose(pfile);

	for (int i = 0; i < classes.size(); i++) {
		ClassProfile thisClass = classes[i];
		// find the name of the class
		std::string className = thisClass.getProfile();
		std::vector<std::string> classFunctions = thisClass.getMethods();
		//printf("Are we here?\n");
		for (int s = 0; s < classFunctions.size(); s++) {
			std::string thisFuncName = classFunctions[s];
			std::vector<BPatch_function *> funcsMatchingName;
			std::string fullFuncName = className;
			// append the class name to the function name, so that it looks like:
			// class::function
			fullFuncName.append("::");
			fullFuncName.append(thisFuncName);
			// if we find the function in the appImage matching this funcName
			if (appImage->findFunction(fullFuncName.c_str(), funcsMatchingName)) {
				BPatch_function *this_function = funcsMatchingName[0];
				
				// setup a vector of funcpoints and set to the entry point of this_function
				// NOTE - the 'entry' point is essentially a memory address
				std::vector<BPatch_point *> *func_points;
				func_points = this_function->findPoint(BPatch_entry);
				// Create a snippet that calls printf every time a function is called
				std::vector<BPatch_snippet *> printfArgs;
				// Setup the funcString - format is "<className>,<functionName>,<on/off>
				// remember that our shared object printf function will prepend a timestamp
				// to this.
				std::string funcString = className;
				funcString.append(",");
				funcString.append(this_function->getName());
				funcString.append(",on\n");
				// Create a BPatch_constExpr using the funcString
				BPatch_snippet *fmt = new BPatch_constExpr(funcString.c_str());
				printfArgs.push_back(fmt);

				std::vector<BPatch_function *> printfFuncs;
				// Find printf in the system library
				appImage->findFunction("printf", printfFuncs);
				// create a BPatch_funcCallExpr using printf and the string argument defined earlier
				BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
				// inject the printf into the running process at the function
				// entry point
				injectFuncIntoFunc(printfCall, func_points);

				// modify the printf string so that it reads off instead of off
				funcString.erase(funcString.end()-4, funcString.end());
				funcString.append(",off\n");
				// change the func_points variable to the exit point
				func_points = this_function->findPoint(BPatch_exit);
				printfArgs.pop_back();
				BPatch_snippet *fmt1 = new BPatch_constExpr(funcString.c_str());
				printfArgs.push_back(fmt1);

				BPatch_funcCallExpr printfCallEnd(*(printfFuncs[0]), printfArgs);
				// inject the printf into the running process at the function
				// exit point
				injectFuncIntoFunc(printfCallEnd, func_points);
			}
			
		}
	}

	appProc->continueExecution();
	// wait until the app is terminated and the status changes, then we will break 
	// out of this.
	while (!appProc->isTerminated()) {
		bpatch->waitForStatusChange();
	}
}

// helper functio to inject a function into a process
void DynamicRunner::injectFuncIntoFunc(BPatch_funcCallExpr funcToInject, std::vector<BPatch_point *> *func_points) {
	appProc->insertSnippet(funcToInject, *func_points);
}

/** 
* Find a function based on the function class name and the 
* function name in the mutatee
* NOTE -- NOT USED -- DEBUG FUNCTION
**/
BPatch_function* DynamicRunner::findFunctionWithClassName(
	std::string funcName, 
	std::string className, 
	std::vector<BPatch_function *> funcsMatchingName
	) {
	for (int i = 0; i < funcsMatchingName.size(); i++) {

	}
	return NULL;
}


