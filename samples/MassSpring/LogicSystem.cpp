#include "LogicSystem.h"

#include "Archetype.hpp"
#include "Editor/InspectorWindow.h"
#include "LightWindow/Runtime/Input.h"
#include "LightWindow/Runtime/Window.h"
#include "LightECS/Runtime/View.hpp"
#include "LightWindow/Runtime/Time.h"

#include "Public/Component.hpp"
#include "Rendering/RenderingSystem.h"

using namespace Light;

void LogicSystem::Start()
{
    Input::PushInputHandler(inputHandler);
}
void LogicSystem::Stop()
{
    if (Input::TopInputHandler() == inputHandler)
        Input::PopInputHandler(inputHandler);
}

void LogicSystem::OnMovePoint()
{
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        fixedPoint = coveringPoint;
        InspectorWindow.target = fixedPoint;
    }

    if (Input::GetMouseButtonUp(MouseButton::Left))
        fixedPoint = Entity::Null;

    if (fixedPoint != Entity::Null)
        World::GetComponent<Point>(fixedPoint).position = mousePositionWS;
}
void LogicSystem::OnCreatePoint() const
{
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        const Entity entity = World::AddEntity(MassPointArchetype, Point{mousePositionWS});
        InspectorWindow.target = entity;
    }
}

std::vector<Entity> lines;
void LogicSystem::OnDeletePoint()
{
    if (Input::GetMouseButtonDown(MouseButton::Left) && coveringPoint != Entity::Null)
    {
        lines.clear();
        View<SpringPhysics>::Each([this](const Entity entity, SpringPhysics& springPhysics)
        {
            if (springPhysics.pointA == coveringPoint || springPhysics.pointB == coveringPoint)
                lines.push_back(entity);
        });
        for (Entity entity : lines)
            World::RemoveEntity(entity);
        World::RemoveEntity(coveringPoint);
    }
}
void LogicSystem::OnCreateSpring()
{
    if (springPointA == Entity::Null)
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            if (coveringPoint != Entity::Null)
            {
                springPointA = coveringPoint;
                tempLine = World::AddEntity(LineArchetype);
            }
        }
    }
    else
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            if (coveringPoint != Entity::Null && coveringPoint != springPointA)
            {
                Point pointA = World::GetComponent<Point>(springPointA);
                Point pointB = World::GetComponent<Point>(coveringPoint);
                World::AddEntity(
                    SpringArchetype,
                    SpringPhysics{
                        springPointA,
                        coveringPoint,
                        distance(pointA.position, pointB.position),
                    });
            }

            springPointA = Entity::Null;
            World::RemoveEntity(tempLine);
        }
    }

    if (tempLine != Entity::Null)
    {
        Point pointA = World::GetComponent<Point>(springPointA);
        World::SetComponents(tempLine, Line{pointA.position, mousePositionWS});
    }
}

void LogicSystem::Update()
{
    //将输入回调处理权释放给UI
    if (Input::GetKeyDown(KeyCode::LeftAlt))
    {
        if (Input::TopInputHandler() == inputHandler)
            Input::PopInputHandler(inputHandler);
        else
            Input::PushInputHandler(inputHandler);
    }

    //根据当前数值状态运行游戏逻辑
    if (Input::TopInputHandler() == inputHandler)
    {
        if (Input::GetKeyDown(KeyCode::Space))
            simulating = true;
        if (Input::GetKeyUp(KeyCode::Space))
            simulating = false;
        //获取鼠标位置
        mousePositionWS = RenderingSystem.ScreenToWorldPoint(Input::GetMousePosition());
        //获取当前鼠标覆盖的顶点
        coveringPoint = Entity::Null;
        View<Point>::Each([this](const Entity entity, const Point& point)
        {
            if (distance(point.position, mousePositionWS) < 1)
                coveringPoint = entity;
        });

        switch (editMode)
        {
        case EditMode::MovePoint: OnMovePoint();
            break;
        case EditMode::CreatePoint: OnCreatePoint();
            break;
        case EditMode::DeletePoint: OnDeletePoint();
            break;
        case EditMode::CreateSpring: OnCreateSpring();
            break;
        }
    }

    Time::SetTimeScale(simulating ? 1 : 0);
}
