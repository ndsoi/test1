#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GLFW/glfw3.h>

class Transform {
public:
    Transform(GLFWwindow* window);
    ~Transform();

    void handleMouseDrag(double xPos, double yPos);
    void handleScroll(double xOffset, double yOffset);

private:
    GLFWwindow* m_window;
    float m_rotationAngle;
    float m_scale;
};

#endif
#pragma once
