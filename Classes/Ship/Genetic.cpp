//
//  Genetic.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "Genetic.h"

void CGenetic::createStartGeneration()
{
    
}

void CGenetic::epoch()
{
    updateFittestScore();
    if (!m_bBusy)
    {
        return;
    }
    
}

void CGenetic::crossover(const std::vector<int> &mum, const std::vector<int> &dad, std::vector<int> &baby1, std::vector<int> &baby2)
{
    
}

void CGenetic::mutate(std::vector<int> &vecBits)
{
    
}

void CGenetic::updateFittestScore()
{
    
}