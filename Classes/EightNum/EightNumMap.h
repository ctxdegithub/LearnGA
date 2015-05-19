//
//  EightNumMap.h
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#ifndef __LearnGA__EightNumMap__
#define __LearnGA__EightNumMap__

#include "cocos2d.h"
USING_NS_CC;

class EightNumMap : public Node
{
public:
    CREATE_FUNC(EightNumMap);
    bool init();
    int testFittnessScore(const std::vector<int> &path);
    
    
private:
    int getScore(const int map[]);
    bool canMove(int dir);
    void move(int dir);
    void calcBestScore();
    void drawEight();
    void drawGrid();
    void reset();
    
private:
    DrawNode* _drawNode, *_drawGrid;
    Label* _labelNum[9];
    int _map[9];
    int _zeroPos;
    CC_SYNTHESIZE(float, _bestScore, BestScore);
};
#endif /* defined(__LearnGA__EightNumMap__) */
