#include "pch.h"
#include "Assets/Fonts/Font.h"
#include "Rendering/Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/gl.h>

namespace SoulEngine
{
    const char* characterSet = R"(ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`-=[]\;',./~!@#$%^&*()_+{}|:""<>? )";
    const int characterLength = 96;

	Font::Font(const std::string& path, int size) : _size(size), _path(path)
	{
        auto last = path.find_last_of(PATH_SEPARATOR);
        _name = path.substr(last, path.size() - last);

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            throw std::runtime_error("Could not init FreeType Library");

        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face))
            throw std::runtime_error("Failed to load font");
        else {
            FT_Set_Pixel_Sizes(face, 0, 96);

            FT_GlyphSlot glyphy = face->glyph;

            int atlasWidth = 0;
            int atlasHeight = 0;
            for (int i = 0; i < characterLength; i++)
            {
                unsigned char c = characterSet[i];
                if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    fprintf(stderr, "Loading character %c failed!\n", c);
                    continue;
                }
                atlasWidth += glyphy->bitmap.width;
                atlasHeight = std::max(atlasHeight, static_cast<int>(glyphy->bitmap.rows));
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            _texture = CreateRef<Texture>(GL_RED, GL_RED, GL_UNSIGNED_BYTE, atlasWidth, atlasHeight);

            int x = 0, y = 0;
            for (int i = 0; i < characterLength; i++)
            {
                unsigned char c = characterSet[i];
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    continue;

                Character* character = new Character();
                characters.emplace(c, character);

                float px = x / static_cast<float>(atlasWidth);
                float py = y / static_cast<float>(atlasHeight);
                float aw = static_cast<float>(glyphy->bitmap.width) / static_cast<float>(atlasWidth);
                float ah = static_cast<float>(glyphy->bitmap.rows) / static_cast<float>(atlasHeight);

                character->Advance = { glyphy->advance.x >> 6, glyphy->advance.y >> 6 };
                character->Size = { glyphy->bitmap.width, glyphy->bitmap.rows };
                character->Bearing = { glyphy->bitmap_left, glyphy->bitmap_top };
                character->TextureRect = { px, py, aw, ah };

                _texture->SetPixels(x, y, glyphy->bitmap.width, glyphy->bitmap.rows, glyphy->bitmap.buffer);
                x += glyphy->bitmap.width + 1;
            }
            _texture->UnBind();
            glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}
}