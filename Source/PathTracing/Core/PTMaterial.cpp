#include "PTMaterial.h"

namespace PT
{
	PTMaterial::PTMaterial()
		: ReflectType(EMRT_DIFFUSE)
		, Diffuse({0.0f, 0.0f ,0.0f})
		, Emission({ 0.0f, 0.0f ,0.0f })
	{
	}

	PTMaterial::PTMaterial(MaterialReflectType reflectType, const Eigen::Vector3f& diffuse,
		const Eigen::Vector3f& emission)
		: ReflectType(reflectType)
		, Diffuse(diffuse)
		, Emission(emission)
	{
	}
}
