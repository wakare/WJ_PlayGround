#pragma once
#include "Eigen/Dense"
#include "PTBasicDefine.h"

namespace PT
{
	class PTShapeDesc;
	
	struct PT_API Ray
	{
		PTVector3d Origin;
        PTVector3d Direction;
	};
	
	struct PT_API RayIntersectDesc
	{
		RayIntersectDesc()
			: bHit(false)
			, t(NO_HIT_DIST)
			, HitShape(nullptr)
		{
			
		}
		
		bool bHit;
		double t;
		std::shared_ptr<PTShapeDesc> HitShape;
	};
}
