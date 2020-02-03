#include "AnimMng.h"

std::unique_ptr<AnimMng, AnimMng::AnimMngDeleter> AnimMng::s_Instance(new AnimMng());	// �ݽ�ݽ�p�ư��߲���

void AnimMng::AddAnimCache(const std::string spriteName, const std::string animName, const int endNum, const float delay, const bool returnAnim)
{
	// plist�̓ǂݍ���
	auto cache = cocos2d::SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("image/" + spriteName + +"/" + spriteName + ".plist");

	cocos2d::Animation *animation = cocos2d::Animation::create();

	// ��Ұ��݂̺ϐ����J��Ԃ�
	for (int k = 1; k <= endNum; k++)
	{
		// plist���畡���̽��ײĂ�ǉ�
		cocos2d::SpriteFrame* sp = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName + "_" + std::to_string(k) + ".png");
		animation->addSpriteFrame(sp);
	}

	// ��Ұ��݂̊Ԋu
	animation->setDelayPerUnit(delay);

	// �Ō�ɂȂ�����ŏ��ɖ߂�
	animation->setRestoreOriginalFrame(returnAnim);

	// ������ɓo�^
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	animationCache->addAnimation(animation, animName);
}

void AnimMng::SetAnim(cocos2d::Sprite* sprite,const std::string animName, const bool repeat)
{
	//if (_animName == animName 
	//	&& _oldAnimReverseFlag == _animReverseFlag)
	//{
	//	return;
	//}

	// ��Ұ��݂��~�߂�
	sprite->stopAllActions();

	// ��������玝���Ă���
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	cocos2d::Animation *animation = animationCache->getAnimation(animName);
	auto action = cocos2d::Animate::create(animation);
	
	// ���]���邩�ǂ���(true�Ŕ��])
	auto flip = cocos2d::FlipX::create(_animReverseFlag);
	_oldAnimReverseFlag = _animReverseFlag;
	sprite->runAction(flip);

	// ��Ұ���
	if (repeat)
	{
		_animName = animName;
		// ٰ�ߗL
		sprite->runAction(cocos2d::RepeatForever::create(action));
	}
	else
	{
		_animName = animName;
		// ����
		sprite->runAction(action);
	}
}

void AnimMng::SetAnimReverseFlag(bool animReverseFlag)
{
	_animReverseFlag = animReverseFlag;
}

AnimMng::AnimMng()
{
	_animName = "";
	_animReverseFlag = false;
}

AnimMng::~AnimMng()
{
}

