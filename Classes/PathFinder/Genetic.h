//
//  Genetic.h
//  LearnGA
//
//  Created by DONG on 15/5/12.
//
//

#ifndef __LearnGA__Genetic__
#define __LearnGA__Genetic__

#include "cocos2d.h"
USING_NS_CC;

const float CROSSOVER_RATE = 0.7f;
const float MUTATION_RATE = 0.01f;
const int POP_SIZE = 140;
const int CHROMO_LENGTH = 48;
const int GENE_LENGTH = 2;
const int FITTEST_NUM = 200;

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

class BobMap;

class CGenetic
{
public:
    CGenetic(float crossRate, float mutRate, int popSize, int chromoLength, int geneLen) :
        m_fCrossoverRate(crossRate),
        m_fMutationRate(mutRate),
        m_iPopSize(popSize),
        m_iChromoLength(chromoLength),
        m_iGeneLength(geneLen),
        m_iFittestNum(0),
        m_iFittestLength(0),
        m_iFittestGenome(0.f),
        m_fTotalFitnessScore(0.f),
        m_fBestFitnessScore(0.f),
        m_iGeneration(0),
        m_bBusy(true)
    {
        
    }
    
    void createStartPopulation();
    int getGeneration() { return m_iGeneration; }
    int getFittest() { return m_iFittestGenome; }
    std::vector<int> getFittestPath() { return decode(m_vecGenomes[m_iFittestGenome].vecBits); }
    void epoch();
    
private:
    void mutate(std::vector<int> &vecBits);
    void crossover(const std::vector<int> &mum, const std::vector<int> &dad,
                   std::vector<int> &baby1, std::vector<int> &baby2);
    SGenome& rouletteWheelSelection();
    void updateFitnessScores();
    std::vector<int> decode(const std::vector<int> &bits);
    int binToInt(const std::vector<int> &v);
    
    
private:
    std::vector<SGenome> m_vecGenomes;      // 染色体
    int m_iPopSize;                         // 群体个数
    float m_fCrossoverRate;                 // 杂交率
    float m_fMutationRate;                  // 变异率
    int m_iChromoLength;                    // 每条染色体多少bits
    int m_iGeneLength;                      // 每条基因多少bits
    int m_iFittestGenome;                   // 最适应的基因
    int m_iFittestNum;
    int m_iFittestLength;                   // 最适应长度
    float m_fBestFitnessScore;              // 最好的适应分数
    float m_fTotalFitnessScore;             //
    int m_iGeneration;                      // 遗传代数
    
    CC_SYNTHESIZE_READONLY(bool, m_bBusy, Busy);
    
    CC_SYNTHESIZE(BobMap*, m_map, Map);
};

#endif /* defined(__LearnGA__Genetic__) */
