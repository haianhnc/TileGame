#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // setViewPointCenter
    void setViewPointCenter(cocos2d::Point _point);
    
    
    // player move
    // Bắt sự kiện Touch
    void onEnter();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void playerMoveEnd();
    void setPlayerPosition(cocos2d::Point pos);
    cocos2d::Point tileCoordForPosition(cocos2d::Point _pos);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    cocos2d::TMXTiledMap* _tileMap;
    cocos2d::TMXLayer* _background;
    cocos2d::Sprite* _player;
    cocos2d::Action* _moveAction;
    cocos2d::TMXLayer* _meta;
    bool ismoving = false;
};

#endif // __HELLOWORLD_SCENE_H__
