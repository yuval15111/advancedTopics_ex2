#include "FileHandler.h"

int main(int argc, char* argv[]) {
	/*for (int i = 0; i < argc; i++) {
		cout << "argument " << i<< ": "<< argv[i] << endl;
	}*/

	FileHandler fh(argc, argv);
	//if (fh.invalidArgs()) {
	//	printWrongArgumentsFormatError();
	//	return EXIT_FAILURE;	// Arguments Errors - can't parse!
	//}
	return 0;
}