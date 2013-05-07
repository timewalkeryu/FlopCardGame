//
//  GameScene.cpp
//  CardTest1
//
//  Created by HyeonGyu Yu on 13. 4. 27..
//
//

#include "StartScene.h"
#include "GameScene.h"

CCScene* Start::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Start *layer = Start::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Start::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *background = CCSprite::create("intro.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(background);
    
    CCControlButton *bt = CCControlButton::create(CCScale9Sprite::create("transparentButton.png"));
    bt->setPosition(ccp(winSize.width/2, winSize.height/2));
    bt->setPreferredSize(CCSizeMake(winSize.width, winSize.height));
    bt->addTargetWithActionForControlEvents(this, cccontrol_selector(Start::touchImage), CCControlEventTouchDown);
    this->addChild(bt);
    
    CCSprite *startButton = CCSprite::create("btn_start.png");
    startButton->setPosition(ccp(winSize.width/2, winSize.height/2 - 100.0f));
    startButton->setOpacity(0.0f);
    this->addChild(startButton);

    
    CCSequence *sq = CCSequence::create(CCEaseOut::create(CCFadeIn::create(1.2f), 0.8f), CCEaseIn::create(CCFadeOut::create(1.2f), 0.8f), NULL);
    ;
        startButton->runAction(CCRepeatForever::create(sq));
    
    
    return true;
}

void Start::touchImage(CCNode *pSender) {
    CCScene *pScene = Game::scene();
    
    CCDirector::sharedDirector()->setDepthTest(false);
	CCTransitionScene *transition = CCTransitionFadeTR::create(1.0f, pScene);
	
	CCDirector::sharedDirector()->replaceScene(transition);
}