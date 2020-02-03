#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <jni.h>

/// cricket audio�֌W�̃w�b�_�[�t�@�C��
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"


/// Android Studio�ł̏�����(���R�����g)

#if CK_PLATFORM_ANDROID
#pragma execution_character_set("utf-8")

#ifndef __ANDROID_JNI__
#define __ANDROID_JNI__

#ifdef __cplusplus
extern "C"
{
#endif
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initCricket(JNIEnv *env, jclass obj, jobject context);
#ifdef __cplusplus
}
#endif
#endif

#endif

// bank�f�[�^�ێ��p��unordered_map
using CkBankMap  = std::unordered_map<std::string, CkBank*>;
// bank�f�[�^�ێ��p��unordered_map
using CkSoundMap = std::unordered_map<std::string, CkSound*>;

class CCAudioMng
{
public:
	static CCAudioMng& GetInstance()
	{
		if (s_Instance == nullptr)
		{
			/// ���������������ꂸ�A�X�^�b�N�I�[�o�[�t���[���N����
			s_Instance.reset(new CCAudioMng);
		}
		return *s_Instance;
	}

	// cricket audio���Đ����邽�߂̂���
	void Update();

	// cricket audio�̋@�\���I��������
	void ShutDown();

	// bank�f�[�^�̓o�^���s���Ă���(�p�X�ǂݍ��݂̌`���̓t�@�C���p�X�̎w��������ݒ�ɂ��Ă���)
	void RegistBank(const std::string& pathName, const std::string& key);
	
	// bank�f�[�^������ʉ��̓o�^���s���Ă���
	void RegistBankSE(const std::string& bankKey, const std::string& bankName, const std::string& seKey);

	// cks�t�@�C������BGM�̓o�^���s���Ă���
	void RegistStreamBGM(const std::string& pathName, const std::string& key);

	// ���ʉ��̍Đ�
	void CkPlaySE(const std::string& key);
	void CkPlaySE(const std::string& key, const float& rate);

	// BGM�̍Đ�
	void CkPlayBGM(const std::string& key);
	void CkPlayBGM(const std::string& key, const float& rate);

	void CkStopBGM(const std::string& key);
private:
	CCAudioMng();
	~CCAudioMng();

	void CkStopSE(const std::string& key);

	// ���͈͂̊�����Ԃ�����(minRate : ���ʂ̍ŏ��l, maxRate : ���̍ő�l)
	float Clamp(const float& rate, 
				const float& minRate = 0.2f, 
				const float& maxRate = 1.0f);

	// �o�^���Ă���T�E���h�f�[�^�̉���p
	void Destroy();

	// �V���O���g���̃f���[�^�[
	struct AudioDeleter
	{
		void operator()(CCAudioMng* memory)
		{
			delete memory;
		}
	};
	static std::unique_ptr<CCAudioMng, AudioDeleter> s_Instance;

	/// unordered_map���g�p���ĊǗ����s��
	CkBankMap _bankMap;		// bank�f�[�^�̊Ǘ��p
	CkSoundMap _seMap;		// ���ʉ��f�[�^�̊Ǘ��p
	CkSoundMap _bgmMap;		// BGM�f�[�^�̊Ǘ��p
};

