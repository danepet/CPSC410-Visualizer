#include <stdio.h>
#include <vector>
#include <iostream>
#include "DynamicRunner.h"

/**
* This is unused in the final submission. It is left here to indicate the testing done
* for the dynamic analyzer
**/

using namespace Dyninst;

int main(int argc, const char *argv[]) {
	DynamicRunner *newRunner = new DynamicRunner(7352, "test");

	/**
	std::vector<BPatch_process*> *getProcesses();

	BPatch *bpatch = new BPatch();
    BPatch_process *appProc = bpatch->processAttach("test", 5167);
	
	BPatch_image *appImage = appProc->getImage();

	std::vector<BPatch_function *> functions;
	std::vector<BPatch_point *> *points;

	
	//std::vector<BPatch_module *>* modules = appImage->getModules();

	//BPatch_module *appMod = modules[0][0];

	//functions = *(appMod->getProcedures());

	//for (int i = 0; i < functions.size(); i++) {
	//	BPatch_function *this_function = functions[i];
	//	std::cout << this_function->getName() << "\n";
	//}
	
	appImage->findFunction("testFunction", functions);
	//std::cout << functions.size() << "\n";
	points = functions[0]->findPoint(BPatch_entry);

	BPatch_variableExpr *intCounter = appProc->malloc(*(appImage->findType("int")));

	BPatch_arithExpr addOne(BPatch_assign, *intCounter,
		BPatch_arithExpr(BPatch_plus, *intCounter, BPatch_constExpr(1)));

	// Create a snippet that calls printf with each effective address
	std::vector<BPatch_snippet *> printfArgs;
	BPatch_snippet *fmt = new BPatch_constExpr("function called! Name is: testFunction\n");
	printfArgs.push_back(fmt);
	std::vector<BPatch_function *> printfFuncs;
	appImage->findFunction("printf", printfFuncs);
	BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);

	appProc->insertSnippet(printfCall, *points);
	appProc->continueExecution();
	int lastCount = 0;
	while (!appProc->isTerminated()) {

		//appProc->stopExecution();
		//int count;
		//intCounter->readValue(&count);
		//if (count != lastCount)
		//	printf("testFunction() has been called %d times so far\n",count);

		//lastCount = count;
		//appProc->continueExecution();
	}
	**/
}

void itFinished() {
	printf("The callback worked!");
}