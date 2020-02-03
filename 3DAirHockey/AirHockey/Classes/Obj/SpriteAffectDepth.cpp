#include "SpriteAffectDepth.h"

SpriteAffectDepth::SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, const std::string & filename)
{
	Init();

	// ���W
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// ø����
	setTexture(filename);

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();
}

SpriteAffectDepth::SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	Init();

	// �ð��ø����(��)�̻���
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// �摜
	setTextureRect(deformationRect);
	// �F�̐ݒ�
	setColor(color);

	// ���W
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();
}

SpriteAffectDepth::SpriteAffectDepth()
{
	init();
}

SpriteAffectDepth::~SpriteAffectDepth()
{
}

bool SpriteAffectDepth::Init(void)
{
	// ����init()�ɕK���ʂ�悤�ɂ���
	if (!cocos2d::Sprite::init())
	{
		return false;
	}
	return true;
}

void SpriteAffectDepth::update(float dt)
{
	// ���W�̍X�V
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}

cocos2d::Vec3 SpriteAffectDepth::GetLocalPos(void) const
{
	return _localPos;
}
