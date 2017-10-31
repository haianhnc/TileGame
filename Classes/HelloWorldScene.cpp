#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    // STEP1 INIT TILE MAP
    _tileMap = CCTMXTiledMap::create("desert_space.tmx");
    _background = _tileMap->layerNamed("backGround");
    
    
//        _tileMap = CCTMXTiledMap::create("TileMap.tmx");
//        _background = _tileMap->layerNamed("Background");
    
    this->addChild(_tileMap);
    
        // META DATA
    _meta = _tileMap->layerNamed("Meta");
    
    
    // STEP 2 MAKING OBJECT GROUP
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    if(objectGroup==NULL)
    {
        CCLOG("tile map has no objects object layer");
        return false;
    }
    float x = 0;
    float y = visibleSize.height;
    
    _player = Sprite::create("Player.png");
//    _player = Sprite::create("CloseNormal.png");
    _player->setPosition(visibleSize.width/2,y);
    
    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::setViewPointCenter(Point _point)               // setPossition for layer(camera)
{
    auto winSize = Director::getInstance()->sharedDirector()->getWinSize();
    int x = MAX(_point.x,winSize.width/2);
    int y = MAX(_point.y, winSize.height/2);
    // bo qua 2 dong code xem sao
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    
    Point actuatPossition = Point(x, y);
    Point centerOfView = Point(winSize.width/2,winSize.height/2);
    Point viewPoint = ccpSub(centerOfView, actuatPossition);
    this->setPosition(viewPoint);
    
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
 void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    // do nothing
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
   /* here is old version of moving
    if(ismoving == true)
        return;
    
    ismoving = true;
    auto _touch = touch->getLocation();
    _touch = this->convertToNodeSpace(_touch);
    float velocity = 240/3;         // van toc 240px/3s
    float dx = _touch.x - _player->getPosition().x;
    float dy = _touch.y - _player->getPosition().y;
    float s = sqrt(dx*dx + dy*dy);
    
    // flip
    if(dx <0)
        _player->setFlippedX(true);
    else
        _player->setFlippedX(false);
    
    auto moveAction = MoveTo::create(s/velocity, _touch);
    _moveAction = Sequence::create(moveAction,CallFuncN::create(CC_CALLBACK_0(HelloWorld::playerMoveEnd, this)), NULL);
    _player->runAction(_moveAction);
    */
    
    // try to move by tile
    auto touchLocaiton = touch->getLocationInView();
    touchLocaiton = Director::getInstance()->convertToGL(touchLocaiton);
    touchLocaiton = this->convertToNodeSpace(touchLocaiton);
    
    Point playerPros = _player->getPosition();
    Point diff = ccpSub(touchLocaiton, playerPros);
    
    // neu x  dai hon y thi di chuyen theo duong x
    if(abs(diff.x)>abs(diff.y))
    {
        if(diff.x>0)
        {
            playerPros.x += _tileMap->getTileSize().width;
        }
        else
        {
            playerPros.x -= _tileMap->getTileSize().width;
        }
    }
    else
    {
        if(diff.y>0)
        {
            playerPros.y += _tileMap->getTileSize().height;
        }
        else
        {
            playerPros.y -= _tileMap->getTileSize().height;
        }
    }
    
    // check on bounds of the map
    if (playerPros.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPros.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPros.y >= 0 &&
        playerPros.x >= 0 )
    {
        this->setPlayerPosition(playerPros);
    }
    
    this->setViewPointCenter(playerPros);
    
}

void HelloWorld::playerMoveEnd()
{
    _player->stopAction(_moveAction);
    ismoving = false;
}
void HelloWorld::onEnter()
{
    // override method of parent class
    Layer::onEnter();
    
    auto _listener1 = EventListenerTouchOneByOne::create();
    _listener1->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _listener1->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    _listener1->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);
}

// convert usual point to tile coordinates
Point HelloWorld::tileCoordForPosition(Point _pos)
{
//    float x = (( _tileMap->getMapSize().width * _tileMap->getTileSize().width) -_pos.x)/ _tileMap->getTileSize().width ;
    float x = _pos.x/_tileMap->getTileSize().width;
    float y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - _pos.y)/_tileMap->getTileSize().height;
    return Point(x, y);
}

void HelloWorld::setPlayerPosition(Point pos)
{
    /*
     * check for contactable
     */
    
    Point tileCoord = this->tileCoordForPosition(pos);         // covert to tile possition
    int tileGid = _meta->tileGIDAt(tileCoord);
    if(tileGid)
    {
//        int GID = tileCoord.y * _tileMap->getMapSize().width + tileCoord.x + 1;
        unsigned int GID = _meta->tileGIDAt(tileCoord);
        if(GID)
        {
            auto properties = _tileMap->getPropertiesForGID(GID).asValueMap();
            if(!properties.empty())
            {
                auto isCollisioAble = properties["Collidable"].asString();
                if("True" == isCollisioAble)
                    return;
            }
        }
    }
        _player->setPosition(pos);
}
