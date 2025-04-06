#pragma once
#include "Src\ImGui\Context.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


struct ParticleUBO
{
	glm::mat4 projection;
	glm::mat4 view;
};

void computeMatricesFromInputs(glm::mat4& projection, glm::mat4& view, glm::vec3 position);

GLuint loadDDS(const char* imagepath);

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
