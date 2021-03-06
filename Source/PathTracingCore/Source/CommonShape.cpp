#include "CommonShape.h"

namespace PT
{
	ShapeSphere::ShapeSphere(const PTVector3d & center, double radius)
		: Center(center)
		, Radius(radius)
	{
		assert(Radius > 0.0);
	}

	RayIntersectDesc ShapeSphere::Hit(const Ray& ray) const
	{
		RayIntersectDesc result;

		// Important: must use double precision to calculate op!
		PT::PTVector3d op = Center - ray.Origin.cast<double>();
		double b = op.dot(ray.Direction.cast<double>());
		double det = b * b - op.dot(op) + Radius * Radius;

		static const double eps = DBL_EPSILON;
		if (det < 0.0)
		{
			return result;
		}
		
		det = sqrt(det);
		result.t = ( b - det) > eps ?  b - det : (( b + det) > eps ?  b + det : 0.0);
		result.bHit = (result.t > 0.0f);

		return result;
	}

	PTVector3f ShapeSphere::HitNormal(const Ray& ray, const RayIntersectDesc& hitInfo, bool& outInto)
	{
		const PTVector3f intersect = ray.Origin + ray.Direction * hitInfo.t;
		const PTVector3f n = (intersect - Center.cast<float>()).normalized();

		outInto = n.dot(ray.Direction) < 0;
		return outInto ? n : -n;
	}

	AABB ShapeSphere::BoundingBox() const
	{
		const Eigen::Vector3d min = { Center.x() - Radius,Center.y() - Radius ,Center.z() - Radius };
		const Eigen::Vector3d max = { Center.x() + Radius,Center.y() + Radius ,Center.z() + Radius };
		
		return Eigen::AlignedBox3d(min, max);
	}
}
