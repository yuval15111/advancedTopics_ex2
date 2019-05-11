#include "FileHandler.h"

int main(int argc, char* argv[]) {
	// MAZEPATH_INDEX = 0, ALGOPATH_INDEX = 1, OUTPUTPATH_INDEX = 2
	string paths[3] = { ".", ".", "" };
	bool pathExists[3];
	bool validArgs = initPaths(argc, argv, paths, pathExists);
	if (!validArgs) {
		printWrongArgumentsFormatError();
		return -1;
	}
	FileHandler fh(paths);
	fh.getAlgorithms();
	fh.getMatches();
	fh.createOutput();
	return 0;
}