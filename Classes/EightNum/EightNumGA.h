//
//  EightNumGA.h
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#ifndef __LearnGA__EightNumGA__
#define __LearnGA__EightNumGA__

#include "cocos2d.h"
USING_NS_CC;

#define EIGHT_POP_SIZE 150
#define EIGHT_CHROMO_LEN 140
#define EIGHT_CROSSOVER_RATE 0.75f
#define EIGHT_MUTATION_RATE 0.1f

struct SEightGenome
{
    int fitness;
    std::vector<int> vecPath;
    SEightGenome() : fitness(0.f) {}
    SEightGenome(int pathLen) : fitness(0)
    {
        for (int i=0; i<pathLen; ++i)
        {
            vecPath.push_back(random(0, 3));
        }
    }
};

class EightNumMap;

class EightNumGA
{
public:
    EightNumGA(int popSize, int chromoLen, float crossoverRate, float mutationRate) :
    _popSize(popSize),
    _chromoLength(chromoLen),
    _crossoverRate(crossoverRate),
    _mutationRate(mutationRate),
    _generation(0),
    _fittestChromo(0),
    _bBusy(true)
    {
        createStartGeneration();
    }
    
    void epoch();
    
private:
    void createStartGeneration();
    void crossover(const std::vector<int>& mum, const std::vector<int>& dad, std::vector<int>& baby1, std::vector<int>& baby2);
    void mutate(std::vector<int>& gene);
    SEightGenome rouletteWheelSelection();
	void updateFitnessScore();
    void reset();
    
private:
    int _generation;
    std::vector<SEightGenome> _vecChromo;
    int _popSize;
    int _chromoLength;
    int _fittestChromo;
    
    int _shortestLength;
    int _longestLength;
    
    int _totalFitnessScore;
    float _crossoverRate;
    float _mutationRate;

    CC_SYNTHESIZE(EightNumMap*, _eightMap, EightMap);
    CC_SYNTHESIZE(bool, _bBusy, Busy);
};

#endif /* defined(__LearnGA__EightNumGA__) */
