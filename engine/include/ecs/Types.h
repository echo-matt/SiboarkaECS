#pragma once

#include <cstdint>   
#include <cstddef>

using Entity = uint32_t;

constexpr Entity NULL_ENTITY = 0;


static constexpr uint32_t GenerationBits = 12;
static constexpr uint32_t IndexBits = 20;

static constexpr uint32_t GenerationMask = (1u << GenerationBits) -1;
static constexpr uint32_t IndexMask = (1u << IndexBits) -1;

constexpr uint32_t MAX_ENTITIES = 5000;

using ComponentID = std::size_t;

constexpr uint32_t entityIndex (Entity entity)
{
    return entity >> GenerationBits;
}

constexpr uint32_t entityGeneration (Entity entity)
{
    return entity & GenerationMask;
}

constexpr Entity makeEntity(uint32_t index, uint32_t generation)
{
    return (index << GenerationBits) | generation;
}