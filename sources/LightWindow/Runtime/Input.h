﻿#pragma once
#include <functional>
#include <stack>
#include <string>
#include <GLFW/glfw3.h>
#include "LightMath/Runtime/Math.h"

namespace LightRuntime
{
    enum class KeyCode:uint16_t
    {
        Backspace = GLFW_KEY_BACKSPACE,
        Delete = GLFW_KEY_DELETE,
        Tab = GLFW_KEY_TAB,
        Return = GLFW_KEY_ENTER,
        Esc = GLFW_KEY_ESCAPE,
        Space = GLFW_KEY_SPACE,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        Alpha0 = GLFW_KEY_0,
        Alpha1 = GLFW_KEY_1,
        Alpha2 = GLFW_KEY_2,
        Alpha3 = GLFW_KEY_3,
        Alpha4 = GLFW_KEY_4,
        Alpha5 = GLFW_KEY_5,
        Alpha6 = GLFW_KEY_6,
        Alpha7 = GLFW_KEY_7,
        Alpha8 = GLFW_KEY_8,
        Alpha9 = GLFW_KEY_9,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
    };

    enum class MouseButton:std::uint8_t
    {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    };

    struct InputHandler
    {
        std::string name;
        std::function<void()> event;
    };

    class Window;
    class Input
    {
    public:
        static void PushInputHandler(const InputHandler& inputHandler);
        static void PopInputHandler(const InputHandler& inputHandler);

        static bool GetMouseButtonDown(MouseButton mouseButton);
        static bool GetMouseButton(MouseButton mouseButton);
        static bool GetMouseButtonUp(MouseButton mouseButton);
        static bool GetKeyDown(KeyCode keyCode);
        static bool GetKey(KeyCode keyCode);
        static bool GetKeyUp(KeyCode keyCode);

        static Vector2 GetMouseScrollDelta();
        static Vector2 GetMousePosition();
        static Vector2 GetMouseMoveDelta();

    private:
        inline static std::stack<InputHandler> inputHandlers;

        inline static GLFWwindow* glfwWindow;
        inline static bool mouseButtonState[3][3];
        inline static bool keyboardState[349][3];
        inline static Vector2 mouseScrollDelta[2];
        inline static Vector2 mousePosition[2];
        inline static Vector2 mousePositionDelta;

        friend Window;
        static void Initialize(GLFWwindow* glfwWindow);
        /**
         * 每帧将GLFW传入的用户输入解析成Light所用的输入格式
         */
        static void Update();
    };
}
