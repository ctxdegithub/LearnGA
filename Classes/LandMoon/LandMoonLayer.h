#ifndef __LandMoonLayer_h__
#define __LandMoonLayer_h__

#include "cocos2d.h"
USING_NS_CC;

class Lander;
class LandGenetic;
class LandMoonLayer : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(LandMoonLayer);
	bool init();
	void update(float dt);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
    void reset();
    
private:
    int m_updateCount;
    bool m_bLandOK;
    Lander* _bestLander;
	Lander* _lander;
    LandGenetic* _landGA;
    Label* _labelGAInfo;
    std::vector<Lander*> _vecLander;
};

#endif
