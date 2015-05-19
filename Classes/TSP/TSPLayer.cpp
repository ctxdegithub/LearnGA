//
//  TSPLayer.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "TSPLayer.h"
#include "TSPMap.h"
#include "TSPGenetic.h"

Scene* TSPLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = TSPLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool TSPLayer::init()
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
    

    this->scheduleUpdate();
    return true;
}

void TSPLayer::update(float dt)
{
    if (_tspGenetic->getBusy())
    {
        _tspGenetic->epoch();
    }
   
}