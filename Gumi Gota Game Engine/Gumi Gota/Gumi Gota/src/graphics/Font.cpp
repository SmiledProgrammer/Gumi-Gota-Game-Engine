
/* INCLUDES */
// Self Include
#include "Font.h"

// OpenGL Includes
#include "GL/glew.h"

// Header Includes
#include "ErrorHandling.h"



namespace gg
{

	/* Constructors */
	Font::Font(const char* filename, const float size)
		: m_atlas(ftgl::texture_atlas_new(512, 512, 1)), m_font(ftgl::texture_font_new_from_file(m_atlas, size, filename))
	{}

	/* Functions */
	void Font::bindFontTexture() const
	{
		GLcall( glBindTexture(GL_TEXTURE_2D, m_atlas->id) );
	}

	// Setters
	void Font::setSize(const unsigned int size)
	{
		/// check l8r
		///m_font = ftgl::texture_font_new_from_file(m_atlas, size, filename);
	}

}