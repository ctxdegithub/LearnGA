//
//  Genetic.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "TSPGenetic.h"
#include "TSPMap.h"

void CTSPGenetic::createStartGeneration()
{
    for (int i=0; i<m_iPopSize; ++i)
    {
        m_vecGenomes.push_back(STSPGenome(m_iChromoLength));
    }
    m_iFittestGenome = 0;
    m_iGeneration = 0;
    m_fTotalFittestScore = 0.f;
    m_fShortestRoute = 999999999.f;
}

void CTSPGenetic::epoch()
{
    reset();
    updateFittestScore();
    if (!m_bBusy)
    {
        return;
    }
    int i;

    std::vector<STSPGenome> newVecPop;
    for (i=0; i<NUM_TO_ADD; ++i)
    {
        newVecPop.push_back(m_vecGenomes[m_iFittestGenome]);
    }
    STSPGenome baby1, baby2;
    if (m_selType == RouletteWheel)
    {
        for (; i<m_iPopSize; i+=2)
        {
            auto mum = rouletteWheelSelection();
            auto dad = rouletteWheelSelection();
            crossoverPMX(mum.vecCities, dad.vecCities, baby1.vecCities, baby2.vecCities);
            //        mutateEM(baby1.vecCities);        // 交换变异
            //        mutateEM(baby2.vecCities);
            //        mutateSM(baby1.vecCities);        // 散播变异
            //        mutateSM(baby2.vecCities);
            //        mutateDM(baby1.vecCities);        // 移位变异
            //        mutateDM(baby2.vecCities);
            mutateIM(baby1.vecCities);          // 插入变异
            mutateIM(baby2.vecCities);
            newVecPop.push_back(baby1);
            newVecPop.push_back(baby2);
        }
    }
    else if (m_selType == SUS)
    {
        std::vector<STSPGenome> vecPop;
        SUSSelection(vecPop);
        for (int gen = 0; gen < vecPop.size(); gen += 2)
        {
            crossoverPMX(vecPop[gen].vecCities, vecPop[gen+1].vecCities, baby1.vecCities, baby2.vecCities);
            //        mutateEM(baby1.vecCities);        // 交换变异
            //        mutateEM(baby2.vecCities);
            //        mutateSM(baby1.vecCities);        // 散播变异
            //        mutateSM(baby2.vecCities);
            //        mutateDM(baby1.vecCities);        // 移位变异
            //        mutateDM(baby2.vecCities);
            mutateIM(baby1.vecCities);          // 插入变异
            mutateIM(baby2.vecCities);
            newVecPop.push_back(baby1);
            newVecPop.push_back(baby2);
        }
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

void CTSPGenetic::mutateSM(std::vector<int> &chromo)
{
    if (CCRANDOM_0_1() > m_fMutationRate)
    {
        return;
    }
    const int MinSpanSize = 3;
    int beg, end;
    beg = random(0, (int)chromo.size() - MinSpanSize);
    end = random(beg + MinSpanSize - 1, (int)chromo.size() - 1);
    int span = end - beg;
    int leftNum = span;
    while (--leftNum)
    {
        auto gene1 = chromo.begin();
        auto gene2 = chromo.begin();
        std::advance(gene1, beg + random(0, span));
        std::advance(gene2, beg + random(0, span));
        std::swap(*gene1, *gene2);
    }
}

void CTSPGenetic::mutateDM(std::vector<int> &chromo)
{
    if (CCRANDOM_0_1() > m_fMutationRate)
    {
        return;
    }
    const int MinSpanSize = 1;
    int beg, end;
    beg = random(0, (int)chromo.size() - MinSpanSize);
    end = random(beg + MinSpanSize - 1, (int)chromo.size() - 1);
    auto startIt = chromo.begin() + beg;
    auto endIt = chromo.begin() + end;
    
    std::vector<int> theSelection;
    theSelection.assign(startIt, endIt);
    chromo.erase(startIt, endIt);
    
    int curPos = random(0, (int)chromo.size() - 1);
    chromo.insert(chromo.begin() + curPos, theSelection.begin(), theSelection.end());
}

void CTSPGenetic::mutateIM(std::vector<int> &chromo)
{
    if (CCRANDOM_0_1() > m_fMutationRate)
    {
        return;
    }
    int selected = random(0, (int)chromo.size() - 1);
    int num = chromo[selected];
    chromo.erase(chromo.begin() + selected);
    int curPos = random(0, (int)chromo.size() - 1);
    chromo.insert(chromo.begin() + curPos, num);
}

void CTSPGenetic::updateFittestScore()
{
    for (int i=0; i < m_iPopSize; ++i)
    {
        float tourLength = _tspMap->getDistance(m_vecGenomes[i].vecCities);
        m_vecGenomes[i].fFitness = tourLength;
        if (tourLength <= m_fShortestRoute)
        {
            m_fShortestRoute = tourLength;
            m_iFittestGenome = i;
            if (fabs(m_fShortestRoute - _tspMap->getBestDistance()) < 0.01)
            {
                m_bBusy = false;
                std::stringstream str;
                for (auto index : m_vecGenomes[i].vecCities)
                {
                    str << index << ",";
                }
                CCLOG("%s", str.str().c_str());
                CCLOG("generation: %d, shortest: %f, longest: %f", m_iGeneration, m_fShortestRoute, m_fLongestRoute);
                return;
            }
        }
        if (tourLength > m_fLongestRoute)
        {
            m_fLongestRoute = tourLength;
        }
    }

    for (auto& pop : m_vecGenomes)
    {
        pop.fFitness = m_fLongestRoute - pop.fFitness;
        m_fTotalFittestScore += pop.fFitness;
    }
    
    CCLOG("generation: %d, shortest: %f, longest: %f", m_iGeneration, m_fShortestRoute, m_fLongestRoute);
}

STSPGenome CTSPGenetic::rouletteWheelSelection()
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

void CTSPGenetic::SUSSelection(std::vector<STSPGenome> & newPop)
{
    int curGen = 0;
    float sum = 0.f;
    float pointerGap = m_fTotalFittestScore / (m_iPopSize - newPop.size());
    float ptr = CCRANDOM_0_1() * pointerGap;
    while (newPop.size() < m_iPopSize)
    {
        for (sum += m_vecGenomes[curGen].fFitness; sum > ptr; ptr += pointerGap)
        {
            newPop.push_back(m_vecGenomes[curGen]);
            if (newPop.size() == m_iPopSize)
                return;
        }
        ++curGen;
    }
    
}

void CTSPGenetic::reset()
{
    m_fTotalFittestScore = 0.f;
    m_fLongestRoute = 0.f;
    m_fShortestRoute = 999999999.f;
}











