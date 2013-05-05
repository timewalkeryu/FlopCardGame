//
//  StartScene.h
//  FlopCardGame
//
//  Created by HyeonGyu Yu on 13. 5. 5..
//
//

#ifndef __FlopCardGame__StartScene__
#define __FlopCardGame__StartScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "DefineConstants.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

class Start : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Start);
    
    void touchImage(CCNode *pSender);
};


#endif /* defined(__FlopCardGame__StartScene__) */
