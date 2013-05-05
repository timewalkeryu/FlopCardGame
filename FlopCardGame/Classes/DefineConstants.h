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

#define CARD_TYPE_COUNT         4   // 생성되는 카드 종류의 개수
#define INIT_CARD_PAIR_COUNT    6   // 초기에 생성하는 카드 쌍 개수
#define ADD_CARD_PAIR_COUNT     1   // 일정 주기마다 추가되는 카드 쌍 개수
#define ADD_CARD_CYCLE_SECONDS  10  // 카드가 생성되는 주기 (초)

#define TOUCH_SOUND_EFFECT      "button.mp3"
#define MATCH_SOUND_EFFECT      "Ting-Popup_Pixels-349896185.mp3"
#define BGM_SOUND               "bgm.mp3"

#endif
