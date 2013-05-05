//
//  FlopCardGameAppDelegate.cpp
//  FlopCardGame
//
//  Created by HyeonGyu Yu on 13. 5. 1..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "StartScene.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    
    std::vector<std::string> searchPaths;
    std::vector<std::string> resDirOrders;
    
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    
    if (platform == kTargetIphone || platform == kTargetIpad){
        searchPaths.push_back("Published-iOS"); // Resources/Published-iOS
        if (screenSize.height > 480){
            // Retaina 의한 폭 조정
            resDirOrders.push_back("resources-iphonehd");  //Resources/Published-iOS/resources-iphonehd
            pDirector->setContentScaleFactor(2.0f); // 2배 규모 크기
            if (screenSize.width==1136.0 || screenSize.height==1136.0 ) {
                // 4 인치
                CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 568, kResolutionShowAll);
            } else {
                // 3.5 인치
                CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionShowAll);
            }
        } else {
            // 보통 크기
            resDirOrders.push_back("resources-iphone");    //Resources/Published-iOS/resources-iphone
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionShowAll);
        }
        
    } else {
        // iOS以外（Androidなど)
        searchPaths.push_back("Published-iOS"); // Resources/Published-iOS
        resDirOrders.push_back("resources-iphonehd");
        pDirector->setContentScaleFactor(2.0f);
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionExactFit);
        
    }
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
    
    // create a scene. it's an autorelease object
    CCScene *pScene = Start::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
