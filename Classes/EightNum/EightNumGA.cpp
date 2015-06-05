//
//  EightNumGA.cpp
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#include "EightNumGA.h"
#include "EightNumMap.h"

void EightNumGA::epoch()
{
    reset();
    updateFitnessScore();
    if (!_bBusy)
    {
        return;
    }
    
    SEightGenome baby1, baby2;
    std::vector<SEightGenome> newPop;
    const int NEW_POP_ADD = 10;
    int i;
    for (i=0; i<NEW_POP_ADD; ++i)
    {
        newPop.push_back(_vecChromo[_fittestChromo]);
    }
    for (; i<_popSize; i+=2)
    {
        auto mum = rouletteWheelSelection();
        auto dad = rouletteWheelSelection();
        
        crossover(mum.vecPath, dad.vecPath, baby1.vecPath, baby2.vecPath);
        mutate(baby1.vecPath);
        mutate(baby2.vecPath);
        
        newPop.push_back(baby1);
        newPop.push_back(baby2);
    }
    _vecChromo = newPop;
    ++_generation;
}

void EightNumGA::createStartGeneration()
{
    _vecChromo.clear();
    for (int i=0; i<_popSize; ++i)
    {
        _vecChromo.push_back(SEightGenome(_chromoLength));
    }
}

void EightNumGA::crossover(const std::vector<int>& mum, const std::vector<int>& dad, std::vector<int>& baby1, std::vector<int>& baby2)
{
    baby1 = mum;
    baby2 = dad;
    if (CCRANDOM_0_1() > _crossoverRate || mum == dad)
        return;

    int beg = random(0, _chromoLength - 2);
    int end = random(beg + 1, _chromoLength - 1);

    for (int i=beg; i<=end; ++i)
    {
        std::swap(baby1[i], baby2[i]);
    }
}

void EightNumGA::mutate(std::vector<int>& gene)
{
    if (CCRANDOM_0_1() > _mutationRate)
        return;
    const int MinSpanSize = 3;
    int size = random(MinSpanSize, _chromoLength - 1);
    int beg = random(0, _chromoLength - size - 1);
    for (int i=beg; i<beg+size; ++i)
    {
        gene[i] = random(0, 3);
    }
}

SEightGenome EightNumGA::rouletteWheelSelection()
{
    float selScore = CCRANDOM_0_1() * _totalFitnessScore;
    float curScore = 0.f;
    int selIndex = 0;
    for (int i=0; i<_popSize; ++i)
    {
        curScore += _vecChromo[i].fitness;
        if (curScore > selScore)
        {
            selIndex = i;
            break;
        }
    }
    return _vecChromo[selIndex];
}

void EightNumGA::updateFitnessScore()
{
    int fitnessScore;
    for (int i=0; i<_popSize; ++i)
    {
        fitnessScore = _eightMap->testFittnessScore(_vecChromo[i].vecPath);
       
        _vecChromo[i].fitness = fitnessScore;
		_totalFitnessScore += _vecChromo[i].fitness;
        
        if (fitnessScore - _eightMap->getBestScore() == 0)
        {
            std::stringstream str;
            for (int pos : _vecChromo[i].vecPath)
            {
                str << pos << ",";
            }
            CCLOG("path: %s", str.str().c_str());
            _longestLength = _eightMap->getBestScore();
            _bBusy = false;
            break;
        }
        
        if (fitnessScore > _longestLength)
        {
            _longestLength = fitnessScore;
            _fittestChromo = i;
        }
    }
    CCLOG("cur best score: %d", _longestLength);
}

void EightNumGA::reset()
{
    _fittestChromo = 0;
    _shortestLength = 999999;
    _longestLength = -1;
}
