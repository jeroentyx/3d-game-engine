#ifndef GRAPHIC_HELPER_H_
#define GRAPHIC_HELPER_H_

#include <glew.h>
#include "Src\pch.h"
#include <iostream>
#include <string>
#include <vector>
#include "stb_image\stb_image.h"


uint32_t loadCubemap(std::vector<std::string> faces);

uint32_t loadTexture(char const* path);

#endif