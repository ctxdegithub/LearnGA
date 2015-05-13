//
//  Genetic.cpp
//  LearnGA
//
//  Created by DONG on 15/5/12.
//
//

#include "Genetic.h"
#include "BobMap.h"

void CGenetic::epoch()
{
    updateFitnessScores();
    int newBobies = 0;
    std::vector<SGenome> vecBabyGenomes;
    while (newBobies < m_iPopSize)
    {
        auto mum = rouletteWheelSelection();
        auto dad = rouletteWheelSelection();
        SGenome baby1, baby2;
        crossover(mum.vecBits, dad.vecBits, baby1.vecBits, baby2.vecBits);
        mutate(baby1.vecBits);
        mutate(baby2.vecBits);
        
        vecBabyGenomes.push_back(baby1);
        vecBabyGenomes.push_back(baby2);
        
        newBobies += 2;
    }
    
    m_vecGenomes = vecBabyGenomes;
    m_iGeneration++;
}

void CGenetic::crossover(const std::vector<int> &mum, const std::vector<int> &dad,
                         std::vector<int> &baby1, std::vector<int> &baby2)
{
    if (CCRANDOM_0_1() > m_fCrossoverRate || mum == dad)
    {
        baby1 = mum;
        baby2 = dad;
        return;
    }
    baby1.resize(m_iChromoLength);
    baby2.resize(m_iChromoLength);
    int cp = random(0, m_iChromoLength - 1);
    int i;
    for (i=0; i<cp; ++i)
    {
        baby1[i] = mum[i];
        baby2[i] = dad[i];
    }
    for (i=cp; i<m_iChromoLength; ++i)
    {
        baby2[i] = mum[i];
        baby1[i] = dad[i];
    }
}


void CGenetic::mutate(std::vector<int> &vecBits)
{
    for (int i=0; i<vecBits.size(); ++i)
    {
        if (CCRANDOM_0_1() < m_fMutationRate)
        {
            vecBits[i] = ~vecBits[i];
        }
    }
}

SGenome& CGenetic::rouletteWheelSelection()
{
    float fSlice = CCRANDOM_0_1() * m_fTotalFitnessScore;
    float fTotal = 0;
    int selectedGenome = 0;
    for (int i=0; i<m_iPopSize; ++i)
    {
        fTotal += m_vecGenomes[i].fFitness;
        if (fTotal > fSlice)
        {
            selectedGenome = i;
            break;
        }
    }
    
    return m_vecGenomes[selectedGenome];
}

void CGenetic::updateFitnessScores()
{
    m_fTotalFitnessScore = 0.f;
    m_fBestFitnessScore = 0.f;
    m_iFittestGenome = 0;
    
    for (int i=0; i<m_vecGenomes.size(); ++i)
    {
        std::vector<int> vecDirections = decode(m_vecGenomes[i].vecBits);
        m_vecGenomes[i].fFitness = m_map->testRoute(vecDirections);
        m_fTotalFitnessScore += m_vecGenomes[i].fFitness;
        if (m_vecGenomes[i].fFitness > m_fBestFitnessScore)
        {
            m_fBestFitnessScore = m_vecGenomes[i].fFitness;
            if (m_fBestFitnessScore == 1.f)
            {
                m_iFittestGenome = i;
                m_bBusy = false;
                return;
            }
        }
    }
}

std::vector<int> CGenetic::decode(const std::vector<int> &bits)
{
    std::vector<int> directions;
    for (int gene=0; gene<bits.size(); gene += m_iGeneLength)
    {
        std::vector<int> thisGene(bits.begin() + gene, bits.begin() + gene + m_iGeneLength);
        directions.push_back(binToInt(thisGene));
    }
    return directions;
}

int CGenetic::binToInt(const std::vector<int> &v)
{
    
    int ret = 0;
    for (auto bin : v)
    {
        ret <<= 1;
        ret += bin;
    }
    return ret;
}

void CGenetic::createStartPopulation()
{
    m_vecGenomes.clear();
    for (int i=0; i<m_iChromoLength; ++i)
    {
        m_vecGenomes.push_back(SGenome(m_iChromoLength));
    }
    m_iGeneration = 0;
    m_iFittestGenome = 0;
    m_fBestFitnessScore = 0.f;
    m_fTotalFitnessScore = 0.f;
}