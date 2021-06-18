#pragma once
#include <Eigen/Dense>
#include "PTMacroDefine.h"
#include "PTAPI.h"

namespace PT
{
	class PTShapeDesc;
	
	struct PT_API Ray
	{
		Eigen::Vector3d Origin;
		Eigen::Vector3d Direction;
	};
	
	struct PT_API RayIntersectDesc
	{
		RayIntersectDesc()
			: bHit(false)
			, t(PT_NO_INTERSECT_VALUE)
			, HitShape(nullptr)
		{
			
		}
		
		bool bHit;
		double t;
		std::shared_ptr<PTShapeDesc> HitShape;
	};
}
