#include "ColliderType.h"

const std::map<ColliderType::TYPE, std::string> ColliderType::TYPE_NAME_MAP =
{
	{TYPE::NONE, "none"},
	{TYPE::CAPSULE, "capsule"},
	{TYPE::LINE, "line"},
	{TYPE::MODEL, "model"},
	{TYPE::SPHERE, "sphere"},
	{TYPE::BOX, "box"},
};