#pragma once
#include "cocos2d.h"

// ��`
#define lpAnimMng AnimMng::GetInstance()

class AnimMng : public cocos2d::Ref
{
public:
	static AnimMng& GetInstance()	// ���ڽ���~���������߲������Q�Ƃ�
	{
		return (*s_Instance);		// *�Œ��g(�׽�̎���)��Ԃ�
	}

	// ��Ұ��݂̓o�^(̧�ق̖��O,�t���������O,�o�^���閇��,��Ұ���1����������ڰѐ�,ٰ�߂��邩)
	void AddAnimCache(const std::string spriteName, const std::string animName, const int endNum, const float delay, const bool returnAnim);
	void SetAnim(cocos2d::Sprite* sprite, const std::string animName, const bool repeat = false);	// ��Ұ��݂̾��(true�Ŕ��])

	void SetAnimReverseFlag(bool animReverseFlag);
private:
	// �֐���޼ު��
	// �޽�׸����Ă�
	struct AnimMngDeleter
	{
		void operator()(AnimMng* animMng)const
		{
			delete animMng;
		}
	};
	AnimMng();
	~AnimMng();

	static std::unique_ptr<AnimMng, AnimMngDeleter> s_Instance;	// �ݽ�ݽ�p�ư��߲���

	bool _animReverseFlag;		// ��Ұ��ݗp���]�׸�
	bool _oldAnimReverseFlag;	// 1�ڰёO�̱�Ұ��ݗp���]�׸�
	std::string _animName;		// ���݂̱�Ұ��݂̖��O�ۑ��p
};