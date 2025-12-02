#pragma once
#include <map>
#include <string>

class ColliderType
{
public:

	/// <summary>
	/// 種類
	/// </summary>
	enum class TYPE
	{
		NONE,
		CAPSULE,
		SPHERE,
		LINE,
		MODEL,
		BOX,
		MAX
	};

	// コライダー種別名前管理マップ
	static const std::map<TYPE, std::string> TYPE_NAME_MAP;

	// コライダー種類数
	static constexpr int COLLIDER_TYPES = static_cast<int>(TYPE::MAX);
};