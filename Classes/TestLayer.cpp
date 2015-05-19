//
//  TestLayer.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "TestLayer.h"
#include "TSP/TSPMap.h"
#include "TSP/TSPGenetic.h"

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
    srand(time(nullptr));
    _tspGenetic = new CTSPGenetic(POP_SIZE, NUM_CITIES, CROSSOVER_RATE, MUTATION_RATE);
    _tspMap = CTSPMap::create(NUM_CITIES);
    addChild(_tspMap);
    auto size = Director::getInstance()->getVisibleSize();
    _tspMap->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
    _tspGenetic->setTSPMap(_tspMap);
    
    m_streak = ThunderSlot::create(0.5, 0.1, 70, Color3B(255,255,255), "thunderbolt_tex.png");
    m_streak->setPosition(Vec2(100, 100));

    m_streak->setFastMode(false);
    addChild(m_streak);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TestLayer::touchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TestLayer::touchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TestLayer::touchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    this->scheduleUpdate();
    return true;
}

bool TestLayer::touchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    m_streak->setPosition(touch->getLocation());
    return true;
}

void TestLayer::touchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_streak->setAngle(m_streak->getAngle() - touch->getLocation().getAngle(touch->getPreviousLocation()));
}

void TestLayer::touchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void TestLayer::update(float dt)
{
    if (_tspGenetic->getBusy())
    {
        _tspGenetic->epoch();
    }
   
}