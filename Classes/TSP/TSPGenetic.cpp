//
//  Genetic.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "TSPGenetic.h"

void CTSPGenetic::createStartGeneration()
{
    for (int i=0; i<m_iPopSize; ++i)
    {
        m_vecGenomes.push_back(SGenome(m_iChromoLength));
    }
}

void CTSPGenetic::epoch()
{
    updateFittestScore();
    if (!m_bBusy)
    {
        return;
    }
    reset();
    std::vector<SGenome> newVecPop;
    SGenome baby1, baby2;
    for (int i=0; i<m_iPopSize; i+=2)
    {
        auto mum = rouletteWheelSelection();
        auto dad = rouletteWheelSelection();
        crossoverPMX(mum.vecCities, dad.vecCities, baby1.vecCities, baby2.vecCities);
        mutateEM(baby1.vecCities);
        mutateEM(baby2.vecCities);
        
        newVecPop.push_back(baby1);
        newVecPop.push_back(baby2);
    }
    m_vecGenomes = newVecPop;
    ++m_iGeneration;
}

void CTSPGenetic::crossoverPMX(const std::vector<int> &mum, const std::vector<int> &dad, std::vector<int> &baby1, std::vector<int> &baby2)
{
    baby1 = mum;
    baby2 = dad;
    if (CCRANDOM_0_1() > m_fCrossoverRate || mum == dad)
    {
        return;
    }
    
    int beg = random(0, (int)mum.size() - 2);
    int end = random(beg, (int)mum.size() - 1);
    
    for (int pos = beg; pos < end + 1; ++pos)
    {
        int gene1 = mum[pos];
        int gene2 = dad[pos];
        if (gene1 != gene2)
        {
            int posGene1 = *std::find(baby1.begin(), baby1.end(), gene1);
            int posGene2 = *std::find(baby1.begin(), baby1.end(), gene2);
            std::swap(posGene1, posGene2);
            
            posGene1 = *std::find(baby2.begin(), baby2.end(), gene1);
            posGene2 = *std::find(baby2.begin(), baby2.end(), gene2);
            std::swap(posGene1, posGene2);
        }
    }
}

void CTSPGenetic::mutateEM(std::vector<int> &chromo)
{
    if (CCRANDOM_0_1() > m_fMutationRate)
    {
        return;
    }
    
    int pos1 = random(0, (int)chromo.size() - 2);
    int pos2 = random(pos1 + 1, (int)chromo.size() - 1);
    std::swap(chromo[pos1], chromo[pos2]);
}

void CTSPGenetic::updateFittestScore()
{
    for (int i=0; i < m_iPopSize; ++i)
    {
        float tourLength = 0.f;
        if (tourLength < m_fShortestRoute)
        {
            m_fShortestRoute = tourLength;
        }
        if (tourLength > m_fLongestRoute)
        {
            tourLength = m_fLongestRoute;
        }
    }
    for (auto& pop : m_vecGenomes)
    {
        pop.fFitness = m_fLongestRoute - pop.fFitness;
    }
}

SGenome CTSPGenetic::rouletteWheelSelection()
{
    float fFittest = CCRANDOM_0_1() * m_fTotalFittestScore;
    float curScore = 0;
    int selectIndex = 0;
    for (int i=0; i<m_iPopSize; ++i)
    {
        curScore += m_vecGenomes[i].fFitness;
        if (curScore > fFittest)
        {
            selectIndex = i;
            break;
        }
    }
    return m_vecGenomes[selectIndex];
}

void CTSPGenetic::reset()
{
    m_fTotalFittestScore = 0.f;
    m_fLongestRoute = 0.f;
    m_fShortestRoute = 999999999.f;
}











