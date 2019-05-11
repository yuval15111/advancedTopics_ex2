#include "AlgorithmRegistration.h"
#include "MatchManager.h"

//registers a new algorithm
AlgorithmRegistration::AlgorithmRegistration(AlgorithmFactory factoryMethod) {
	AlgorithmRegistrar::getInstance().registerAlgorithm(factoryMethod);
}
