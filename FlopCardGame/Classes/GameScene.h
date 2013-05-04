//
//  GameScene.h
//  CardTest1
//
//  Created by HyeonGyu Yu on 13. 4. 27..
//
//

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "Box2D.h"
#include "PhysicsSprite.h"
#include "DefineConstants.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;




class Game : public CCLayer
{
    bool isEnded;
    bool touchValue;
    int cardDictionaryIndex;
    float gameTime;
    float addCardsTime;
    
    CCDictionary* cardDictionary;
    CCArray* checkNumberArray;
    CCArray* checkCardArray;
    
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    CCParticleBatchNode *pMatchBatchNode;
    CCParticleSystemQuad *pMatchSystemQuad;
    CCParticleSystemQuad *pMatchSystemQuad2;
    
    CCParticleBatchNode *pEndBatchNode;
    CCParticleSystemQuad *pEndSystemQuad;
    
    CCLabelTTF *timeText;
    
public:
    ~Game();
    Game();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Game);
    
    void flip(CCNode* sender, void* value);
    void changeFace(CCNode* sender, void* value);
    void reverseFlip(CCNode* sender, void* value);
    
    bool checkPreCondition(CCObject *pSender);
    bool checkCard(CCObject *pSender);
    void restoreCards(Card* pCard1, Card* pCard2);
    
    void menuTouchUpHandler(CCObject *pSender);
    void menuTouchDownHandler(CCObject *pSender);
    
    void setTouchEnable();
    void setTouchDisable();
    
    CCControlButton* createButton(int tagNumber);

    // #######################
    
    void update(float dt);
    void initPhysics();
    void removeCards();
    void showMatchParticle(CCPoint point, CCPoint point2);
    void showGameEndParticle();
    void flipCardAndRemoveTwoCards(CCObject *pSender);
    void addFourCards();
    void drawTimeText();
};

#endif // __GAME_SCENE_H__
