#include "Transform.h"
#include <iostream>

// 构造函数
Transform::Transform(GLFWwindow* window) : m_window(window), m_rotationAngle(0.0f), m_scale(1.0f) {
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 1); // 开启鼠标按钮事件的粘滞模式
}

// 析构函数
Transform::~Transform() {
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 0); // 关闭鼠标按钮事件的粘滞模式
}

// 处理鼠标拖动事件
void Transform::handleMouseDrag(double xPos, double yPos) {
    static bool isDragging = false;
    static double startX = 0.0, startY = 0.0;

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!isDragging) {
            isDragging = true;
            startX = xPos;
            startY = yPos;
        }
        else {
            double deltaX = xPos - startX;
            double deltaY = yPos - startY;

            // 根据鼠标拖动的距离更新旋转角度
            m_rotationAngle += deltaX * 0.5f;

            startX = xPos;
            startY = yPos;
        }
    }
    else {
        isDragging = false;
    }
}

// 处理滚轮事件
void Transform::handleScroll(double xOffset, double yOffset) {
    // 根据滚轮的偏移量更新缩放比例
    m_scale += yOffset * 0.1f;
    if (m_scale < 0.1f) {
        m_scale = 0.1f;
    }
}
