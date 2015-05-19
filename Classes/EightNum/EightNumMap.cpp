//
//  EightNumMap.cpp
//  LearnGA
//
//  Created by DONG on 15/5/19.
//
//

#include "EightNumMap.h"

const int MAP_EIGHT[] = {2, 4, 5, 8, 1, 9, 6, 3, 7};
const int ZERO_POS = 5;
const int MAP_TARGET[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const float GRID_INTERVAL = 100.f;
const Vec2 OFFSET = Vec2(-150.f, -150.f);

bool EightNumMap::init()
{
    if (!Node::init())
    {
        return false;
    }

    for (int i=0; i<=8; ++i)
    {
        _labelNum[i] = Label::createWithSystemFont(StringUtils::format("%d", i), "Arial", 60);
        _labelNum[i]->setColor(Color3B::BLUE);
        addChild(_labelNum[i]);
    }
    
    _drawGrid = DrawNode::create();
    addChild(_drawGrid);
    _drawNode = DrawNode::create();
    addChild(_drawNode);
    reset();
    drawEight();
    drawGrid();
    
    calcBestScore();
    return true;
}

int EightNumMap::testFittnessScore(const std::vector<int> &path)
{
    reset();
    for (int dir : path)
    {
        if (canMove(dir))
        {
            move(dir);
            if (getScore(_map) == _bestScore)
            {
                drawEight();
                return _bestScore;
            }
                
        }
    }
    drawEight();
    return getScore(_map);
}

bool EightNumMap::canMove(int dir)
{
    switch (dir) {
        case 0: // left
            if (_zeroPos % 3 == 0)
                return false;
            break;
        case 1: // right
            if ((_zeroPos + 1) % 3 == 0)
                return false;
            break;
        case 2: // top
            if (_zeroPos < 3)
                return false;
            break;
        case 3: // bottom
            if (_zeroPos > 5)
                return false;
            break;
        default:
            break;
    }
    return true;
}

void EightNumMap::move(int dir)
{
    int old = _zeroPos;
    switch (dir) {
        case 0: // left
            _zeroPos--;
            break;
        case 1: // right
            _zeroPos++;
            break;
        case 2: // top
            _zeroPos -= 3;
            break;
        case 3: // bottom
            _zeroPos += 3;
            break;
        default:
            break;
    }
    std::swap(_map[old], _map[_zeroPos]);
}

void EightNumMap::reset()
{
    memcpy(_map, MAP_EIGHT, sizeof(MAP_EIGHT));
    _zeroPos = ZERO_POS;
}

int EightNumMap::getScore(const int map[])
{
    int ret = 0;
//    int tmp;
//    for (int i=0; i<9; ++i)
//    {
//        tmp = 0;
//        for (int j=i+1; j<9; ++j)
//        {
//            if (map[i] < map[j])
//            {
//                tmp++;
//            }
//        }
//        ret += Fract[tmp] * map[i];
//    }
    for (int i=0; i<9; ++i)
    {
        if (map[i] == i+1 && map[i] != 9)
        {
            ret += 100 - (i+1) * 10;
        }
    }
    return ret;
}

void EightNumMap::calcBestScore()
{
    _bestScore = getScore(MAP_TARGET);
}

void EightNumMap::drawEight()
{
    for (int i=0; i<9; ++i)
    {
        _labelNum[_map[i] % 9]->setPosition(OFFSET + Vec2(GRID_INTERVAL * (i % 3) + 50.f, GRID_INTERVAL * (i / 3) + 50.f));
    }
}

void EightNumMap::drawGrid()
{
    _drawGrid->clear();
    for (int i=0; i<4; ++i)
    {
        _drawGrid->drawLine(Vec2(GRID_INTERVAL * i, 0) + OFFSET, Vec2(GRID_INTERVAL * i, GRID_INTERVAL * 3) + OFFSET, Color4F(0.f, 1.f, 0.f, 1.f));
    }
    
    for (int i=0; i<4; ++i)
    {
        _drawGrid->drawLine(Vec2(0, GRID_INTERVAL * i) + OFFSET, Vec2(GRID_INTERVAL * 3, GRID_INTERVAL * i) + OFFSET, Color4F(0.f, 1.f, 0.f, 1.f));
    }
}