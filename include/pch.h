#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
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


namespace mixi
{
    namespace
    {
        using std::int32_t;
        using std::uint32_t;
        namespace fs = std::filesystem;
    }
}
