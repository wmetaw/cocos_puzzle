//
//  GameScene.cpp
//  NyanGame
//
//  Created by RYO on 2014/01/03.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"


USING_NS_CC;
using namespace CocosDenshion;
using namespace std;


CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

// 初期化
bool GameScene::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    // タップイベントの取得
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne); // kCCTouchesOneByOne シングルタップのイベント
    
    // 変数初期化
    initForVariables();
    
    // 背景表示
    showBackground();
    
    // コマを表示
    showBlock();
    
    // ラベル作成
    showLabel();
    showHighScore();
    
    // リセットボタン作成
    showResetButton();
    
    // 効果音の読み込み
    SimpleAudioEngine::sharedEngine()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}

// 背景表示
void GameScene::showBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 背景を生成
    m_background = CCSprite::create(PNG_BACKGROUND);
    m_background->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(m_background, kZOrderBackGround, kTagBackground);
    
}

// 変数初期化
void GameScene::initForVariables()
{
    //乱数の初期化
    srand((unsigned)time(NULL));
    
    // コマ一辺の長さを取得
    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    m_blockSize = pBlock->getContentSize().height;
    
    // コマ種類の配列生成
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);
    
    // 変数初期化
    m_animation = false;
    m_score = 0;
}

// 新しい位置をセット
void GameScene::setNewPosition1(int tag, GameScene::PositionIndex posIndex)
{
    
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if (nextPosY) {
        nextPosY = posIndex.y;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(posIndex.x, --nextPosY);
}

// 消えたコマを埋めるように新しい位置をセット
void GameScene::searchNewPosition1(list<int> blocks)
{
    // 消えるコマ数分ループ
    list<int>::iterator it1 = blocks.begin();
    while (it1 != blocks.end())
    {
        
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        vector<kBlock>::iterator it2 = blockTypes.begin();
        
        while (it2 != blockTypes.end() )
        {
            
            // コマの種類分ループ
            list<int>::iterator it3 = m_blockTags[*it2].begin();
            while (it3 != m_blockTags[*it2].end())
            {
                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if(posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y) {
                    
                    // 消えるコマの上に位置するコマに対して、移動先の位置をセットする。
                    setNewPosition1(*it3, posIndex2);
                }
                it3++;
            }
            
            it2++;
        }
        
        it1++;
    }
}

// コマを移動する
void GameScene::moveBlock()
{
    // コマ種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類のコマ数分のループ
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end())
        {
            BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(*it2);
            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();
            
            if (nextPosX != -1 || nextPosY != -1)
            {
                // 新しいタグをセットする
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);
                
                // タグ一覧の値も新しいタグに変更する
                *it2 = newTag;
                
                // アニメーションをセットする
                CCMoveTo* move = CCMoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
            }
            
            it2++;
        }
        
        it1++;
    }
}

// コマ削除後のアニメーション
void GameScene::movingBlockAnimation1(list<int> blocks)
{
    // コマの新しい位置をセットする
    searchNewPosition1(blocks);
    
    // 新しい位置がセットされたアニメーション
    moveBlock();
    
    // アニメーション終了時に次のアニメーション処理を開始する
    scheduleOnce(schedule_selector(GameScene::movingBlocksAnimation2), MOVING_TIME);
    
}

// コマのインデックスを取得
GameScene::PositionIndex GameScene::getPositionIndex(int tag)
{

    int pos_x = (tag - kTagBaseBlock) / 100;
    int pos_y = (tag - kTagBaseBlock) % 100;
    
    return PositionIndex(pos_x, pos_y);
}


// 位置取得
CCPoint GameScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.168;
    float offsetY = m_background->getContentSize().height * 0.029;
    
    return CCPoint((posIndexX + 0.5) * m_blockSize + offsetX, (posIndexY + 0.5) * m_blockSize + offsetY);
}

// タグを取得
int GameScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

// コマの表示
void GameScene::showBlock()
{
    // 8 * 8のコマを作成する
    for (int x = 0; x < MAX_BLOCK_X; x++) {
        
        for (int y = 0; y<MAX_BLOCK_Y; y++) {
            
            //ランダムでコマを作成
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            //対応するコマ配列にタグを追加
            int tag = getTag(x, y);
            m_blockTags[blockType].push_back(tag);
            
            // コマを生成
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            m_background->addChild(pBlock, kZOrderBlock, tag);
            
        }
    }
}

// タップされたコマのタグを取得
void GameScene::getTouchBlockTag(CCPoint touchPoint, int &tag, kBlock &blockType)
{
    for (int x=0; x<MAX_BLOCK_X; x++) {
        for (int y=0; y<MAX_BLOCK_Y; y++) {
            
            // 基準値からタグを取得(位置も取得)
            int currentTag = getTag(x, y);
            
            // コマを取得
            CCNode* node = m_background->getChildByTag(currentTag);
            
            // コマが消えてなくて、タップ上に存在するか
            if(node && node->boundingBox().containsPoint(touchPoint)) {
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
                
                return;
            }
        }
    }
}

// コマ配列にあるか検索
bool GameScene::hasSameColorBlack(list<int> blockTagList, int searchBlockTag)
{
    list<int>::iterator it;
    for (it = blockTagList.begin(); it != blockTagList.end(); ++it) {
        
        if(*it == searchBlockTag) {
            
            return true;
        }
    }
    
    return false;
}

// タップされたコマと同色かつ隣同士のコマの配列を返す
list<int> GameScene::getSameColorBlockTag(int baseTag, kBlock blockType)
{
    // 同色のコマを格納する配列の初期化
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end()) {
        
        int tags[] = {
            *it + 100 , // right block tag
            *it - 100 , // left block tag
            *it + 1  , // up block tag
            *it - 1  , // down block tag
        };
        
        for (int i=0; i<sizeof(tags)/sizeof(tags[0]); i++) {
            
            // 既にリストにあるか検索
            if(!hasSameColorBlack(sameColorBlockTags, tags[i]))
            {
                // コマ配列にあるか検索
                if(hasSameColorBlack(m_blockTags[blockType], tags[i])) {
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        
        it++;
    }
    
    return sameColorBlockTags;
}

// 配列のコマを削除
void GameScene::removeBlock(list<int> blockTags, kBlock blockType)
{
    
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while (it != blockTags.end())
    {
        
        // 既存配列から該当コマを削除
        m_blockTags[blockType].remove(*it);
        
        // 対象となるコマを取得
        CCNode* block = m_background->getChildByTag(*it);
        if(block)
        {
            // コマが消えるアニメーションを生成
            CCScaleTo* scale = CCScaleTo::create(REMOVE_TIME, 0);
            
            // コマを削除するアクションを作成
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removingBlock));
            
            // アクションをつなげる
            CCFiniteTimeAction* sequence = CCSequence::create(scale, func, NULL);
            
            CCFiniteTimeAction* action;
            if(first) {

                // コマが消えるサウンドアクションを作成
                m_del_se = CCPlaySE::create(MP3_REMOVE_BLOCK);
                
                // アクションをつなげる
                action = CCSpawn::create(sequence, m_del_se, NULL);
                
                first = false;
            } else {
                action = sequence;
            }
            
            // アクションをセットする
            block->runAction(action);
            
        }
        
        it++;
    }
    
}

void GameScene::removingBlock(CCNode* block) {
    block->removeFromParentAndCleanup(true);
}


// タップ開始イベント
bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    
    // アニメーション中は処理を受け付けない
    return !m_animation;
}

// タップ終了イベント
void GameScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    // タップポイントの取得
    CCPoint touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    // タップしたコマのtagとコマの種類を取得
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    
    if(tag != 0)
    {
        // 隣接するコマを検索する
        list<int> sameColorBlockTags = getSameColorBlockTag(tag, blockType);
        
        if(sameColorBlockTags.size() > 1)
        {
            // 得点加算(消したコマ数 - 2 )の2乗
            m_score += pow(sameColorBlockTags.size() - 2 , 2);
            
            // アニメーション開始
            m_animation = true;
            
            // 隣接するコマを削除する
            removeBlock(sameColorBlockTags, blockType);
            
            // コマ削除後のアニメーション
            movingBlockAnimation1(sameColorBlockTags);
        }
    }
}

/*
 * 存在する列を取得する
 */
map<int , bool> GameScene::getExistsBlockColum()
{
    // 検索用配列初期化
    map<int, bool> xBlock;
    for (int  i=0; i<MAX_BLOCK_X; i++)
    {
        xBlock[i] = false;
    }
    
    // コマ種類分ループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類のコマ数分ループ
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end())
        {
            // 存在するコマのX位置を取得
            xBlock[getPositionIndex(*it2).x] = true;
            
            it2++;
        }
        
        it1++;
    }
    
    return xBlock;
}

/*
 * 消えたコマ列を埋めるように新しい位置をセット
 */
void GameScene::searchNewPosition2()
{
    // 存在する列を取得
    map<int, bool> xBlock = getExistsBlockColum();
    
    // コマが存在しないX位置を求める
    bool first = true;
    for (int i=MAX_BLOCK_X - 1 ; i>0; i--)
    {
        if(xBlock[i]) {
            
            // コマが存在する
            first = false;
            continue;
        
        } else {
            
            // コマが存在しない
            if(first) {
                // 右側にコマがない
                continue;
            } else {
             
                // この位置より右側にあるコマを左にひとつよせる
                // コマの種類分ループ
                vector<kBlock>::iterator it1 = blockTypes.begin();
                while (it1 != blockTypes.end())
                {
                    // 各種類分のコマ数分ループ
                    list<int>::iterator it2 = m_blockTags[*it1].begin();
                    while (it2 != m_blockTags[*it1].end())
                    {
                        PositionIndex posIndex = getPositionIndex(*it2);
                        
                        if(i < posIndex.x) {
                            // 移動先をセットする
                            setNewPosition2(*it2, posIndex);
                        }
                        it2++;
                    }
                    
                    it1++;
                }
            }
        }
    }
    
    
}

// 新しい位置をセット
void GameScene::setNewPosition2(int tag, PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosX();
    if(nextPosX == -1) {
        nextPosX = posIndex.x ;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(--nextPosX, posIndex.y);
    
}

/*
 * コマのアニメーション
 */
void GameScene::movingBlocksAnimation2()
{
    // コマの新しい位置をセットする
    searchNewPosition2();
    
    // 新しい位置がセットされたコマのアニメーション
    moveBlock();
    
    // アニメーション終了時に次の処理を開始する
    scheduleOnce(schedule_selector(GameScene::movedBlocks), MOVING_TIME);

}


// コマの移動完了
void GameScene::movedBlocks() {
    
    // ラベル再表示
    showLabel();
    
    // アニメーション終了
    m_animation = false;
    
    // ゲーム終了チェック
    if(!existsSameBlock())
    {
        CCSize bgSize = m_background->getContentSize();
        
        // ハイスコア記録・表示
        saveHighScore();
        
        // ゲーム終了画面
        CCSprite* gameOver = CCSprite::create(PNG_GAMEOVER);
        gameOver->setPosition(ccp(bgSize.width / 2 , bgSize.height * 0.8));
        m_background->addChild(gameOver, kZOrderGameOver, kTagGameOver);
        
        setTouchEnabled(false);
    }
}

// ラベル表示
void GameScene::showLabel()
{
    CCSize bgSize = m_background->getContentSize();
    
    // 残数表示
    int tagsForLabel[] = {kTagRedLabel, kTagBlueLabel, kTagYellowLabel, kTagGreenLabel, kTagGrayLabel};
    const char* fontNames[] = {FONT_RED, FONT_BLUE, FONT_YELLOW, FONT_GREEN, FONT_GRAY};
    float heightRate[] = {0.61, 0.51, 0.41, 0.31, 0.21};
    
    // コマ種類分ループ
    vector<kBlock>::iterator it = blockTypes.begin();
    while (it != blockTypes.end())
    {
        // コマ残数表示
        int count = m_blockTags[*it].size();
        const char* countStr = ccsf("%02d", count);
        CCLabelBMFont* label = (CCLabelBMFont*)m_background->getChildByTag(tagsForLabel[*it]);
        
        if(!label){
            // コマ残数生成
            label = CCLabelBMFont::create(countStr, fontNames[*it]);
            label->setPosition(ccp(bgSize.width * 0.78, bgSize.height * heightRate[*it]));
            m_background->addChild(label, kZOrderLabel, tagsForLabel[*it]);
        } else {
            label->setString(countStr);
        }
        
        // スコア表示
        const char* scoreStr = ccsf("%02d", m_score);
        CCLabelBMFont* scoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagScoreLabel);
        if(!scoreLabel){
            // コマ残数生成
            scoreLabel = CCLabelBMFont::create(countStr, FONT_WHITE);
            scoreLabel->setPosition(ccp(bgSize.width * 0.78, bgSize.height * 0.75));
            m_background->addChild(scoreLabel, kZOrderLabel, kTagScoreLabel);
        } else {
            scoreLabel->setString(scoreStr);
        }
        
        *it++;
    }
}

// 全コマに対して、隣り合うコマがあるかチェック
bool GameScene::existsSameBlock()
{
    // 各種類のコマ数分のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類分のコマ数分ループ
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end())
        {
            if(getSameColorBlockTag(*it2, *it1).size() > 1 ){
                // 隣り合うコマが存在する場合はtrue
                return true;
            }
            it2++;
        }
        it1++;
    }
    
    // 隣り合うコマが存在しない場合はfalse
    return false;
}

// ハイスコアラベルを表示
void GameScene::showHighScore()
{
    CCSize bgSize = m_background->getContentSize();
    
    // ハイスコア表示
    int highScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_HIGHSCORE, 0);
    const char* highScoreStr = ccsf("%d", highScore);
    CCLabelBMFont* highScoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagHighScoreLabel);
    if(!highScoreLabel)
    {
        // ハイスコア生成
        highScoreLabel = CCLabelBMFont::create(highScoreStr, FONT_WHITE);
        highScoreLabel->setPosition(ccp(bgSize.width * 0.78, bgSize.height * 0.87));
        m_background->addChild(highScoreLabel, kZOrderLabel, kTagHighScoreLabel);
    } else {
        highScoreLabel->setString(highScoreStr);
    }
}

// ハイスコア記録・表示
void GameScene::saveHighScore()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    
    // ハイスコアを取得する
    int oldHighScore = userDefault->getIntegerForKey(KEY_HIGHSCORE, 0);
    if( oldHighScore < m_score) {
        
        // ハイスコアをセーブ
        userDefault->setIntegerForKey(KEY_HIGHSCORE, m_score);
        userDefault->flush(); // 永続化する
        
        //ハイスコアを表示する
        showHighScore();
    }
}


//　リセットボタンタップ時の処理
void GameScene::menuResetCallBack(cocos2d::CCObject *pSender)
{
    GameScene* scene = GameScene::create();
    
    // 新しい画面に書き換える
    CCDirector::sharedDirector()->replaceScene((CCScene*)scene);
}

// リセットボタンの作成
void GameScene::showResetButton()
{
    CCSize bgSize = m_background->getContentSize();
    
    // リセットボタン作成
    CCMenuItemImage* resetButton = CCMenuItemImage::create(PNG_RESET, PNG_RESET, this, menu_selector(GameScene::menuResetCallBack));
    resetButton->setPosition(ccp(bgSize.width * 0.78, bgSize.height * 0.1));
    
    // メニュー作成
    CCMenu* menu = CCMenu::create(resetButton, NULL);
    menu->setPosition(CCPointZero);
    m_background->addChild(menu);
}







