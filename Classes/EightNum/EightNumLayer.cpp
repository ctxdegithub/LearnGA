//
//  EightNumLayer.cpp
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#include "EightNumLayer.h"
#include "EightNumGA.h"
#include "EightNumMap.h"

Scene* EightNumLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = EightNumLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool EightNumLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    srand(time(nullptr));
    auto size = Director::getInstance()->getVisibleSize();
    _eightMap = EightNumMap::create();
    addChild(_eightMap);
    _eightMap->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
    _eightGA = new EightNumGA(EIGHT_POP_SIZE, EIGHT_CHROMO_LEN, EIGHT_CROSSOVER_RATE, EIGHT_MUTATION_RATE);
    _eightGA->setEightMap(_eightMap);
    
    this->scheduleUpdate();
    return true;
}

void EightNumLayer::update(float dt)
{
    if (_eightGA->getBusy())
    {
        _eightGA->epoch();
    }
    
}