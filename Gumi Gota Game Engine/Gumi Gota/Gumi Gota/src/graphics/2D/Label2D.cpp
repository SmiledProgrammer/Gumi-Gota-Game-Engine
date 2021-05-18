
/* INCLUDES */
// Self Include
#include "Label2D.h"



namespace gg
{

	/* Constructors */
	Label2D::Label2D(const std::string& text, const vector2& pos, const vector2& size, Font* font)
		: Renderable2D(pos, size), m_text(text), m_font(font)
	{}

	Label2D::Label2D(const std::string& text, const vector2& pos, const vector2& size, Font* font, const Color& color)
		: Renderable2D(pos, size), m_text(text), m_font(font), m_color(color)
	{}

	/* Functions */
	void Label2D::bindFontTexture() const
	{
		m_font->bindFontTexture();
	}

	// Setters
	void Label2D::setText(const std::string& text)
	{
		m_text = text;
	}

	void Label2D::setColor(const Color& color)
	{
		m_color = color;
	}

	void Label2D::setFont(Font* font)
	{
		m_font = font;
	}

}