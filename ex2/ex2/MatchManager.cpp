#include "MatchManager.h"
#include "AlgorithmRegistration.h"


//define the static variable
MatchManager MatchManager::matchManagerObj;

MatchManager::MatchManager(Parser p)
{

}

void MatchManager::registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> factoryMethod)
{
	factoryVector.push_back(factoryMethod);
}

MatchManager & MatchManager::getMatchManager()
{
	return matchManagerObj;
}
