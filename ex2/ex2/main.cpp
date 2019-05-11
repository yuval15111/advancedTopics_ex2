#include "FileHandler.h"

int main(int argc, char* argv[]) {
	// MAZEPATH_INDEX = 0, ALGOPATH_INDEX = 1, OUTPUTPATH_INDEX = 2

	vector<string> paths = { ".", ".", "" };		// We define default values for the paths
	bool validArgs = initPaths(argc, argv, paths);	// We change them according to the program's arguments
	if (!validArgs) {
		printWrongArgumentsFormatError();
		return -1;
	}
	FileHandler fh(paths);							// We create a FileHandler object to parse everything needed:
	fh.getAlgorithms();								// Finding all the algorithms in [algorithm_path]
	fh.getMatchesAndPlay();							// Finding all the mazes in [maze_path] and solve them using the algorithms
	fh.createOutput();								// Finally we create the output to the screen, and if [output_path] exists we provide log files of every algorithm's moves on every maze.
	return 0;										// Ba-bye!
}