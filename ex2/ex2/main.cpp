#include "MatchManager.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
	Parser p(argc, argv);
	if (p.invalidArgs()) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}
	vector<MatchManager *> matchVector;
	vector<MazePair> & mazeVector = p.getMazeVector();
	for (unsigned int i = 0; i < mazeVector.size(); i++) {
		MatchManager * m = p.parseInput(mazeVector[i].second);
		if (m == nullptr) {
			// TODO: deallocate all memory allocations
			return EXIT_FAILURE;
		}
		m->createGameManagers();
		m->activateGameManagers();
		matchVector.push_back(m);
	}
	/*pushLogsToOutputFiles(matchVector, p.outputPathExists());*/
	return 0;
}