#pragma once
#include "PTBasicDefine.h"
#include "PTSceneDesc.h"

namespace PTMain
{
	class PTScene
	{
	public:
		static PTScene& Get();
		PTScene();
		
		void DoPathTracing(int width, int height, int sampleCount);
		Eigen::Vector3f Radiance(const PT::Ray& ray, int depth);
		
	private:
		std::shared_ptr<PT::PTSceneDesc> SceneDesc;
		std::shared_ptr<PT::PTCamera> Camera;
	};
}
