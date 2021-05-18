#ifndef GUMIGOTA_LABEL2D_H
#define GUMIGOTA_LABEL2D_H



/* INCLUDES */
// Normal Includes
#include <string>

// Header Includes
#include "Renderable2D.h"

#include "../Color.h"
#include "../Font.h"



namespace gg
{

	class Label2D : public Renderable2D
	{
	protected:
		/* Variables */
		std::string m_text;
		Color m_color;
		Font* m_font;

	public:
		/* Constructors */
		Label2D(const std::string& text, const vector2& pos, const vector2& size, Font* font);
		Label2D(const std::string& text, const vector2& pos, const vector2& size, Font* font, const Color& color);

		/* Functions */
		void bindFontTexture() const;

		// Setters
		void setText(const std::string& text);
		void setColor(const Color& color);
		void setFont(Font* font);

		// Getters
		inline const std::string& getText()  const { return m_text;  }
		inline const Color&       getColor() const { return m_color; }

		inline ftgl::texture_font_t* getFTGLFont() const { return m_font->getFTGLFont(); }

	};

}



#endif