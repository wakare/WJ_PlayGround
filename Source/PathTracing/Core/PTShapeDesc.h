#pragma once
#include "RayIntersectDesc.h"

namespace PT
{
	typedef Eigen::AlignedBox3d AABB;
	
	class PT_API PTShapeDesc
	{
	public:
		virtual RayIntersectDesc Hit(const Ray& ray) const = 0;
		virtual Eigen::Vector3d HitNormal(const Ray& ray, const RayIntersectDesc& hitInfo, bool& outInto) = 0;
		virtual AABB BoundingBox() const = 0;
	};

	
}
