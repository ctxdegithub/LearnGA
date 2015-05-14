//
//  TestLayer.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "TestLayer.h"

Scene* TestLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = TestLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool TestLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    m_streak = ThunderSlot::create(0.5, 0.1, 70, Color3B(255,255,255), "thunderbolt_tex.png");
    m_streak->setPosition(Vec2(100, 100));

    m_streak->setFastMode(false);
    addChild(m_streak);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TestLayer::touchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TestLayer::touchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TestLayer::touchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool TestLayer::touchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_streak->setPosition(touch->getLocation());
    return true;
}

void TestLayer::touchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_streak->setAngle(m_streak->getAngle() - 0.1f * touch->getLocation().getAngle(touch->getPreviousLocation()));
}

void TestLayer::touchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}