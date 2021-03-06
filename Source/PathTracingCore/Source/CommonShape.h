#pragma once
#include "PTShapeDesc.h"

namespace PT
{
	class PT_API ShapeSphere : public PTShapeDesc
	{
	public:
		ShapeSphere(const PTVector3d& center, double radius);

		RayIntersectDesc Hit(const Ray& ray) const override;
        PTVector3f HitNormal(const Ray& ray, const RayIntersectDesc& hitInfo, bool& outInto) override;
		AABB BoundingBox() const override;

	private:
	    // Must use double precision
		PTVector3d Center;

		double Radius;
	};
}
