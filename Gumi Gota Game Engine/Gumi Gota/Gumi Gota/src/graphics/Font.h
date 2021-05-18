#ifndef GUMIGOTA_FONT_H
#define GUMIGOTA_FONT_H



/* INCLUDES */
// Normal Includes
#include <string>

// FreeType Includes
#include <ft2build.h>
#include FT_FREETYPE_H

// FreeType-GL Includes
#include "freetype-gl.h"



namespace gg
{

	class Font
	{
	private:
		/* Variables */
		ftgl::texture_atlas_t* m_atlas;
		ftgl::texture_font_t* m_font;

	public:
		/* Constructors */
		Font(const char* filename, const float size);
		
		/* Functions */
		void bindFontTexture() const;

		// Setters
		void setSize(const unsigned int size); /// check l8r (in .cpp file)

		// Getters
		inline ftgl::texture_font_t* getFTGLFont() const { return m_font; }

		inline const unsigned int& getTextureID() const { return m_atlas->id; } /// consider removing l8r

	};

}



#endif