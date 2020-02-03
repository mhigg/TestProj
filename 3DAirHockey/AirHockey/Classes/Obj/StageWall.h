#pragma once
#include "SpriteAffectDepth.h"

class StageWall :
	public SpriteAffectDepth
{
public:
	// (���W,���s,ø�����̑傫��,�F)
	StageWall(cocos2d::Vec2 pos, float zDepth,cocos2d::Point size, cocos2d::Color3B color);
	// (���W,���s,�ǂ̔ԍ�)
	StageWall(float zDepth, int num);
	StageWall();
	~StageWall();

	void SetWallColorChangeFlag(bool flag);	// �F��ς����׸ނ̾���(Ball�׽�Ŏg�p)
private:
	void update(float dt)override;
	void WallColor(cocos2d::Color3B color);	// �ǂ̐F�̕ύX

	cocos2d::Color3B _nowColor;		// ���݂̕ǂ̐F
	cocos2d::Color3B _normalColor;	// �ʏ�̕ǂ̐F
	cocos2d::Color3B _changeColor;	// �ްْʉߎ��ɕς��ǂ̐F
	bool _colorChangeFlag;			// �F��ς����׸�(true��_changeColor,false��_normalColor)

	int _wallNum;					// �ǂ̔ԍ�(��O��0�Ԗڂ�1�������Ă���)
};