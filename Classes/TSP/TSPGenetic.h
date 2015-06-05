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

#define NUM_CITIES			100

#define	MUTATION_RATE		0.4f
#define	CROSSOVER_RATE	    0.75f
#define	POP_SIZE			600

#define NUM_TO_ADD 2

struct STSPGenome
{
    std::vector<int> vecCities;
    float fFitness;
    STSPGenome() : fFitness(0) {}
    STSPGenome(int numCities) : fFitness(0)
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
    
    friend bool operator<(const STSPGenome& left, const STSPGenome& right)
    {
        return left.fFitness < right.fFitness;
    }
};

class CTSPMap;

class CTSPGenetic
{
public:
    enum SelectionType
    {
        RouletteWheel,
        SUS,
    };
public:
    CTSPGenetic(int popSize, int chromoLen, float crossoverRate, float mutationRate) :
    m_iPopSize(popSize),
    m_iChromoLength(chromoLen),
    m_fCrossoverRate(crossoverRate),
    m_fMutationRate(mutationRate),
    m_fShortestRoute(999999999),
    m_fLongestRoute(0.f),
    m_fFittestScore(0.f),
    m_fTotalFittestScore(0.f),
    m_bBusy(true)
    {
        m_selType = SUS;
        createStartGeneration();
    }
    
    void createStartGeneration();
    void epoch();
    
private:
    STSPGenome rouletteWheelSelection();
    void SUSSelection(std::vector<STSPGenome> & newPop);
    void crossoverPMX(const std::vector<int> &mum, const std::vector<int> &dad, std::vector<int> &baby1, std::vector<int> &baby2);
    void mutateEM(std::vector<int> &chromo);   // 交换变异
    void mutateSM(std::vector<int> &chromo);   // 散播变异
    void mutateDM(std::vector<int> &chromo);   // 移位变异
    void mutateIM(std::vector<int> &chromo);   // 插入变异
    void updateFittestScore();
    void reset();
    
private:
    std::vector<STSPGenome> m_vecGenomes;      // 染色体
    int m_iFittestGenome;
    int m_iPopSize;                 // 群体个数
    int m_iChromoLength;            // 染色体长度
    float m_fFittestScore;          // 适应分数
    float m_fTotalFittestScore;
    int m_iGeneration;              // 遗传代数
    float m_fCrossoverRate;         // 杂交率
    float m_fMutationRate;          // 变异率
    float m_fShortestRoute;
    float m_fLongestRoute;
    
    CC_SYNTHESIZE(SelectionType, m_selType, SelType);
    CC_SYNTHESIZE(CTSPMap*, _tspMap, TSPMap);
    CC_SYNTHESIZE(bool, m_bBusy, Busy);
};


#endif /* defined(__LearnGA__Genetic__) */
