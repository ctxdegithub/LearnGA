#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BobMap.h"
#include "Genetic.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto map = BobMap::create();
    addChild(map);
    m_genetic = new CGenetic(CROSSOVER_RATE, MUTATION_RATE, POP_SIZE, CHROMO_LENGTH, GENE_LENGTH);
    m_genetic->createStartPopulation();
    m_genetic->setMap(map);
    m_time = 0.f;
    
    srand(time(nullptr));
    
    scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)
{
    m_genetic->epoch();
    if (!m_genetic->getBusy())  // 找到路径
    {
        std::stringstream str;
        auto path = m_genetic->getFittestPath();
        m_genetic->getMap()->drawPath(path);
        for (int dir : path)
        {
            str << dir << ",";
        }
        CCLOG(str.str().c_str());
        unscheduleUpdate();
    }
}