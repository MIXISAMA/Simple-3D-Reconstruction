# Simple 3D Reconstruction

## Dependent

```txt
├─ opencv
├─ openMVS
│  └─ boost
just a mock, not a fact
```

## Current Progress

* 2021/10/10: Add the libuvc-detha library, responsible for the camera using the uvc protocol.
* 2021/10/02: I temporarily gave up the maintenance of CMake on the windows platform. I will follow up when i have a stable version.
* 2021/10/01: The assets resource will be pushed to the code repository in the stable version. If the picture is missing, it can be replaced by any other picture.
* 2021/09/26: Due to the use of a large number of third-party libraries, it is difficult to maintain CMake, so we hope that users can install dependent libraries by themselves in advance.
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
        "include/",
        "3rdparty/glad/include/",
        "3rdparty/stb/",
        "3rdparty/imgui/include/",
        "3rdparty/imgui-filebrowser/",
        "build/_deps/glfw-src/include/",
        "build/_deps/glog-build/",
        "build/_deps/glog-src/src/"
    ]
}
```
