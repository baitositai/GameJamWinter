#pragma once
#include <DxLib.h>
#include <nlohmann/json.hpp>
#include <functional>
#include <string>
#include <map>
#include "../../Template/Singleton.h"
#include "../../Manager/Game/CollisionTags.h"

class ActorBase;
class ColliderBase;
class ColliderCapsule;
class ColliderLine;
class ColliderModel;
class ColliderSphere;
class ColliderBox;

// JSON名前空間
using Json = nlohmann::json;

class ColliderFactory : public Singleton<ColliderFactory>
{

	friend class Singleton<ColliderFactory>;

public:

	/// <summary>
	/// 指定したコライダーを生成して返す
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="info">コライダー生成に関する情報</param>
	/// <returns>コライダー</returns>
	std::shared_ptr<ColliderBase> Create(ActorBase& owner, const Json& info);

private:

	// 生成処理の管理マップ
	std::map<std::string, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const Json&)>> createColliderMap_;

	// コライダー生成処理の登録処理
	void RegisterCreate(const std::string& type, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const Json&)> func);

	// 各種コライダーの生成
	std::shared_ptr<ColliderCapsule> CreateCapsule(ActorBase& owner, const Json& info);
	std::shared_ptr<ColliderLine> CreateLine(ActorBase& owner, const Json& info);
	std::shared_ptr<ColliderModel> CreateModel(ActorBase& owner, const Json& info);
	std::shared_ptr<ColliderSphere> CreateSphere(ActorBase& owner, const Json& info);
	std::shared_ptr<ColliderBox> CreateBox(ActorBase& owner, const Json& info);

	// タグをストリング型から列挙型へ変換
	CollisionTags::TAG GetTagFromStringTag(const std::string& tag);

	// コンストラクタ
	ColliderFactory();

	// デストラクタ
	~ColliderFactory();
};