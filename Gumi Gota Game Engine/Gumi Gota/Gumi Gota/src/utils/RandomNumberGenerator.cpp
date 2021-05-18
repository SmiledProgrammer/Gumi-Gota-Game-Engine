
/* INCLUDES */
// Self Include
#include "RandomNumberGenerator.h"



namespace gg
{

	namespace rng
	{

		long long unsigned generateRandomNumber()
		{
			return distribution(generator);
		}

		long long unsigned generateRandomNumber(const long long unsigned maxValue)
		{
			return distribution(generator) % maxValue;
		}

	}

}