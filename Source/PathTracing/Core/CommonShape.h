#pragma once
#include "PTShapeDesc.h"

namespace PT
{
	class PT_API ShapeSphere : public PTShapeDesc
	{
	public:
		ShapeSphere(const Eigen::Vector3d& center, double radius);

		RayIntersectDesc Hit(const Ray& ray) const override;
		Eigen::Vector3d HitNormal(const Ray& ray, const RayIntersectDesc& hitInfo, bool& outInto) override;
		AABB BoundingBox() const override;

	private:
		Eigen::Vector3d Center;
		double Radius;
	};
}
