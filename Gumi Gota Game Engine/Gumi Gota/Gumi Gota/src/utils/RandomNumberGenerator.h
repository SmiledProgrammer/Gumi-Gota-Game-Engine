#ifndef GUMIGOTA_RANDOM_NUMBER_GENERATOR_H
#define GUMIGOTA_RANDOM_NUMBER_GENERATOR_H



/* INCLUDES */
// Normal Includes
#include <random>



namespace gg
{

	namespace rng //stands for random number generator
	{

		static std::random_device randomDevice;
		static std::default_random_engine generator = std::default_random_engine(rng::randomDevice());
		static std::uniform_int_distribution<long long unsigned> distribution = std::uniform_int_distribution<long long unsigned>(0, 0xFFFFFFFFFFFFFFFF);

		long long unsigned generateRandomNumber();
		long long unsigned generateRandomNumber(const long long unsigned maxValue);

	}

}



#endif