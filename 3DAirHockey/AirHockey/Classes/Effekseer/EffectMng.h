#pragma once
#include "cocos2d.h"
#include "Effekseer.h"

// ��`
#define lpEffectMng EffectMng::GetInstance()

// �̪�Ă̏��ۑ��pMap
using ManagerMap = std::map<std::string, efk::EffectManager*>;
using EmitterMap = std::map<std::string, efk::EffectEmitter*>;

class EffectMng : public cocos2d::Ref
{
public:
	static EffectMng& GetInstance()	// ���ڽ���~���������߲������Q�Ƃ�
	{
		return (*s_Instance);		// *�Œ��g(�׽�̎���)��Ԃ�
	};

	ManagerMap GetManager(void);						// Scene��visit��update�Ŏg��_manager�̹ޯ��

	// �̪�Ă̓o�^(�̪�Ă�efk��,�t���������O,�傫��,�Đ���߰��)
	efk::EffectEmitter* EffectInit(const std::string& fileName, const std::string useName, float scale, float speed);
	bool PlayEffect(std::string useName);								// �̪�Ă̍Đ�(���łɍĐ����̏ꍇ�͒ʂ�Ȃ�)
	bool PlayEffect(std::string useName , cocos2d::Vec2 pos);			// �̪�Ă̍Đ�(���łɍĐ����̏ꍇ�͒ʂ�Ȃ�)
	bool SetRotateEffect(std::string useName, cocos2d::Vec3 rotate);	// �̪�Ẳ�]
	bool SetPosEffect(std::string useName, cocos2d::Vec2 pos);			// �̪�Ă̈ړ�
	bool StopEffect(std::string useName);								// �̪�Ă̒�~
	bool CheckEffect(std::string useName);								// ���Ă��Đ������ǂ���(�Đ����Ȃ�true)

private:
	// �֐���޼ު��
	// �޽�׸����Ă�
	struct EffectMngDeleter
	{
		void operator()(EffectMng* effectMng)const
		{
			delete effectMng;
		}
	};
	EffectMng();
	~EffectMng();

	static std::unique_ptr<EffectMng, EffectMngDeleter> s_Instance;		// �ݽ�ݽ�p�ư��߲���

	ManagerMap _manager;						// EffectManager�ݽ�ݽ�p�߲���
	EmitterMap _emitter;						// �̪�ĕۑ��pMap
};
