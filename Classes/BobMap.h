//
//  BobMap.h
//  LearnGA
//
//  Created by DONG on 15/5/13.
//
//

#ifndef __LearnGA__BobMap__
#define __LearnGA__BobMap__

#include "cocos2d.h"
USING_NS_CC;

const int MAP_WIDTH = 15;
const int MAP_HEIGHT = 10;

class BobMap : public Node
{
public:
    static int s_map[MAP_HEIGHT][MAP_WIDTH];
    CREATE_FUNC(BobMap);
    bool init();
    float testRoute(const std::vector<int>& path);
    
private:
    void drawMap();
    bool canMove(int x, int y);
    
private:
    int m_startX, m_startY;
    int m_endX, m_endY;
    DrawNode* m_map;
    DrawNode* m_bob;
};

#endif /* defined(__LearnGA__BobMap__) */
