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
	{"seCheers", SoundType::SE::CHEERS},
	{"seGameStart", SoundType::SE::GAME_START},
	{"seFinish", SoundType::SE::FINISH},
	{"seWhistle", SoundType::SE::WHISTLE},
	{"seScreem", SoundType::SE::SCREEM},
	{"seGameStartVoice", SoundType::SE::GAME_START_VOICE},
	{"seGameStart2", SoundType::SE::GAME_START2},
	{"seSetPitFall", SoundType::SE::SET_PIT_FALL},
	{"seApplause", SoundType::SE::APPLAUSE},
};