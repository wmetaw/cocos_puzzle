//
//  BlockSprite.cpp
//  NyanGame
//
//  Created by RYO on 2014/01/04.
//
//

#include "BlockSprite.h"

BlockSprite::BlockSprite()
{
    initNextPos();
}

BlockSprite::~BlockSprite()
{
    
}

/*
 ブロックの生成
 
 @param blockType ブロックタイプ
*/
BlockSprite* BlockSprite::createWithBlockType(kBlock blockType)
{
    BlockSprite *pRet = new BlockSprite();
    
    if(pRet && pRet->initWithBlockType(blockType)) {
        
        pRet->autorelease();
        return pRet;
    
    } else {
        
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

/*
* ブロックの初期化
*
* blockType ブロックタイプ
*/
bool BlockSprite::initWithBlockType(kBlock blockType){
    
    // 画像のファイルが存在するか
    if(!CCSprite::initWithFile(getBlockImageFileName(blockType))) {
        return false;
    }
    
    m_blockType = blockType;
    
    return true;
}

// ファイル名を取得
const char* BlockSprite::getBlockImageFileName(kBlock blockType){
    
    switch (blockType) {
            
        case kBlockRed:
            return "red.png";
            break;
            
        case kBlockBlue:
            return "blue.png";
            break;
            
        case kBlockYellow:
            return "yellow.png";
            break;
            
        case kBlockGreen:
            return "green.png";
            break;
            
        case kBlockGray:
            return "gray.png";
            break;
            
        default:
            CCAssert(false, "invalid blockType");
            break;
    }
}


/*
 * 移動先の初期化
 */
void BlockSprite::initNextPos()
{
    m_next_PosX = -1;
    m_next_PosY = -1;
}

/*
 * 移動先インデックスをセット
 */
void BlockSprite::setNextPos(int next_posX, int next_PosY)
{
    m_next_PosX = next_posX;
    m_next_PosY = next_PosY;
}










