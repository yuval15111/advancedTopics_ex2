#include "AlgorithmRegistration.h"
#include "MatchManager.h"

//registers a new player algorithm
AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> factoryMethod)
{
	MatchManager::getMatchManager().registerAlgorithm(factoryMethod);
}
