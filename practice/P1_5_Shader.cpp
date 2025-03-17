#include "P1_5_Shader.h"

#include "../pch.h"
#include "Shader.h"

static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

P1_5_Shader::P1_5_Shader()
{
}

P1_5_Shader::~P1_5_Shader()
{
}

void P1_5_Shader::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = create_window("LearnOpenGl");
}

int P1_5_Shader::draw_triangle_uniform_color()
{
    init();

    Shader m_shader("../practice/shader/shader_uniform_color.vs", "../practice/shader/shader_uniform_color.fs");

    // 顶点数组对象
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };
    // 创建顶点数组对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    // 绑定 之后的 VBO 绑定都会记录在这个 VAO 上
    glBindVertexArray(VAO);

    // 创建顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针, 如何解析顶点数据， 任何随后的顶点属性调用都会储存在这个VAO中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);

    // VBO在设置顶点属性指针之后即可解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO 也可解绑
    glBindVertexArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理键盘输入
        process_input(window);

        // 清屏，显示指定背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制三角形
        // glUseProgram(shader_program);
        m_shader.use();

        // 更新uniform颜色
        float time_val = glfwGetTime();
        float green_val = sin(time_val) / 2.0f + 0.5f;
        m_shader.set4F("ourColor", 0.0f, green_val, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    m_shader.delete_shader();

    glfwTerminate();
    return 0;
}

int P1_5_Shader::draw_triangle_VAO_color()
{
    init();

    Shader m_shader("../practice/shader/shader_VAO_color.vs", "../practice/shader/shader_VAO_color.fs");

    // 顶点数组对象
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    };
    // 创建顶点数组对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    // 绑定 之后的 VBO 绑定都会记录在这个 VAO 上
    glBindVertexArray(VAO);

    // 创建顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针, 如何解析顶点数据， 任何随后的顶点属性调用都会储存在这个VAO中
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // VBO在设置顶点属性指针之后即可解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO 也可解绑
    glBindVertexArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理键盘输入
        process_input(window);

        // 清屏，显示指定背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制三角形
        m_shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    m_shader.delete_shader();

    glfwTerminate();
    return 0;
}

GLFWwindow *P1_5_Shader::create_window(const std::string &title)
{
    // 创建窗口
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), nullptr, nullptr);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return window;
    }
    // 设置当前线程渲染窗口
    glfwMakeContextCurrent(window);
    // 设置窗口大小变化时的回调，一般用于调整视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

    //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        glfwTerminate();
        return window;
    }
    return window;
}

void P1_5_Shader::framebuffer_size_cb(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void P1_5_Shader::process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        spdlog::info("press KEY_ESCAPE, Window Should Close");
        glfwSetWindowShouldClose(window, true);
    }
}
