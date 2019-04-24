#include "MatchManager.h"
#include "FileHandler.h"

int main(int argc, char* argv[]) {
	FileHandler fileH(argc, argv);
	if (!fileH.parsingIsAllowed()) return EXIT_FAILURE;	// IO Errors - can't parse!
	fileH.parseInput();
	return 0;
}