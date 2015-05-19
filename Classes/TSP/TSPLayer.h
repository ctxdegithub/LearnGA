//
//  TestLayer.h
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#ifndef __LearnGA__TSPLayer__
#define __LearnGA__TSPLayer__

#include "cocos2d.h"
USING_NS_CC;

class CTSPMap;
class CTSPGenetic;

class TSPLayer : public Layer
{
public:
    static Scene* createScene();
    CREATE_FUNC(TSPLayer);
    bool init();
    void update(float dt);
    
private:
    CTSPGenetic* _tspGenetic;
    CTSPMap* _tspMap;
};

#endif /* defined(__LearnGA__TSPLayer__) */
