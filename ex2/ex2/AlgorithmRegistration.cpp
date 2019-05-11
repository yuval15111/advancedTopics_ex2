#include "AlgorithmRegistration.h"
#include "MatchManager.h"

/* This function registers a new algorithm into the registrar. */
AlgorithmRegistration::AlgorithmRegistration(AlgorithmFactory factoryMethod) {
	AlgorithmRegistrar::getInstance().registerAlgorithm(factoryMethod);
}
