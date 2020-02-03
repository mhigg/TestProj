﻿#include "TrajectControl.h"
#include "../Manager/GameManager.h"
#include "../Obj/Ball.h"
#include "../Obj/Player.h"

USING_NS_CC;

TrajectControl::TrajectControl() : _speed(8.f,8.f,8.f)
{
	_vel = _speed;
}

TrajectControl::~TrajectControl()
{
}

/// 正規化済みのベクトルを取得している
bool TrajectControl::CalBezierPoint(const cocos2d::Vec2& vec)
{
	/// 端点の生成に必要なものを取得している
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// 画面サイズ / 2 - ボールの半径をカーブできる移動範囲に設定してみるか？　◆
	Vec2 distance = Vec2(500 * vec.x, 400 * vec.y);
	/// 一次ベジェの曲線を生成するために必要なもの
	Vec3 start, mid, end;
	float a, b, endDepth;

	/// 終端の深度値を設定している
	endDepth = (std::get<2>(ball->GetIsReverse()) ? gameMng->GetDepths()[gameMng->GetWallMax() - 1] : gameMng->GetDepths()[0]);

	start	= Vec3(ball->GetLocalPos().x, ball->GetLocalPos().y, ball->GetLocalPos().z);
	mid = Vec3(ball->GetLocalPos().x + distance.x, ball->GetLocalPos().y + distance.y, gameMng->GetDepths()[gameMng->GetWallMax() / 2]);
	end		= Vec3(ball->GetLocalPos().x - distance.x, ball->GetLocalPos().y - distance.y, endDepth);				/// 仮の設定　◆

	for (int i = 0; i < _points.size(); ++i)
	{
		b = (float)i / _points.size();
		a = 1.f - b;

		/// 端点の登録をしている
		_points[i].x = (a * a * start.x) + (2 * a * b * mid.x) + (b * b * end.x);
		_points[i].y = (a * a * start.y) + (2 * a * b * mid.y) + (b * b * end.y);
		_points[i].z = (a * a * start.z) + (2 * a * b * mid.z) + (b * b * end.z);
	}
	return true;
}

void TrajectControl::SetVel(const cocos2d::Vec2 & vec)
{
	/// ベクトルの最低値を設定するためのもの
	auto Clamp = [](const float& vec)
	{
		if (vec >= 0.05f)
		{
			return fmin(1.f, fmax(0.1f, vec));
		}
		return vec;
	};

	_vel = Vec3(_speed.x * Clamp(abs(vec.x)), _speed.y * Clamp(abs(vec.y)), _speed.z);
}

cocos2d::Vec3 TrajectControl::GetVel(const State& state)
{
	if (state == State::NORMAL)
	{
		/// デフォルトの速度を返す
		return CalNormalVel();
	}
	else if (state == State::CURVE)
	{
		/// 計算した速度を返す
		return CalCurveVel();
	}
	else{}
	return Vec3::ZERO;
}

cocos2d::Vec3 TrajectControl::CalNormalVel()
{
	/// ボールの情報を取得している
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// 速度を反転させるかのフラグを取得している
	auto isReverse	= ball->GetIsReverse();
	cocos2d::Vec3 vel;

	/// 速度の設定
	vel.x = (!std::get<0>(isReverse) ? _vel.x : -_vel.x);
	vel.y = (!std::get<1>(isReverse) ? _vel.y : -_vel.y);
	vel.z = (!std::get<2>(isReverse) ? _vel.z : -_vel.z);

	return vel;
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// ボールの情報を取得している
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng  = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball	  = (Ball*)gameMng->getChildByName("ball");

	/// 進行方向のベクトルを保存するもの
	Vec3 vec;
	float rate = 1.5f;
	
	/// ベクトルの取得をしている
	for (int i = 0; i < _points.size(); ++i)
	{
		if (!std::get<2>(ball->GetIsReverse()))
		{
			/// 手前→奥のZ軸の取得
			if (_points[i].z > ball->GetLocalPos().z)
			{
				vec = (_points[i] - ball->GetLocalPos());
				vec.normalize();
				_vel = Vec3(abs(_speed.x * vec.x * rate), abs(_speed.y * vec.y * rate), _speed.z);
				return Vec3(_speed.x * vec.x * rate, _speed.y * vec.y * rate, _speed.z * vec.z * rate);
			}
		}
		else
		{
			/// 奥→手前のZ軸の取得
			if (_points[i].z < ball->GetLocalPos().z)
			{
				vec = (_points[i] - ball->GetLocalPos());
				vec.normalize();
				_vel = Vec3(abs(_speed.x * vec.x * rate), abs(_speed.y * vec.y * rate), _speed.z);
				return Vec3(_speed.x * vec.x * rate, _speed.y * vec.y * rate, _speed.z * vec.z * rate);
			}
		}
	}

	/// 一定の深度値になった場合、真っ直ぐ飛ばすようにしている
	float velZ = (!std::get<2>(ball->GetIsReverse()) ? _speed.z : -_speed.z);
	return cocos2d::Vec3(0, 0, velZ);
}
