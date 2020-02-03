#include "EffectMng.h"

std::unique_ptr<EffectMng, EffectMng::EffectMngDeleter> EffectMng::s_Instance(new EffectMng());	// �ݽ�ݽ�p�ư��߲���

// �̪�Ă̓o�^(�̪�Ă�efk��,�t���������O,�傫��,�Đ���߰��)
efk::EffectEmitter* EffectMng::EffectInit(const std::string& fileName,const std::string useName,float scale, float speed)
{
	// manager�쐬
	_manager.emplace(useName, efk::EffectManager::create(cocos2d::Director::getInstance()->getVisibleSize()));
	
	// ̧�ق̓ǂݍ���
	auto effect = efk::Effect::create(fileName);
	// map�ɒǉ�
	_emitter.emplace(useName, efk::EffectEmitter::create(_manager[useName]));
	_emitter[useName]->setEffect(effect);
	// �傫��
	_emitter[useName]->setScale(scale);
	// �Đ���߰��
	_emitter[useName]->setSpeed(speed);
	// �Đ����I����Ă��j�����Ȃ��悤�ɐݒ�
	_emitter[useName]->setRemoveOnStop(false);
	return _emitter[useName];
}

ManagerMap EffectMng::GetManager(void)
{
	return _manager;
}

// �̪�Ă̍Đ�(���łɍĐ����̏ꍇ�͒ʂ�Ȃ�)
bool EffectMng::PlayEffect(std::string useName)
{
	// �w�肵���̪�Ă��Đ����Ă��Ȃ�������
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->play();
	}

	return true;
}

// �̪�Ă̍Đ�(���łɍĐ����̏ꍇ�͒ʂ�Ȃ�)
bool EffectMng::PlayEffect(std::string useName, cocos2d::Vec2 pos)
{
	// �w�肵���̪�Ă��Đ����Ă��Ȃ�������
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->setPosition(pos);
		_emitter[useName]->play();
	}

	return true;
}

// �̪�Ẳ�]
bool EffectMng::SetRotateEffect(std::string useName, cocos2d::Vec3 rotate)
{
	// ���݂ƈ��������߂ľ��
	if (_emitter[useName]->getRotation3D() != rotate)
	{
		_emitter[useName]->setRotation3D(rotate);
		
		return true;
	}
	return false;
}

// �̪�Ă̈ړ�
bool EffectMng::SetPosEffect(std::string useName, cocos2d::Vec2 pos)
{
	// ���݂ƈ��������߂ľ��
	if (_emitter[useName]->getPosition() != pos)
	{
		_emitter[useName]->setPosition(pos);
		return true;
	}
	return false;
}

// �̪�Ă̒�~
bool EffectMng::StopEffect(std::string useName)
{
	// �w�肵���̪�Ă��Đ����Ă���
	if (_emitter[useName]->isPlaying())
	{
		_emitter[useName]->stop();
	}
	return true;
}

// �̪�Ă��Đ������ǂ���(true�ōĐ���)
bool EffectMng::CheckEffect(std::string useName)
{
	if (_emitter[useName]->isPlaying())
	{
		return true;
	}
	return false;
}

EffectMng::EffectMng()
{
	
}

EffectMng::~EffectMng()
{

}
