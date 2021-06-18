#pragma once
#include "PTSceneObjectDesc.h"

namespace PT
{
	class PT_API PTSceneDesc
	{
	public:
		void AddSceneObject(std::shared_ptr<PTSceneObjectDesc> Object);

		std::shared_ptr<PTSceneObjectDesc> Intersect(const PT::Ray& ray, RayIntersectDesc& out_result);
		
	private:
		std::vector<std::shared_ptr<PTSceneObjectDesc>> Objects;
	};
}
