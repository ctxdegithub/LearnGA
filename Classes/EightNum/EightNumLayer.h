//
//  EightNumLayer.h
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#ifndef __LearnGA__EightNumLayer__
#define __LearnGA__EightNumLayer__

#include "cocos2d.h"
USING_NS_CC;

class EightNumMap;
class EightNumGA;

class EightNumLayer : public Layer
{
public:
    static Scene* createScene();
    CREATE_FUNC(EightNumLayer);
    bool init();
    void update(float dt);
    
private:
    EightNumGA* _eightGA;
    EightNumMap* _eightMap;
};

#endif /* defined(__LearnGA__EightNumLayer__) */
