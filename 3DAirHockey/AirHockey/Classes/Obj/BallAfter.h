#pragma once
#include <array>
#include "SpriteAffectDepth.h"

/// �{�[���̎c�������p�N���X
class BallAfter :
	public SpriteAffectDepth
{
public:
	BallAfter();
	~BallAfter();

	/// ���W�X�V�p(�{�[���̃��[�J�����W���擾����)
	void Update(const cocos2d::Vec3& lPos);
private:
	void Init();
	void update(float dt);

	int _cnt = 0;				// ���Ԍv���p

	std::array<cocos2d::Vec3, 6> _points;
	std::array<cocos2d::Sprite*, 6> _images;

	const int _invTime;			// �c�����ړ�������Ԋu
};

