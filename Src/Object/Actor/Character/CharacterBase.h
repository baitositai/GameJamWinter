#pragma once
#include <unordered_map>
#include <functional>
#include "../ActorBase.h"

class ColliderCapsule;
class AnimationController;

class CharacterBase : public ActorBase
{
public:

	/// <summary>
	/// キャラクター共通状態
	/// </summary>
	enum class STATE
	{
		NONE,	// なし
		ACTION,	// メインアクション
		FALL,	// 落下(今回は処理が共通)
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="initPos">初期位置</param>
	CharacterBase(const VECTOR& initPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase() override;
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override;	
	
	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(const STATE state);

	/// <summary>
	/// コライダー情報を返す
	/// </summary>
	/// <returns>コライダー情報</returns>
	const std::shared_ptr<ColliderCapsule>& GetCapsule() const { return collider_; }

protected:

	//カプセル用半径
	static constexpr float RADIUS = 60.0f;

	//プレイヤーカプセル用相対座標
	static constexpr VECTOR PL_CAP_UP_LOCAL_POS = { 0.0f, 200.0f, 0.0f };
	static constexpr VECTOR PL_CAP_DOWN_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	// 初期位置
	const VECTOR INIT_POS;

	// コライダー
	std::shared_ptr<ColliderCapsule> collider_;

	//アニメーション
	std::shared_ptr<AnimationController> anim_;

	// 状態
	STATE state_;

	// 状態遷移管理
	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	// 更新処理
	std::function<void()> update_;

	// 状態遷移処理
	void ChangeStateNone();
	void ChangeStateAction();
	virtual void ChangeStateFall();

	// 更新処理
	void UpdateNone() {};
	virtual void UpdateAction();
	virtual void UpdateFall();

	//アニメーション初期化処理
	virtual void InitAnimation(){}

	// トランスフォームの初期化処理
	void InitTransform() override;

	// コライダーの初期化
	void InitCollider() override;

	// デバッグ描画
	void DebugDraw() override;
};

