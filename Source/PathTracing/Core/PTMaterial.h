#pragma once
#include "PTAPI.h"
#include <Eigen/Dense>

namespace PT
{
	enum MaterialReflectType
	{
		EMRT_DIFFUSE,
		EMRT_SPECULAR,
		EMRT_REFRACT
	};
	
	class PT_API PTMaterial
	{
	public:
		PTMaterial();
		PTMaterial(MaterialReflectType reflectType, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& emission);
		
		MaterialReflectType ReflectType;
		Eigen::Vector3f Diffuse;
		Eigen::Vector3f Emission;
	};

}
