﻿#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Controller/OPRT_Network.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

USING_NS_CC;

Player::Player(bool isHost, const float& zdepth)
{
	/// 座標の初期化
	_prePos	  = Vec2::ZERO;
	_localPos = { 0,0,zdepth };

	Init(zdepth);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_oprtState.reset(new MouseCtl(this));
#else
	_oprtState.reset(new Oprt_Touch(this));
#endif

	this->scheduleUpdate();
}

Player::Player(bool isHost, const float& zdepth, int provIsFront)
{
	/// 座標の初期化
	_prePos	  = Vec2::ZERO;
	_localPos = { 0,0,zdepth };

	Init(zdepth);

	if (provIsFront == 0)
	{
		// 手前のマレットは自操作
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		_oprtState.reset(new MouseCtl(this));
#else
		_oprtState.reset(new Oprt_Touch(this));
#endif
	}
	else
	{
		// 奥側のマレットは通信
		_oprtState.reset(new OPRT_Network(this, isHost));
	}

	this->scheduleUpdate();
}

Player::~Player()
{
}

void Player::Init(const float & zdepth)
{
	/// 座標の初期化
	_prePos = Vec2::ZERO;

	_vPoint = _vel = Vec2::ZERO;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 座標を真ん中にセット
	this->setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

	// 消失点の変更
	//lpPointWithDepth.SetVanishingPoint(-Vec2(visibleSize.width, visibleSize.height));

	/// 表示時間の初期化
	_dispCnt = -1;

	/// プレイヤーの差分用画像の初期化
	InitTextureInfo();
}

void Player::InitTextureInfo()
{
	// 中央
	auto sprite = Sprite::create("image/player/player_center.png");
	sprite->setName("center");
	sprite->setTag(static_cast<int>(PL_ANC::CENTER));
	this->addChild(sprite);

	// 左上
	sprite = Sprite::create("image/player/player_leftup.png");
	sprite->setAnchorPoint({ 1, 0 });
	sprite->setName("leftup");
	sprite->setTag(static_cast<int>(PL_ANC::LEFTUP));
	this->addChild(sprite);

	// 左下
	sprite = Sprite::create("image/player/player_leftdown.png");
	sprite->setAnchorPoint({ 1, 1 });
	sprite->setName("leftdown");
	sprite->setTag(static_cast<int>(PL_ANC::LEFTDOWN));
	this->addChild(sprite);

	// 右上
	sprite = Sprite::create("image/player/player_rightup.png");
	sprite->setAnchorPoint({ 0, 0 });
	sprite->setName("rightup");
	sprite->setTag(static_cast<int>(PL_ANC::RIGHTUP));
	this->addChild(sprite);

	// 右下
	sprite = Sprite::create("image/player/player_rightdown.png");
	sprite->setAnchorPoint({ 0, 1 });
	sprite->setName("rightdown");
	sprite->setTag(static_cast<int>(PL_ANC::RIGHTDOWN));
	this->addChild(sprite);
	/// 画像名保存用
	std::string imageName;

	/// 画像情報の登録
	for (auto anchor : this->getChildren())
	{
		/// ボールと当たっていない時の画像登録
		imageName = "image/player/player_" + anchor->getName() + ".png";
		_texInfo[anchor->getTag()].first = Director::getInstance()->getTextureCache()->addImage(imageName);

		/// ボールと当たった時の画像登録
		imageName = "image/player/player_" + anchor->getName() + "_hit" + ".png";
		_texInfo[anchor->getTag()].second = Director::getInstance()->getTextureCache()->addImage(imageName);
	}
}

float Player::GetDepth() const
{
	return _localPos.z;
}

cocos2d::Vec2 Player::GetAnchorPos(const std::string& name)
{
	/// アンカーポイントに設定してある座標を返している
	if (name == "center")
	{
		return Vec2(_localPos.x, _localPos.y);
	}
	else if (name == "leftup")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(-size.width / 2, size.height / 2);
	}
	else if (name == "rightup")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(size.width / 2, size.height / 2);
	}
	else if (name == "leftdown")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(-size.width / 2, -size.height / 2);
	}
	else if (name == "rightdown")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(size.width / 2, -size.height / 2);
	}
	else {}
	return Vec2::ZERO;
}

cocos2d::Vec2 Player::GetMoveDistance() const
{
	return (Vec2(_localPos.x, _localPos.y) - _prePos);
}

void Player::ChangeImage(const int & num)
{
	/// ボールと当たった時の画像に変更している
	Sprite* sp = (Sprite*)this->getChildByTag(num);
	sp->setTexture(_texInfo[num].second);

	/// 画像を表示する時間
	_dispCnt = 40;
}

void Player::MoveUpdate()
{
	/// 画面サイズの取得
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 pos	 = _oprtState->GetPoint() - visibleSize / 2;
	Vec2 offset  = visibleSize / 2;

	/// サイズの取得
	float width, height;

	width  = (this->getChildByName("leftup")->getContentSize().width +
			  this->getChildByName("rightup")->getContentSize().width);
	height = (this->getChildByName("leftdown")->getContentSize().height +
			  this->getChildByName("rightdown")->getContentSize().height);

	Size size = { width, height };

	/// X軸の移動範囲チェック
	if ((pos.x + offset.x + size.width / 2 < visibleSize.width) &&
		(pos.x + offset.x - size.width / 2 > 0))
	{
		_localPos.x = pos.x;
	}
	// 右の範囲外
	else if (pos.x + offset.x + size.width / 2 > visibleSize.width)
	{
		_localPos.x = (visibleSize.width - size.width )/2;
	}
	// 左の範囲外
	else if (pos.x + offset.x - size.width / 2 < 0)
	{
		_localPos.x = (-visibleSize.width + size.width) / 2;
	}
	else{}

	/// Y軸の移動範囲チェック
	if ((pos.y + offset.y + size.height / 2 < visibleSize.height) &&
		(pos.y + offset.y - size.height / 2 > 0))
	{
		_localPos.y = pos.y;
	}
	else if (pos.y + offset.y + size.height / 2 > visibleSize.height)
	{
		_localPos.y = (visibleSize.height - size.height) / 2;
	}
	else if (pos.y + offset.y - size.height / 2 < 0)
	{
		_localPos.y = (-visibleSize.height + size.height) / 2;
	}


	// 奥行きの深さによって、サイズを変更するようにしている
	setScale(lpPointWithDepth.GetScale(_localPos.z));
	
	if (this->getName() == "player1")
	{
		VanishPointUpdate(pos);
	}

	/// ここの座標を使ってみるか
	/// ワールド座標に変換している
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}

void Player::VanishPointUpdate(const cocos2d::Vec2& pos)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto Clamp = [](const float& vel, const float& speed)
	{
		return fmin(3.f, fmax(-3.f, (vel + speed)));
	};

	/// 現状、仮の消失点の更新をしている
	auto distance = (pos - _vPoint).getNormalized();
	_vel = Vec2(Clamp(_vel.x, distance.x * 0.2f), Clamp(_vel.y, distance.y * 0.2f));
	_vPoint += _vel;

	//lpPointWithDepth.SetVanishingPoint(-_vPoint + Vec2(visibleSize.width / 2, visibleSize.height / 2));
}

void Player::ResetTexture()
{
	Sprite* sp;
	for (auto anchor : this->getChildren())
	{
		sp = (Sprite*)anchor;
		sp->setTexture(_texInfo[anchor->getTag()].first);
	}
	_dispCnt = -1;
}

void Player::update(float dt)
{
	_prePos = Vec2(_localPos.x, _localPos.y);
	_oprtState->Update();

	MoveUpdate();

	if (_dispCnt == 0)
	{
		ResetTexture();
	}
	else
	{
		--_dispCnt;
	}
}
