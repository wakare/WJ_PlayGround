#pragma once
#include "PTAPI.h"
#include "PTMacroDefine.h"
#include <Eigen/Dense>

namespace PT
{
	class PT_API PTCamera
	{
	public:
		PTCamera();
		
		Eigen::Vector3d Pos;
		Eigen::Vector3d Dir;
	};

}
