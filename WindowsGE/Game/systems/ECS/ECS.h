#pragma once
#include <iostream>
#include <unordered_map>
#include <SDL.h>
#include "../../Functions.h"
#include <bitset>
#include <queue>
#include <array>
#include <set>


namespace GE {
	// A simple type alias
	using Entity = std::uint32_t;
	// Used to define the size of arrays later on
	const Entity MAX_ENTITIES = 50;
	// A simple type alias
	using ComponentType = std::uint8_t;
	// Used to define the size of arrays later on
	const ComponentType MAX_COMPONENTS = 8;
	using Signature = std::bitset<MAX_COMPONENTS>;
}