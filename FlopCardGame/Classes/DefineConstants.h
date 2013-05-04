//
//  DefineConstants.h
//  FlopCardGame
//
//  Created by HyeonGyu Yu on 13. 5. 3..
//
//

#ifndef FlopCardGame_DefineConstants_h
#define FlopCardGame_DefineConstants_h

#define CARD_WIDTH              70.0f
#define CARD_HEIGHT             70.0f
#define GAP                     30.0f
#define FLIP_DELAY              0.6f
#define TICK(__X__)             CCDelayTime::create((float)__X__)
#define PTM_RATIO               32
#define CARD_BACK_IMAGE         "card_back.png"

#define INIT_CARD_PAIR_COUNT    7
#define ADD_CARD_PAIR_COUNT     1
#define ADD_CARD_CYCLE_SECONDS  10

#endif
