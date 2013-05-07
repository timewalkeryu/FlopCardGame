//
//  GameScene.cpp
//  CardTest1
//
//  Created by HyeonGyu Yu on 13. 4. 27..
//
//

#include "GameScene.h"

Game::Game()
{
    setTouchEnabled(true);
    setAccelerometerEnabled(true);
    
    // init physics
    this->initPhysics();
}

Game::~Game()
{
}

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);

    this->touchValue = true;
    this->cardDictionary = new CCDictionary();
    this->checkNumberArray = new CCArray();
    this->checkCardArray = new CCArray();
    cardDictionaryIndex = 0;
    gameTime = 0.0f;
    addCardsTime = 0.0f;
    isEnded = false;
    lastSeconds = 0.0f;
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *background = CCSprite::create("bg.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    this->addChild(background);
    
    CCLayer *cardTable = CCLayer::create();
    cardTable->setPosition(ccp(CARD_WIDTH / 2 + 10.0f, CARD_HEIGHT / 2 + 10.0f));
    this->addChild(cardTable);
    
    for (int i = 0; i < INIT_CARD_PAIR_COUNT; i++) {
        char *cardFrontImage = new char[100];
        int ra = arc4random() % 4 + 1;
        sprintf(cardFrontImage, "card_front%02d.png", ra, NULL);
        ++cardDictionaryIndex;
        this->cardDictionary->setObject(new Card(cardDictionaryIndex, ra, CARD_BACK_IMAGE, cardFrontImage, ccp(winSize.width/2.0f, winSize.height*0.7f), world), cardDictionaryIndex);
        ++cardDictionaryIndex;
        this->cardDictionary->setObject(new Card(cardDictionaryIndex, ra, CARD_BACK_IMAGE, cardFrontImage, ccp(winSize.width/2.0f, winSize.height*0.7f), world), cardDictionaryIndex);
        delete cardFrontImage;
    }

    // 카드에 버튼 추가 ("배열 인덱스 + 1"의 값을 가짐)
    for (int i = 0; i < this->cardDictionary->count(); i++) {
        ((Card *)this->cardDictionary->objectForKey(i + 1))->getParentSprite()->addChild(createButton(i + 1));
        this->addChild(((Card *)this->cardDictionary->objectForKey(i + 1))->getParentSprite());
    }

    
    this->pMatchBatchNode = CCParticleBatchNode::create("matchParticle.png");
    this->pMatchBatchNode->setPosition(ccp(0.0f,0.0f));
    this->pMatchBatchNode->setContentSize(winSize);
    this->pMatchBatchNode->setAnchorPoint(CCPointZero);
    this->addChild(pMatchBatchNode);
    
    this->pMatchSystemQuad = CCParticleSystemQuad::create("matchParticle.plist");
    this->pMatchSystemQuad->setPosition(ccp(-1000, -1000));
    this->addChild(this->pMatchSystemQuad);
    
    this->pMatchSystemQuad2 = CCParticleSystemQuad::create("matchParticle.plist");
    this->pMatchSystemQuad2->setPosition(ccp(-1000, -1000));
    this->addChild(this->pMatchSystemQuad2);
    
    this->pEndBatchNode = CCParticleBatchNode::create("gameEndParticle.png");
    this->pEndBatchNode->setPosition(ccp(0.0f,0.0f));
    this->pEndBatchNode->setContentSize(winSize);
    this->pEndBatchNode->setAnchorPoint(CCPointZero);
    this->addChild(pEndBatchNode);
    
    drawTimeText();
    drawPauseButton();

    scheduleUpdate();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.01f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(TOUCH_SOUND_EFFECT);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.01f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MATCH_SOUND_EFFECT);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(BGM_SOUND, true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.2f);    
    
    return true;
}

void Game::drawPauseButton() {
    pauseButton = CCControlButton::create(CCScale9Sprite::create("btn_stop.png"));
    pauseButton->setPosition(ccp(winSize.width - 50.0f, winSize.height - 50.0f));
    pauseButton->setAnchorPoint(ccp(0.5f, 0.5f));
    pauseButton->setPreferredSize(CCSize(64, 64));
    pauseButton->setZoomOnTouchDown(true);
    pauseButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Game::pauseButtonTouchUpHandler), CCControlEventTouchDown);
    this->addChild(pauseButton);
}

void Game::showPopup() {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    popup = CCSprite::create("popup.png");
    popup->setPosition(ccp(winSize.width/2, winSize.height/2));
    CCMenuItemImage *resumeButton = CCMenuItemImage::create("btn_play_n.png", "btn_play_f.png", this,  menu_selector(Game::resumeButtonTouchUpHandler));
    resumeButton->setPosition(ccp(winSize.width/2 - 40.0f, winSize.height/2 + 4.0f));
    
    CCMenuItemImage *restartButton = CCMenuItemImage::create("btn_restart_n.png", "btn_restart_f.png", this,  menu_selector(Game::restartButtonTouchUpHandler));
    restartButton->setPosition(ccp(winSize.width/2 + 40.0f, winSize.height/2 + 4.0f));
    
    ccColor4B color;
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 200.0f;
    
    lc = CCLayerColor::create(color, winSize.width, winSize.height);
    lc->setAnchorPoint(CCPointZero);
    lc->setPosition(CCPointZero);
    
    CCMenu *menu = CCMenu::create(resumeButton, restartButton, NULL);
    menu->setPosition(CCPointZero);
    lc->addChild(popup);
    lc->addChild(menu);
    this->addChild(lc);
}

void Game::hidePopup() {
    this->removeChild(lc);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void Game::pauseButtonTouchUpHandler(CCObject *pSender) {
    CCLog("resumeButtonTouchUpHandler", NULL);
    pauseButton->setTouchEnabled(false);
    showPopup();
    CCDirector::sharedDirector()->pause();
}

void Game::resumeButtonTouchUpHandler(CCObject *pSender) {
    CCLog("resumeButtonTouchUpHandler", NULL);
    pauseButton->setTouchEnabled(true);
    hidePopup();
    CCDirector::sharedDirector()->resume();
}

void Game::restartButtonTouchUpHandler(CCObject *pSender) {
    CCLog("restartButtonTouchUpHandler", NULL);
    CCDirector::sharedDirector()->resume();
	
	CCScene *pScene = Game::scene();
	
	CCDirector::sharedDirector()->setDepthTest(false);
	CCTransitionFade * transition = CCTransitionFade::create(2.0f, pScene);
	CCDirector::sharedDirector()->replaceScene(transition);

}

void Game::drawTimeText() {
    timeText = CCLabelBMFont::create("00", "ScoreFont.fnt");
    timeText->setPosition(ccp(winSize.width / 2 ,winSize.height - 50.0f));
    this->addChild(timeText);
}

void Game::addFourCards() {
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    for (int i = 0 ; i < ADD_CARD_PAIR_COUNT; i++) {
        char *cardFrontImage = new char[100];
        int ra = arc4random() % CARD_TYPE_COUNT + 1;
        sprintf(cardFrontImage, "card_front%02d.png", ra, NULL);
        
        ++cardDictionaryIndex;
        int a = arc4random() % (int)size.width;
        int b = arc4random() % ((int)(size.height / 3));
        CCLog("%d %d", a, b, NULL);
        Card *cd1 = new Card(cardDictionaryIndex, ra, CARD_BACK_IMAGE, cardFrontImage, ccp(a, b), world);
        
        this->cardDictionary->setObject(cd1, cardDictionaryIndex);
        
        cd1->getParentSprite()->addChild(createButton(cardDictionaryIndex));
        this->addChild(cd1->getParentSprite());
        
        ++cardDictionaryIndex;
        a = arc4random() % (int)size.width;
        b = arc4random() % (int)size.height;
        CCLog("%d %d", a, b, NULL);
        Card *cd2 = new Card(cardDictionaryIndex, ra, CARD_BACK_IMAGE, cardFrontImage, ccp(a, b), world);
        this->cardDictionary->setObject(cd2, cardDictionaryIndex);
        delete cardFrontImage;
        
        cd2->getParentSprite()->addChild(createButton(cardDictionaryIndex));
        this->addChild(cd2->getParentSprite());
    }
    
}

CCControlButton* Game::createButton(int tagNumber) {
    CCControlButton *button = CCControlButton::create(CCScale9Sprite::create("transparentButton.png"));
    button->setPosition(ccp(0.0f, 0.0f));
    button->setAnchorPoint(ccp(0, 0));
    button->setPreferredSize(CCSize(CARD_WIDTH, CARD_HEIGHT));
    button->setTag(tagNumber);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(Game::menuTouchDownHandler), CCControlEventTouchDown);
    return button;
}

bool Game::checkPreCondition(CCObject *pSender) {
    int tagValue = ((CCControlButton *)pSender)->getTag();
    Card *pCard = ((Card *)cardDictionary->objectForKey(tagValue));
    if (pCard->getReveal()) {
        return false;
    } else {
        return true;
    }
}

void Game::menuTouchDownHandler(CCObject *pSender) {
    
    // 터치 제스쳐 가능 여부 변수 체크
    if (!this->touchValue) {
        return;
    }
    
    // 클릭한 카드가 이미 앞면이면 무시함
    if (!checkPreCondition(pSender)) {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(TOUCH_SOUND_EFFECT);
    
    // 게임 규칙 연산 (현재 클릭된 카드를 인자로 넘김)
    this->checkCard(pSender);
}

bool Game::checkCard(CCObject *pSender) {
    // 누른 버튼의 태그를 기반으로 카드 배열에서 찾아옴
    int tagValue = ((CCMenuItemImage *)pSender)->getTag();
    Card *pCard = ((Card *)cardDictionary->objectForKey(tagValue));
    int cardNumber = pCard->getNumber();
    
    // 카드를 배열에 넣음
    this->checkNumberArray->addObject(new CCInteger(cardNumber));
    this->checkCardArray->addObject(pCard);
    
    // 카드를 앞면으로 설정
    pCard->setReveal(true);
    
    // 체크 카드 배열이 한 장일 경우
    if (checkNumberArray->count() == 1) {
        // 카드 앞면으로 뒤집는 애니메이션 실행
        CCFiniteTimeAction *action = CCCallFuncND::create(this, callfuncND_selector(Game::flip), pCard->getChildSprite());
        pCard->getParentSprite()->runAction(action);
    }
    
    // 체크 카드 배열이 두 장일 경우
    else if (checkNumberArray->count() == 2) {
        // 우선 터치 불가능하게 설정
        this->setTouchDisable();
        
        // 체크할 카드의 종류 비교 (동일한 카드 넘버인지)
        CCInteger *valueOne = (CCInteger *)checkNumberArray->objectAtIndex(0);
        CCInteger *valueTwo = (CCInteger *)checkNumberArray->objectAtIndex(1);
        
        Card *pCard1 = (Card *)checkCardArray->objectAtIndex(0);
        Card *pCard2 = (Card *)checkCardArray->objectAtIndex(1);
        
        CCFiniteTimeAction *action = CCCallFuncND::create(this, callfuncND_selector(Game::flip), pCard->getChildSprite());
        pCard->getParentSprite()->runAction(action);
        
        // 카드가 서로 같을 경우
        if (valueOne->getValue() == valueTwo->getValue()) {
            // 나머지 한장의 카드도 뒤집고, 일정 딜레이 후 카드 제거
            this->flipCardAndRemoveTwoCards(pSender);
        }
        // 카드가 서로 같지 않을 경우
        else {
            this->restoreCards(pCard1, pCard2);
        }
    }
    
    return false;
    
}

void Game::flipCardAndRemoveTwoCards(CCObject *pSender) {
    // 누른 버튼의 태그를 기반으로 카드 배열에서 찾아옴
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(Game::removeCards));
    this->runAction(CCSequence::create(TICK(FLIP_DELAY * 2), cf, NULL));
}

void Game::removeCards() {
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MATCH_SOUND_EFFECT);
    
    Card *pCard1 = (Card *)checkCardArray->objectAtIndex(0);
    Card *pCard2 = (Card *)checkCardArray->objectAtIndex(1);
    
    this->cardDictionary->removeObjectForKey(pCard1->getIndex());
    this->cardDictionary->removeObjectForKey(pCard2->getIndex());
    
    b2Vec2 p1 = pCard1->getParentSprite()->getPhysicsBody()->GetPosition();
    b2Vec2 p2 = pCard2->getParentSprite()->getPhysicsBody()->GetPosition();
    
    this->showMatchParticle(ccp(p1.x * PTM_RATIO, p1.y * PTM_RATIO), ccp(p2.x * PTM_RATIO, p2.y * PTM_RATIO));
    
    pCard1->removeCard(world);
    pCard2->removeCard(world);
    
    this->setTouchEnable();
    
    // 체크할 카드의 배열을 초기화
    this->checkNumberArray->removeAllObjects();
    this->checkCardArray->removeAllObjects();
    
    // 카드 배열이 비워졌을 경우, 즉 끝나는 조건
    if (this->cardDictionary->count() == 0) {
        CCLog("End", NULL);
        isEnded = true;
        CCSequence *squence = CCSequence::create(TICK(FLIP_DELAY), CCCallFunc::create(this, callfunc_selector(Game::showGameEndParticle)), NULL);
        this->runAction(squence);
    }
}

void Game::restoreCards(Card* pCard1, Card* pCard2) {
    pCard1->setReveal(false);
    pCard2->setReveal(false);
    
    CCFiniteTimeAction *action1 = CCCallFuncND::create(this, callfuncND_selector(Game::reverseFlip), pCard1->getChildSprite());
    CCFiniteTimeAction *action2 = CCCallFuncND::create(this, callfuncND_selector(Game::reverseFlip), pCard2->getChildSprite());
    
    CCSequence *squence1 = CCSequence::create(TICK(FLIP_DELAY * 2), action1, NULL);
//    CCSequence *squence2 = CCSequence::create(TICK(FLIP_DELAY * 2), action2, NULL);
    CCSequence *squence2 = CCSequence::create(TICK(FLIP_DELAY * 2), action2, CCCallFunc::create(this, callfunc_selector(Game::setTouchEnable)), NULL);
    
    setTouchEnable();
    
    pCard1->getParentSprite()->runAction(squence1);
    pCard2->getParentSprite()->runAction(squence2);
    
    this->checkCardArray->removeAllObjects();
    this->checkNumberArray->removeAllObjects();
}

void Game::update(float dt)
{
    gameTime += dt;
    addCardsTime += dt;
    
    if (gameTime - lastSeconds > 1.0f && !isEnded) {
        char *timeString = new char[25];
        sprintf(timeString, "%02d", (int)gameTime, NULL);
        timeText->setString(timeString);
        lastSeconds = gameTime;
        delete timeString;
    }
    
    if (addCardsTime > ADD_CARD_CYCLE_SECONDS && !isEnded) {
        addFourCards();
        addCardsTime = 0.0f;
    }
    
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
    }
}

void Game::changeFace(CCNode* sender, void *value) {
    ((CCSprite *)value)->setOpacity( ((CCSprite *)value)->getOpacity() == 255.0f ? 0.0f : 255.0f );
}

void Game::flip(CCNode *sender, void *value) {
    CCActionInterval *firstAction = CCOrbitCamera::create(FLIP_DELAY / 2, 1.0f, 0, 0.0f, 90.0f, 0, 0);
    CCActionInterval *secondAction = CCOrbitCamera::create(FLIP_DELAY / 2, 1.0f, 0, 270.0f, 90.0f, 0, 0);
    
    CCScaleTo *sb1 = CCScaleTo::create(FLIP_DELAY / 2, 0.9f);
    CCScaleTo *sb2 = CCScaleTo::create(FLIP_DELAY / 2, 1.0f);
    
    CCSequence *action = CCSequence::create(
                                            CCSpawn::create(CCEaseIn::create(firstAction, 2.0f), CCEaseIn::create(sb1, 2.0f), NULL),
                                            CCCallFuncND::create(this, callfuncND_selector(Game::changeFace), value),
                                            CCSpawn::create(CCEaseOut::create(sb2, 2.0f), CCEaseOut::create(secondAction, 2.0f), NULL),
                                            NULL);
    
    sender->runAction(action);
    
    CCLog("%f, %f", sender->getPosition().x, sender->getPosition().y, NULL);
    
}

void Game::reverseFlip(CCNode *sender, void *value) {
    // 인자 순서대로 : 거리, 목표 거리, 방위각, 목표 방위각, 목표 위도, 위도
    CCActionInterval *firstAction = CCOrbitCamera::create(FLIP_DELAY / 2, 1.0f, 0, 0.0f, 90.0f, 0, 0);
    CCActionInterval *secondAction = CCOrbitCamera::create(FLIP_DELAY / 2, 1.0f, 0, 270.0f, 90.0f, 0, 0);
    
    CCScaleTo *sb1 = CCScaleTo::create(FLIP_DELAY / 2, 0.95f);
    CCScaleTo *sb2 = CCScaleTo::create(FLIP_DELAY / 2, 1.0f);
    
    CCSequence *action = CCSequence::create(
                                            CCSpawn::create(CCEaseIn::create(secondAction->reverse(), 2.0f), CCEaseIn::create(sb1, 2.0f), NULL),
                                            CCCallFuncND::create(this, callfuncND_selector(Game::changeFace), value),
                                            CCSpawn::create(CCEaseOut::create(firstAction->reverse(), 2.0f), CCEaseOut::create(sb2, 2.0f), NULL),
                                            NULL);
    
    sender->runAction(action);
}

void Game::setTouchEnable() {
    this->touchValue = true;
}

void Game::setTouchDisable() {
    
    if(checkNumberArray->count() < 2)
        return;
    
    this->touchValue = false;
}

void Game::showMatchParticle(CCPoint point, CCPoint point2) {

    this->pMatchSystemQuad->setPosition(ccp(point.x,point.y));
    this->pMatchSystemQuad->resetSystem();
    
    this->pMatchSystemQuad2->setPosition(ccp(point2.x,point2.y));
    this->pMatchSystemQuad2->resetSystem();
}

void Game::showGameEndParticle() {
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLog("%f", size.width);
    this->pEndSystemQuad = CCParticleSystemQuad::create("gameEndParticle.plist");
    this->pEndSystemQuad->setAutoRemoveOnFinish(true);
    this->pEndBatchNode->addChild(pEndSystemQuad);
    this->pEndSystemQuad->setPosition(ccp(size.width / 2, size.height / 2));
}

void Game::initPhysics()
{
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);
    
    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);
    
    world->SetContinuousPhysics(true);
    
    //     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //     world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);
    
    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    
    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    // bottom
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    
    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);
    
    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}