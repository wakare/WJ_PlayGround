#pragma once
#include "PTShapeDesc.h"
#include "PTBasicDefine.h"
#include <cassert>
#include <memory>
#include <vector>

namespace PT
{
	class PT_API PTSceneObjectDesc
	{
	public:
		PTSceneObjectDesc(std::shared_ptr<PTMaterial> material);
		void AddShapeDesc(std::shared_ptr<PTShapeDesc> shape);

		// Return whether update result.t ?
		bool Intersect(const Ray& ray, RayIntersectDesc& out_result);
		const AABB& GetAABB();
		const PTMaterial& GetMaterial() const;
		
	private:
		std::shared_ptr<PTMaterial> Material;
		std::vector<std::shared_ptr<PTShapeDesc>> Shapes;
		AABB BoundingBox;
		bool NeedUpdateBoundingBox;
	};

	
}
