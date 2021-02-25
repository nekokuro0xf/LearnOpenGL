#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void pressInput(GLFWwindow *window);

/** 知识点
 * 着色器(shader)
 * GLSL
 * 着色器是使用一种教GLSL的类C语言写成的.GLSL是为图形计算量身定制的,
 * 它包含一些针对向量和矩阵操作的有用特性.
 * 着色器的开头总是要声明版本,接着是输入和输出变量 niform和main函数.
 * 每个着色器的入口点都是main函数,在这个函数中
 * 我们处理所有的输入变量,并将结果输出到输出变量中
 * 
 * 数据类型
 * 和其他编程语言一样,GLSL有数据类型可以来指定变量的种类.GLSL中包含C等其他语言大部分的默认基础数据类型:
 * int float double unit 和 bool. GLSL也有两种容器类型,分别是向量 Vector 和矩阵 Matrix
 * 
 * 向量
 * GLSL中的向量是一个可以包含1,2,3 或者 4个分量的容器,分量的类型可以是默认基础类型中的任意一个.他们可以是下面的型是
 * vecn  包含n个float分量的默认项链
 * bvecn 包含n个bool分量
 * ivecn 包含n个int分量
 * uvecn 包含n个unsigned int分量
 * dvecn 包含n个double分量
 * 
 * 一个向量的分量可以用vec.x这种方式获取
 * vec2 someVec;
 * vec4 differentVec = someVec.xyxx;
 * vec3 anotherVec = differentVec.zyw;
 * 向量这一数据类型允许一些有趣而灵活的分量选择方式,叫做重组(Swizzling)
 * 
 * 输入与输出
 * 虽然着色器是各自独立的小程序,但是它们都是一个整体的一部分,出于这样的原因,我们希望每个着色器都有输入和输出,
 * 这样才能进行数据交流和传递.GLSL定义了 `in` 和 `out` 关键字专门来实现这个目的.每个着色器使用这两个关键字设定
 * 输入和输出,只要一个输出变量与下一个着色器阶段的输入匹配,他就会传递下去.但在顶点和片段着色器中会有点不同.
 * 
 * 
 * Uniform
 * UniForm是一种从CPU中的应用想GPU中的着色器发送数据的方式,但uniform和顶点属性
 * 有些不同.首先,uniform是全局的(Global).全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的
 * 而且她可以被着色器程序的任意着色器在任意阶段访问.
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

    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;   // 位置变量的属性值为0\n"
        "layout (location = 1) in vec3 aColor; // 颜色变量的属性值为1\n"
        "\n"
        "out vec3 ourColor;//为片段着色器指定一个颜色输出\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(aPos,1.0);\n"
        "  ourColor = aColor;\n"
        "}\n";

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "  FragColor = vec4(ourColor, 1.0);\n"
        "}\n";

    const char *uniformFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "  FragColor = ourColor;\n"
        "}\n";


    int success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED SHADER编译失败\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED SHADER编译失败\n"
                  << infoLog << std::endl;
    }

    unsigned int uniformFragmentShader;
    uniformFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(uniformFragmentShader,1,&uniformFragmentShaderSource,NULL);
    glCompileShader(uniformFragmentShader);
    glGetShaderiv(uniformFragmentShader,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(uniformFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED SHADER编译失败\n"
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(uniformFragmentShader);

    float vertices[] = {
        //位置              //颜色
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 顶部
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // 左下
    };
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window))
    {
        pressInput(window);
        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        glUseProgram(shaderProgram);
        //使用glUniform4f更新颜色值
        // glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
