#include <cmath>
#include "CCAudioMng.h"

std::unique_ptr<CCAudioMng, CCAudioMng::AudioDeleter> CCAudioMng::s_Instance;

/// criket audio�̏�����(Android��)
#if CK_PLATFORM_ANDROID
#ifdef __cplusplus
extern "C"
{
#endif
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initCricket(JNIEnv *env, jclass obj, jobject context)
	{
		CkConfig config(env, context);
		CkInit(&config);
	}
#ifdef __cplusplus
	}
#endif
#endif

CCAudioMng::CCAudioMng()
{
/// criket audio�̏�����(windows��)
#if CK_PLATFORM_WIN	
	CkConfig config;
	CkInit(&config);   
#endif	
}

CCAudioMng::~CCAudioMng()
{
}

void CCAudioMng::Destroy()
{
	/// bank�f�[�^�̉��
	if (_bankMap.size() > 0)
	{
		for (auto& bank : _bankMap)
		{
			_bankMap[bank.first]->destroy();
		}
		_bankMap.clear();
	}

	/// ���ʉ��f�[�^�̉��
	if (_seMap.size() > 0)
	{
		for (auto& se : _seMap)
		{
			_seMap[se.first]->destroy();
		}
		_seMap.clear();
	}

	/// BGM�f�[�^�̉��
	if (_bgmMap.size() > 0)
	{
		for (auto& bgm : _bgmMap)
		{
			_bgmMap[bgm.first]->destroy();
		}
		_bgmMap.clear();
	}
}

void CCAudioMng::ShutDown()
{
	Destroy();
	CkShutdown();
}

float CCAudioMng::Clamp(const float & rate, const float & minRate, const float & maxRate)
{
	return fmin(maxRate, fmax(minRate, rate));
}

void CCAudioMng::RegistBank(const std::string & pathName, const std::string& key)
{
	CkPathType pathType;
/// ������ɂ���āA�ǂݍ��ރp�X�̃^�C�v��؂�ւ��Ă���
#if CK_PLATFORM_WIN
	pathType = CkPathType::kCkPathType_FileSystem;
#else
	pathType = CkPathType::kCkPathType_Default;
#endif

	/// �o���N�̓o�^���s���Ă���
	if (_bankMap.find(key) == _bankMap.end())
	{
		/// Sound�t�@�C���̐擪�p�X���w�肵�Ă���
		std::string bankName = "Sound/" + pathName;
		_bankMap[key] = CkBank::newBank(bankName.c_str(), pathType);
	}
}

void CCAudioMng::RegistBankSE(const std::string & bankKey, const std::string & bankName, const std::string & seKey)
{
	/// bank�̃f�[�^���}�b�v�ɓo�^����Ă��邩�̔���
	if (_bankMap.find(bankKey) != _bankMap.end())
	{
		/// �w�肵���L�[�̌��ʉ����o�^����Ă��邩�̔���
		if (_seMap.find(seKey) == _seMap.end())
		{
			/// ���ʉ��̓o�^���s��
			_seMap[seKey] = CkSound::newBankSound(_bankMap[bankKey], bankName.c_str());
		}
	}
}

void CCAudioMng::RegistStreamBGM(const std::string & pathName, const std::string & key)
{
	CkPathType pathType;
	/// ������ɂ���āA�ǂݍ��ރp�X�̃^�C�v��؂�ւ��Ă���
#if CK_PLATFORM_WIN
	pathType = CkPathType::kCkPathType_FileSystem;
#else
	pathType = CkPathType::kCkPathType_Default;
#endif

	if (_bgmMap.find(key) == _bgmMap.end())
	{
		/// Sound�t�@�C���̐擪�p�X���w�肵�Ă���
		std::string bgmName = "Sound/" + pathName;
		_bgmMap[key] = CkSound::newStreamSound(bgmName.c_str(), pathType);
		/// ���̑傫���𔼕��ɂ��Ă݂�
		//_bgmMap[key]->setVolume(0.5f);
	}
}

void CCAudioMng::CkPlaySE(const std::string& key)
{
	/// ���ʉ����o�^����Ă��邩�̔���
	if (_seMap.find(key) != _seMap.end())
	{
		if (_seMap[key] != nullptr)
		{
			/// �Đ����łȂ���΁A�Đ����s��
			if (_seMap[key]->isPlaying())
			{
				CkStopSE(key);
				_seMap[key]->play();
			}
			else
			{
				_seMap[key]->play();
			}
		}
	}
}

void CCAudioMng::CkPlaySE(const std::string & key, const float & rate)
{
	/// ���ʉ����o�^����Ă��邩�̔���
	if (_seMap.find(key) != _seMap.end())
	{
		/// �Đ����łȂ���΁A�Đ����s��
		if (_seMap[key]->isPlaying())
		{
			CkStopSE(key);
			_seMap[key]->setVolume(Clamp(rate));
			_seMap[key]->play();
		}
		else
		{
			_seMap[key]->setVolume(Clamp(rate));
			_seMap[key]->play();
		}
	}
}

void CCAudioMng::CkPlayBGM(const std::string& key)
{
	/// BGM���o�^����Ă��邩�̔���
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// �Đ���
		if (!_bgmMap[key]->isPlaying())
		{
			_bgmMap[key]->play();
		}
	}
}

void CCAudioMng::CkPlayBGM(const std::string & key, const float & rate)
{
	/// BGM���o�^����Ă��邩�̔���
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// �Đ���
		if (!_bgmMap[key]->isPlaying())
		{
			/// ���ʂ̒��������čĐ�����
			_bgmMap[key]->setVolume(Clamp(rate));
			_bgmMap[key]->play();
		}
	}
}

void CCAudioMng::CkStopBGM(const std::string & key)
{
	/// BGM���o�^����Ă��邩�̔���
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// �Đ���
		if (_bgmMap[key]->isPlaying())
		{
			_bgmMap[key]->stop();
		}
	}
}

void CCAudioMng::CkStopSE(const std::string & key)
{
	/// BGM���o�^����Ă��邩�̔���
	if (_seMap.find(key) != _seMap.end())
	{
		/// �Đ���
		if (_seMap[key]->isPlaying())
		{
			_seMap[key]->stop();
		}
	}
}

void CCAudioMng::Update()
{
	CkUpdate();
}
