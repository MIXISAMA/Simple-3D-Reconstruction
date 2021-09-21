# Simple 3D Reconstruction

## Current Progress

* 2021/09/21: A empty demo main.cpp was be created.

## CMake

This project uses many third-party libraries, which will be downloaded during the CMake configure phase, so the CMake configure phase will last a long time. Even download library failed due to network problems, retrying may solve the problem.

```sh
cd Simple-3D-Reconstruction
cmake -S . -B ./build # you can use cmake-gui instead of this command
cd build
make
```

If executed correctly, the executable file will be in `Simple-3D-Reconstruction/build/bin/Simple3DReconstruction`

## Include Directories

In vscode, you can copy the following includePath to `.vscode/settinngs.json`. If you use other editors/IDEs, please add these directories manually.

```json
{
    "C_Cpp.default.includePath": [
        "3rdparty/glad/include/",
        "build/_deps/glfw-src/include/",
        "build/_deps/imgui-src/",
        "build/_deps/imgui-src/backends/",
        "build/_deps/glog-build/",
        "build/_deps/glog-src/src/",
    ],
}
```
