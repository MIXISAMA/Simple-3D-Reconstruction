#pragma once

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <exception>
#include <shared_mutex>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <atomic>

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


namespace mixi
{
    namespace
    {
        using std::int32_t;
        using std::uint32_t;
        namespace fs = std::filesystem;
    }
}
