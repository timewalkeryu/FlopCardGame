//
//  PhysicsSprite.h
//  FlopCardGame
//
//  Created by HyeonGyu Yu on 13. 5. 1..
//
//

#ifndef __FlopCardGame__PhysicsSprite__
#define __FlopCardGame__PhysicsSprite__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D.h"

#define PTM_RATIO 32

using namespace cocos2d;
using namespace cocos2d::extension;

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    b2Body* getPhysicsBody();
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

#endif /* defined(__FlopCardGame__PhysicsSprite__) */
