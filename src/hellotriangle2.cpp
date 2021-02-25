#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void pressInput(GLFWwindow *window);
/**
 * 练习
 * 画两个相邻的三角形
 * 画个五角星
 * 创建两个着色器程序，第二个程序使用一个不同的片段着色器，输出黄色；再次绘制这两个三角形，让其中一个输出为黄色
 * 
*/

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "学习OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "创建GLFW window 失败(." << std::endl;
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

    //**************************
    //创建shader程序

    const char *vertexShaderSource =
        "#version 330 core \n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED SHADER编译失败\n"
                  << infoLog << std::endl;
    }

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED 片段着色器编译失败\n"
                  << infoLog << std::endl;
    }

    const char *fragmentShaderSource2 =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.92f, 0.23f, 1.0f);\n"
        "}\n";

    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED 片段着色器2编译失败\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED Shader程序链接失败\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED Shader程序链接失败\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //**********************

    std::cout << "hello 三角形2" << std::endl;
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f};

    //画三角
    float vertices[] = {
        -0.4f, 0.0f, 0.0f,
        -0.3f, 0.2f, 0.0f,
        -0.2f, 0.0f, 0.0f};

    unsigned int VBO;
    unsigned int VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    float vertices2[] = {
        -0.2f, 0.0f, 0.0f,
        -0.1f, 0.2f, 0.0f,
        0.0f, 0.0f, 0.0f};

    unsigned int VBO2;
    unsigned int VAO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

/*
    //画五角星
    float center[] = {0.2f, 0.2f};
    float r = 0.1f;
    float r2 = r * 3;
    float PI = 3.141592;
    float starVertices[10 * 3];

    //算出五角星内五边形五个点
    for (int i = 0; i < 5; i++)
    {
        int deg = 54 + (72 * i);
        std::cout << i << ":" << deg << "度" << std::endl;
        starVertices[i * 3] = center[0] + r * cos(deg * PI / 180);
        starVertices[(i * 3) + 1] = center[1] + r * sin(deg * PI / 180);
        starVertices[(i * 3) + 2] = 0.0f;
    }

    //算出五角星的五个点
    for (int i = 5; i < 10; i++)
    {
        int deg = (72 * (i - 5)) + 18;
        std::cout << i << ":" << deg << "度" << std::endl;
        starVertices[i * 3] = center[0] + r2 * cos(deg * PI / 180);
        starVertices[(i * 3) + 1] = center[1] + r2 * sin(deg * PI / 180);
        starVertices[(i * 3) + 2] = 0.0f;
    }

    unsigned int starIndices[] = {
        // 注意索引从0开始!
        7, 5, 3, // 第一个三角形
        8, 6, 4, // 第二个三角形
        9, 7, 0  // 第三个三角形
    };
    unsigned int starVBO;
    unsigned int starVAO;

    glGenBuffers(1, &starVBO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(starVertices), starVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &starVAO);
    glBindVertexArray(starVAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(starIndices), starIndices, GL_STATIC_DRAW);

    */


    //开启线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        pressInput(window);
        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        //画两个三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //画五角星
        // glBindVertexArray(starVAO);
        // glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        //执行新的代码
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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
