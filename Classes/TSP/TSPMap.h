//
//  TSPMap.h
//  LearnGA
//
//  Created by DONG on 15/5/18.
//
//

#ifndef __LearnGA__TSPMap__
#define __LearnGA__TSPMap__

#include "cocos2d.h"
USING_NS_CC;

class CTSPMap : public Node
{
public:
    CREATE_FUNC(CTSPMap);
    bool init();
    void update(float dt);
    float getDistance(const std::vector<int>& path);
    
private:
    void calculateBestPath();
    void drawPath(const std::vector<int>& path);
    void drawCities();
    
private:
    CC_SYNTHESIZE(float, _bestDistance, BestDistance);
    CC_SYNTHESIZE(int, _numCities, NumCities);
    DrawNode* _drawNode;
    DrawNode* _drawPath;
    std::vector<Vec2> _vecCitiesPos;
};

#endif /* defined(__LearnGA__TSPMap__) */
