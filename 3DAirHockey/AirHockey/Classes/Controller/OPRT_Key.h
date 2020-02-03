﻿#pragma once
#include "OPRT_State.h"

// ｷｰﾎﾞｰﾄﾞ入力用ｸﾗｽ
class OPRT_Key :
	public OPRT_State
{
public:
	OPRT_Key();
	OPRT_Key(cocos2d::Node* sprite);
	~OPRT_Key();

	cocos2d::Vec2 GetPoint(void) const;
private:
	void Update(void);		// 状態更新
};


