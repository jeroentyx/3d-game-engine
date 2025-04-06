#pragma once

#include <glm/glm.hpp>
#include <map>

#include "Src\Engine\Systems\Graphics\Platform\OpenGLTexture.h"

class FontCompiler
{
public:
    using CodePoints = std::map<int, uint32_t>;
    using Advances = std::map<uint32_t, double>;
    using UVs = std::map<uint32_t, std::tuple<float, float, float, float>>;
    using Quads = std::map<uint32_t, std::tuple<float, float, float, float>>;
    using Verticals = std::tuple<double, double, double>;
    using Kernings = std::map<std::pair<uint32_t, uint32_t>, double>;


    static unsigned int LoadOwnFormat(char const* path);

    static void readTTF_to_ownFormat(char const* filepath);
    static void readOwnFormat(char const* filepath);

    // MSDF
    GLuint texHandleOut = 0;    // TODO: Store this texhandleout elsewhere

    // Metrics
    std::map<int, uint32_t> indexToCodePoint;
    std::map<uint32_t, double> fontAdvances;
    std::map<uint32_t, std::tuple<float, float, float, float>> fontUVBounds;
    std::map<uint32_t, std::tuple<float, float, float, float>> fontQuadBounds;
    std::tuple<double, double, double> fontVerticalMetrics;
    std::map<std::pair<uint32_t, uint32_t>, double> fontKerns;

    void readTTF_to_MSDF(const std::string& filepath, float emSizeMin = 32.0f);

    void readTTF_to_ownFormat(const std::string& filepath, 
        CodePoints outputPoints, Advances outputAdvances, UVs outputUVs, 
        Quads outputQuads, Verticals outputVerticals, Kernings outputKernings,
        float emSizeMin = 32.0f);
};
