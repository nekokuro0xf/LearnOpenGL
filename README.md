* 安装MinGW环境
* 将库复制到项目目录下（省略
* 点击Run/Run Without Debugging 生成.vscode目录下的task.json 和 launch.json 配置文件
* 修改task.json参数
```json
...
//task.json 参数配置
"args": [
        "-g",
        "${file}",
        "-std=c++17",
        "-I./include",
        "-L./lib",
        "src/glad.c",
        "-lglfw3dll",
        "-o",
        "${workspaceFolder}\\build\\${fileBasenameNoExtension}.exe"
    ]
...

//launch.json 运行参数
"program": "${workspaceFolder}\\build\\${fileBasenameNoExtension}.exe",
```
* 打开helloworld.cpp 点击Run/Run Without Debugging（或者Ctrl + F5） 运行hello world文件
* 打开main.cpp 点击Run/Run Without Debugging（或者Ctrl + F5） 运行OpenGL空窗口文件

我正在看的教程链接
<https://learnopengl-cn.github.io/intro/>

> 学习顺序

1. hellowindow.cpp
2. hellotriangle.cpp
3. hellotriangle2.cpp
4. shader.cpp