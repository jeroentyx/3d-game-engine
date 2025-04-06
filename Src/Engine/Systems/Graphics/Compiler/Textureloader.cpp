#include "TextureLoader.h"
#include "SOIL2.h"
#include "glew.h"

unsigned int DDSCompiler::LoadOwnFormat(char const* path)
{

    //
    //  Using soil2 to load the texture 
    //
    std::string fileDirectory = "Assets/Textures/";
    std::string fullPath = fileDirectory + path;

    GLuint TextureID = SOIL_load_OGL_texture
    (
        fullPath.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | 
        SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_TEXTURE_REPEATS
    );

    EosAssert(TextureID != 0, "Error Loading Soil Texture ! ");
    
    //
    //  once it has been successfully loaded
    //  Save it to own format
    std::string F = path;
    //Convert to hex
    std::ostringstream ret;

    std::string fp = "Assets/CompiledTexture/";
    fp.append(path); //Append the new fileName to the assets path

    std::ifstream existingFile(fullPath, std::ios::ate | std::ios::binary);
    if (!existingFile.is_open())
    {
        throw std::runtime_error("File doesn't exist ! ");
    }

    size_t fileSize = (size_t)existingFile.tellg();
    std::vector<char> buffer(fileSize);

    existingFile.seekg(0);
    existingFile.read(buffer.data(), fileSize);
    existingFile.close();

    std::ofstream(fp.c_str(), std::ios::binary) << std::ifstream(fullPath, std::ios::binary).rdbuf();
    
    return TextureID;
}
