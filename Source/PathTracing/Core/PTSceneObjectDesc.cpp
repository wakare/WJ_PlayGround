#include "PTSceneObjectDesc.h"

namespace PT
{
	PTSceneObjectDesc::PTSceneObjectDesc(std::shared_ptr<PTMaterial> material)
		: Material(material)
		, NeedUpdateBoudingBox(true)
	{
	}

	void PTSceneObjectDesc::AddShapeDesc(std::shared_ptr<PTShapeDesc> shape)
	{
		Shapes.push_back(shape);
		NeedUpdateBoudingBox = true;
	}

	bool PTSceneObjectDesc::Intersect(const Ray& ray, RayIntersectDesc& out_result)
	{
		bool bUpdate = false;
		for (auto& shape : Shapes)
		{
			RayIntersectDesc result = shape->Hit(ray);
			if (!result.bHit)
			{
				continue;
			}

			// Compare current hit results
			if (!out_result.bHit)
			{
				out_result = result;
				out_result.HitShape = shape;
				bUpdate = true;
				continue;
			}

			if (out_result.t > result.t)
			{
				out_result = result;
				out_result.HitShape = shape;
				bUpdate = true;
			}
		}

		return bUpdate;
	}

	const AABB& PTSceneObjectDesc::GetAABB()
	{
		if (NeedUpdateBoudingBox)
		{
			for (const auto& Desc : Shapes)
			{
				assert(Desc.get());
				BoundingBox.extend(Desc->BoundingBox());
			}

			NeedUpdateBoudingBox = false;
		}

		return BoundingBox;
	}

	const PTMaterial& PTSceneObjectDesc::GetMaterial() const
	{
		return *Material;
	}
}
