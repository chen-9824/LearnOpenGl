#ifndef P1_5_SHADER_H
#define P1_5_SHADER_H

#pragma once

class P1_5_Shader
{
public:
    P1_5_Shader();
    ~P1_5_Shader();

    // 通过全局变量动态设定颜色
    int draw_triangle_uniform_color();
    // 通过顶点数组直接指定颜色
    int draw_triangle_VAO_color();

protected:
    GLFWwindow *window;

    void init();
    GLFWwindow *create_window(const std::string &title);
    static void framebuffer_size_cb(GLFWwindow *window, int width, int height);
    static void process_input(GLFWwindow *window);

private:
};

#endif