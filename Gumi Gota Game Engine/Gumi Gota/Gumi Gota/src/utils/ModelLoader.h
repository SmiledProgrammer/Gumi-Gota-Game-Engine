#ifndef GUMIGOTA_MODELLOADER_H
#define GUMIGOTA_MODELLOADER_H



/* INCLUDES */
// Header Includes
#include "../graphics/3D/Model.h"



namespace gg
{

	/* FUNCTIONS */
	Model loadModelFromFile(const std::string& objFilepath, const std::string& textureFilepath); //loading model from OBJ file and texture from file
	Model loadModelFromFile(const std::string& objFilepath, Texture* text); //loading model from OBJ file and texture

}



#endif