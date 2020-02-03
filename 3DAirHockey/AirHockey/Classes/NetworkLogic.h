﻿#pragma once

#include <array>
#include <queue>

#include "OutputListener.h"
#include "LoadBalancing-cpp/inc/Client.h"

#ifdef _EG_PS4_PLATFORM
#	include "AuthenticationParameters.h"
#endif

enum State
{
	STATE_INITIALIZED = 0,	// 初期接続
	STATE_CONNECTING,		// 通信接続中
	STATE_CONNECTED,		// 通信接続状態
	STATE_JOINING,			// ｹﾞｰﾑに参加中
	STATE_JOINED,			// ｹﾞｰﾑに参加状態
	STATE_LEAVING,			// ｹﾞｰﾑから離脱中
	STATE_LEFT,				// ｹﾞｰﾑから離脱状態
	STATE_DISCONNECTING,	// 通信切断中
	STATE_DISCONNECTED		// 通信切断状態
};

enum Input
{
	INPUT_NON = 0,	// 未接続
	INPUT_1,		// 接続1番目:ルームの作成
	INPUT_2,		// 接続2番目以降:ルームへの参加
	INPUT_3,
	INPUT_4,
	INPUT_EXIT
};

class NetworkLogicListener : public ExitGames::Common::ToString
{
public:
	using ExitGames::Common::ToString::toString;
	virtual void stateUpdate(State newState) = 0;
	virtual ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, bool withTypes = false) const;
};

class StateAccessor
{
public:
	State getState(void) const;
	void setState(State newState);
	void registerForStateUpdates(NetworkLogicListener* listener);

private:
	State mState;
	ExitGames::Common::JVector<NetworkLogicListener*> mStateUpdateListeners;
};

class NetworkLogic : private ExitGames::LoadBalancing::Listener
{
public:
	NetworkLogic(OutputListener* listener, const EG_CHAR* appID);
	void registerForStateUpdates(NetworkLogicListener* listener);
	void run(void);
	void disconnect(void);
	void connect(void);

	void opCreateRoom(const ExitGames::Common::JString& roomName, int maxPlayers, nByte directMode);
	void opJoinOrCreateRoom(void);
	void opJoinRandomOrCreateRoom(void);
	void opJoinRoom(const ExitGames::Common::JString& roomID, bool rejoin = false);
	void opJoinRandomRoom(void);

	// イベントを送信するメソッド
	void sendEvent(void);
	void sendEvent(nByte code, ExitGames::Common::Hashtable *eventContent);

	Input getLastInput(void) const;
	void setLastInput(Input newInput);
	State getState(void) const;

	// ルームが存在するか否かを返すメソッド
	bool isRoomExists(void);

	// 自分のプレイヤー番号
	int playerNr = 0;
	// イベントキュー
	std::queue<std::array<float, 3>> eventQueue;

#ifdef _EG_XB1_PLATFORM
	void setXSTSToken(const ExitGames::Common::JVector<nByte>& mXSTSToken);
#elif defined _EG_SWITCH_NX_PLATFORM
	void setIDToken(const ExitGames::Common::JString& IDToken);
#endif
private:
	void sendDirect(int64 count);

// ----------------------------------- mListener's methods --------------------------------------
	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);

	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);
	virtual void onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay);
	// callbacks for operations on PhotonLoadBalancing server = server's response?
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);
//------------------------------------------------------------------------------------------

	ExitGames::LoadBalancing::Client mLoadBalancingClient;
	ExitGames::Common::JString mLastJoinedRoom;
	ExitGames::Common::JString mUserID;
	ExitGames::Common::Logger mLogger;
	StateAccessor mStateAccessor;
	Input mLastInput;
	OutputListener* mpOutputListener;
	bool mAutoJoinRoom;
	ExitGames::Common::EGTime mLastSendTime;
#ifdef _EG_XB1_PLATFORM
	ExitGames::Common::JVector<nByte> mXSTSToken;
	bool mReauthenticateRequired;
#elif defined _EG_SWITCH_NX_PLATFORM
	ExitGames::Common::JString mIDToken;
#endif
};