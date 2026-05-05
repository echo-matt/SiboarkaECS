#pragma once

#include <cstdint>   
#include <cstddef>
using Entity = uint32_t;

using ComponentID = std::size_t;

constexpr Entity NULL_ENTITY = 0;

constexpr uint32_t MAX_ENTITIES = 5000;
