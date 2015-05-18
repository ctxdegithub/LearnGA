//
//  Genetic.h
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#ifndef __LearnGA__Genetic__
#define __LearnGA__Genetic__

#include "cocos2d.h"
USING_NS_CC;

struct SGenome
{
    std::vector<int> vecCities;
    float fFitness;
    SGenome() : fFitness(0) {}
    SGenome(int numCities) : fFitness(0)
    {
        grabPermutation(numCities, vecCities);
    }
    
    void grabPermutation(int numCities, std::vector<int>& cities)
    {
        int next;
        for (int i=0; i<numCities; ++i)
        {
            while(testNum(next = random(0, numCities-1)));
            vecCities.push_back(next);
        }
    }
    
    bool testNum(int num)
    {
        auto it = std::find(vecCities.begin(), vecCities.end(), num);
        return (it != vecCities.end());
    }
    
    friend bool operator<(const SGenome& left, const SGenome& right)
    {
        return left.fFitness < right.fFitness;
    }
};

class CTSPGenetic
{
public:
    CTSPGenetic(int popSize, int chromoLen, int geneLen, float crossoverRate, float mutationRate) :
    m_iPopSize(popSize),
    m_iChromoLength(chromoLen),
    m_iGeneLength(geneLen),
    m_fCrossoverRate(crossoverRate),
    m_fMutationRate(mutationRate),
    m_fShortestRoute(999999999),
    m_fLongestRoute(0.f),
    m_fFittestScore(0.f),
    m_fTotalFittestScore(0.f)
    {
        createStartGeneration();
    }
    
    void createStartGeneration();
    void epoch();
    
private:
    SGenome rouletteWheelSelection();
    void crossoverPMX(const std::vector<int> &mum, const std::vector<int> &dad, std::vector<int> &baby1, std::vector<int> &baby2);
    void mutateEM(std::vector<int> &vecBits);
    void updateFittestScore();
    void reset();
    
private:
    std::vector<SGenome> m_vecGenomes;      // 染色体
    int m_iFittestGenome;
    int m_iPopSize;                 // 群体个数
    int m_iChromoLength;            // 染色体长度
    int m_iGeneLength;              // 基因长度
    float m_fFittestScore;          // 适应分数
    float m_fTotalFittestScore;
    int m_iGeneration;              // 遗传代数
    float m_fCrossoverRate;         // 杂交率
    float m_fMutationRate;          // 变异率
    float m_fShortestRoute;
    float m_fLongestRoute;
    
    CC_SYNTHESIZE(bool, m_bBusy, Busy);
};


#endif /* defined(__LearnGA__Genetic__) */
