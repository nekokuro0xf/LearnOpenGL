#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void pressInput(GLFWwindow* window);

int main()
{
    glfwInit();
    //glfwInit是用来初始化GLFW的
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint函数是用来配置GLFW的，第一个参数是参数名称，第二个参数是一个整形的值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //如果系统支持OpenGL3.3或更高版本我用的3.3.2，所以没问题

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
        //GLAD是用来管理OpenGL指针的，如果GLAD初始化失败就关闭窗口。
        return -1;
    }

    glViewport(0, 0, 800, 600);
    //设置窗口位置和大小

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //设置窗口变化回调
    //当窗口变化时会运行framebuffer_size_callback中的代码重新设置glViewport

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        pressInput(window);
        //glfwWindowShouldClose函数在我们每次循环开始前检查glfw是否被要求关闭

        //...渲染指令
        glClearColor(0.5f,0.5f,0.7f,1.0f);
        //设置清空屏幕颜色
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        //glfwSwapBuffers 会交换颜色缓存，它在这一次循环中被用来绘制，并将作为输出显示在屏幕上

        glfwPollEvents();
        //glfwPollEvents 检查有没有触发事件（输入，窗口变动等）
    }

    glfwTerminate();
    return 0;
}

void pressInput(GLFWwindow *window)
{
    //如果ESC被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        //设置窗口应该关闭为真
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
