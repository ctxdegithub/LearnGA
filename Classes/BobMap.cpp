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
    m_startX = 14;
    m_startY = 7;
    m_endX = 0;
    m_endY = 2;
    
    m_map = DrawNode::create();
    addChild(m_map);
    
    m_bob = DrawNode::create();
    addChild(m_bob);
    m_bob->drawDot(Vec2(GRID_WIDTH * (m_startX + 0.5f), GRID_HEIGHT * (MAP_HEIGHT - m_startY - 0.5f)), 20.f, Color4F(1.f, 0.f, 0.f, 1.f));
//    m_bob->setPosition(Vec2(GRID_WIDTH * (m_startX + 0.5f), GRID_HEIGHT * (MAP_HEIGHT - m_startY - 0.5f)));
    drawMap();
    return true;
}

float BobMap::testRoute(const std::vector<int> &path)
{
    m_bob->clear();
    int x = m_startX, y = m_startY;
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
        if (x == m_endX && y == m_endY)
        {
            return 1;
        }
        if (!canMove(x, y))
        {
            break;
        }
        if (x == 6 && y == 3)
        {
            CCLOG("~~~~~~~");
        }
        m_bob->drawDot(Vec2(GRID_WIDTH * (x + 0.5f), GRID_HEIGHT * (MAP_HEIGHT - y - 0.5f)), 20.f, Color4F(1.f, 1.f, 0.f, 1.f));
    }
    return 1.f / (1.f + fabs(m_endX - x) + fabs(m_endY - y));
}

bool BobMap::canMove(int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
    {
        return false;
    }
    return s_map[y][x] == 0;
}

void BobMap::drawMap()
{
    for (int i=0; i<MAP_HEIGHT; ++i)
    {
        for (int j=0; j<MAP_WIDTH; ++j)
        {
            float rgb = s_map[i][j];
            m_map->drawRect(Vec2(GRID_WIDTH * j + 1, GRID_HEIGHT * (MAP_HEIGHT - i) + 1),
                            Vec2(GRID_WIDTH * (j + 1), GRID_HEIGHT * (MAP_HEIGHT - i - 1)),
                                 Color4F(rgb, rgb, rgb, 1.f));
        }
    }
    
    m_map->drawRect(Vec2(GRID_WIDTH * m_endX, GRID_HEIGHT * (MAP_HEIGHT - m_endY)),
                    Vec2(GRID_WIDTH * (m_endX+1), GRID_HEIGHT * (MAP_HEIGHT - m_endY - 1)),
                    Color4F(0.1f, 1.f, 1.f, 1.f));
}