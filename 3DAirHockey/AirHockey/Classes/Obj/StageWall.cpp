#include "StageWall.h"
#include "Manager/GameManager.h"

USING_NS_CC;

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	// �ð��ø����(��)�̻���
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// �摜
	//setTextureRect(deformationRect);
	setTexture("image/wall.png");
	// �ݶ��߲�Ă̐ݒ�
	setAnchorPoint({ 0.5,0.5 });
	// �F�̐ݒ�
// ����
	_normalColor = cocos2d::Color3B(0, 255, 0);
	setColor(_normalColor);
	// ���݂̐F
	_nowColor = getColor();
	// �ύX�����ۂ̐F
	_changeColor = cocos2d::Color3B(0, 0, 255);
	// �F�̕ύX�׸�
	_colorChangeFlag = false;

	// ���W
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// ��ʉ𑜓x�̎擾
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// �����_�܂ł̐��̕`��
	auto line = cocos2d::DrawNode::create();
	// ���̑���(cocos�͔��a�ɂȂ�)
	float radius = 2.0f;
	//drawSegment(�n�_,�I�_,����,�F);
	// ����
	line->drawSegment(cocos2d::Vec2(0, size.y), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// ����
	line->drawSegment(cocos2d::Vec2(0, 0), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// �E��
	line->drawSegment(cocos2d::Vec2(size.x, size.y), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// �E��
	line->drawSegment(cocos2d::Vec2(size.x, 0), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	addChild(line);

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

}

StageWall::StageWall(float zDepth, int num)
{
	// �摜
	setTexture("image/wall.png");

	// �ݶ��߲�Ă̐ݒ�
	setAnchorPoint({ 0.5,0.5 });
	// ���W
	_localPos = Vec3{ 0,0,zDepth };

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// �F�̐ݒ�
	// ����
	_normalColor = Color3B(0, 255, 0);
	setColor(_normalColor);
	// ���݂̐F
	_nowColor = getColor();
	// �ύX�����ۂ̐F
	_changeColor = Color3B(0, 0, 255);
	// �F�̕ύX�׸�
	_colorChangeFlag = false;

	// �����̕ǂ̔ԍ�
	_wallNum = num;

	// ���̕`��
	if (_wallNum == 0)
	{
		/// �Ō�̕ǂ��擾
		auto gameMng = GameManager::createGameMng();

		// �����_�܂ł̕`��
		auto line = DrawNode::create();
		// �摜�̻��ގ擾
		auto size = getContentSize();
		// �Ō�̕ǂ�Pos
		auto endWallPos = size / 2;
		// �Ō�̕ǂ̑傫��
		auto endWallScale = lpPointWithDepth.GetScale(gameMng->GetMaxDepth() - 1);
		// ���̑���(cocos�͔��a�ɂȂ�)
		float radius = 1.0f;
		// �F
		auto lineColor = Color4F::WHITE;
		// ����
		line->drawSegment(
			Vec2(0, size.height),
			Vec2((endWallPos.width - ((size.width / 2) * endWallScale)), (endWallPos.height + ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// ����
		line->drawSegment(
			Vec2(0, 0),
			Vec2((endWallPos.width - ((size.width / 2) * endWallScale)), (endWallPos.height - ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// �E��
		line->drawSegment(
			Vec2(size.width, size.height),
			Vec2((endWallPos.width + ((size.width / 2) * endWallScale)), (endWallPos.height + ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// �E��
		line->drawSegment(
			Vec2(size.width, 0),
			Vec2((endWallPos.width + ((size.width / 2) * endWallScale)), (endWallPos.height - ((size.height / 2) * endWallScale))),
			radius, lineColor);

		// �e�����O�ɕ\������
		line->setGlobalZOrder(this->getGlobalZOrder() + 1);
		addChild(line);
	}

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();
}

StageWall::StageWall()
{
}

StageWall::~StageWall()
{
}

void StageWall::SetWallColorChangeFlag(bool flag)
{
	_colorChangeFlag = flag;
}

void StageWall::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// �Q�[���V�[���ȊO�̎��͏����ɓ���Ȃ��悤�ɂ���
		return;
	}

	// ���W�̍X�V
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

	// �F�̕ύX
	if (_colorChangeFlag && _nowColor != _changeColor)
	{
		// �ްْʉߎ��̐F�ɕύX
		WallColor(_changeColor);
	}
	else if (!_colorChangeFlag && _nowColor != _normalColor)
	{
		// �ʏ펞�̐F
		WallColor(_normalColor);
	}
	else{}

	if (_wallNum ==0)
	{
		/// �Ō�̕ǂ��擾
		auto gameMng = GameManager::createGameMng();
		auto layer = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
		auto endWall = (SpriteAffectDepth*)layer->getChildByName("Wall" + std::to_string(gameMng->GetWallMax()-1));

		// �q�̗v�f�S�폜(DrawLine)
		removeAllChildrenWithCleanup(true);

		// �����_�܂ł̕`��
		auto line = DrawNode::create();
		// �摜�̻��ގ擾
		auto size = getContentSize();
		// �Ō�̕ǂ�Pos
		auto endWallPos = endWall->getPosition();
		// �Ō�̕ǂ̑傫��
		auto endWallScale =endWall->getScale();
		// ���̑���(cocos�͔��a�ɂȂ�)
		float radius = 1.0f;
		// �F
		auto lineColor = Color4F::WHITE;
		// ����
		line->drawSegment(
			Vec2(0, size.height),
			Vec2((endWallPos.x - ((size.width / 2) * endWallScale)), (endWallPos.y + ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// ����
		line->drawSegment(
			Vec2(0, 0),
			Vec2((endWallPos.x - ((size.width / 2) * endWallScale)), (endWallPos.y - ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// �E��
		line->drawSegment(
			Vec2(size.width,size.height),
			Vec2((endWallPos.x + ((size.width / 2) * endWallScale)), (endWallPos.y + ((size.height / 2) * endWallScale))),
			radius, lineColor);
		// �E��
		line->drawSegment(
			Vec2(size.width, 0),
			Vec2((endWallPos.x + ((size.width / 2) * endWallScale)), (endWallPos.y - ((size.height / 2) * endWallScale))),
			radius, lineColor);

		//// ����
		//line->drawSegment(
		//	Vec2(0, size.height),
		//	Vec2(-(a  - b), size.height -((getPosition().y + (size.height / 2)* getScale())-(nextWallPos.y + (size.height / 2)* nextWallScale))),
		//	radius, lineColor);
		//// ����
		//line->drawSegment(
		//	Vec2(0, 0),
		//	Vec2(nextWallPos.x - (size.width / 2)* nextWallScale, nextWallPos.y - (size.height / 2)* nextWallScale),
		//	radius, lineColor);
		//// �E��
		//line->drawSegment(
		//	Vec2(size.width, size.height),
		//	Vec2(nextWallPos.x + (size.width / 2)* nextWallScale, nextWallPos.y + (size.height / 2)* nextWallScale),
		//	radius, lineColor);
		//// �E��
		//line->drawSegment(
		//	Vec2(size.width, 0),
		//	Vec2(nextWallPos.x + (size.width / 2)* nextWallScale, nextWallPos.y - (size.height / 2)* nextWallScale),
		//	radius, lineColor);

		// �e�����O�ɕ\������
		line->setGlobalZOrder(this->getGlobalZOrder()+1);
		addChild(line);
	}

}

void StageWall::WallColor(cocos2d::Color3B color)
{
	// ���݂̐F�̍X�V
	_nowColor = color;
	// �F�ύX
	setColor(_nowColor);
}
