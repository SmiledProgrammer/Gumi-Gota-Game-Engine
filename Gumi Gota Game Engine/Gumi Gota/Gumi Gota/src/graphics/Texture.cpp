
/* INCLUDES */
// Self Include
#include "Texture.h"

// OpenGL Includes
#include <GL/glew.h>

// STBI Image Includes
#include <stb_image.h>

// Header Includes
#include "ErrorHandling.h"

#include "../Log.h"



/* DEFINITIONS */
#define TEXTURE_DEFAULT_FILTER	LINEAR
#define TEXTURE_DEFAULT_WRAP	REPEAT
#define TEXTURE_DEFAULT_ANISOTROPIC_FILTERING 0



namespace gg
{

	/* Constructors */
	Texture::Texture(const std::string& filepath)
		: m_atlasRows(1)
	{
		m_loadImage(filepath, TEXTURE_DEFAULT_FILTER, TEXTURE_DEFAULT_WRAP, TEXTURE_DEFAULT_ANISOTROPIC_FILTERING);
	}

	Texture::Texture(const std::string& filepath, const unsigned int atlasRows)
		: m_atlasRows(atlasRows)
	{
		m_loadImage(filepath, TEXTURE_DEFAULT_FILTER, TEXTURE_DEFAULT_WRAP, TEXTURE_DEFAULT_ANISOTROPIC_FILTERING);
	}

	Texture::Texture(const std::string& filepath, const TextureFilter filter, const TextureWrap wrap, const unsigned int anisotropicFiltering)
		: m_atlasRows(1)
	{
		m_loadImage(filepath, filter, wrap, anisotropicFiltering);
	}

	Texture::Texture(const std::string& filepath, const unsigned int atlasRows, const TextureFilter filter, const TextureWrap wrap, const unsigned int anisotropicFiltering)
		: m_atlasRows(atlasRows)
	{
		m_loadImage(filepath, filter, wrap, anisotropicFiltering);
	}


	Texture::Texture(const unsigned int textureID, const unsigned int width, const unsigned int height)
		: m_id(textureID),
		  m_width(width), m_height(height),
		  m_atlasRows(1)
	{}


	Texture::Texture(const Color& color)
		: m_width(1), m_height(1),
		  m_atlasRows(1)
	{
		m_createSingleColorTexture(color);
	}

	/* Destructor */
	Texture::~Texture()
	{
		GLcall( glDeleteTextures(1, &m_id) );
	}

	/* Functions */
	void Texture::bind() const
	{
		GLcall( glActiveTexture(GL_TEXTURE0) );
		GLcall( glBindTexture(GL_TEXTURE_2D, m_id) );
	}

	void Texture::bind(const unsigned int slot) const
	{
		if (slot < 32)
		{
			GLcall( glActiveTexture(GL_TEXTURE0 + slot) );
			GLcall( glBindTexture(GL_TEXTURE_2D, m_id) );
		}
		else
		{
			LOG("[Warning] Texture Slot is out of range.");
		}
	}

	void Texture::unbind()
	{
		GLcall( glBindTexture(GL_TEXTURE_2D, 0) );
	}

	// Setters
	void Texture::setAtlasRows(const unsigned int amount)
	{
		m_atlasRows = amount;
	}

	// Private Functions
	void Texture::m_loadImage(const std::string& filepath, const TextureFilter filter, const TextureWrap wrap, const unsigned int anisotropicFiltering)
	{
		// Init
		unsigned char* textureData;
		int BPP;

		// Loading Image
		stbi_set_flip_vertically_on_load(1);
		textureData = stbi_load(filepath.c_str(), &m_width, &m_height, &BPP, 4);

		if (!textureData)
			LOG("[Warning] Couldn't load texture '" << filepath << "'.");

		// Creating Texture
		GLcall( glGenTextures(1, &m_id) );
		GLcall( glBindTexture(GL_TEXTURE_2D, m_id) );

		// Texture Setup
		switch (filter)
		{
		case LINEAR :
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
			break;

		case NEAREST :
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
			break;
		}

		switch (wrap)
		{
		case CLAMP_TO_EDGE :
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
			break;
			
		case REPEAT :
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
			break;

		case MIRROR_REPEAT :
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) );
			GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) );
			break;
		}

		if (anisotropicFiltering > 0)
		{
			if (glewIsSupported("GL_EXT_texture_filter_anisotropic") || GLEW_EXT_texture_filter_anisotropic)
			{
				float anistropicFilteringAmount;

				// Checking Maximum Amount of Anisotropic Filtering
				float maxAnisotropicFiltering;
				GLcall( glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropicFiltering) );

				// Getting the Maximum/Desired Amount of Anisotropic Filtering
				if (anisotropicFiltering > maxAnisotropicFiltering)
				{
					anistropicFilteringAmount = maxAnisotropicFiltering;

					LOG("[Warning] The driver's maximal anisotropic filtering amount is " << (unsigned int)maxAnisotropicFiltering << "!");
				}
				else
				{
					anistropicFilteringAmount = (float)anisotropicFiltering;
				}

				// Applying Anisotropic Filtering
				GLcall( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropicFilteringAmount) );
			}
			else
			{
				LOG("[Warning] The driver doesn't support anisotropic filtering!");
			}
		}

		// Loading Up Data
		GLcall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData) );

		// Finishing
		GLcall( glBindTexture(GL_TEXTURE_2D, 0) );

		// Freeing Data
		stbi_image_free(textureData);
	}

	void Texture::m_createSingleColorTexture(const Color& color)
	{
		// Creating Texture
		GLcall( glGenTextures(1, &m_id) );
		GLcall( glBindTexture(GL_TEXTURE_2D, m_id) );

		// Texture Setup
		GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
		GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
		GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
		GLcall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );

		// Loading Up Data
		unsigned char* textureData = new unsigned char[4];
		textureData[0] = (unsigned int)(color.r * 255.0f);
		textureData[1] = (unsigned int)(color.g * 255.0f);
		textureData[2] = (unsigned int)(color.b * 255.0f);
		textureData[3] = (unsigned int)(color.a * 255.0f);

		///for (int i = 0; i < 4; i++)
			///std::cout << +textureData[i] << std::endl;

		///std::cout << std::endl;

		GLcall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData) );

		// Finishing
		GLcall( glBindTexture(GL_TEXTURE_2D, 0) );

		delete[] textureData;
	}

}