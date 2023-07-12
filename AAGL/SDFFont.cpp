#include "SDFFont.h"

#include <iostream>

#include <AAGL.h>
#include <glad/glad.h>

#include <nlohmann/json.hpp>

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <hb.h>

void SDFFont::loadAtlas(std::filesystem::path path) {
	size_t atlasSize = 0;
	char* atlasData = graphics->assetFilesystem->loadAsset(path, atlasSize);

	if (!atlasData || atlasSize == 0) {
		std::cout << "SDFFont::loadAtlas Error loading atlas" << std::endl;
		return;
	}

	int components = 0;
	unsigned char* atlasImageData = stbi_load_from_memory((const unsigned char*)atlasData, atlasSize, &atlasWidth, &atlasHeight, &components, 0);

	if (components != 1) {
		std::cout << "SDFFont::loadAtlas Error, atlas contained " << components << " components, expected 1" << std::endl;
	}
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		atlasWidth,
		atlasHeight,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		atlasImageData
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	std::cout << "FontAtlas::genTexture() -> Generated texture: " << textureId << std::endl;

	stbi_image_free(atlasImageData);
	graphics->assetFilesystem->freeFile(atlasData);
}

void SDFFont::initHarfBuzz(std::filesystem::path path) {
	hbFontSize = 0;
	hbFontData = graphics->assetFilesystem->loadAsset(path, hbFontSize);
	hbFontBlob = hb_blob_create(hbFontData, hbFontSize, HB_MEMORY_MODE_READONLY, hbFontData, nullptr);
	hbFontFace = hb_face_create(hbFontBlob, 0);
	hbFont = hb_font_create(hbFontFace);
	hb_font_set_scale(hbFont, size * 64.0f * retinaScale, size * 64.0f * retinaScale);
}

void SDFFont::loadManifest(std::filesystem::path path) {
	size_t sdfManifestSize = 0;
	char* sdfManifestData = graphics->assetFilesystem->loadAsset(path, sdfManifestSize);
	if (!sdfManifestData || sdfManifestSize == 0) {
		std::cout << "SDFFont::loadManifest Error loading manifest" << std::endl;
		return;
	}

	nlohmann::json manifest = nlohmann::json::parse(std::string_view(sdfManifestData, sdfManifestSize));

	atlasWidth = manifest["width"];
	atlasHeight = manifest["height"];
	fontSize = manifest["size"];
	size = fontSize;

	if (manifest.contains("retina")) {
		if (manifest["retina"]) {
			retinaScale = manifest["retina_scale"].get<int>();
		}
	}

	if (manifest.contains("type")) {
		if (manifest["type"] == "bitmap") {
			shader = graphics->lazyLoadShader("shaders/atlas_text_bitmap");
		}
		else {
			shader = graphics->lazyLoadShader("shaders/atlas_text");
		}
	}
	else {
		shader = graphics->lazyLoadShader("shaders/atlas_text");
	}



	loadAtlas(path.parent_path() / manifest["atlas"]);
	initHarfBuzz(path.parent_path() / manifest["font"]);

	float retinaScaleInv = 1.f / retinaScale;

	for (auto& i : manifest["characters"]) {
		float s = (float)i["sx"].get<int>() / (float)atlasWidth;
		float t = (float)i["sy"].get<int>() / (float)atlasHeight;
		float w = i["ex"].get<int>() - i["sx"].get<int>();
		float h = i["ey"].get<int>() - i["sy"].get<int>();
		float bearingX = i["bx"].get<int>();
		float bearingY = i["by"].get<int>();
		float advance = i["a"].get<int>();
		float uvW = w / (float)atlasWidth;
		float uvH = h / (float)atlasHeight;

		atlasEntries[i["i"].get<int>()] = {
			glm::vec4(s, t, uvW, uvH),
			glm::vec3(bearingX, bearingY, advance),
			glm::mat4(
				glm::scale(
					glm::translate(
						glm::identity<glm::mat4>(),
						glm::vec3(bearingX * retinaScaleInv, -bearingY * retinaScaleInv, 0)
					),
					glm::vec3(w * retinaScaleInv, h * retinaScaleInv, 1.)
				)
			)
		};
	}

	graphics->assetFilesystem->freeFile(sdfManifestData);
}

void SDFFont::renderText(glm::mat4 projection, glm::vec3 position, glm::vec3 col, std::string_view text, float weight, float italics) {
	hb_buffer_t* buf;
	buf = hb_buffer_create();
	hb_buffer_add_utf8(buf, text.data(), text.size(), 0, -1);

	hb_buffer_set_direction(buf, hbDirection);
	hb_buffer_set_script(buf, hbScript);
	hb_buffer_set_language(buf, hb_language_from_string("en", -1));

	hb_shape(hbFont, buf, NULL, 0);

	unsigned int glyph_count;
	hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
	hb_glyph_position_t* glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

	hb_position_t cursor_x = 0;
	hb_position_t cursor_y = 0;

	glUseProgram(shader->id);
	glUniform4fv(glGetUniformLocation(shader->id, "col"), 1, glm::value_ptr(col));

	glBindVertexArray(mesh->vao);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	for (unsigned int i = 0; i < glyph_count; i++) {
		hb_codepoint_t glyphid = glyph_info[i].codepoint;
		hb_position_t x_offset = glyph_pos[i].x_offset;
		hb_position_t y_offset = glyph_pos[i].y_offset;
		hb_position_t x_advance = glyph_pos[i].x_advance;
		hb_position_t y_advance = glyph_pos[i].y_advance;

		glm::vec3 offset = glm::vec3(
			(float)cursor_x + (float)x_offset,
			(float)cursor_y + (float)y_offset,
			0
		) * glm::vec3(1.f / 64.f) * (1.f / retinaScale);

		auto ch = atlasEntries[glyphid];
		glm::mat4 view = glm::translate(
			glm::identity<glm::mat4>(),
			glm::vec3(position + offset)
		);
		glUniformMatrix4fv(glGetUniformLocation(shader->id, "mvp"), 1, false, glm::value_ptr(
			projection *
			view *
			glm::shear(ch.model, glm::vec3(0.f, 0.f, 0), glm::vec2(-italics * .25, 0), glm::vec2(0.0f, 0), glm::vec2(0))
		));
		glUniform4fv(glGetUniformLocation(shader->id, "atlasMetrics"), 1, glm::value_ptr(ch.atlasMetrics));
		glUniform1fv(glGetUniformLocation(shader->id, "weight"), 1, &weight);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->indexCount);
		//position.x += ch.fontMetrics.z;

		cursor_x += x_advance;
		cursor_y += y_advance;
	}

	hb_buffer_destroy(buf);
}

//TODO: create a text layout object that will take a string, beak it into a layout object that will use
//harfbuzz to layout the glyph positions, the bounding box will be a member and the object can be cached
//so that text isn't needlessly re-laid out and recalculated each frame

SDFFont::SDFFont(Graphics* graphics, std::filesystem::path path, hb_direction_t dir, hb_script_t script) :graphics(graphics), hbDirection(dir), hbScript(script) {
	retinaScale = 1;
	textureId = 0;
	shader = nullptr;
	mesh = graphics->findMesh("tlquad");
	loadManifest(path);
}

SDFFont::~SDFFont() {
	hb_font_destroy(hbFont);
	hb_face_destroy(hbFontFace);
	hb_blob_destroy(hbFontBlob);
	graphics->assetFilesystem->freeFile(hbFontData);
}
