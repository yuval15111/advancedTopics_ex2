#pragma once

#include "AlgorithmRegistration.h"
#include <vector>
#include <functional>
using namespace std;

class MatchManager {
	static MatchManager matchManagerObj;
	vector <function<unique_ptr<AbstractAlgorithm>()>> factoryVector;

public:
	void registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod);
	static MatchManager& getMatchManager();
};