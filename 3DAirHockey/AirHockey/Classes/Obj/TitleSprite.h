#pragma once
#include "Obj.h"

class TitleSprite :
	public Obj
{
public:
	// (���W,���s,ø�����̑傫��,�F)
	TitleSprite(cocos2d::Vec2 pos, float zDepth, const std::string &filename);
	TitleSprite();
	~TitleSprite();
	
private:
	bool Init(void);
	void update(float dt) override;

protected:
	cocos2d::Vec3 _localPos;	// �ްя�̍��W(��ذ݂ɕ\���������W�Ƃ͈Ⴄ)
	float _localSize;			// �ްя�̽��ײĂ̑傫��(��ذ݂ɕ\�������傫���Ƃ͈Ⴄ)
};
