//#include "msdfgen.h"
//#include "msdfgen-ext.h"

#include <msdf-atlas-gen.h>
#include <iostream>
#include <cmath>
#include <thread>
#include "FontSerializer.h"


//using namespace msdfgen;
using namespace msdf_atlas;

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define DEFAULT_MITER_LIMIT 1.0
#define DEFAULT_PIXEL_RANGE 2.0
#define SDF_ERROR_ESTIMATE_PRECISION 19
#define GLYPH_FILL_RULE msdfgen::FILL_NONZERO
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull



static char toupper(char c) 
{
    return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
}

static bool cmpExtension(const char* path, const char* ext) 
{
    for (const char* a = path + strlen(path) - 1, *b = ext + strlen(ext) - 1; b >= ext; --a, --b)
        if (a < path || toupper(*a) != toupper(*b))
            return false;
    return true;
}


struct FontInput
{
    const char* fontFilename;
    msdf_atlas::GlyphIdentifierType glyphIdentifierType;
    const char* charsetFilename;
    double fontScale;
    const char* fontName;
};

struct Configuration 
{
    msdf_atlas::ImageType imageType;
    msdf_atlas::ImageFormat imageFormat;
    msdf_atlas::YDirection yDirection;
    int width, height;
    double emSize;
    double pxRange;
    double angleThreshold;
    double miterLimit;
    void (*edgeColoring)(msdfgen::Shape&, double, unsigned long long);
    bool expensiveColoring;
    unsigned long long coloringSeed;
    msdf_atlas::GeneratorAttributes generatorAttributes;
    bool preprocessGeometry;
    bool kerning;
    int threadCount;
    const char* arteryFontFilename;
    const char* imageFilename;
    const char* jsonFilename;
    const char* csvFilename;
    const char* shadronPreviewFilename;
    const char* shadronPreviewText;
};

template <typename T, typename S, int N, GeneratorFunction<S, N> GEN_FN>
static bool makeAtlas(const std::vector<GlyphGeometry>& glyphs, const std::vector<FontGeometry>& fonts, const Configuration& config) 
{
    ImmediateAtlasGenerator<S, N, GEN_FN, BitmapAtlasStorage<T, N> > generator(config.width, config.height);
    generator.setAttributes(config.generatorAttributes);
    generator.setThreadCount(config.threadCount);
    generator.generate(glyphs.data(), (int)glyphs.size());
    msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>) generator.atlasStorage();

    bool success = true;

    if (config.imageFilename) {
        if (saveImage(bitmap, config.imageFormat, config.imageFilename, config.yDirection))
            puts("Atlas image file saved.");
        else {
            success = false;
            puts("Failed to save the atlas as an image file.");
        }
    }

    /*if (config.arteryFontFilename) {
        ArteryFontExportProperties arfontProps;
        arfontProps.fontSize = config.emSize;
        arfontProps.pxRange = config.pxRange;
        arfontProps.imageType = config.imageType;
        arfontProps.imageFormat = config.imageFormat;
        arfontProps.yDirection = config.yDirection;
        if (exportArteryFont<float>(fonts.data(), fonts.size(), bitmap, config.arteryFontFilename, arfontProps))
            puts("Artery Font file generated.");
        else {
            success = false;
            puts("Failed to generate Artery Font file.");
        }
    }*/

    return success;
}

int main() 
{
    const char* inputfile = "Lato-Regular.ttf";

    std::cout << "Font compiler started" << std::endl;

#ifdef SINGLEPCHAR_SINGLEPNG
    // Standard single png
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    if (ft) {
        msdfgen::FontHandle* font = loadFont(ft, "Lato-Regular.ttf");
        if (font) {
            msdfgen::Shape shape;
            if (loadGlyph(shape, font, 'A')) {
                shape.normalize();
                // max. angle
                edgeColoringSimple(shape, 3.0);
                // image width, height
                msdfgen::Bitmap<float, 3> msdf(32, 32);
                // range, scale, translation
                generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));

                // returns 1
                bool check = msdfgen::savePng(msdf, "output.png");
                if (!check)
                    std::cout << "Saving failed" << std::endl;
                else
                    std::cout << "image saved" << std::endl;
            }
            msdfgen::destroyFont(font);
        }
        deinitializeFreetype(ft);
    }
#endif // SINGLEPCHAR_SINGLEPNG

    

    int result = 0;
    std::vector<FontInput> fontInputs;
    FontInput fontInput = { };
    Configuration config = { };
    fontInput.glyphIdentifierType = GlyphIdentifierType::UNICODE_CODEPOINT;
    fontInput.fontScale = -1;
    config.imageType = ImageType::MSDF;
    config.imageFormat = ImageFormat::UNSPECIFIED;
    config.yDirection = YDirection::BOTTOM_UP;
    config.edgeColoring = msdfgen::edgeColoringInkTrap;
    config.kerning = true;
    const char* imageFormatName = nullptr;
    int fixedWidth = -1, fixedHeight = -1;
    config.preprocessGeometry = (
#ifdef MSDFGEN_USE_SKIA
        true
#else
        false
#endif
        );
    config.generatorAttributes.config.overlapSupport = !config.preprocessGeometry;
    config.generatorAttributes.scanlinePass = !config.preprocessGeometry;
    double minEmSize = 0;
    enum {
        /// Range specified in EMs
        RANGE_EM,
        /// Range specified in output pixels
        RANGE_PIXEL,
    } rangeMode = RANGE_PIXEL;
    double rangeValue = 0;
    TightAtlasPacker::DimensionsConstraint atlasSizeConstraint = TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE;
    config.angleThreshold = DEFAULT_ANGLE_THRESHOLD;
    config.miterLimit = DEFAULT_MITER_LIMIT;
    config.threadCount = 0;


    bool explicitErrorCorrectionMode = false;

    // This chunk is purposed to replace parsing of command line

    //type
    config.imageType = ImageType::MSDF;

    //format
    config.imageFormat = ImageFormat::PNG;

    //font
    fontInput.fontFilename = inputfile;

    //-charset
    fontInput.charsetFilename;  //unspecified uses acsii
    fontInput.glyphIdentifierType = GlyphIdentifierType::UNICODE_CODEPOINT;

    //-glyphset
    // covered under charset

    //-fontscale
    fontInput.fontScale = 12.0;

    //-fontname
    fontInput.fontName = nullptr;

    //-and


    //-arfont


    //-imageout
    config.imageFilename = "msdf.png";

    //-json
    config.jsonFilename = "msdfMetrics.json";

    //-csv


    //-shadronpreview


    //-dimensions
    fixedWidth = 512;
    fixedHeight = 256;
    
    //-pots POWER_OF_TWO_SQUARE


    //-potr POWER_OF_TWO_RECTANGLE

    
    //-square SQUARE


    //-square2 EVEN_SQUARE


    //-square4 MULTIPLE_OF_FOUR_SQUARE


    //-yorigin


    //-size


    //-minsize


    //-emrange


    //-pxrange


    //-angle


    //-errorcorrection


    //-errordeviationratio


    //-errorimproveratio


    //-coloringstrategy


    //-miterlimit


    //-nokerning / kerning


    //-nopreprocess / preprocess


    //-nooverlap / overlap


    //-noscanline / scanline

    
    //-seed


    //-threads


    if (!fontInput.fontFilename)
    {
        std::cout << "No font specified." << std::endl;
        return 0;
    }
    if (!(config.arteryFontFilename || config.imageFilename || config.jsonFilename || config.csvFilename || config.shadronPreviewFilename))
    {
        std::cout << "No output specified." << std::endl;
        return 0;
    }

    bool layoutOnly = !(config.arteryFontFilename || config.imageFilename);

    // Finalize font inputs
    const FontInput* nextFontInput = &fontInput;
    for (std::vector<FontInput>::reverse_iterator it = fontInputs.rbegin(); it != fontInputs.rend(); ++it) {
        if (!it->fontFilename && nextFontInput->fontFilename)
            it->fontFilename = nextFontInput->fontFilename;
        if (!it->charsetFilename && nextFontInput->charsetFilename) {
            it->charsetFilename = nextFontInput->charsetFilename;
            it->glyphIdentifierType = nextFontInput->glyphIdentifierType;
        }
        if (it->fontScale < 0 && nextFontInput->fontScale >= 0)
            it->fontScale = nextFontInput->fontScale;
        nextFontInput = &*it;
    }
    if (fontInputs.empty() || memcmp(&fontInputs.back(), &fontInput, sizeof(FontInput)))
        fontInputs.push_back(fontInput);


    // Fix up configuration based on related values
    if (!(config.imageType == ImageType::PSDF || config.imageType == ImageType::MSDF || config.imageType == ImageType::MTSDF))
        config.miterLimit = 0;
    if (config.emSize > minEmSize)
        minEmSize = config.emSize;
    if (!(fixedWidth > 0 && fixedHeight > 0) && !(minEmSize > 0)) {
        puts("Neither atlas size nor glyph size selected, using default...");
        minEmSize = MSDF_ATLAS_DEFAULT_EM_SIZE;
    }
    if (!(config.imageType == ImageType::SDF || config.imageType == ImageType::PSDF || config.imageType == ImageType::MSDF || config.imageType == ImageType::MTSDF)) {
        rangeMode = RANGE_PIXEL;
        rangeValue = (double)(config.imageType == ImageType::SOFT_MASK);
    }
    else if (rangeValue <= 0) {
        rangeMode = RANGE_PIXEL;
        rangeValue = DEFAULT_PIXEL_RANGE;
    }
    if (config.kerning && !(config.arteryFontFilename || config.jsonFilename || config.shadronPreviewFilename))
        config.kerning = false;
    if (config.threadCount <= 0)
        config.threadCount = std::max((int)std::thread::hardware_concurrency(), 1);
    if (config.generatorAttributes.scanlinePass) {
        if (explicitErrorCorrectionMode && config.generatorAttributes.config.errorCorrection.distanceCheckMode != msdfgen::ErrorCorrectionConfig::DO_NOT_CHECK_DISTANCE) {
            const char* fallbackModeName = "unknown";
            switch (config.generatorAttributes.config.errorCorrection.mode) {
            case msdfgen::ErrorCorrectionConfig::DISABLED: fallbackModeName = "disabled"; break;
            case msdfgen::ErrorCorrectionConfig::INDISCRIMINATE: fallbackModeName = "distance-fast"; break;
            case msdfgen::ErrorCorrectionConfig::EDGE_PRIORITY: fallbackModeName = "auto-fast"; break;
            case msdfgen::ErrorCorrectionConfig::EDGE_ONLY: fallbackModeName = "edge-fast"; break;
            }
            printf("Selected error correction mode not compatible with scanline mode, falling back to %s.\n", fallbackModeName);
        }
        config.generatorAttributes.config.errorCorrection.distanceCheckMode = msdfgen::ErrorCorrectionConfig::DO_NOT_CHECK_DISTANCE;
    }


    // Finalize image format
    ImageFormat imageExtension = ImageFormat::UNSPECIFIED;
    if (config.imageFilename) {
        if (cmpExtension(config.imageFilename, ".png")) imageExtension = ImageFormat::PNG;
        else if (cmpExtension(config.imageFilename, ".bmp")) imageExtension = ImageFormat::BMP;
        else if (cmpExtension(config.imageFilename, ".tif") || cmpExtension(config.imageFilename, ".tiff")) imageExtension = ImageFormat::TIFF;
        else if (cmpExtension(config.imageFilename, ".txt")) imageExtension = ImageFormat::TEXT;
        else if (cmpExtension(config.imageFilename, ".bin")) imageExtension = ImageFormat::BINARY;
    }
    if (config.imageFormat == ImageFormat::UNSPECIFIED) {
        config.imageFormat = ImageFormat::PNG;
        imageFormatName = "png";
        // If image format is not specified and -imageout is the only image output, infer format from its extension
        if (imageExtension != ImageFormat::UNSPECIFIED && !config.arteryFontFilename)
            config.imageFormat = imageExtension;
    }
    if (config.imageType == ImageType::MTSDF && config.imageFormat == ImageFormat::BMP)
    {
        std::cout << "Atlas type not compatible with image format. MTSDF requires a format with alpha channel." << std::endl;
        return 0;
    }
    if (config.arteryFontFilename && !(config.imageFormat == ImageFormat::PNG || config.imageFormat == ImageFormat::BINARY || config.imageFormat == ImageFormat::BINARY_FLOAT)) {
        config.arteryFontFilename = nullptr;
        result = 1;
        puts("Error: Unable to create an Artery Font file with the specified image format!");
        // Recheck whether there is anything else to do
        if (!(config.arteryFontFilename || config.imageFilename || config.jsonFilename || config.csvFilename || config.shadronPreviewFilename))
            return result;
        layoutOnly = !(config.arteryFontFilename || config.imageFilename);
    }
    if (imageExtension != ImageFormat::UNSPECIFIED) {
        // Warn if image format mismatches -imageout extension
        bool mismatch = false;
        switch (config.imageFormat) {
        case ImageFormat::TEXT: case ImageFormat::TEXT_FLOAT:
            mismatch = imageExtension != ImageFormat::TEXT;
            break;
        case ImageFormat::BINARY: case ImageFormat::BINARY_FLOAT: case ImageFormat::BINARY_FLOAT_BE:
            mismatch = imageExtension != ImageFormat::BINARY;
            break;
        default:
            mismatch = imageExtension != config.imageFormat;
        }
        if (mismatch)
            printf("Warning: Output image file extension does not match the image's actual format (%s)!\n", imageFormatName);
    }
    imageFormatName = nullptr; // No longer consistent with imageFormat
    bool floatingPointFormat = (
        config.imageFormat == ImageFormat::TIFF ||
        config.imageFormat == ImageFormat::TEXT_FLOAT ||
        config.imageFormat == ImageFormat::BINARY_FLOAT ||
        config.imageFormat == ImageFormat::BINARY_FLOAT_BE
        );


    // Load fonts
    std::vector<GlyphGeometry> glyphs;
    std::vector<FontGeometry> fonts;
    bool anyCodepointsAvailable = false;
    {
        class FontHolder {
            msdfgen::FreetypeHandle* ft;
            msdfgen::FontHandle* font;
            const char* fontFilename;
        public:
            FontHolder() : ft(msdfgen::initializeFreetype()), font(nullptr), fontFilename(nullptr) { }
            ~FontHolder() {
                if (ft) {
                    if (font)
                        msdfgen::destroyFont(font);
                    msdfgen::deinitializeFreetype(ft);
                }
            }
            bool load(const char* fontFilename) {
                if (ft && fontFilename) {
                    if (this->fontFilename && !strcmp(this->fontFilename, fontFilename))
                        return true;
                    if (font)
                        msdfgen::destroyFont(font);
                    if ((font = msdfgen::loadFont(ft, fontFilename))) {
                        this->fontFilename = fontFilename;
                        return true;
                    }
                    this->fontFilename = nullptr;
                }
                return false;
            }
            operator msdfgen::FontHandle* () const {
                return font;
            }
        } font;

        for (FontInput& fontInput : fontInputs) {
            if (!font.load(fontInput.fontFilename))
            {
                std::cout << "ABORT " << "Failed to load specified font file." << std::endl;
                return 0;
            }
            if (fontInput.fontScale <= 0)
                fontInput.fontScale = 1;

            // Load character set
            Charset charset;
            if (fontInput.charsetFilename) {
                if (!charset.load(fontInput.charsetFilename, fontInput.glyphIdentifierType != GlyphIdentifierType::UNICODE_CODEPOINT))
                    std::cout << (fontInput.glyphIdentifierType == GlyphIdentifierType::GLYPH_INDEX ? "Failed to load glyph set specification." : "Failed to load character set specification.") << std::endl;
            }
            else {
                charset = Charset::ASCII;
                fontInput.glyphIdentifierType = GlyphIdentifierType::UNICODE_CODEPOINT;
            }

            // Load glyphs
            FontGeometry fontGeometry(&glyphs);
            int glyphsLoaded = -1;
            switch (fontInput.glyphIdentifierType) {
            case GlyphIdentifierType::GLYPH_INDEX:
                glyphsLoaded = fontGeometry.loadGlyphset(font, fontInput.fontScale, charset, config.preprocessGeometry, config.kerning);
                break;
            case GlyphIdentifierType::UNICODE_CODEPOINT:
                glyphsLoaded = fontGeometry.loadCharset(font, fontInput.fontScale, charset, config.preprocessGeometry, config.kerning);
                anyCodepointsAvailable |= glyphsLoaded > 0;
                break;
            }
            if (glyphsLoaded < 0)
            {
                std::cout << "ABORT " << "Failed to load glyphs from font." << std::endl;
                return 0;
            }
            printf("Loaded geometry of %d out of %d glyphs", glyphsLoaded, (int)charset.size());
            if (fontInputs.size() > 1)
                printf(" from font \"%s\"", fontInput.fontFilename);
            printf(".\n");
            // List missing glyphs
            if (glyphsLoaded < (int)charset.size()) {
                printf("Missing %d %s", (int)charset.size() - glyphsLoaded, fontInput.glyphIdentifierType == GlyphIdentifierType::UNICODE_CODEPOINT ? "codepoints" : "glyphs");
                bool first = true;
                switch (fontInput.glyphIdentifierType) {
                case GlyphIdentifierType::GLYPH_INDEX:
                    for (unicode_t cp : charset)
                        if (!fontGeometry.getGlyph(msdfgen::GlyphIndex(cp)))
                            printf("%c 0x%02X", first ? ((first = false), ':') : ',', cp);
                    break;
                case GlyphIdentifierType::UNICODE_CODEPOINT:
                    for (unicode_t cp : charset)
                        if (!fontGeometry.getGlyph(cp))
                            printf("%c 0x%02X", first ? ((first = false), ':') : ',', cp);
                    break;
                }
                printf("\n");
            }

            if (fontInput.fontName)
                fontGeometry.setName(fontInput.fontName);

            fonts.push_back((FontGeometry&&)fontGeometry);
        }
    }
    if (glyphs.empty())
        std::cout << "ABORT " << "No glyphs loaded." << std::endl;


    // Determine final atlas dimensions, scale and range, pack glyphs
    {
        double unitRange = 0, pxRange = 0;
        switch (rangeMode) {
        case RANGE_EM:
            unitRange = rangeValue;
            break;
        case RANGE_PIXEL:
            pxRange = rangeValue;
            break;
        }
        bool fixedDimensions = fixedWidth >= 0 && fixedHeight >= 0;
        bool fixedScale = config.emSize > 0;
        TightAtlasPacker atlasPacker;
        if (fixedDimensions)
            atlasPacker.setDimensions(fixedWidth, fixedHeight);
        else
            atlasPacker.setDimensionsConstraint(atlasSizeConstraint);
        atlasPacker.setPadding(config.imageType == ImageType::MSDF || config.imageType == ImageType::MTSDF ? 0 : -1);
        // TODO: In this case (if padding is -1), the border pixels of each glyph are black, but still computed. For floating-point output, this may play a role.
        if (fixedScale)
            atlasPacker.setScale(config.emSize);
        else
            atlasPacker.setMinimumScale(minEmSize);
        atlasPacker.setPixelRange(pxRange);
        atlasPacker.setUnitRange(unitRange);
        atlasPacker.setMiterLimit(config.miterLimit);
        if (int remaining = atlasPacker.pack(glyphs.data(), (int)glyphs.size())) {
            if (remaining < 0) 
            {
                std::cout << "ABORT " << "Failed to pack glyphs into atlas." << std::endl;
            }
            else {
                printf("Error: Could not fit %d out of %d glyphs into the atlas.\n", remaining, (int)glyphs.size());
                return 1;
            }
        }
        atlasPacker.getDimensions(config.width, config.height);
        if (!(config.width > 0 && config.height > 0))
            std::cout << "ABORT " << "Unable to determine atlas size." << std::endl;
        config.emSize = atlasPacker.getScale();
        config.pxRange = atlasPacker.getPixelRange();
        if (!fixedScale)
            printf("Glyph size: %.9g pixels/EM\n", config.emSize);
        if (!fixedDimensions)
            printf("Atlas dimensions: %d x %d\n", config.width, config.height);
    }

    // Generate atlas bitmap
    if (!layoutOnly) 
    {
        // Edge coloring
        if (config.imageType == ImageType::MSDF || config.imageType == ImageType::MTSDF) {
            if (config.expensiveColoring) {
                Workload([&glyphs, &config](int i, int threadNo) -> bool {
                    unsigned long long glyphSeed = (LCG_MULTIPLIER * (config.coloringSeed ^ i) + LCG_INCREMENT) * !!config.coloringSeed;
                    glyphs[i].edgeColoring(config.edgeColoring, config.angleThreshold, glyphSeed);
                    return true;
                    }, (int)glyphs.size()).finish(config.threadCount);
            }
            else {
                unsigned long long glyphSeed = config.coloringSeed;
                for (GlyphGeometry& glyph : glyphs) {
                    glyphSeed *= LCG_MULTIPLIER;
                    glyph.edgeColoring(config.edgeColoring, config.angleThreshold, glyphSeed);
                }
            }
        }

        bool success = false;
        switch (config.imageType) {
        case ImageType::HARD_MASK:
            if (floatingPointFormat)
                success = makeAtlas<float, float, 1, scanlineGenerator>(glyphs, fonts, config);
            else
                success = makeAtlas<byte, float, 1, scanlineGenerator>(glyphs, fonts, config);
            break;
        case ImageType::SOFT_MASK:
        case ImageType::SDF:
            if (floatingPointFormat)
                success = makeAtlas<float, float, 1, sdfGenerator>(glyphs, fonts, config);
            else
                success = makeAtlas<byte, float, 1, sdfGenerator>(glyphs, fonts, config);
            break;
        case ImageType::PSDF:
            if (floatingPointFormat)
                success = makeAtlas<float, float, 1, psdfGenerator>(glyphs, fonts, config);
            else
                success = makeAtlas<byte, float, 1, psdfGenerator>(glyphs, fonts, config);
            break;
        case ImageType::MSDF:
            if (floatingPointFormat)
                success = makeAtlas<float, float, 3, msdfGenerator>(glyphs, fonts, config);
            else
                success = makeAtlas<byte, float, 3, msdfGenerator>(glyphs, fonts, config);
            break;
        case ImageType::MTSDF:
            if (floatingPointFormat)
                success = makeAtlas<float, float, 4, mtsdfGenerator>(glyphs, fonts, config);
            else
                success = makeAtlas<byte, float, 4, mtsdfGenerator>(glyphs, fonts, config);
            break;
        }
        if (!success)
            result = 1;
    }

    if (config.csvFilename) {
        if (exportCSV(fonts.data(), (int)fonts.size(), config.width, config.height, config.yDirection, config.csvFilename))
            puts("Glyph layout written into CSV file.");
        else {
            result = 1;
            puts("Failed to write CSV output file.");
        }
    }
    if (config.jsonFilename) {
        if (exportJSON(fonts.data(), (int)fonts.size(), config.emSize, config.pxRange, config.width, config.height, config.imageType, config.yDirection, config.jsonFilename, config.kerning))
            puts("Glyph layout and metadata written into JSON file.");
        else {
            result = 1;
            puts("Failed to write JSON output file.");
        }
    }

    if (config.shadronPreviewFilename && config.shadronPreviewText) {
        if (anyCodepointsAvailable) {
            std::vector<unicode_t> previewText;
            utf8Decode(previewText, config.shadronPreviewText);
            previewText.push_back(0);
            if (generateShadronPreview(fonts.data(), (int)fonts.size(), config.imageType, config.width, config.height, config.pxRange, previewText.data(), config.imageFilename, floatingPointFormat, config.shadronPreviewFilename))
                puts("Shadron preview script generated.");
            else {
                result = 1;
                puts("Failed to generate Shadron preview file.");
            }
        }
        else {
            result = 1;
            puts("Shadron preview not supported in -glyphset mode.");
        }
    }

    FontSerializer FontSerial;
    std::ofstream outfile;
    std::ifstream imagefile;
    std::fstream trans;

    std::cout << "opening file" << std::endl;
    outfile.open("transferred.eosfont");
    imagefile.open("msdf.png", std::ios::binary | std::ios::ate);
    

    // packing the json 
    FontSerial.Load("msdfMetrics.json", outfile);

    // packing the png
    if (imagefile.is_open())
    {
        std::streampos end = imagefile.tellg();
        imagefile.seekg(0, std::ios::beg);
        size_t size = std::size_t(end - imagefile.tellg());
        if (size == 0)
            return 0;

        char* data = new char[size];
        if (!imagefile.read((char*)data, size))
        {
        }

        outfile << data;
        delete[] data;

        /*imagefile.seekg(0, std::ios::end);
        size_t size = imagefile.tellg();
        char* data = new char[size];
        imagefile.seekg(0, std::ios::beg);
        imagefile >> data;*/
    }
    
    //image.open("test.png", std::ios::in | std::ios::binary);
    //std::copy(std::istreambuf_iterator<char>(&image), {}, std::ostreambuf_iterator<char>(&outfile))
    

    imagefile.close();
    outfile.close();
    std::cout << "file closed" << std::endl;

    std::cout << "Font compiler ended" << std::endl;
    return result;
}