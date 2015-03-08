#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

/**
* This is unused in the final submission. It is left here to indicate the testing done
* for the dynamic analyzer
**/

int testFunction() {
	//std::cout << "Called me!\n";
	return 1;
}

int main(int argc, const char *argv[]) {

	int i = 5;
	pid_t pid = getpid();
	printf("%ld\n", (long)pid);

	while (i != 0) {
		//char input;

		//std::cin >> input;

		//if (input == 'c') {
		//	testFunction();
		//}
		sleep(2);
		testFunction();
	}

	return 0;
}