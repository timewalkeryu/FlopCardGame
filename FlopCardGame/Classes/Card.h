//
//  Card.h
//  CardTest1
//
//  Created by HyeonGyu Yu on 13. 4. 27..
//
//

#ifndef __CardTest1__Card__
#define __CardTest1__Card__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "Box2D.h"
#include "PhysicsSprite.h"
#include "DefineConstants.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

class Card : public cocos2d::CCObject {
    bool isRevealed;
    int number;
    int index;
    PhysicsSprite *parentSprite;
    CCSprite *childSprite;
    
public:
    Card(int index, int number, const char *backImageName, const char *frontImageName, CCPoint location, b2World *world);
    bool getReveal();
    void setReveal(bool revealed);
    int getIndex();
    int getNumber();
    PhysicsSprite* getParentSprite();
    CCSprite* getChildSprite();
    
    void removeCard(b2World *world);
};

#endif /* defined(__CardTest1__Card__) */
