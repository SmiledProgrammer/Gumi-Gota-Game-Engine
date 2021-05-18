
/* INCLUDES */
// Self Include
#include "Trigonometry.h"

// Normal Includes
#include <cmath>

// Header Includes
#include "../general/Constants.h"



namespace gg
{

	/* FUNCTIONS */
	float sine(const float radians)
	{
		return sin(radians);
	}

	float cosine(const float radians)
	{
		return cos(radians);
	}

	float tangent(const float radians)
	{
		return tan(radians);
	}

	float cotangent(const float radians)
	{
		return 1.0f / tangent(radians);
	}


	float sineAngle(const float value)
	{
		return asin(value);
	}

	float cosineAngle(const float value)
	{
		return acos(value);
	}

	float tangentAngle(const float value)
	{
		return atan(value);
	}

	float cotangentAngle(const float value)
	{
		return GUMIGOTA_PI_DIVIDED_BY_2 - tangentAngle(value);
	}

}