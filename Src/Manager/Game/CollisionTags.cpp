#include "CollisionTags.h"

const std::map<CollisionTags::TAG, std::string> CollisionTags::TAG_NAME_MAP =
{
	{CollisionTags::TAG::NONE, "none"},
	{CollisionTags::TAG::PLAYER, "player"},
	{CollisionTags::TAG::STAGE_MAIN, "stageMain"},
};