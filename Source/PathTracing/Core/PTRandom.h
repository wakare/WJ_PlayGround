#pragma once
#include <cstdlib>
#include <cassert>

namespace PT
{
	template<typename scalar>
	class PTRandom
	{
	public:
		static void SetSeed(int seed)
		{
			srand(seed);
		}
		
		static scalar Generate()
		{
			assert(false);
			return scalar();
		}
	};

	template<>
	inline float PTRandom<float>::Generate()
	{
		return 1.0f * rand() / (RAND_MAX + 1);
	}
}
