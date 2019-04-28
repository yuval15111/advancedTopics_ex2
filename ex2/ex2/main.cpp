#include "MatchManager.h"
#include "FileHandler.h"

int main(int argc, char* argv[]) {
	FileHandler fileH(argc, argv);
	if (fileH.wrongArgumentsFormat) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}
	fileH.parseInput();
	return 0;
}