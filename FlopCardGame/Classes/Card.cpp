//
//  Card.cpp
//  CardTest1
//
//  Created by HyeonGyu Yu on 13. 4. 27..
//
//

#include "Card.h"

Card::Card(int index, int number, const char *backImageName, const char *frontImageName, CCPoint location, b2World *world) {
    this->index = index;
    this->number = number;
    this->isRevealed = false;
    
    PhysicsSprite *backImage = new PhysicsSprite();
    backImage->initWithTexture(CCTextureCache::sharedTextureCache()->addImage(backImageName), CCRectMake(0, 0, CARD_WIDTH, CARD_HEIGHT));
    backImage->setAnchorPoint(ccp(0.5f, 0.5f));
    backImage->setPosition(location);
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    b2Vec2 *vcs = new b2Vec2[4];
    vcs[0] = b2Vec2(-CARD_WIDTH/2.0f/PTM_RATIO, -CARD_HEIGHT/2.0f/PTM_RATIO);
    vcs[1] = b2Vec2(CARD_WIDTH/2.0f/PTM_RATIO, -CARD_HEIGHT/2.0f/PTM_RATIO);
    vcs[2] = b2Vec2(CARD_WIDTH/2.0f/PTM_RATIO, CARD_HEIGHT/2.0f/PTM_RATIO);
    vcs[3] = b2Vec2(-CARD_WIDTH/2.0f/PTM_RATIO, CARD_HEIGHT/2.0f/PTM_RATIO);
    
    dynamicBox.Set(vcs, 4);
    
    //dynamicBox.SetAsBox(1.0f, 1.4f, b2Vec2(0.5f, 0.5f), arc4random()%360);
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.1f;

    body->CreateFixture(&fixtureDef);

    backImage->setPhysicsBody(body);
    
    body->SetAngularVelocity((float)(arc4random() % 360));
    parentSprite = backImage;
    
    CCSprite *frontImage = CCSprite::create(frontImageName);
    frontImage->setPosition(CCPointZero);
    frontImage->setOpacity(0.0f);
    frontImage->setAnchorPoint(CCPointZero);
    parentSprite->addChild(frontImage);
    childSprite = frontImage;
    
}

void Card::removeCard(b2World *world) {
    world->DestroyBody(this->getParentSprite()->getPhysicsBody());
    
    this->getParentSprite()->removeFromParentAndCleanup(true);
}

bool Card::getReveal() {
    return this->isRevealed;
}

void Card::setReveal(bool revealed) {
    this->isRevealed = revealed;
}

int Card::getIndex() {
    return this->index;
}

int Card::getNumber() {
    return this->number;
}

PhysicsSprite* Card::getParentSprite() {
    return this->parentSprite;
}

CCSprite* Card::getChildSprite() {
    return this->childSprite;
}
