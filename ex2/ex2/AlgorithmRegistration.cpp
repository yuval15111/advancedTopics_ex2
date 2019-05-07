#include "AlgorithmRegistration.h"
#include "MatchManager.h"

//registers a new algorithm
AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> factoryMethod)
{
	AlgorithmRegistrar::getInstance().registerAlgorithm(factoryMethod);
}
