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
    std::vector<int> vecBits;
    float fFitness;
    SGenome() : fFitness(0) {}
    SGenome(const int numBits) : fFitness(0)
    {
        vecBits.resize(numBits);
        for (int i=0; i<numBits; ++i)
        {
            vecBits[i] = random(0, 1);
        }
    }
};

class CGenetic
{
public:
    CGenetic(int popSize, int chromoLen, int geneLen, float crossoverRate, float mutationRate) :
    m_iPopSize(popSize),
    m_iChromoLength(chromoLen),
    m_iGeneLength(geneLen),
    m_fFittestScore(0.f),
    m_fTotalFittestScore(0.f),
    m_fCrossoverRate(crossoverRate),
    m_fMutationRate(mutationRate)
    {
        
    }
    
    void createStartGeneration();
    void epoch();
    
private:
    void crossover(const std::vector<int> &mum, const std::vector<int> &dad, std::vector<int> &baby1, std::vector<int> &baby2);
    void mutate(std::vector<int> &vecBits);
    void updateFittestScore();
    
private:
    std::vector<SGenome> m_vecGenomes;      // 染色体
    int m_iPopSize;                 // 群体个数
    int m_iChromoLength;            // 染色体长度
    int m_iGeneLength;              // 基因长度
    float m_fFittestScore;          // 适应分数
    float m_fTotalFittestScore;
    int m_iGeneration;              // 遗传代数
    float m_fCrossoverRate;         // 杂交率
    float m_fMutationRate;          // 变异率
    
    CC_SYNTHESIZE(bool, m_bBusy, Busy);
};


#endif /* defined(__LearnGA__Genetic__) */
