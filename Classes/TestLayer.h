//
//  TestLayer.h
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#ifndef __LearnGA__TestLayer__
#define __LearnGA__TestLayer__

#include "cocos2d.h"
#include "ThunderSlot.h"
USING_NS_CC;

class TestLayer : public Layer
{
public:
    static Scene* createScene();
    CREATE_FUNC(TestLayer);
    bool init();
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    
private:
    ThunderSlot* m_streak;
};

#endif /* defined(__LearnGA__TestLayer__) */
