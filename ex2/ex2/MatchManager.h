#pragma once

#include "AlgorithmRegistration.h"
#include "Parser.h"
#include <vector>
#include <functional>
using namespace std;

class MatchManager {
	static MatchManager matchManagerObj;
	vector <function<unique_ptr<AbstractAlgorithm>()>> factoryVector;

public:
	MatchManager(Parser p);
	void registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod);
	static MatchManager& getMatchManager();
};