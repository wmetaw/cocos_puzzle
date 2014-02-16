//
//  GameScene.h
//  NyanGame
//
//  Created by RYO on 2014/01/03.
//
//

#ifndef __NyanGame__GameScene__
#define __NyanGame__GameScene__

#include "cocos2d.h"
#include "Config.h"
#include "CCPlaySE.h"

#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8
#define REMOVE_TIME 0.1f
#define MP3_REMOVE_BLOCK "removeBlock.mp3"
#define PNG_BACKGROUND "background.png"
#define MOVING_TIME 0.2f


class GameScene : public cocos2d::CCLayer
{
protected:
    
    enum kTag {
        kTagBackground = 1,
        kTagBaseBlock = 10000,
    };
    
    enum kZOrder {
        kZOrderBackGround,
        kZOrderBlock,
    };
    
    struct PositionIndex
    {
        PositionIndex(int x1, int y1) {
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    // 2-2-3
    cocos2d::CCSprite* m_background;
    void showBackground();
    
    // 2-2-4
    float m_blockSize; // コマ一辺のサイズ
    
    // mapは連想配列 listは配列
    std::map<kBlock, std::list<int> >m_blockTags; // コマの種類ごとのグループ
    void initForVariables();
    void showBlock();
    cocos2d::CCPoint getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    
    // 2-2-5
    void getTouchBlockTag(cocos2d::CCPoint touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTag(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlack(std::list<int>blockTagList, int searchBlockTag);
    
    // 2-3-1
    void removingBlock(cocos2d::CCNode* block);
    
    // 2-3-2
    std::vector<kBlock> blockTypes;
    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveBlock();
    void movingBlockAnimation1(std::list<int> blocks);
    
    // 2-3-3
    bool m_animation;
    void movedBlocks();
    std::map<int, bool> getExistsBlockColum();
    void searchNewPosition2();
    void setNewPosition2(int tag, PositionIndex posIndex);
    void movingBlocksAnimation2();
    
public:
    // サウンド
    CCPlaySE* m_del_se;
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent); // タッチ開始
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent); // タッチエンド
};

#endif /* defined(__NyanGame__GameScene__) */









