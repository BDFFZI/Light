﻿#include "GameUISystem.h"

#include <imgui.h>
#include <magic_enum.hpp>

#include "LogicSystem.h"
#include "Editor/EditorUIUtility.h"
#include "LightWindow/Runtime/Window.h"

using namespace Light;

void GameUISystem::Update()
{
    if (Input::TopInputHandler() != LogicSystem.GetInputHandler())
    {
        //显示编辑模式
        const char* editModeOptions[] = {
            magic_enum::enum_name(static_cast<EditMode>(0)).data(),
            magic_enum::enum_name(static_cast<EditMode>(1)).data(),
            magic_enum::enum_name(static_cast<EditMode>(2)).data(),
            magic_enum::enum_name(static_cast<EditMode>(3)).data(),
        };
        ImGui::Combo("EditMode", reinterpret_cast<int*>(&LogicSystem.GetEditMode()), editModeOptions, std::size(editModeOptions));
        //显示模拟状态
        ImGui::Checkbox("Simulating", &LogicSystem.GetSimulating());
        //显示鼠标位置
        ImGui::InputFloat2("MousePosition", Input::GetMousePosition().data);
        //显示鼠标所在的点
        ImGui::Text("CoveringPoint");
        ImGui::SameLine();
        EditorUIUtility::DrawEntityButton(LogicSystem.GetCoveringPoint());
        //退出游戏按钮
        if (ImGui::Button("ExitGame"))
            Window::Stop();
    }
}
