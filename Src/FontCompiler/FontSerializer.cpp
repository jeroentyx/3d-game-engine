#include "FontSerializer.h"

std::string FontSerializer::OpenFile(const std::string& filepath)
{
	std::string result;
	std::ifstream fp(filepath, std::ios::in | std::ios::binary);
	if (fp)
	{
		fp.seekg(0, std::ios::end);
		result.resize(fp.tellg());
		fp.seekg(0, std::ios::beg);
		fp.read(&result[0], result.size());
	}

	return result;
}

bool FontSerializer::Load(const std::string& filepath, std::ofstream& out)
{

	std::string json = OpenFile(filepath);

	JsonReader Reader;
	if (Reader.CheckForError(json.c_str())) //If this false means json syntax wrong
	{
		size_t numofglyphs = 0;

		Reader.StartObject();
		//Reading atlas
		Reader.Member("atlas");
		Reader.StartObject();
		Reader.Member("type")& type;
		Reader.Member("distanceRange")& distanceRange;
		Reader.Member("size")& size;
		Reader.Member("width")& width;
		Reader.Member("height")& height;
		Reader.Member("yOrigin")& yOrigin;
		Reader.EndObject();
		//Reading Metrics Object
		Reader.Member("metrics");
		Reader.StartObject();
		Reader.Member("emSize")& emSize;
		Reader.Member("lineHeight")& lineHeight;
		Reader.Member("ascender")& ascender;
		Reader.Member("descender")& descender;
		Reader.Member("underlineY")& underlineY;
		Reader.Member("underlineThickness")& underlineThickness;
		Reader.EndObject();
		//Reading of Glyphs
		Reader.Member("glyphs");
		Reader.StartArray(&numofglyphs);
		for (size_t i = 0; i < numofglyphs; ++i)
		{
			std::pair<int, Data> glyphs;
			Reader.StartObject();
			Reader.Member("unicode")& glyphs.first;
			Reader.Member("advance")& glyphs.second.advance;

			if (Reader.HasMember("planeBounds"))
			{
				Reader.Member("planeBounds");
				Reader.StartObject();
				Reader.Member("left")& glyphs.second.planebound.x;
				Reader.Member("bottom")& glyphs.second.planebound.y;
				Reader.Member("right")& glyphs.second.planebound.z;
				Reader.Member("top")& glyphs.second.planebound.w;
				Reader.EndObject();
			}

			if (Reader.HasMember("atlasBounds"))
			{

				Reader.Member("atlasBounds");
				Reader.StartObject();
				Reader.Member("left")& glyphs.second.atlasbound.x;
				Reader.Member("bottom")& glyphs.second.atlasbound.y;
				Reader.Member("right")& glyphs.second.atlasbound.z;
				Reader.Member("top")& glyphs.second.atlasbound.w;
				Reader.EndObject();
			}
			
			Reader.EndObject();

			FontData.insert(glyphs);
		}
		Reader.EndArray();
		
		if (Reader.HasMember("kerning"))
		{

		}


		Reader.EndObject();


		for (auto data : FontData)
		{
			out << "Alphabet : " << data.first << std::endl;
			out << "Data     : " << data.second.advance << std::endl;
			out << "planebounds  : " << data.second.planebound.x << std::endl;
			out << "planebounds  : " << data.second.planebound.y << std::endl;
			out << "planebounds  : " << data.second.planebound.z << std::endl;
			out << "planebounds  : " << data.second.planebound.w << std::endl;
			out << "atlasBounds  : " << data.second.atlasbound.x << std::endl;
			out << "atlasBounds  : " << data.second.atlasbound.y<< std::endl;
			out << "atlasBounds  : " << data.second.atlasbound.z << std::endl;
			out << "atlasBounds  : " << data.second.atlasbound.w << std::endl;
		}

		

		return true;
	}

	return false;
}
