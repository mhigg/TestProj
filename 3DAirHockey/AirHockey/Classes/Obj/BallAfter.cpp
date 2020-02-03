#include "BallAfter.h"

USING_NS_CC;

BallAfter::BallAfter() : _invTime(8)
{
	Init();
}

BallAfter::~BallAfter()
{
}

void BallAfter::Update(const cocos2d::Vec3 & lPos)
{
	if ((_cnt / _invTime) % 2)
	{
		/// 3�������W�̍X�V
		for (int i = _images.size() - 2; i >= 0; --i)
		{
			_points[i + 1] = _points[i];
		}
		_cnt = 0;
		_points[0] = lPos;

		/// �X�V�������W���g���āA�c���̈ʒu�ƃT�C�Y���X�V����
		for (int i = 0; i < _images.size(); ++i)
		{
			///// �ʒu�����̂��߂ɁA��ʃT�C�Y�̔��������Z���Ă���(���l�Ȃ̂ŁA�C����K���s��) ��
			_images[i]->setPosition(lpPointWithDepth.SetWorldPosition(_points[i]) - cocos2d::Vec2(1024 / 2, 576 / 2));
			_images[i]->setScale(lpPointWithDepth.GetScale(_points[i].z));
		}
	}
	// ���Ԃ̍X�V
	++_cnt;
}

void BallAfter::Init()
{
	// �v�Z�����摜�̔{����ۑ��������
	float rate;
	for (int i = 0; i < _images.size(); ++i)
	{
		/// �摜�T�C�Y�̔{�����v�Z���Ă���
		rate	= (float)(_images.size() - i) / (_images.size());

		/// �摜�̎擾
		_images[i]	= Sprite::create("image/ball/new_ball/ball_13.png");

		/// �摜�T�C�Y�̐ݒ�
		_images[i]->setContentSize(_images[i]->getContentSize()/* * rate*/);

		/// �摜�����x�̐ݒ�
		_images[i]->setOpacity(140 * rate);
		
		/// �������W�̐ݒ�
		setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

		/// �摜�X�P�[���̐ݒ�
		setScale(lpPointWithDepth.GetScale(_localPos.z));

		this->addChild(_images[i]);
	}
}

void BallAfter::update(float dt)
{
}
