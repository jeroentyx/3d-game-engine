#include "MeshLoader.h"
#pragma once

unsigned int MeshLoader::LoadOwnFormat(char const* path)
{
    //
    //  Save the files to own format
    //

    static int IndexObj = 1;

    std::ifstream E(path, std::ios::ate | std::ios::binary);
    bool isOpen = E.is_open();
    EosAssert(isOpen, "Obj file doesn't exist ! ");
    UNREFERENCED_PARAMETER(isOpen);
    std::string fp = "Assets/CompiledMeshes/";
    
    std::string fileName = std::to_string(IndexObj);
    fp.append(fileName);
    fp.append(".eobj");
    size_t fs = static_cast<size_t>(E.tellg());
    std::vector<char> buffer(fs);
    E.seekg(0);
    E.read(buffer.data(), fs);
    E.close();

    std::ofstream(fp.c_str(), std::ios::binary) << std::ifstream(path, std::ios::binary).rdbuf();
    IndexObj += 1;
    return 0;
}
