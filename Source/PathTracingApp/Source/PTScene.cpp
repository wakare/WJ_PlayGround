#include "PTScene.h"
#include <Eigen/Dense>
#include <ppl.h>
#include <fstream>

#include "CommonShape.h"
#include "PTBasicDefine.h"
#include "PTUtil.h"

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

			std::shared_ptr<PT::ShapeSphere> Sphere1 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(1e5 + 1.0, 40.8, 81.6), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere2 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(-1e5 + 99.0, 40.8, 81.6), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere3 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(50.0, 40.8, 1e5), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere4 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(50.0, 40.8, -1e5 + 170.0), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere5 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(50.0, 1e5, 81.6), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere6 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(50.0, -1e5 + 81.6, 81.6f), 1e5);
			std::shared_ptr<PT::ShapeSphere> Sphere7 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(27.0, 16.5, 47.0), 16.5);
			std::shared_ptr<PT::ShapeSphere> Sphere8 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(73.0, 16.5, 78.0), 16.5);
			std::shared_ptr<PT::ShapeSphere> Sphere9 = std::make_shared<PT::ShapeSphere>(PT::PTVector3d(50.0, 681.6 - 0.27, 81.6), 600.0);

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
			Camera = std::make_shared<PT::PTCamera>(PT::PTVector3f { 50.0, 52.0, 295.6 },PT::PTVector3f { 0.0, -0.042612, -1.0 });
		}
	}

	void PTScene::DoPathTracing(int width, int height, int sampleCount)
	{
        PT::PTVector3f cx = { width * 0.5135f / height, 0.0f, 0.0f };
        PT::PTVector3f cy = 0.5135f * cx.cross(Camera->Dir).normalized();

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
					ray.Direction = (cx * ((x + PT::PTRandom<float>::Generate01()) / width - 0.5f)
						+ cy * ((y + PT::PTRandom<float>::Generate01()) / height - 0.5f))
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

        const bool bExport = PTUtil::WritePNGChannel("Output.png", _rgbdata, width, height, 3);
        assert(bExport);

        free(_rgbdata);
	}

	Eigen::Vector3f PTScene::Radiance(const PT::Ray& ray, int depth)
	{
		static auto Op = [](const Eigen::Vector3f& V1, const Eigen::Vector3f& V2) -> Eigen::Vector3f
		{
			return Eigen::Vector3f { (float)(V1.x() * V2.x()), (float)(V1.y() * V2.y()), (float)(V1.z() * V2.z()) };
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
			if (RRValue <= PT::PTRandom<float>::Generate01())
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
		const PT::PTVector3f hitNormal = result.HitShape->HitNormal(ray, result, into).cast<float>();
		const PT::PTVector3f hitPos = (ray.Origin + ray.Direction * result.t).cast<float>();
		
		switch (hitObj->GetMaterial().ReflectType)
		{
		case PT::EMRT_DIFFUSE:
			{
				// calculate diffuse direction -- from smallpt
				float r1 = 2 * PT_PI * PT::PTRandom<float>::Generate01();
				float r2 = PT::PTRandom<float>::Generate01();
				float r2s = sqrt(r2);

                PT::PTVector3f w = hitNormal.normalized();
                PT::PTVector3f u = (fabs(w.x()) > FLT_EPSILON ? PT::PTVector3f{0.0f, 1.0f, 0.0f} : PT::PTVector3f{1.0f, 0.0f, 0.0f})
					.cross(w).normalized();
                PT::PTVector3f v = w.cross(u).normalized();
				
				PT::Ray diffRay;
				diffRay.Origin = hitPos;
				diffRay.Direction = (u * cosf(r1) * r2s + v * sinf(r1) * r2s + w * sqrtf(1 - r2)).normalized();

				float test = diffRay.Direction.cast<float>().dot(hitNormal);
				if (test < -FLT_EPSILON)
				{
					assert(false);
					printf("[ERROR] test >= -FLT_EPSILON");
				}

				const Eigen::Vector3f result = hitObj->GetMaterial().Emission + Op(diff, Radiance(diffRay, depth + 1));
				return result;
			}
			break;

		case PT::EMRT_SPECULAR:
			{
                PT::PTVector3f n = hitNormal;
                if (!into) n = -n;

				PT::Ray reflRay;
				reflRay.Origin = hitPos;
				reflRay.Direction = ray.Direction - n * 2 * n.dot(ray.Direction);
				reflRay.Direction.normalize();
				
				return hitObj->GetMaterial().Emission + Op(diff, (Radiance(reflRay, depth + 1)));
			}
			break;

		case PT::EMRT_REFRACT:
			{
				PT::PTVector3f n = hitNormal;
				if (!into) n = -n;
				
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

                PT::PTVector3f tdir = (ray.Direction.cast<float>() * nnt - hitNormal * (ddn * nnt + sqrt(cos2t))).normalized();
				double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : -tdir.dot(hitNormal));
				double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = 0.25 + 0.5 * Re, RP = Re / P, TP = Tr / (1 - P);

				if (depth > 2)
				{
					if (PT::PTRandom<float>::Generate01() < P)
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
