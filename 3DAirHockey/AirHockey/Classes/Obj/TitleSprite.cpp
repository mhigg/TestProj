#include "TitleSprite.h"

TitleSprite::TitleSprite(cocos2d::Vec2 pos, float zDepth, const std::string &filename)
{
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

TitleSprite::TitleSprite()
{
}

TitleSprite::~TitleSprite()
{
}

bool TitleSprite::Init(void)
{
	return false;
}

void TitleSprite::update(float dt)
{
	// ���W�̍X�V
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}
