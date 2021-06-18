#include "CommonShape.h"

namespace PT
{
	ShapeSphere::ShapeSphere(const Eigen::Vector3d& center, double radius)
		: Center(center)
		, Radius(radius)
	{
		assert(Radius > 0.0);
	}

	RayIntersectDesc ShapeSphere::Hit(const Ray& ray) const
	{
		RayIntersectDesc result;
		
		Eigen::Vector3d op = Center - ray.Origin;
		double t, b = op.dot(ray.Direction), det = b * b - op.dot(op) + Radius * Radius;
		static const double eps = 1e-3;
		if (det < 0.0)
		{
			return result;
		}
		
		det = sqrt(det);
		result.t = (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0.0);
		result.bHit = (result.t > eps);

		//const Eigen::Vector3d intersect = ray.Origin + ray.Direction * result.t;
		//const Eigen::Vector3d n = (intersect - Center).normalized();

		//bool into = n.dot(ray.Direction) < 0;
		//result.t += (into ? -eps : eps);
		//result.t -= eps;
		return result;
	}
	
	/*RayIntersectDesc ShapeSphere::Hit(const Ray& ray) const
	{
		RayIntersectDesc result;
		result.t = PT_NO_INTERSECT_VALUE;
		result.bHit = false;
		
		const Eigen::Vector3d a = (Center - ray.Origin).cast<double>().normalized();
		const Eigen::Vector3d dir = ray.Direction.cast<double>().normalized();

		double cosValue = a.dot(dir);
		cosValue = std::max(-1.0, cosValue);
		cosValue = std::min(1.0, cosValue);
		
		const double angle = acos(cosValue);
		if (angle > 0.5 * PT_PI)
		{
			return result;
		}

		const double linelength = (Center - ray.Origin).norm();
		if (linelength < Radius)
		{
			return result;
		}
		
		const double distance = linelength * sin(angle);
		if (distance > Radius)
		{
			return result;
		}
		
		const double b = Radius * Radius - distance * distance;
		assert(b >= 0.0);

		result.bHit = true;
		result.t = linelength * cos(angle) - sqrt(b);
		assert(result.t >= -FLT_EPSILON);
		
		return result;
	}*/

	Eigen::Vector3d ShapeSphere::HitNormal(const Ray& ray, const RayIntersectDesc& hitInfo, bool& outInto)
	{
		const Eigen::Vector3d intersect = ray.Origin + ray.Direction * hitInfo.t;
		const Eigen::Vector3d n = (intersect - Center).normalized();

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
