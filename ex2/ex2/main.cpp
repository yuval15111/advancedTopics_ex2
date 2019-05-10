#include "FileHandler.h"

int main(int argc, char* argv[]) {
	FileHandler fh(argc, argv);
	if (fh.invalidArgs()) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}
	return 0;
}