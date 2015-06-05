#include "LandMoonLayer.h"
#include "Lander.h"
#include "LandGenetic.h"

Vec2 START_POS;
Vec2 PAD_POS;

#define SPEED_UP

Scene* LandMoonLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = LandMoonLayer::create();
	scene->addChild(layer);
	return scene;
}

bool LandMoonLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    srand(time(nullptr));
	auto size = Director::getInstance()->getVisibleSize();
    START_POS = Vec2(size.width * 0.3f, 400);
    PAD_POS = Vec2(size.width * 0.5f, 55.f);
    m_updateCount = 1;
    m_bLandOK = false;
    
    _landGA = new LandGenetic(LAND_POP_SIZE, LAND_CHROMO_LEN, LAND_CROSSOVER_RATE, LAND_MUTATION_RATE);
    _landGA->createStartGeneration();
    
    _lander = Lander::create();
    addChild(_lander);
    _lander->setPosition(START_POS);
    
    auto padNode = DrawNode::create();
    addChild(padNode, 10);
    padNode->drawRect(PAD_POS - Vec2(15.f, 0), PAD_POS + Vec2(15.f, 4), Color4F(0.f, 1.f, 0.f, 1.f));

    for (int i=0; i<LAND_POP_SIZE; ++i)
    {
        _vecLander.push_back(Lander::create());
        _vecLander[i]->setPosition(START_POS);
        _vecLander[i]->setTargetPos(PAD_POS);
        _vecLander[i]->setActions(_landGA->getActionsByIndex(i));
        addChild(_vecLander[i]);
    }
    
    _labelGAInfo = Label::createWithSystemFont("generation: 1, best: 0", "Arail", 24);
    _labelGAInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_labelGAInfo);
    _labelGAInfo->setPosition(Vec2(size.width * 0.02f, size.height * 0.9f));
    
	auto keyListener = EventListenerKeyboard::create();
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	keyListener->onKeyPressed = CC_CALLBACK_2(LandMoonLayer::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(LandMoonLayer::onKeyReleased, this);

	eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	this->scheduleUpdate();

	return true;
}

void LandMoonLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		_lander->actionFlag(THRUST);
		break;
	case EventKeyboard::KeyCode::KEY_A:
        _lander->actionFlag(ROT_RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_D:
        _lander->actionFlag(ROT_LEFT);
		break;
	}
}

void LandMoonLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		_lander->unActionFlag(THRUST);
		break;
	case EventKeyboard::KeyCode::KEY_A:
        _lander->unActionFlag(ROT_RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_D:
        _lander->unActionFlag(ROT_LEFT);
		break;
    case EventKeyboard::KeyCode::KEY_U:
            m_updateCount = 700;
            break;
        case EventKeyboard::KeyCode::KEY_I:
            m_updateCount = 1;
            break;
        case EventKeyboard::KeyCode::KEY_R:
            reset();
        break;
	}
}

void LandMoonLayer::reset()
{
    m_bLandOK = false;
    _landGA->createStartGeneration();
    _bestLander = nullptr;
    for (int i=0; i<LAND_POP_SIZE; ++i)
    {
        _vecLander[i]->reset();
        _vecLander[i]->setVisible(true);
        _vecLander[i]->setPosition(START_POS);
        _vecLander[i]->setActions(_landGA->getActionsByIndex(i));
    }

}

void LandMoonLayer::update(float dt)
{
    _lander->setRotation(30.f);
    int count = m_updateCount;
    while (count--)
    {
        bool bAllFinished = true;
        if (m_bLandOK)
        {
            _bestLander->update(dt);
            return;
        }
        
        for (int i=0; i<LAND_POP_SIZE; ++i)
        {
            _vecLander[i]->update(dt);
            _landGA->setFitnessByIndex(i, _vecLander[i]->getFitnessScore());
            if (_vecLander[i]->getLandOK())
            {
                _bestLander = _vecLander[i];
                _bestLander->reset();
                _vecLander[i]->setPosition(START_POS);
                for (int j=0; j<LAND_POP_SIZE; ++j)
                {
                    _vecLander[j]->setVisible(false);
                }
                _bestLander->setVisible(true);
                m_updateCount = 1;
                m_bLandOK = true;
                break;
            }
            if (!_vecLander[i]->getLandOver())
            {
                bAllFinished = false;
            }
            
            
        }
        if (bAllFinished)
        {
            _landGA->epoch();
            _labelGAInfo->setString(StringUtils::format("generation: %d, best: %f", _landGA->getGeneration(), _landGA->getFittestScore()));
            for (int i=0; i<LAND_POP_SIZE; ++i)
            {
                _vecLander[i]->reset();
                _vecLander[i]->setPosition(START_POS);
                _vecLander[i]->setActions(_landGA->getActionsByIndex(i));
            }
            if (_landGA->getGeneration() > 700)
            {
                reset();
            }
        }
        _lander->update(dt);
    }
    
    
}
