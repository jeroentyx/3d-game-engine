#include "FontCompiler.h"

#include <Src/pch.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>


#define DEFAULT_PIXEL_RANGE 2.0
#define DEFAULT_MITER_LIMIT 1.0
#define DEFAULT_ANGLE_THRESHOLD 3.0

unsigned int FontCompiler::LoadOwnFormat(char const* path)
{
    //
    //  Save the files to own format
    //

    static int IndexObj = 1;

    std::ifstream E(path, std::ios::ate | std::ios::binary);
    bool isOpen = E.is_open();
    EosAssert(isOpen, "TTF file doesn't exist ! ");
    UNREFERENCED_PARAMETER(isOpen);
    std::string fp = "Assets/CompiledFonts/";

    std::string fileName = std::to_string(IndexObj);
    fp.append(fileName);
    fp.append(".efonts");
    size_t fs = static_cast<size_t>(E.tellg());
    std::vector<char> buffer(fs);
    E.seekg(0);
    E.read(buffer.data(), fs);
    E.close();

    std::ofstream(fp.c_str(), std::ios::binary) << std::ifstream(path, std::ios::binary).rdbuf();
    IndexObj += 1;
    return 0;
}

// Using freetype
void FontCompiler::readTTF_to_ownFormat(char const* filepath)
{
    //FT_Library library;
    //FT_Face face;
    //GLuint texture;

    // open a custom file
    //std::string outputFileName = Eos::FileManager::GetFileNameWithoutExtension(path);
    //outputFileName += ".eosfont";
    //std::ofstream outFile;
    //outFile.open(outputFileName, std::ios::out, std::ios::trunc);

    //FT_Error error = FT_Init_FreeType(&library);
    //if (error)
    //{
    //    PE_CORE_ERROR("Error : Initialization of freetype failed");
    //    return;
    //}

    //error = FT_New_Face(library, filepath, 0, &face);
    //if (error)
    //{
    //    PE_CORE_ERROR("Error: failed to load font file");
    //    return;
    //}

    //FT_Set_Pixel_Sizes(face, 0, 48);

    //// First loop: Calculate textureWidth and textureHeight
    //GLuint textureWidth = 0, textureHeight = 0;
    //for (GLubyte c = 0; c < 128; c++)
    //{
    //    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //    {
    //        PE_CORE_WARN("Error: Freetype failed to load glyph");
    //        continue;
    //    }

    //    // in this sample we are building one row texture
    //    textureWidth += face->glyph->bitmap.width;
    //    textureHeight = std::max(textureHeight, face->glyph->bitmap.rows);

    //    // Store character for later use
    //    Character character;
    //    character.TextureID = texture;
    //    character.Size = glm::ivec2{ face->glyph->bitmap.width, face->glyph->bitmap.rows };
    //    character.Bearing = glm::ivec2{ face->glyph->bitmap_left, face->glyph->bitmap_top };
    //    character.Advance = face->glyph->advance.x;

    //    Characters.insert(std::pair<GLchar, Character>(c, character));
    //}

    //// Second loop to build one atlas texture map for the whole font
    //int UJumps = 0, VJumps = 0;
    //for (GLubyte c = 0; c < 128; c++)
    //{
    //    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //    {
    //        PE_CORE_CRITICAL("Cannot load Glyph");
    //        std::cout << "Cannot load Glyph" << std::endl;
    //        continue;
    //    }

    //    // startUV
    //    float uS = (float)(UJumps) / (float)(textureWidth);
    //    float vS = (float)(VJumps) / (float)(textureHeight);

    //    //endUV
    //    float uE = (float)(UJumps + (int)face->glyph->bitmap.width);
    //    uE = uE / (float)(textureWidth);
    //    float vE = (float)(VJumps + face->glyph->bitmap.rows);
    //    vE = vE / (float)(textureHeight);

    //    // CharacterGlyph structure has now 2 additional members: vec2 startUV, vec2 endUV
    //    Characters[c].startUV = glm::vec2(uS, vS);
    //    Characters[c].endUV = glm::vec2(uE, vE);

    //    if (face->glyph->bitmap.width != 0)
    //    {
    //        glTexSubImage2D(
    //            GL_TEXTURE_2D,
    //            0,
    //            UJumps,
    //            VJumps,
    //            face->glyph->bitmap.width,
    //            face->glyph->bitmap.rows,
    //            GL_RED,
    //            GL_UNSIGNED_BYTE,
    //            face->glyph->bitmap.buffer
    //        );
    //    }

    //    // Offset along u
    //    UJumps += face->glyph->bitmap.width;
    //}

    //// Close the output file
    //outFile.close();
}

// Working function
void FontCompiler::readTTF_to_MSDF(const std::string& filepath, float emSizeMin)
{
    // Storage for glyph geomtry and their coordinates in the atlas
    std::vector<msdf_atlas::GlyphGeometry> glyphs;

    // FontGeometry is a helper class that loads a set of glyphs from a single font.
    // It can also be used to get additional font metrics, kerning information, etc.
    msdf_atlas::FontGeometry fontGeometry(&glyphs);

    // Initialize instance of FreeType Library
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    if (ft)
    {
        // Load the ttf font file
        msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.c_str());
        if (font)
        {
            // Load a set of character glyhs
            // 2nd arg - ignored unless mixing different font sizes in one atlas
            // 3rd arg - can specify a charset other than ASCII
            // If loading specific glyph then use loadGlyphs 
            int glyphsLoaded = fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
            if (glyphsLoaded < 0)
            {
                PE_CORE_ERROR("Could not load glyphs from font {}", filepath.c_str());
                return;
            }

            if (glyphsLoaded < msdf_atlas::Charset::ASCII.size())
            {
                PE_CORE_WARN("Font \"{}\" is missing {} codepoints!  Codepoints:", filepath.c_str(), msdf_atlas::Charset::ASCII.size() - glyphsLoaded);
                bool first = true;
                for (msdfgen::unicode_t cp : msdf_atlas::Charset::ASCII)
                {
                    if (!fontGeometry.getGlyph(cp))
                        printf("%c 0x%02X", first ? ((first = false), ':') : ',', cp);
                }
                printf("\n");
            }
            // Free memory
            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }

    // Assigning glyph edge colors
    if (glyphs.empty())
    {
        PE_CORE_ERROR("No glyphs loaded from font \"{}\"", filepath.c_str());
        return;
    }
    else
    {
        for (msdf_atlas::GlyphGeometry& glyph : glyphs)
        {
            glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, 1);
        }
    }

    // Atlas packer class computes the layout for the class
    msdf_atlas::TightAtlasPacker atlasPacker;

    // Set atlas parameter
    atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);

    // SetScale for a fixed size or setminimumscale to use the largest that fits
    atlasPacker.setMinimumScale(emSizeMin);
    atlasPacker.setPadding(0);

    atlasPacker.setPixelRange(DEFAULT_PIXEL_RANGE);
    atlasPacker.setMiterLimit(DEFAULT_MITER_LIMIT);

    // Pack glyphs
    //std::cout << "Attempting to pack: " << glyphs.size() << " glyphs" << std::endl;
    int glyphPackFailureCount = atlasPacker.pack(glyphs.data(), glyphs.size());
    if (glyphPackFailureCount > 0)
    {
        PE_CORE_WARN("Unable to pack glyphs into atlas, Unpacked glyphs: {}", glyphPackFailureCount);
    }

    // Get final atlas dimensions
    int width = -1, height = -1;
    atlasPacker.getDimensions(width, height);

    // ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap
    msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<unsigned char, 3>> generator(width, height);
    
    // GeneratorAttributes can be modified to change the generator's default settings.
    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = false;
    generator.setAttributes(attributes);
    generator.setThreadCount(std::max((int)std::thread::hardware_concurrency(), 1));

    // Generate atlas bitmap
    generator.generate(glyphs.data(), glyphs.size());

    auto bitmap = (msdfgen::BitmapConstRef<unsigned char, 3>)generator.atlasStorage();

    // Generates a png image
    msdfgen::savePng(generator.atlasStorage(), "output.png");

    // Send to GPU
    if (bitmap.width * bitmap.height > 0)
    {
        glGenTextures(1, &texHandleOut);
        glBindTexture(GL_TEXTURE_2D, texHandleOut);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Store font metrics
    for (const msdf_atlas::GlyphGeometry& glyph : glyphs)
    {
        indexToCodePoint[glyph.getIndex()] = glyph.getCodepoint();
        fontAdvances[glyph.getCodepoint()] = glyph.getAdvance();

        double l, b, r, t;
        glyph.getQuadAtlasBounds(l, b, r, t);
        l /= bitmap.width;
        r /= bitmap.width;
        b /= bitmap.height;
        t /= bitmap.height;
        fontUVBounds[glyph.getCodepoint()] = std::tuple{ l, r, b, t };

        glyph.getQuadPlaneBounds(l, b, r, t);
        fontQuadBounds[glyph.getCodepoint()] = std::tuple{ l, r, b, t };
    }

    msdfgen::FontMetrics metrics = fontGeometry.getMetrics();
    fontVerticalMetrics = std::tuple(metrics.lineHeight, metrics.ascenderY, -metrics.descenderY);

    for (auto& [indicesKey, kernVal] : fontGeometry.getKerning())
    {
        std::pair<uint32_t, uint32_t> codePointsKey{
            indexToCodePoint[indicesKey.first],
            indexToCodePoint[indicesKey.second]
        };
        fontKerns[codePointsKey] = kernVal;
    }

    // In example
    /*
    save the texture reference count
    save the path of texture
    return texHandleOut
    */
}


// Testing function, same as readTTF_to_MSDF but saving into references passed into function
void FontCompiler::readTTF_to_ownFormat(const std::string& filepath, CodePoints outputPoints, Advances outputAdvances, UVs outputUVs, Quads outputQuads, Verticals outputVerticals, Kernings outputKernings, float emSizeMin)
{
    // Storage for glyph geomtry and their coordinates in the atlas
    std::vector<msdf_atlas::GlyphGeometry> glyphs;

    // FontGeometry is a helper class that loads a set of glyphs from a single font.
    // It can also be used to get additional font metrics, kerning information, etc.
    msdf_atlas::FontGeometry fontGeometry(&glyphs);

    // Initialize instance of FreeType Library
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    if (ft)
    {
        // Load the ttf font file
        msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.c_str());
        if (font)
        {
            // Load a set of character glyhs
            // 2nd arg - ignored unless mixing different font sizes in one atlas
            // 3rd arg - can specify a charset other than ASCII
            // If loading specific glyph then use loadGlyphs 
            int glyphsLoaded = fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
            if (glyphsLoaded < 0)
            {
                PE_CORE_ERROR("Could not load glyphs from font {}", filepath.c_str());
                return;
            }

            if (glyphsLoaded < msdf_atlas::Charset::ASCII.size())
            {
                PE_CORE_WARN("Font \"{}\" is missing {} codepoints!  Codepoints:", filepath.c_str(), msdf_atlas::Charset::ASCII.size() - glyphsLoaded);
                bool first = true;
                for (msdfgen::unicode_t cp : msdf_atlas::Charset::ASCII)
                {
                    if (!fontGeometry.getGlyph(cp))
                        printf("%c 0x%02X", first ? ((first = false), ':') : ',', cp);
                }
                printf("\n");
            }
            // Free memory
            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }

    // Assigning glyph edge colors
    if (glyphs.empty())
    {
        PE_CORE_ERROR("No glyphs loaded from font \"{}\"", filepath.c_str());
        return;
    }
    else
    {
        for (msdf_atlas::GlyphGeometry& glyph : glyphs)
        {
            glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, 1);
        }
    }

    // Atlas packer class computes the layout for the class
    msdf_atlas::TightAtlasPacker atlasPacker;

    // Set atlas parameter
    atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);

    // SetScale for a fixed size or setminimumscale to use the largest that fits
    atlasPacker.setMinimumScale(emSizeMin);
    atlasPacker.setPadding(0);

    atlasPacker.setPixelRange(DEFAULT_PIXEL_RANGE);
    atlasPacker.setMiterLimit(DEFAULT_MITER_LIMIT);

    // Pack glyphs
    //std::cout << "Attempting to pack: " << glyphs.size() << " glyphs" << std::endl;
    int glyphPackFailureCount = atlasPacker.pack(glyphs.data(), glyphs.size());
    if (glyphPackFailureCount > 0)
    {
        PE_CORE_WARN("Unable to pack glyphs into atlas, Unpacked glyphs: {}", glyphPackFailureCount);
    }

    // Get final atlas dimensions
    int width = -1, height = -1;
    atlasPacker.getDimensions(width, height);

    // ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap
    msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<unsigned char, 3>> generator(width, height);

    // GeneratorAttributes can be modified to change the generator's default settings.
    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = false;
    generator.setAttributes(attributes);
    generator.setThreadCount(std::max((int)std::thread::hardware_concurrency(), 1));

    // Generate atlas bitmap
    generator.generate(glyphs.data(), glyphs.size());

    auto bitmap = (msdfgen::BitmapConstRef<unsigned char, 3>)generator.atlasStorage();

    // Generates a png image
    msdfgen::savePng(generator.atlasStorage(), "output.png");

    // Send to GPU
    if (bitmap.width * bitmap.height > 0)
    {
        glGenTextures(1, &texHandleOut);
        glBindTexture(GL_TEXTURE_2D, texHandleOut);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Store font metrics
    for (const msdf_atlas::GlyphGeometry& glyph : glyphs)
    {
        outputPoints[glyph.getIndex()] = glyph.getCodepoint();
        outputAdvances[glyph.getCodepoint()] = glyph.getAdvance();

        double l, b, r, t;
        glyph.getQuadAtlasBounds(l, b, r, t);
        l /= bitmap.width;
        r /= bitmap.width;
        b /= bitmap.height;
        t /= bitmap.height;
        outputUVs[glyph.getCodepoint()] = std::tuple{ l, r, b, t };

        glyph.getQuadPlaneBounds(l, b, r, t);
        outputQuads[glyph.getCodepoint()] = std::tuple{ l, r, b, t };
    }

    msdfgen::FontMetrics metrics = fontGeometry.getMetrics();
    outputVerticals = std::tuple(metrics.lineHeight, metrics.ascenderY, -metrics.descenderY);

    for (auto& [indicesKey, kernVal] : fontGeometry.getKerning())
    {
        std::pair<uint32_t, uint32_t> codePointsKey{
            outputPoints[indicesKey.first],
            outputPoints[indicesKey.second]
        };
        outputKernings[codePointsKey] = kernVal;
    }
}
