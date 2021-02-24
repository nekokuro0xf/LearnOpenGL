#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void pressInput(GLFWwindow *window);

/**
 * 知识点
 * 新单词
 * 顶点数组对象： Vertex Array Object, VAO
 * 顶点缓冲对象： Vertex Buffer Object, VBO
 * 索引缓冲对象： Element Buffer Object, EBO 或者 Index Buffer Object, IBO
 * 
 * 图形渲染管线：图形渲染管线接受一组3D坐标，然后把他们转变为你屏幕上的有色2D像素输出(.图形渲染管线可以被划分为几个阶段，
 * 每个阶段将会把前一个阶段的输出作为输入(所有这些阶段都是高度专门化的(他们都有一个特定的函数),并且很容并行执行(GPU的强项)的特性，
 * 当今大多数显卡都有成千上万的小处理核心，他们在GPU上为每一个(渲染管线)阶段运行各自的小程序,从而在图形渲染管线中快速处理你的数据.
 * 这些小程序叫做 着色器(Shader).
 * 
 * 有些着色器允许开发者自己配置,这就允许我们用自己写的着色器来替换默认的.这样我们就可以更细致的控制图形渲染管线中的特定部分了,
 * 而且因为他们运行在GPU上,所以它们可以给我们节省宝贵的CPU时间.OpenGL着色器时用OpenGl着色器语言(OpenGL Shading Language,GLSL)
 * 写成的.
 * 
 * 图形渲染管线阶段
 * 顶点着色器 > 形状(图元)装配 > 几何着色器 > 光栅化 > 片段着色器 > 测试与混合
 * VERTEX SHADER > SHAPE ASSEMBLY > GEOMETRY SHADER > RASTERIZATION > FRAGMENT SHADER > TESTS AND BLENDING
 * 我们可以在 顶点着色器/几何着色器/片段着色器 注入自定义的着色器
 * 
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

    //这里是新的代码
    std::cout << "hello 三角形" << std::endl;
    //创建一组顶点数据
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f};

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    //
    //生成一个VBO对象(顶点缓冲对象)
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //复制索引素组到一个索引缓冲中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //glGenBuffers函数和一个缓冲ID可以生成一个VBO对象

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    //使用glBindBuffer将新创建的缓冲绑定到GL_ARRAY_BUFFER目标上

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //使用glBufferData将顶一个顶点数据复制到缓冲的内存中
    /** 
     * glBufferData是一个专门用来把用户自定义的数据复制到当前绑定缓存的函数.
     * 它的第一个参数是目标缓冲的类型: 顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上.
     * 第二个参数指定传输数据的大小.
     * 第三个参数是我们希望发送的数据.
     * 第四个参数制定了我们希望显卡如何管理给定的数据.
     * 它有三种型是:
     * GL_STATIC_DRAW:数据不会或几乎不会改变.
     * GL_DYNAMIC_DRAW:数据会被改变很多.
     * GL_STREAM_DRAW:数据每次回至时都会改变
     * */

    /**
     * 顶点着色器 
     * 顶点着色器(Vertex Shader)是几个可编程着色器中的一个
     */

    //创建一个shader字符串
    const char *vertexShaderSource =
        "#version 330 core \n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    //接下来创建一个着色器对象,注意还是用ID来引用
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //用glShaderSource附加shader源码,用glCompileShader来编译

    int success;
    char infoLog[512];
    //检查shader是否成功编译
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //如果shader编译失败了
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED SHADER编译失败\n"
                  << infoLog << std::endl;
        //输出错误信息
    }

    /**
     * 片段着色器(Fragment Shader)
     * 片段着色器所做的是计算像素最后的颜色输出.
     * 
     */
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
        //如果shader编译失败了
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED 片段着色器编译失败\n"
                  << infoLog << std::endl;
        //输出错误信息
    }

    /** 
     * 着色器程序(Shader Program Object)
     * 着色器程序是多个着色器合并之后冰最终链接完成的版本.
     * 如果要使用刚在编译的着色器我们必须把它们链接(Link)为一个着色器程序对象,然后
     * 在渲染对象的时候激活这个着色器程序.已激活的着色器程序将在我们发送渲染调用的时候被使用.
     */

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
    //使用shader程序,并在之后删除着色器对象,链接之后着色器对象就不需要了
    //现在,我们已经把顶点数据发送给GPU,并指示GPU如何在顶点和片段着色器中处理它了,但是OpenGL
    //还不知道它该如何解释内存中的顶点数据

    /** 
     * 链接顶点属性
     * 顶点着色器允许我们指定任何顶点属性作为型是的输入.
     */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    /**
     * 使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
     * 参数1: 指定我们要配置的顶点属性.我们在顶点着色器中使用`layout(location =0)`定义了position顶点属性的位置值
     * 它可以把顶点属性的位置设置为0
     * 参数2: 指定顶点属性的大小,顶点属性是一个vec3,它由3个值组成,所以大小是3
     * 参数3: 指定数据的类型,这里是GL_FLOAT(GLSL中的vec*都是由浮点数组成的)
     * 参数4: 定义我们是否希望数据被标准化(Normalize).如果我们设置为GL_TRUE,
     * 所有数据都会被映射到0到1之间.我们把它设置为GL_FALSE.
     * 参数5: 这个参数叫做步长(Stride)它告诉我们在连续的顶点属性之间的间隔.由于下个组的位置在3个float之后,
     * 我们把补偿设置为3* sizeof(float).
     * 参数6: 这个参数的类型是void*,所以需要我们进行这个奇怪的强制类型转换,它表示位置数据
     * 在缓冲中起始的位置的偏移量(Offset).由于位置数据在数组的开头,所以这里是0
     */

    //开启线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        pressInput(window);
        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //执行新的代码

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
