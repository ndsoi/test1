#include "Transform.h"
#include <iostream>

// ���캯��
Transform::Transform(GLFWwindow* window) : m_window(window), m_rotationAngle(0.0f), m_scale(1.0f) {
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 1); // ������갴ť�¼���ճ��ģʽ
}

// ��������
Transform::~Transform() {
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 0); // �ر���갴ť�¼���ճ��ģʽ
}

// ��������϶��¼�
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

            // ��������϶��ľ��������ת�Ƕ�
            m_rotationAngle += deltaX * 0.5f;

            startX = xPos;
            startY = yPos;
        }
    }
    else {
        isDragging = false;
    }
}

// ��������¼�
void Transform::handleScroll(double xOffset, double yOffset) {
    // ���ݹ��ֵ�ƫ�����������ű���
    m_scale += yOffset * 0.1f;
    if (m_scale < 0.1f) {
        m_scale = 0.1f;
    }
}
