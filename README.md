


```json
...
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
```