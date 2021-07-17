#pragma once
#include "Eigen/Dense"
#include "PTBasicDefine.h"

namespace PT
{
	class PTShapeDesc;
	
	struct PT_API Ray
	{
		PTVector3f Origin;
        PTVector3f Direction;
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
