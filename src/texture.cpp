#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void pressInput(GLFWwindow *window);

/**
 * 添加 stb_image.h 来加载图像
 * 
*/

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "学习OpenGL Texture", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "创建GLFW window 失败。" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE, texture);
    //为当前绑定问了对象设置环绕 过滤 方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures\\container.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----         - 纹理坐标 -
         0.5f,  0.5f, 0.0f,         1.0f, 0.0f, 0.0f,     1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,     1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,         0.0f, 0.0f, 1.0f,     0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,         1.0f, 1.0f, 0.0f,     0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
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

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //纹理属性
    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader textureShader("shader\\texturevs.glsl","shader\\texturefs.glsl");

    while (!glfwWindowShouldClose(window))
    {
        pressInput(window);
        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //在这里绑定会变黑
        //glBindTexture(GL_TEXTURE_2D, texture);
        textureShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void pressInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
