#ifndef __LandGenetic_H__
#define __LandGenetic_H__

#include "cocos2d.h"
#include "Lander.h"
USING_NS_CC;

const float LAND_CROSSOVER_RATE = 0.7f;
const float LAND_MUTATION_RATE = 0.01f;
const int LAND_POP_SIZE = 200;
const int LAND_CHROMO_LEN = 30;
const int MAX_ACTION_DURATION = 30;
const int MAX_MUTATION_DURATION = MAX_ACTION_DURATION / 2;

//const float LAND_CROSSOVER_RATE = 0.7f;
//const float LAND_MUTATION_RATE = 0.012f;
//const int LAND_POP_SIZE = 200;
//const int LAND_CHROMO_LEN = 50;
//const int MAX_ACTION_DURATION = 30;

struct SLandGene
{
	ActionType action;
	int duration;
	SLandGene() 
	{
		action = (ActionType)random(0, 3);
		duration = random(1, MAX_ACTION_DURATION);
	}
	bool operator == (const SLandGene& rhs) const
	{
		return (action == rhs.action) && (duration == rhs.duration);
	}
};

struct SLandGenome 
{
	float fitness;
	std::vector<SLandGene> vecActions;
	SLandGenome() : fitness(0) {}
	SLandGenome(int numActions) : fitness(0) 
	{
		for (int i = 0; i < numActions; ++i)
		{
			vecActions.push_back(SLandGene());
		}
	}
	friend bool operator<(const SLandGenome& lhs, const SLandGenome& rhs)
	{
		return (lhs.fitness < rhs.fitness);
	}
};

class LandGenetic
{
public:
	LandGenetic(int popSize, int chromoLen, float crossoverRate, float mutationRate) :
		_popSize(popSize),
		_chromoLength(chromoLen),
		_crossoverRate(crossoverRate),
		_mutationRate(mutationRate),
		_fittestChromo(0),
		_generation(0)
	{}
	void createStartGeneration();
	void epoch();
	void reset();
    std::vector<ActionType> getActionsByIndex(int index);
    void setFitnessByIndex(int index, float fitness);
    int getGeneration() { return _generation; }
    float getFittestScore() { return _fittestScore; }
    
private:
	void updateFitness();
	void crossover(const std::vector<SLandGene> &mum, const std::vector<SLandGene> &dad, std::vector<SLandGene>& baby1, std::vector<SLandGene> &baby2);
	void mutate(std::vector<SLandGene>& vecBits);
	SLandGenome rouletteWheelSelection();
	
private:
	int _generation;
	int _popSize;
	int _chromoLength;
	int _fittestChromo;
	float _totalFitnessScore;
	float _crossoverRate;
	float _mutationRate;
	float _fittestScore;
	std::vector<SLandGenome> _vecChromo;
	CC_SYNTHESIZE(bool, _bBusy, Busy);
};

#endif