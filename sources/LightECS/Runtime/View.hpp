﻿#pragma once
#include "LightECS/Runtime/EntitySet.h"

template <class TFunction, class... TComponents>
concept Iterator = requires(TFunction function, TComponents&... components) { function(components...); };

template <class TFunction, class... TComponents>
concept IteratorWithEntity = requires(TFunction function, Entity& entity, TComponents&... components) { function(entity, components...); };

/**
 * @brief 针对实体堆的检视工具
 * 
 * 实体堆 <code> std::vector<Heap> </code> 是一种根据原形顺序生成的实体容器。默认情况下实体及其组件是以字节序列的形式存放在实体堆中，所以并不利于读写。
 * 利用\c View 则可自动识别所需组件的存储位置，并将其转换成组件引用的形式供使用者遍历，从而方便的对实体的批量处理。
 * 
 * @tparam TComponents 
 */
template <class... TComponents>
    requires (sizeof...(TComponents) != 0)
class View
{
public:
    View(EntitySet& entitySet)
        : entitySet(&entitySet)
    {
        if (isQueried == false)
        {
            for (const Archetype* archetype : Archetype::allArchetypes)
            {
                if (archetype->Contains<TComponents...>())
                {
                    targetArchetypes.emplace_back(archetype);
                    targetComponentOffsets.emplace_back(archetype->MemoryMap<TComponents...>());
                }
            }

            targetArchetypeCount = static_cast<int>(targetArchetypes.size());
            isQueried = true;
        }
    }

    template <class TFunction> requires Iterator<TFunction, TComponents...>
    void Each(TFunction function) const
    {
        Each_Inner(function, std::make_index_sequence<sizeof...(TComponents)>());
    }
    template <class TFunction> requires IteratorWithEntity<TFunction, TComponents...>
    void Each(TFunction function) const
    {
        Each_Inner(function, std::make_index_sequence<sizeof...(TComponents)>());
    }

    template <class TComponent>
    TComponent& Fetch(const Entity entity)
    {
        EntityInfo* entityInfo = entitySet->GetEntityInfo(entity);
        int offset = entityInfo->archetype->GetOffset(&typeid(TComponent));
        return *reinterpret_cast<TComponent*>(entityInfo->components + offset);
    }

private:
    inline static bool isQueried = false;
    inline static std::vector<const Archetype*> targetArchetypes = {};
    inline static std::vector<std::array<int, sizeof...(TComponents)>> targetComponentOffsets = {};
    inline static int targetArchetypeCount = {};

    EntitySet* entitySet;

    template <class TFunction, size_t... Indices> requires Iterator<TFunction, TComponents...>
    void Each_Inner(TFunction function, std::index_sequence<Indices...>) const
    {
        for (int i = 0; i < targetArchetypeCount; i++)
        {
            const Archetype& archetype = *targetArchetypes[i];
            const std::array<int, sizeof...(TComponents)>& componentOffset = targetComponentOffsets[i];

            Heap* entityHeap = entitySet->GetEntityHeap(archetype);
            if (entityHeap != nullptr)
                entityHeap->ForeachElements([function,componentOffset](std::byte* item)
                {
                    function(*reinterpret_cast<TComponents*>(item + componentOffset[Indices])...);
                });
        }
    }
    template <class TFunction, size_t... Indices> requires IteratorWithEntity<TFunction, TComponents...>
    void Each_Inner(TFunction function, std::index_sequence<Indices...>) const
    {
        for (int i = 0; i < targetArchetypeCount; i++)
        {
            const Archetype& archetype = *targetArchetypes[i];
            const std::array<int, sizeof...(TComponents)>& componentOffset = targetComponentOffsets[i];

            Heap* entityHeap = entitySet->GetEntityHeap(archetype);
            if (entityHeap != nullptr)
                entityHeap->ForeachElements([function,componentOffset](std::byte* item)
                {
                    function(*reinterpret_cast<Entity*>(item), *reinterpret_cast<TComponents*>(item + componentOffset[Indices])...);
                });
        }
    }
};
