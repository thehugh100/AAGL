#pragma once

#include <iostream>
#include <map>

#include <AAGL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <hb.h>


#define AAGL_SDFFONT_HARFBUZZ

struct FontChar {
	glm::vec4 atlasMetrics; //s, t, uvW, uvH
	glm::vec3 fontMetrics; //bearingX, bearingY, advance
	glm::mat4 model;
};


class SDFFont {
public:
	void loadAtlas(std::filesystem::path path);

	void initHarfBuzz(std::filesystem::path path);

	void loadManifest(std::filesystem::path path);

	void renderText(glm::mat4 projection, glm::vec3 position, glm::vec3 col, std::string_view text, float weight = 1.f, float italics = 0.f);

	//TODO: create a text layout object that will take a string, beak it into a layout object that will use
	//harfbuzz to layout the glyph positions, the bounding box will be a member and the object can be cached
	//so that text isn't needlessly re-laid out and recalculated each frame
	SDFFont(
		Graphics* graphics,
		std::filesystem::path path,
		hb_direction_t dir = HB_DIRECTION_LTR,
		hb_script_t script = HB_SCRIPT_LATIN
	);

	~SDFFont();

	Mesh* mesh;
	Shader* shader;
	Graphics* graphics;
	unsigned int textureId;
	std::map<unsigned int, FontChar> atlasEntries;
	int atlasWidth;
	int atlasHeight;
	float size;
	float fontSize;
	float retinaScale;
	hb_blob_t* hbFontBlob;
	hb_face_t* hbFontFace;
	hb_font_t* hbFont;
	size_t hbFontSize;
	char* hbFontData;
	hb_direction_t hbDirection;
	hb_script_t hbScript;
};