//
//  BobMap.cpp
//  LearnGA
//
//  Created by DONG on 15/5/13.
//
//

#include "BobMap.h"

int BobMap::s_map[MAP_HEIGHT][MAP_WIDTH] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

const float GRID_WIDTH = 50.f;
const float GRID_HEIGHT = 50.f;

bool BobMap::init()
{
    if (!Node::init())
    {
        return false;
    }
    m_startX = 1;
    m_startY = 1;
    
    m_map = DrawNode::create();
    addChild(m_map);
    
    m_bob = DrawNode::create();
    addChild(m_bob);
    m_bob->drawDot(Vec2(GRID_WIDTH * (m_startX + 0.5f), GRID_HEIGHT * (m_startY + 0.5f)), 20.f, Color4F(1.f, 0.f, 0.f, 1.f));
    drawMap();
    return true;
}

float BobMap::testRoute(const std::vector<int> &path)
{
    m_bob->clear();
    int x = m_startX, y = m_startY;
    CCLOG("path: %d",  path.size());
    for (int dir : path)
    {
        switch (dir) {
            case 0: // left
                x--;
                break;
            case 1: // right
                x++;
                break;
            case 2: // top
                y++;
                break;
            case 3: // bottom
                y--;
                break;
            default:
                break;
        }
        CCLOG("pos: %d,%d", x, y);
        if (x == m_endX && y == m_endY)
        {
            return 1;
        }
        if (!canMove(x, y))
        {
            break;
        }
        m_bob->setPosition(GRID_WIDTH * (x + 0.5f), GRID_HEIGHT * (y + 0.5f));
    }
    return 1.f / (1.f + fabs(m_endX - x) + fabs(m_endY - y));
}

bool BobMap::canMove(int x, int y)
{
    return s_map[x][y] == 1;
}

void BobMap::drawMap()
{
    for (int i=0; i<MAP_HEIGHT; ++i)
    {
        for (int j=0; j<MAP_WIDTH; ++j)
        {
            float rgb = s_map[i][j];
            m_map->drawRect(Vec2(GRID_WIDTH * j+1, GRID_HEIGHT * i+1), Vec2(GRID_WIDTH * (j+1), GRID_HEIGHT * (i+1)),
                                 Color4F(rgb, rgb, rgb, 1.f));
        }
    }
}