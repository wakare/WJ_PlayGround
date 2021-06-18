#include "PTSceneDesc.h"

namespace PT
{
	void PTSceneDesc::AddSceneObject(std::shared_ptr<PTSceneObjectDesc> Object)
	{
		Objects.push_back(Object);
	}

	std::shared_ptr<PTSceneObjectDesc> PTSceneDesc::Intersect(const PT::Ray& ray, RayIntersectDesc& out_result)
	{
		std::shared_ptr<PTSceneObjectDesc> hitObj = nullptr;
		
		// TODO: Use accelerate struct
		for (auto& obj : Objects)
		{
			if (obj->Intersect(ray, out_result))
			{
				hitObj = obj;
			}
		}

		return hitObj;
	}
}
