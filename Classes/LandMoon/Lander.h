#ifndef __Lander_h__
#define __Lander_h__

#include "cocos2d.h"
USING_NS_CC;

const float ROTATION = CC_RADIANS_TO_DEGREES(3.f);
const float THRUST = 350.f;
const float GRAVITY = -1.63f;
const float LANDER_MASS = 100.f;
const int FRAMES_PER_SECOND = 60;

const float GRAVITY_PER_TICK = GRAVITY/FRAMES_PER_SECOND;
const float THRUST_PER_TICK = THRUST/FRAMES_PER_SECOND;
const float ROTATION_PER_TICK = ROTATION/FRAMES_PER_SECOND;

#define BIG_FITNESS 99999.f
#define CLIENT_WIDTH 1000.f

#define ROTATION_TOLERANCE     CC_RADIANS_TO_DEGREES(M_PI / 16)
#define SPEED_TOLERANCE        0.5
#define DIST_TOLERANCE         4.0

//#define ROTATION_TOLERANCE     10
//#define SPEED_TOLERANCE        0.5
//#define DIST_TOLERANCE         4.0

enum ActionType
{
	None,
	RotateLeft,
	RotateRight,
	Thrust,
};

#define NONE 0x1
#define ROT_LEFT 0x10
#define ROT_RIGHT 0x100
#define THRUST 0x1000

class Lander : public Node
{
public:
	CREATE_FUNC(Lander);
	bool init();
	void update(float dt);
    void reset();
    
	void rotateLeft(float dt);
	void rotateRight(float dt);
	void thrust(float dt);
	void actionFlag(int flag);
	void unActionFlag(int flag);

    void setActions(const std::vector<ActionType>& actions);
    bool getLandOver() { return _bLandOver; }
    
private:
	float calcFitnessScore();
	bool landOK();

private:
	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(Vec2, _targetPos, TargetPos);
	CC_SYNTHESIZE_READONLY(float, _fitnessScore, FitnessScore);
	CC_SYNTHESIZE_READONLY(bool, _bLandOK, LandOK);
	
	bool _bLandOver;
    bool _bCtrlByHuman;
	std::vector<ActionType> _vecActions;
	int _actionFlag;
	int _tickCount;
	float _mass;
	Vec2 _velocity;
	DrawNode* _landerBody;
	DrawNode* _fire;

};
#endif