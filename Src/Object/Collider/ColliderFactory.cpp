#include <algorithm> 
#include <cassert> 
#include "ColliderBase.h"
#include "ColliderCapsule.h"
#include "ColliderLine.h"
#include "ColliderSphere.h"
#include "ColliderModel.h"
#include "ColliderBox.h"
#include "ColliderFactory.h"

std::shared_ptr<ColliderBase> ColliderFactory::Create(ActorBase& owner, const Json& info)
{
	// コライダーの種類を取得
	const std::string TYPE = info["colliderType"];

	// 種類の指定がない場合
	if (TYPE == ColliderType::TYPE_NAME_MAP.at(ColliderType::TYPE::NONE))
	{
		// 空で返す
		return nullptr;
	}

	// 各種のコライダーを生成して返す
	return createColliderMap_[TYPE](owner, info);
}

void ColliderFactory::RegisterCreate(const std::string& type, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const Json&)> func)
{
	createColliderMap_[type] = func;
}

std::shared_ptr<ColliderCapsule> ColliderFactory::CreateCapsule(ActorBase& owner, const Json& info)
{
	// 情報の取得
	const CollisionTags::TAG tag = GetTagFromStringTag(info["collisionTag"]);	// タグを列挙型に変換する
	const VECTOR TOP = { info["colliderTopPos"]["x"],info["colliderTopPos"]["y"],info["colliderTopPos"]["z"] };
	const VECTOR END = { info["colliderEndPos"]["x"],info["colliderEndPos"]["y"],info["colliderEndPos"]["z"] };
	const float RADIUS = info["colliderRadius"];

	// コライダーの生成
	auto capsule = std::make_shared<ColliderCapsule>(owner, tag);

	// 各種情報の設定
	capsule->SetLocalPosTop(TOP);
	capsule->SetLocalPosDown(END);
	capsule->SetRadius(RADIUS);

	// コライダーを返す
	return capsule;
}

std::shared_ptr<ColliderLine> ColliderFactory::CreateLine(ActorBase& owner, const Json& info)
{
	// 情報の取得
	const CollisionTags::TAG TAG = GetTagFromStringTag(info["collisionTag"]);	// タグを列挙型に変換する
	const VECTOR TOP = { info["colliderTopPos"]["x"],info["colliderTopPos"]["y"],info["colliderTopPos"]["z"] };
	const VECTOR END = { info["colliderEndPos"]["x"],info["colliderEndPos"]["y"],info["colliderEndPos"]["z"] };

	// コライダーの生成
	auto line = std::make_shared<ColliderLine>(owner, TAG);

	// 各種情報の設定
	line->SetLocalPosPointHead(TOP);
	line->SetLocalPosPointEnd(END);

	// コライダーを返す
	return line;
}

std::shared_ptr<ColliderModel> ColliderFactory::CreateModel(ActorBase& owner, const Json& info)
{
	// 情報の取得
	const CollisionTags::TAG TAG = GetTagFromStringTag(info["collisionTag"]);	// タグを列挙型に変換する

	// コライダーの生成
	auto model = std::make_shared<ColliderModel>(owner, TAG);

	// コライダーを返す
	return model;
}

std::shared_ptr<ColliderSphere> ColliderFactory::CreateSphere(ActorBase& owner, const Json& info)
{
	// 情報の取得
	const CollisionTags::TAG TAG = GetTagFromStringTag(info["collisionTag"]);	// タグを列挙型に変換する
	const float RADIUS = info["colliderRadius"];

	// コライダーの生成
	auto sphere = std::make_shared<ColliderSphere>(owner, TAG);

	// 情報の設定
	sphere->SetRadius(RADIUS);

	// コライダーを返す
	return sphere;
}

std::shared_ptr<ColliderBox> ColliderFactory::CreateBox(ActorBase& owner, const Json& info)
{
	// 情報の取得
	const CollisionTags::TAG TAG = GetTagFromStringTag(info["collisionTag"]);	// タグを列挙型に変換する
	const VECTOR TOP = { info["colliderTopPos"]["x"],info["colliderTopPos"]["y"],info["colliderTopPos"]["z"] };
	const VECTOR END = { info["colliderEndPos"]["x"],info["colliderEndPos"]["y"],info["colliderEndPos"]["z"] };

	// コライダーの生成
	auto box = std::make_shared<ColliderBox>(owner, TAG);

	// 各種情報の設定
	box->SetVecMax(TOP);
	box->SetVecMin(END);

	// コライダーを返す
	return box;
}

CollisionTags::TAG ColliderFactory::GetTagFromStringTag(const std::string& tag)
{
	auto& map = CollisionTags::TAG_NAME_MAP;
	auto it = std::find_if(map.begin(), map.end(),
		[&tag](const auto& pair) {return pair.second == tag; });

	// 変換が出来なかった場合
	if (it == map.end())
	{
		assert(false && L"対応していない衝突物の種類を受け取った");
		return CollisionTags::TAG::NONE;
	}

	// 変換した型を返す
	return it->first;
}

ColliderFactory::ColliderFactory()
{
	createColliderMap_.clear();

	// 生成処理の登録
	using C_TYPE = ColliderType::TYPE;
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::CAPSULE), [this](ActorBase& owner, const Json& info) -> std::shared_ptr<ColliderBase> { return CreateCapsule(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::LINE), [this](ActorBase& owner, const Json& info) -> std::shared_ptr<ColliderBase> { return CreateLine(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::MODEL), [this](ActorBase& owner, const Json& info)  -> std::shared_ptr<ColliderBase> { return CreateModel(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::SPHERE), [this](ActorBase& owner, const Json& info) -> std::shared_ptr<ColliderBase> { return CreateSphere(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::BOX), [this](ActorBase& owner, const Json& info) -> std::shared_ptr<ColliderBase> { return CreateBox(owner, info); });
}

ColliderFactory::~ColliderFactory()
{
	createColliderMap_.clear();
}