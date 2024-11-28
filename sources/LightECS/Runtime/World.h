﻿#pragma once
#include <set>
#include <cassert>
#include "Heap.h"
#include "LightECS/Runtime/Archetype.hpp"
#include "_Concept.hpp"

namespace Light
{
    struct EntityInfo
    {
        const Archetype* archetype;
        std::byte* components;
        int indexAtHeap;
    };

    struct SystemInfo
    {
        using Function = void(*)();

        std::type_index type = typeid(void);
        int order;
        Function start;
        Function update;
        Function stop;

        bool operator<(const SystemInfo& other) const
        {
            if (order == other.order)
                return type.hash_code() < other.type.hash_code();

            return order < other.order;
        }
    };

    class World
    {
    public:
        static Heap* GetEntityHeap(const Archetype& archetype)
        {
            auto iterator = entities.find(&archetype);
            if (iterator == entities.end())
                return nullptr;
            return &iterator->second;
        }
        static EntityInfo GetEntityInfo(Entity entity);
        static bool HasEntity(Entity entity);

        static Entity AddEntity(const Archetype& archetype);
        template <Component... TComponents>
        static Entity AddEntity(const Archetype& archetype, const TComponents&... components)
        {
            Entity entity = AddEntity(archetype);
            PasteComponents(entity, components...);
            return entity;
        }
        static void AddEntities(const Archetype& archetype, int count, Entity* outEntities = nullptr);
        static void MoveEntity(Entity entity, const Archetype& newArchetype);
        static void RemoveEntity(Entity entity);

        template <System TSystem>
        static void AddSystem()
        {
            SystemInfo systemInfo;
            systemInfo.type = typeid(TSystem);
            systemInfo.order = TSystem::Order;
            systemInfo.start = TSystem::Start;
            systemInfo.update = TSystem::Update;
            systemInfo.stop = TSystem::Stop;
            systemStartQueue.insert(systemInfo);
        }
        template <System TSystem>
        static void RemoveSystem()
        {
            SystemInfo key;
            key.type = typeid(TSystem);
            key.order = TSystem::Order;
            SystemInfo systemInfo = systemUpdateQueue.extract(key).value();
            systemStopQueue.insert(systemInfo);
        }

        static Heap& GetEntities(const Archetype& archetype)
        {
            auto iterator = entities.find(&archetype);
            if (iterator != entities.end())
                return iterator->second;

            entities.insert({&archetype, Heap(archetype.size)});
            return entities.at(&archetype);
        }
        template <Component TComponent>
        static TComponent& GetComponent(const Entity entity)
        {
            assert(entityInfos.contains(entity) && "实体不存在！");
            EntityInfo& entityInfo = entityInfos.at(entity);
            int offset = entityInfo.archetype->GetOffset(typeid(TComponent));
            return *reinterpret_cast<TComponent*>(entityInfo.components + offset);
        }
        template <Component... TComponents>
        static void GetComponents(const Entity entity, TComponents**... outComponents)
        {
            EntityInfo& entityInfo = entityInfos.at(entity);
            const Archetype& archetype = *entityInfo.archetype;
            ((*outComponents = reinterpret_cast<TComponents*>(entityInfo.components + archetype.GetOffset(typeid(TComponents)))), ...);
        }
        template <Component... TComponents>
        static void CopyComponents(const Entity entity, TComponents*... outComponents)
        {
            EntityInfo& entityInfo = entityInfos.at(entity);
            const Archetype& archetype = *entityInfo.archetype;
            ((*outComponents = *reinterpret_cast<TComponents*>(entityInfo.components + archetype.GetOffset(typeid(TComponents)))), ...);
        }
        template <Component... TComponents>
        static void PasteComponents(const Entity entity, const TComponents&... outComponents)
        {
            EntityInfo& entityInfo = entityInfos.at(entity);
            const Archetype& archetype = *entityInfo.archetype;
            ((*reinterpret_cast<TComponents*>(entityInfo.components + archetype.GetOffset(typeid(TComponents))) = outComponents), ...);
        }

        static void Update();
        static void Clear();

    private:
        friend struct HierarchyWindow;
        inline static uint32_t nextEntity = 1;
        inline static std::unordered_map<const Archetype*, Heap> entities = {};
        inline static std::unordered_map<Entity, EntityInfo> entityInfos = {};
        inline static std::set<SystemInfo> systemStartQueue = {};
        inline static std::set<SystemInfo> systemStopQueue = {};
        inline static std::set<SystemInfo> systemUpdateQueue = {};

        static void RemoveHeapItem(const Archetype& archetype, int index);
    };
}