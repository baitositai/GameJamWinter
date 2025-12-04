#pragma once
#include <nlohmann/json.hpp>
#include "../Common/Transform.h"

class ResourceManager;
class SceneManager;
class CollisionManager;
class ColliderFactory;
class ColliderBase;
enum class COLLISION_TAG;

class ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ActorBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ActorBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary> 
	/// メインの更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	virtual void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider);

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns>トランスフォーム</returns>
	const Transform& GetTransform() const { return transform_; }

	/// <summary>
	/// 活動状態判定
	/// </summary>
	/// <returns>活動状態判定</returns>
	const bool IsActive() const { return isActive_; }

	/// <summary>
	/// 削除判定
	/// </summary>
	/// <returns>削除判定</returns>
	const bool IsDelete() const { return isDelete_; }

	/// <summary>
	/// 削除判定を設定
	/// </summary>
	void SetDelete() { isDelete_ = true; }

	/// <summary>
	/// 活動状態判定を設定
	/// </summary>
	/// <param name="isActive">活動状態判定</param>
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

protected:

	// ローカル回転のデフォルト値(度)
	static constexpr float DEFAULT_LOCAL_QUAROT_Y_DEG = 180.0f;

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;
	CollisionManager& collMng_;
	ColliderFactory& collFtr_;

	// 活動状態判定
	bool isActive_;

	// 削除判定	
	bool isDelete_;

	// モデル制御の基本情報
	Transform transform_;

	//コライダー
	std::shared_ptr<ColliderBase> collider_;

	// トランスフォームの初期設定
	virtual void InitTransform();

	// メインの更新処理
	virtual void UpdateBody();

	// 処理の適用
	virtual void UpdateApply();

	// メインの描画
	virtual void DrawMain();

	// 半透明の描画
	virtual void DrawTranslucent();

	// UIの描画
	virtual void DrawUI();

	// コライダーの追加
	virtual void AddCollider();

	// デバッグ時の更新
	virtual void DebugUpdate();

	// デバッグ時の描画
	virtual void DebugDraw();
};