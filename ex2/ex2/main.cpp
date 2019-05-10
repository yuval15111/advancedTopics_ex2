#include "FileHandler.h"

int main(int argc, char* argv[]) {
	FileHandler fh(argc, argv);
	if (fh.invalidArgs()) {
		printWrongArgumentsFormatError();
		return EXIT_FAILURE;	// Arguments Errors - can't parse!
	}
	//cout << "Main - after creating fh. " << endl;
	//vector<MatchManager *> matchVector;
	//vector<ifstream*> & mazeVector = fh.getMazeVector();

	//cout << "Maze vector size:" << mazeVector.size() << endl;
	//cout << "Main - after getting mazeVector. " << endl;
	//for (unsigned int i = 0; i < mazeVector.size(); i++) {

	//	cout << "Main - before parseInput. " << endl;
	//	MatchManager * m = fh.parseInput(mazeVector[i]);
	//	cout << "Main - after parseInput. " << endl;
	//	if (m == nullptr) {
	//		// TODO: deallocate all memory allocations
	//		return EXIT_FAILURE;
	//	}

	//	cout << "Main - before create game managers. " << endl;
	//	m->createGameManagers();
	//	cout << "Main - after create game managers. " << endl;
	//	//m->activateGameManagers();
	//	matchVector.push_back(m);
	//}
	//fh.pushLogsToOutputFiles(matchVector, fh.outputPathExists());
	return 0;
}