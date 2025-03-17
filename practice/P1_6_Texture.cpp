#include "P1_6_Texture.h"

#include "../pch.h"
#include "Shader.h"

P1_6_Texture::P1_6_Texture() : P1_5_Shader()
{
}

P1_6_Texture::~P1_6_Texture()
{
}

int P1_6_Texture::draw_texture()
{
    init();

    Shader m_shader("../practice/shader/shader_1_6_texture.vs", "../practice/shader/shader_1_6_texture.fs");

    // 顶点数组对象
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 创建纹理对象
    unsigned int m_texture1;
    glGenTextures(1, &m_texture1);
    glBindTexture(GL_TEXTURE_2D, m_texture1);

    // 加载图像及绑定
    const char *path1 = "../practice/image/container.jpg";
    load_img(path1, GL_RGB);

    // 创建纹理对象
    unsigned int m_texture2;
    glGenTextures(1, &m_texture2);
    glBindTexture(GL_TEXTURE_2D, m_texture2);

    // 加载图像及绑定
    const char *path2 = "../practice/image/awesomeface.png";
    load_img(path2, GL_RGBA);

    // 设置顶点属性指针, 如何解析顶点数据， 任何随后的顶点属性调用都会储存在这个VAO中
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    // VBO在设置顶点属性指针之后即可解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO 也可解绑
    glBindVertexArray(0);

    m_shader.use();
    // 获取 sampler2D 变量的位置 并 绑定 sampler2D 到一个纹理单元
    m_shader.setInt("m_texture1", 0);
    m_shader.setInt("m_texture2", 1);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理键盘输入
        process_input(window);

        // 清屏，显示指定背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture2);

        m_shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

int P1_6_Texture::load_img(const char *path, GLenum img_format)
{
    int img_width, img_height, img_channels;
    stbi_set_flip_vertically_on_load(true); // 图像坐标系与opengl纹理坐标系y轴相反
    unsigned char *img_data = stbi_load(path, &img_width, &img_height, &img_channels, 0);
    if (img_data)
    {
        /*
        第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
        第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
        第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
        第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
        下个参数应该总是被设为0（历史遗留的问题）。
        第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
        最后一个参数是真正的图像数据。
        */
        glTexImage2D(GL_TEXTURE_2D, 0, img_format, img_width, img_height, 0, img_format, GL_UNSIGNED_BYTE, img_data);
        // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        spdlog::error("加载图像 {} 失败", path);
        return -1;
    }
    // 释放图像的内存
    stbi_image_free(img_data);
    return 0;
}
