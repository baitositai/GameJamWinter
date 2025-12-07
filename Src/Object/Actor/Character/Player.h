#pragma once
#include <memory>
#include <vector>
#include "../../../Manager/Common/InputManager.h"
#include "CharacterBase.h"

class Timer;

class Player : public CharacterBase
{
public:

	enum class ACTION_STATE
	{
		NONE,
		MOVE,
		SET_PITFALL,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="initPos">初期位置</param>
	/// <param name="padNo">PAD番号</param>
	Player(const VECTOR& initPos, const Input::JOYPAD_NO padNo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 落とし穴の生成位置を返す
	/// </summary>
	/// <returns>落とし穴の生成位置</returns>
	const VECTOR& GetCreatePitFallPos() const { return createPitFallPos_; }

	/// <summary>
	/// 落とし穴の生成判定を返す
	/// </summary>
	/// <returns>落とし穴の生成判定</returns>
	const bool IsCreatePitFall() const { return isCreatePitFall_; }

	/// <summary>
	/// 落とし穴が生成可能状態へ戻す
	/// </summary>
	void SetCreatePitFall() { isCreatePitFall_ = true; }

private:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.6f;

	//移動速度
	static constexpr float SPEED_MOVE = 5.0f;

	//プレイヤー回転角度
	static constexpr double ROT_DEG_R = 90;
	static constexpr double ROT_DEG_L = -90;
	static constexpr double ROT_DEG_F = 0;
	static constexpr double ROT_GEG_B = 180;

	// 生成位置までの距離
	static constexpr float DISTANCE = 150.0f;

	// 復帰までの時間
	static constexpr float RESPOWN_TIME = 2.0f;

	// PAD番号
	const Input::JOYPAD_NO MY_PAD_NO;

	//移動速度
	float moveSpeed_;

	// 落とし穴の生成判定
	bool isCreatePitFall_;

	// 落とし穴を生成する位置
	VECTOR createPitFallPos_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// アクション用状態
	ACTION_STATE actionState_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 更新処理
	std::function<void()> actionUpdate_;

	// アクション処理の管理
	std::unordered_map<ACTION_STATE, std::function<void()>> changeActionStateMap_;

	// アクションの更新処理
	void UpdateAction() override;
	void UpdateFall() override;

	// 状態遷移
	void ChangeActionState(const ACTION_STATE state);
	void ChangeActionStateNone();
	void ChangeActionStateMove();
	void ChangeActionStateSetPitFall();

	// 更新処理
	void UpdateActionMove();
	void UpdateActionSetPitFall();

	// 操作
	void ProcessMove();
	void ProcessCreatePitFall();

	// 移動制限
	void MoveLimit();

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate();
};