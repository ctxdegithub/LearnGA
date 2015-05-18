//
//  TSPMap.cpp
//  LearnGA
//
//  Created by DONG on 15/5/18.
//
//

#include "TSPMap.h"

bool CTSPMap::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _drawNode = DrawNode::create();
    addChild(_drawNode);
    
    _drawPath = DrawNode::create();
    addChild(_drawPath);
    
    drawCities();
    
    return true;
}

void CTSPMap::calculateBestPath()
{
    _bestDistance = 0.f;
    auto& pos1 = _vecCitiesPos[0];
    for (int i=1; i<_numCities; ++i)
    {
        _bestDistance += pos1.getDistance(_vecCitiesPos[i]);
        pos1 = _vecCitiesPos[i];
    }
    _bestDistance += pos1.getDistance(_vecCitiesPos[0]);
}

float CTSPMap::getDistance(const std::vector<int>& path)
{
    float pathLength = 0.f;
    auto& pos1 = _vecCitiesPos[path[0]];
    for (int i=1; i<path.size(); ++i)
    {
        pathLength += pos1.getDistance(_vecCitiesPos[path[i]]);
        pos1 = _vecCitiesPos[path[i]];
    }
    pathLength += pos1.getDistance(_vecCitiesPos[path[0]]);
    
    drawPath(path);
    
    return pathLength;
}

void CTSPMap::update(float dt)
{
    
}

void CTSPMap::drawCities()
{
    _drawNode->clear();
    _vecCitiesPos.clear();
    float delta = M_PI / _numCities;
    for (int i=0; i<_numCities; ++i)
    {
        auto curPos = Vec2(cosf(delta * i), sinf(delta * i)) * 200;
        _vecCitiesPos.push_back(curPos);
        _drawNode->drawDot(curPos, 20, Color4F(1.f, 1.f, 0.f, 1.f));
    }
}

void CTSPMap::drawPath(const std::vector<int>& path)
{
    _drawPath->clear();
    int cur = 0;
    Vec2 pos1 = _vecCitiesPos[path[cur]];
    for (int i=1; i<path.size(); ++i)
    {
        Vec2 pos2 = _vecCitiesPos[path[i]];
        _drawPath->drawLine(pos1, pos2, Color4F(1.f, 0.f, 0.f, 1.f));
        pos1 = pos2;
        
    }
}