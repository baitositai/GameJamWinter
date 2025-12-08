#include "SoundType.h"

// •¶Žš—ñ‚©‚çenum ‚Ì‘Î‰ž•\
const std::map<std::string, SoundType::BGM> SoundType::KEY_BGM_MAP =
{
	{"bgmTitle",SoundType::BGM::TITLE},
	{"bgmGame", SoundType::BGM::GAME},
};

const std::map<std::string, SoundType::SE> SoundType::KEY_SE_MAP =
{
	{"seSelect", SoundType::SE::SELECT},
	{"seDecision", SoundType::SE::DECISION},
	{"seFall", SoundType::SE::FALL},
};