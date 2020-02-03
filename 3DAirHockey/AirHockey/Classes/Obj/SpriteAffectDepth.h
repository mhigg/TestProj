#pragma once
#include "cocos2d.h"
#include "../Manager/PointWithDepth.h"

// ���ײĂ�1�_�����}�@�ɑΉ�������̂Ɏg���N���X
class SpriteAffectDepth :
	public cocos2d::Sprite
{
public:
	// (���W,���s,ø�������߽)
	SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, const std::string &filename);
	// (���W,���s,ø�����̑傫��,�F)
	SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color);
	SpriteAffectDepth();
	~SpriteAffectDepth();

	virtual void update(float dt);

	cocos2d::Vec3 GetLocalPos(void) const;					// _localPos�̎擾

private:
	bool Init(void);

protected:
	cocos2d::Vec3 _localPos;	// �ްя�̍��W(��ذ݂ɕ\���������W�Ƃ͈Ⴄ)
	float _localSize;			// �ްя�̽��ײĂ̑傫��(��ذ݂ɕ\�������傫���Ƃ͈Ⴄ)
};

