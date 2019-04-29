#include "MatchManager.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
	Parser p(argc, argv);
	if (p.invalidArgs()) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}

	MatchManager matchManager(p);
	return 0;
}