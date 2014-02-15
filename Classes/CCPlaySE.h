#ifndef NyanGame_CCPlaySE_h
#define NyanGame_CCPlaySE_h

#include "cocos2d.h"

class CCPlaySE : public cocos2d::CCActionInstant
{
public:
    CCPlaySE(std::string sound);
    virtual ~CCPlaySE(){}
    
    virtual void update(float time);
    virtual CCFiniteTimeAction* reverse(void);
    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
    
public:
    // 効果音の作成関数
    static CCPlaySE* create(std::string sound);
    
protected:
    std::string m_sound;
    
};

#endif