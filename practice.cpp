#include "practice.h"

#include "pch.h"

void framebuffer_size_cb(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource1 = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                    "}\n\0";

GLFWwindow *init(const std::string &title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), nullptr, nullptr);
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
};

int create_shader_program(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    unsigned int shader_program;
    // 顶点着色器
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex_shader);
    int sucess;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), nullptr, info_log);
        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED: {}", info_log);
    }

    // 片段着色器
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), nullptr, info_log);
        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: {}", info_log);
    }

    // 着色器程序及链接
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
        spdlog::error("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: {}", info_log);
    }
    // 链接之后着色器即可删除
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

int draw_single_triangle()
{

    GLFWwindow *window = init("draw_single_triangle");
    unsigned int shader_program = create_shader_program(vertexShaderSource1, fragmentShaderSource1);

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
    // VAO将保存：
    // 1.glEnableVertexAttribArray和glDisableVertexAttribArray的调用
    // 2.通过glVertexAttribPointer设置的顶点属性配置
    // 3.通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象（VBO）
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
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}

int draw_double_triangle()
{

    GLFWwindow *window = init("draw_double_triangle");
    unsigned int shader_program1 = create_shader_program(vertexShaderSource1, fragmentShaderSource1);
    unsigned int shader_program2 = create_shader_program(vertexShaderSource1, fragmentShaderSource2);

    // 顶点数组对象
    float vertices1[] = {
        -0.5f, 0.0f, 0.0f, // left
        0.5f, 0.0f, 0.0f,  // right
        0.0f, 0.5f, 0.0f   // top
    };
    float vertices2[] = {
        -0.5f, 0.0f, 0.0f, // left
        0.5f, 0.0f, 0.0f,  // right
        0.0f, -0.5f, 0.0f};

    // 创建顶点数组对象
    int num = 2;
    unsigned int VAO[num], VBO[num];
    glGenVertexArrays(num, VAO);
    // 绑定 之后的 VBO 绑定都会记录在这个 VAO 上
    glBindVertexArray(VAO[0]);

    // 创建顶点缓冲对象
    glGenBuffers(num, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    // 设置顶点属性指针, 如何解析顶点数据， 任何随后的顶点属性调用都会储存在这个VAO中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);
    // VBO在设置顶点属性指针之后即可解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VAO 也可解绑
    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
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
        glUseProgram(shader_program1);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shader_program2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(0, VAO);
    glDeleteBuffers(0, VBO);
    glDeleteProgram(shader_program1);
    glDeleteProgram(shader_program2);

    glfwTerminate();
    return 0;
}

int draw_single_rectangle()
{
    GLFWwindow *window = init("draw_single_rectangle");
    unsigned int shader_program = create_shader_program(vertexShaderSource1, fragmentShaderSource1);

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    // 创建顶点数组对象
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    // 绑定
    glBindVertexArray(VAO);

    // 创建顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 创建元素缓冲对象
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 顶点数据复制到缓冲中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针, 如何解析顶点数据， 任何随后的顶点属性调用都会储存在这个VAO中
    // VAO将保存：
    // 1.glEnableVertexAttribArray和glDisableVertexAttribArray的调用
    // 2.通过glVertexAttribPointer设置的顶点属性配置
    // 3.通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象（VBO）
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
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, &VAO);
    glDeleteBuffers(2, &VBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        spdlog::info("press KEY_ESCAPE, Window Should Close");
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_cb(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}