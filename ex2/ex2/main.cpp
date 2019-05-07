#include "FileHandler.h"

int main(int argc, char* argv[]) {
	FileHandler fh(argc, argv);
	if (fh.invalidArgs()) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}
	vector<MatchManager *> matchVector;
	vector<MazePair> & mazeVector = fh.getMazeVector();
	for (unsigned int i = 0; i < mazeVector.size(); i++) {
		MatchManager * m = fh.parseInput(mazeVector[i].second);
		if (m == nullptr) {
			// TODO: deallocate all memory allocations
			return EXIT_FAILURE;
		}
		m->createGameManagers();
		m->activateGameManagers();
		matchVector.push_back(m);
	}
	pushLogsToOutputFiles(matchVector, fh.outputPathExists());
	return 0;
}