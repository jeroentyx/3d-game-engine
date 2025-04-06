#ifndef PCH_H_
#define PCH_H_

#include <tracy/tracy.hpp>

#include <glew.h>
#include <GLFW\glfw3.h>
#undef APIENTRY
#include <iostream>
#include <sstream>
#include <iomanip>

#include "dependencies\imgui-1.84.2\imgui.h"
#include "dependencies\imgui-1.84.2\imgui_impl_glfw.h"
#include "dependencies\imgui-1.84.2\imgui_impl_opengl3.h"


#include <stdint.h>
#include <iomanip>
#include <utility>
#include <map>
#include <unordered_map>
#include <fstream>
#include <string>
#include <filesystem>


#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>
#include <cstdlib>

#include <memory>
#include <unordered_set>
#include <list>
#include <vector>
#include <functional>
#include <unordered_map>
#include <thread>
#include <variant>
#include <mutex>
#include <array>
#include <forward_list>
#include <string>
#include <assimp\scene.h>
//#include "src/Engine/Systems/Audio/Audio_System.h"
#include "../Src/Engine/Frameworks/Serialization/SerializeManager.h"
#include "../Src/Engine/Frameworks/Serialization/ISerializable.h"
#include "../Src/Engine/Frameworks/FileManager/FileManager.hpp"
#include "../Src/Engine/Frameworks/EosAssert.h"

#include "../Src/Engine/Core/Manager/EntityID.h"


//Debugging Purpose
#include "Src/Engine/Frameworks/Logger/Logger.h"


#endif 