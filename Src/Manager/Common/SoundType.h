#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// ƒTƒEƒ“ƒh‚Ìí—Ş
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGM‚Ìí—Ş
	enum class BGM
	{
		TITLE,
		GAME,
		MAX
	};

	// SE‚Ìí—Ş
	enum class SE
	{
		SELECT,
		DECISION,
		FALL,
		MAX
	};

	// BGM‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SE‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, SE> KEY_SE_MAP;
};

