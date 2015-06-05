#include "Lander.h"

bool Lander::init()
{
	if (!Node::init())
	{
		return false;
	}

	_landerBody = DrawNode::create();
	addChild(_landerBody);
    Vec2 landerVecs[] = {
        Vec2(-20, 0),
        Vec2(-15, 10),
        Vec2(-13, 10),
        Vec2(-10, 15),
        Vec2(10, 15),
        Vec2(13, 10),
        Vec2(15, 10),
        Vec2(20, 0),
    };
    
    _landerBody->drawPoly(landerVecs, 8, true, Color4F(1.f, 0.f, 1.f, 1.f));
	_fire = DrawNode::create();
	addChild(_fire);
	_fire->drawTriangle(Vec2(-6, 0), Vec2(6, 0), Vec2(0, -8), Color4F(1.f, 1.f, 0.f, 0.5f));
	_fire->setVisible(false);

	_fitnessScore = -999999.f;
	_velocity = Vec2::ZERO;
	_actionFlag = NONE;
	_mass = LANDER_MASS;
	_tickCount = 0;
	_bLandOver = false;
	_bLandOK = false;
    _bCtrlByHuman = false;
	return true;;
}

void Lander::reset()
{
    _fitnessScore = -999999.f;
    _velocity = Vec2::ZERO;
    _actionFlag = NONE;
    _mass = LANDER_MASS;
    _tickCount = 0;
    _bLandOver = false;
    _bLandOK = false;
    _bCtrlByHuman = false;
    setRotation(0);
}

void Lander::setActions(const std::vector<ActionType> &actions)
{
    _tickCount = 0;
    _vecActions = actions;
    _bCtrlByHuman = false;
}

void Lander::actionFlag(int flag)
{
	_actionFlag |= flag;
}

void Lander::unActionFlag(int flag)
{
	_actionFlag &= ~flag;
}

void Lander::rotateLeft(float dt)
{
	float rot = getRotation();
//	rot += ROTATION * dt;
    rot += ROTATION_PER_TICK;
    
	if (rot > 360.f)
	{
		rot -= 360.f;
	}
	setRotation(rot);
}

void Lander::rotateRight(float dt)
{
	float rot = getRotation();
    
    rot -= ROTATION_PER_TICK;
	if (rot < -360.f)
	{
		rot += 360.f;
	}
	setRotation(rot);
}

void Lander::thrust(float dt)
{
//	float accel = THRUST * dt / _mass;
    float accel = THRUST_PER_TICK / _mass;
	_velocity.x += accel * sinf(CC_DEGREES_TO_RADIANS(getRotation()));
	_velocity.y += accel * cosf(CC_DEGREES_TO_RADIANS(getRotation()));
	_fire->setVisible(true);
}

void Lander::update(float dt)
{
	if (_bLandOver || _bLandOK)
	{
		return;
	}

    if (!_bCtrlByHuman)
    {
        if (_tickCount == _vecActions.size())
        {
            _actionFlag = NONE;
        }
        else
        {
            _actionFlag = _vecActions[_tickCount++];
        }
        
        switch (_actionFlag)
        {
            case 0:
            
                break;
            case 1:
                rotateLeft(dt);
                break;
            case 2:
                rotateRight(dt);
                break;
            case 3:
                thrust(dt);
                break;
            default:
                break;
        }
        if (_actionFlag == 3)
        {
            _fire->setVisible(true);
        }
        else
        {
            _fire->setVisible(false);
        }
    }
    else
    {
        if (_actionFlag & ROT_LEFT)
        {
            rotateLeft(dt);
        }
        if (_actionFlag & ROT_RIGHT)
        {
            rotateRight(dt);
        }
        if (_actionFlag & THRUST)
        {
            thrust(dt);
            _fire->setVisible(true);
        }
        else
        {
            _fire->setVisible(false);
        }
    }
    


//	_velocity.y += GRAVITY * dt;
    _velocity.y += GRAVITY_PER_TICK;
    
    auto pos = getPosition() + _velocity;
    if (pos.x > CLIENT_WIDTH)
    {
        pos.x = 0;
    }
    if (pos.x < 0)
    {
        pos.x = CLIENT_WIDTH;
    }
    
	setPosition(pos);
	
	if (getPositionY() <= _targetPos.y)
	{
		_bLandOver = true;
        calcFitnessScore();
	}
}

float Lander::calcFitnessScore()
{
	float distFromPad = fabs(getPositionX() - _targetPos.x);
	float distFit = CLIENT_WIDTH - distFromPad;
	float speed = _velocity.getLength();
	float rotFit = 1.f / (fabs(CC_DEGREES_TO_RADIANS(getRotation())) + 1);
	float fitAirTime = _tickCount / (speed + 1);
	_fitnessScore = distFit + 400 * rotFit + 4 * fitAirTime;

	if ((distFromPad < DIST_TOLERANCE) &&
		(speed < SPEED_TOLERANCE) &&
		(fabs(getRotation()) < ROTATION_TOLERANCE))
	{
		_fitnessScore = BIG_FITNESS;
		_bLandOK = true;
	}

	return _fitnessScore;
}

bool Lander::landOK()
{
	return _bLandOK;
}
