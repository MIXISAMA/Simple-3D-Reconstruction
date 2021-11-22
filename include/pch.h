#pragma once

#include <atomic>
#include <ctime>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <future>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <shared_mutex>
#include <string>
#include <tuple>
#include <vector>

#include "glog/logging.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define PROJECT_NAME "Simple-3D-Reconstruction"

namespace mixi
{
    namespace
    {
        using std::int32_t;
        using std::uint32_t;
        namespace fs = std::filesystem;
        namespace bp = boost::process;
    }
}
