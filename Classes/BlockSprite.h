//
//  BlockSprite.h
//  NyanGame
//
//  Created by RYO on 2014/01/04.
//
//

#ifndef __NyanGame__BlockSprite__
#define __NyanGame__BlockSprite__

#include "cocos2d.h"
#include "Config.h"

class BlockSprite : public cocos2d::CCSprite
{
protected:
    const char* getBlockImageFileName(kBlock blockType);
    
public:
    
    // 変数宣言と同時にアクセサを作成してくれる
    // protected メンバとして m_blockType が定義され
    // public    メンバとして virtual kBlock getBlockType(void) const {return m_blockType; } が作られる
    CC_SYNTHESIZE_READONLY(kBlock, m_blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, m_next_PosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_next_PosY, NextPosY);
    
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);
    
    BlockSprite();
    virtual ~BlockSprite();
    virtual bool initWithBlockType(kBlock blockType);
    static BlockSprite* createWithBlockType(kBlock blockType);

};

#endif /* defined(__NyanGame__BlockSprite__) */
