#include "PTScene.h"
#include <Eigen/Dense>
#include <ppl.h>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "CommonShape.h"
#include "PTRandom.h"

namespace PTMain
{
	constexpr int RRMinDepth = 5;
	
	static auto clamp = [](const Eigen::Vector3f& v)
	{
		return Eigen::Vector3f(
			v.x() > 1.0f ? 1.0f : (v.x() < 0.0f ? 0.0f : v.x()),
			v.y() > 1.0f ? 1.0f : (v.y() < 0.0f ? 0.0f : v.y()),
			v.z() > 1.0f ? 1.0f : (v.z() < 0.0f ? 0.0f : v.z())
		);
	};
	
	PTScene& PTScene::Get()
	{
		static PTScene scene;
		return scene;
	}

	PTScene::PTScene()
		: SceneDesc(nullptr)
		, Camera(nullptr)
	{
		// Initialize Scene
		{
			SceneDesc = std::make_shared<PT::PTSceneDesc>();

			std::shared_ptr<PT::ShapeSphere> Sphere1 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(1e5f + 1.0f, 40.8f, 81.6f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere2 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(-1e5f + 99.0f, 40.8f, 81.6f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere3 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(50.0f, 40.8f, 1e5f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere4 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(50.0f, 40.8f, -1e5f + 170.0f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere5 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(50.0f, 1e5f, 81.6f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere6 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(50.0f, -1e5f + 81.6f, 81.6f), 1e5f);
			std::shared_ptr<PT::ShapeSphere> Sphere7 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(27.0f, 16.5f, 47.0f), 16.5f);
			std::shared_ptr<PT::ShapeSphere> Sphere8 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(73.0f, 16.5f, 78.0f), 16.5f);
			std::shared_ptr<PT::ShapeSphere> Sphere9 = std::make_shared<PT::ShapeSphere>(Eigen::Vector3d(50.0f, 681.6f - 0.27f, 81.6f), 600.0f);

			std::shared_ptr<PT::PTMaterial> material1 = std::make_shared<PT::PTMaterial>(PT::EMRT_DIFFUSE,
				Eigen::Vector3f{ 0.75f, 0.25f, 0.25f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PT::PTMaterial> material2 = std::make_shared<PT::PTMaterial>(PT::EMRT_DIFFUSE,
				Eigen::Vector3f{ 0.25f, 0.25f, 0.75f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PT::PTMaterial> material3 = std::make_shared<PT::PTMaterial>(PT::EMRT_DIFFUSE,
				Eigen::Vector3f{ 0.75f, 0.75f, 0.75f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PT::PTMaterial> material4 = std::make_shared<PT::PTMaterial>(PT::EMRT_DIFFUSE,
				Eigen::Vector3f{ 0.0f, 0.0f, 0.0f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PT::PTMaterial> material5 = std::make_shared<PT::PTMaterial>(PT::EMRT_SPECULAR,
				Eigen::Vector3f{ 0.9f, 0.9f, 0.9f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PT::PTMaterial> material6 = std::make_shared<PT::PTMaterial>(PT::EMRT_REFRACT,
				Eigen::Vector3f{ 0.9f, 0.9f, 0.9f }, Eigen::Vector3f{ 0.0f, 0.0f, 0.0f }); 

			std::shared_ptr<PT::PTMaterial> material7 = std::make_shared<PT::PTMaterial>(PT::EMRT_DIFFUSE,
				Eigen::Vector3f{ 0.0f, 0.0f, 0.0f }, Eigen::Vector3f{ 2.0f, 2.0f, 2.0f });

			std::shared_ptr<PT::PTSceneObjectDesc> Obj1 = std::make_shared<PT::PTSceneObjectDesc>(material1);
			Obj1->AddShapeDesc(Sphere1);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj2 = std::make_shared<PT::PTSceneObjectDesc>(material2);
			Obj2->AddShapeDesc(Sphere2);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj3 = std::make_shared<PT::PTSceneObjectDesc>(material3);
			Obj3->AddShapeDesc(Sphere3);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj4 = std::make_shared<PT::PTSceneObjectDesc>(material4);
			Obj4->AddShapeDesc(Sphere4);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj5 = std::make_shared<PT::PTSceneObjectDesc>(material3);
			Obj5->AddShapeDesc(Sphere5);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj6 = std::make_shared<PT::PTSceneObjectDesc>(material3);
			Obj6->AddShapeDesc(Sphere6);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj7 = std::make_shared<PT::PTSceneObjectDesc>(material5);
			Obj7->AddShapeDesc(Sphere7);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj8 = std::make_shared<PT::PTSceneObjectDesc>(material6);
			Obj8->AddShapeDesc(Sphere8);

			std::shared_ptr<PT::PTSceneObjectDesc> Obj9 = std::make_shared<PT::PTSceneObjectDesc>(material7);
			Obj9->AddShapeDesc(Sphere9);

			SceneDesc->AddSceneObject(Obj1);
			SceneDesc->AddSceneObject(Obj2);
			SceneDesc->AddSceneObject(Obj3);
			SceneDesc->AddSceneObject(Obj4);
			SceneDesc->AddSceneObject(Obj5);
			SceneDesc->AddSceneObject(Obj6);
			SceneDesc->AddSceneObject(Obj7);
			SceneDesc->AddSceneObject(Obj8);
			SceneDesc->AddSceneObject(Obj9);
		}

		// Initialize Camera
		{
			Camera = std::make_shared<PT::PTCamera>();
			Camera->Pos = { 50.0, 52.0, 295.6 };
			Camera->Dir = { 0.0, -0.042612, -1.0 };
			Camera->Dir.normalize();
		}
	}

	void PTScene::DoPathTracing(int width, int height, int sampleCount)
	{
		Eigen::Vector3d cx = { width * 0.5135f / height, 0.0f, 0.0f };
		Eigen::Vector3d cy = 0.5135f * cx.cross(Camera->Dir).normalized();

		std::vector<Eigen::Vector3f> pixels(width * height);

		Concurrency::parallel_for(0, height, [&](int y)
		{
			const size_t startIndex = width * (height - y - 1);
			for (int x = 0; x < width; ++x)
			{
				Eigen::Vector3f& pixel = pixels[x + startIndex];
				pixel.setZero();
				
				for (int n = 0; n < sampleCount; ++n)
				{
					PT::Ray ray;
					ray.Direction = cx * ((x + PT::PTRandom<float>::Generate()) / width - 0.5f)
						+ cy * ((y + PT::PTRandom<float>::Generate()) / height - 0.5f)
						+ Camera->Dir;
					ray.Direction.normalize();
					ray.Origin = Camera->Pos + ray.Direction * 140.0f;
					
					pixel += clamp(Radiance(ray, 0));
				}

				pixel /= sampleCount;
			}
		});

        static auto toInt = [](float x)
        {
            return int(pow(x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x), 1.0f / 2.2f) * 255.0f + 0.5f);
        };

		// Convert to rgb data format
        char* _rgbdata = (char*) malloc (3 * sizeof(char) * pixels.size());
        for (int i = 0; i < pixels.size(); ++i)
        {
            const Eigen::Vector3f& Pixel = pixels[i];
            _rgbdata[3 * i] = toInt( Pixel.x());
            _rgbdata[3 * i + 1] = toInt( Pixel.y());
            _rgbdata[3 * i + 2] = toInt( Pixel.z());
        }

        stbi_write_png("output.png", width, height, 3, _rgbdata, width * 3);
        free(_rgbdata);
	}

	Eigen::Vector3f PTScene::Radiance(const PT::Ray& ray, int depth)
	{
		static auto Op = [](const Eigen::Vector3f& V1, const Eigen::Vector3f& V2)
		{
			return (Eigen::Vector3f{ V1.x() * V2.x(), V1.y() * V2.y(), V1.z() * V2.z() });
		};
		
		PT::RayIntersectDesc result;
		std::shared_ptr<PT::PTSceneObjectDesc> hitObj = SceneDesc->Intersect(ray, result);

		if (!result.bHit || !hitObj)
		{
			return {0.0f, 0.0f, 0.0f};
		}

		Eigen::Vector3f diff = hitObj->GetMaterial().Diffuse;
		
		if (depth >= RRMinDepth)
		{
			const float RRValue = diff.maxCoeff();
			if (RRValue <= PT::PTRandom<float>::Generate())
			{
				return /*clamp*/(hitObj->GetMaterial().Emission);
			}
			else
			{
				// BUG: divide zero?
				diff /= RRValue;
				if (diff.hasNaN())
				{
					printf("FATAL ERROR -------- diff value is nan!!");
					return (hitObj->GetMaterial().Emission);
				}
			}
		}

		bool into = false;
		const Eigen::Vector3d hitNormal = result.HitShape->HitNormal(ray, result, into);
		const Eigen::Vector3d hitPos = ray.Origin + ray.Direction * result.t;
		
		switch (hitObj->GetMaterial().ReflectType)
		{
		case PT::EMRT_DIFFUSE:
			{
				// calculate diffuse direction -- from smallpt
				float r1 = 2 * PT_PI * PT::PTRandom<float>::Generate();
				float r2 = PT::PTRandom<float>::Generate();
				float r2s = sqrt(r2);

				Eigen::Vector3d w = hitNormal.normalized();
				Eigen::Vector3d u = (fabs(w.x()) > FLT_EPSILON ? Eigen::Vector3d{0.0f, 1.0f, 0.0f} : Eigen::Vector3d{1.0f, 0.0f, 0.0f})
					.cross(w).normalized();
				Eigen::Vector3d v = w.cross(u).normalized();
				
				PT::Ray diffRay;
				diffRay.Origin = hitPos;
				diffRay.Direction = (u * cosf(r1) * r2s + v * sinf(r1) * r2s + w * sqrtf(1 - r2)).normalized();

				float test = diffRay.Direction.dot(hitNormal);
				if (test < -FLT_EPSILON)
				{
					assert(false);
					printf("[ERROR] test >= -FLT_EPSILON");
				}
				
				//// TEST
				//diffRay.Direction = {0.0f, -1.0f, 0.0f};

				const Eigen::Vector3f result = hitObj->GetMaterial().Emission + Op(diff, Radiance(diffRay, depth + 1));
				return /*clamp*/(result);
			}
			break;

		case PT::EMRT_SPECULAR:
			{
				const Eigen::Vector3d n = into ? hitNormal : -hitNormal;
				
				PT::Ray reflRay;
				reflRay.Origin = hitPos;
				reflRay.Direction = ray.Direction - n * 2 * n.dot(ray.Direction);
				reflRay.Direction.normalize();
				
				return hitObj->GetMaterial().Emission + Op(diff, (Radiance(reflRay, depth + 1)));
			}
			break;

		case PT::EMRT_REFRACT:
			{
				 const Eigen::Vector3d n = into ? hitNormal : -hitNormal;
			//const Eigen::Vector3d n = hitNormal;
				
				PT::Ray reflRay;
				reflRay.Origin = hitPos;
				reflRay.Direction = ray.Direction - n * 2 * n.dot(ray.Direction);
				reflRay.Direction.normalize();

				double nc = 1.0, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = ray.Direction.dot(hitNormal);
				double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
				if (cos2t < 0.0)
				{
					// Total internal reflection
					return hitObj->GetMaterial().Emission + Op(diff, (Radiance(reflRay, depth + 1)));
				}

				Eigen::Vector3d tdir = (ray.Direction * nnt - hitNormal * (ddn * nnt + sqrt(cos2t))).normalized();
				double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : -tdir.dot(hitNormal));
				double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = 0.25 + 0.5 * Re, RP = Re / P, TP = Tr / (1 - P);

				if (depth > 2)
				{
					if (PT::PTRandom<float>::Generate() < P)
					{
						return hitObj->GetMaterial().Emission + Op(diff, (RP * Radiance(reflRay, depth + 1)));
					}
					else
					{
						PT::Ray tempRay;
						tempRay.Origin = hitPos;
						tempRay.Direction = tdir;
						
						return hitObj->GetMaterial().Emission + Op(diff, (TP * Radiance(tempRay, depth + 1)));
					}
				}
				else
				{
					PT::Ray tempRay;
					tempRay.Origin = hitPos;
					tempRay.Direction = tdir;
					
					return hitObj->GetMaterial().Emission + Op(diff, Re * Radiance(reflRay, depth + 1) + Tr * Radiance(tempRay, depth + 1));
				}
			}
			
			break;
			
		default: ;
		}

		return {};
	}
}
