#include "LandGenetic.h"

void LandGenetic::createStartGeneration()
{
	_vecChromo.resize(_popSize);
	for (int i = 0; i < _popSize; ++i)
	{
		_vecChromo[i] = SLandGenome(_chromoLength);
	}
    _bBusy = true;
	_generation = 0;
}

std::vector<ActionType> LandGenetic::getActionsByIndex(int index)
{
    auto& vecGene = _vecChromo[index].vecActions;
    std::vector<ActionType> vecActions;
    for (auto& gene : vecGene)
    {
        for (int i=0; i<gene.duration; ++i)
        {
            vecActions.push_back(gene.action);
        }
        
    }
    return vecActions;
}

void LandGenetic::epoch()
{
	updateFitness();
	if (!_bBusy)
	{
		return;
	}

	std::vector<SLandGenome> newPop;
	const int NEW_POP_ADD = 4;
	for (int i = 0; i < NEW_POP_ADD; ++i)
	{
		newPop.push_back(_vecChromo[_popSize - 1 - i]);
	}
	for (int i = NEW_POP_ADD; i < _popSize; i += 2)
	{
		SLandGenome baby1, baby2;
		auto mum = rouletteWheelSelection();
		auto dad = rouletteWheelSelection();
		crossover(mum.vecActions, dad.vecActions, baby1.vecActions, baby2.vecActions);
		mutate(baby1.vecActions);
		mutate(baby2.vecActions);
		newPop.push_back(baby1);
		newPop.push_back(baby2);
	}
    _vecChromo = newPop;
	++_generation;
}

void LandGenetic::crossover(const std::vector<SLandGene> &mum, const std::vector<SLandGene> &dad, std::vector<SLandGene>& baby1, std::vector<SLandGene> &baby2)
{
    if (CCRANDOM_0_1() > _crossoverRate || (mum == dad))
    {
        baby1 = mum;
        baby2 = dad;
        return;
    }

    baby1.resize(LAND_CHROMO_LEN);
    baby2.resize(LAND_CHROMO_LEN);
    float swapRate = CCRANDOM_0_1() * LAND_CHROMO_LEN;
	for (int i = 0; i < _chromoLength; ++i)
	{
		if (CCRANDOM_0_1() > swapRate)
		{
			baby1[i] = mum[i];
			baby2[i] = dad[i];
		}
		else
		{
			baby1[i] = dad[i];
			baby2[i] = mum[i];
		}

	}

}

void LandGenetic::mutate(std::vector<SLandGene>& vecActions)
{
	for (int gene = 0; gene < vecActions.size(); ++gene)
	{
		if (CCRANDOM_0_1() < _mutationRate)
		{
			vecActions[gene].action = (ActionType)random(0, 3);
		}
		if (CCRANDOM_0_1() < _mutationRate * 0.5f)
		{
			vecActions[gene].duration += CCRANDOM_MINUS1_1() * MAX_MUTATION_DURATION;
			vecActions[gene].duration = clampf(vecActions[gene].duration, 0, MAX_ACTION_DURATION);
		}
	}
}

SLandGenome LandGenetic::rouletteWheelSelection()
{
	float selFitness = CCRANDOM_0_1() * _totalFitnessScore;
	float curFitness = 0;
	int selChromo = 0;
	for (int i = 0; i < _popSize; ++i)
	{
		curFitness += _vecChromo[i].fitness;
		if (curFitness > selFitness)
		{
			selChromo = i;
			break;
		}
	}
	return _vecChromo[selChromo];
}

void LandGenetic::setFitnessByIndex(int index, float fitness)
{
    _vecChromo[index].fitness = fitness;
}

void LandGenetic::updateFitness()
{
	reset();

	for (int i = 0; i < _vecChromo.size(); ++i)
	{
		float fitness = _vecChromo[i].fitness;
		_totalFitnessScore += fitness;
		if (fitness > _fittestScore)
		{
			_fittestScore = fitness;
			_fittestChromo = i;
			if (fabs(fitness - BIG_FITNESS) < FLT_EPSILON)
			{
				_bBusy = false;
				break;
			}
		}
	}
    std::sort(_vecChromo.begin(), _vecChromo.end());
}


void LandGenetic::reset()
{
	_fittestChromo = 0;
	_fittestScore = 0.f;
	_totalFitnessScore = 0.f;
}

