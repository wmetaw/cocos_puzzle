//
//  CCPlaySE.cpp
//  NyanGame
//
//  Created by RYO on 2014/02/06.
//
//

#include "CCPlaySE.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCPlaySE::CCPlaySE(std::string sound)
{
    m_sound = sound;
}

CCPlaySE* CCPlaySE::create(std::string sound)
{
    CCPlaySE* pRet = new CCPlaySE(sound);
    if(pRet) {
        pRet->autorelease();
    }
    
    return pRet;
}

// soundの再生
void CCPlaySE::update(float time)
{
    CC_UNUSED_PARAM(time);
    
    // m_sound.c_str() はstd::string の文字列を吐き出す関数
    SimpleAudioEngine::sharedEngine()->playEffect(m_sound.c_str());
}

CCFiniteTimeAction* CCPlaySE::reverse()
{
    return (CCFiniteTimeAction*)(CCPlaySE::create(m_sound));
}

CCObject* CCPlaySE::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    CCPlaySE* pRet = NULL;
    if(pZone && pZone->m_pCopyObject) {
        pRet = (CCPlaySE*)(pZone->m_pCopyObject);
    } else {
        pRet = new CCPlaySE(m_sound);
        pZone = pNewZone = new CCZone(pRet);
    }
    
    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    
    return pRet;
}






