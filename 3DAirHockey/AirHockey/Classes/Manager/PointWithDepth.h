#pragma once
#include "cocos2d.h"
#include <utility>

// ��`
#define lpPointWithDepth PointWithDepth::GetInstance()		// �ݸ����

// using�錾
using Z_DEPTH		= std::pair<float, float>;	// ���s�̎n�_�ƏI�_
using VANISHIN_P	= std::pair<float, float>;	// �����_��X��Y�̍��W

class PointWithDepth
	: public cocos2d::Node
{
public:
	static PointWithDepth& GetInstance()	// ���ڽ���~���������߲������Q�Ƃ�
	{
		return (*s_Instance);				// *�Œ��g(�׽�̎���)��Ԃ�
	}

	cocos2d::Point SetWorldPosition(cocos2d::Vec3 localPos);	// scale�ƍ��W��1�_�����}�@�Ōv�Z����
	float GetScale(float local_z);								// ���ײėp��scare�̹ޯ��

	cocos2d::Point GetVanishingPoint(void);						// �����_�̍��W�̹ޯ��
	void SetVanishingPoint(cocos2d::Point pos);					// �����_�𓮂��������Ƃ��Ɏg������
	void ResetVanishingPoint(void);								// �������������_�������l�ɖ߂�

private:
	// �֐���޼ު��
	// �޽�׸����Ă�
	struct PointWithDepthDeleter
	{
		void operator()(PointWithDepth* pointWithDepth)const
		{
			delete pointWithDepth;
		}
	};
	PointWithDepth();
	~PointWithDepth();

	static std::unique_ptr<PointWithDepth, PointWithDepthDeleter> s_Instance;	// �ݽ�ݽ�p�ư��߲���

	void PointNormalize(cocos2d::Point VanishingPoint);					// 
	Z_DEPTH _zDepth;							// ���s�̎n�_�ƏI�_
	cocos2d::Point _firstVanishingPoint;		// �����̏����_��X��Y�̍��W
	cocos2d::Point _nowVanishingPoint;			// �X�V��������_��X��Y�̍��W
	cocos2d::Point _normalizePoint;				// 
	cocos2d::Point _magnification;				// �����_�𓮂����{��()
	cocos2d::Rect  _moveVanishingPoint;			// �����_�𓮂�����͈�
	float _scale;								// ���ײĂ̑傫��
};