#include <cstdlib>

#include "PTBasicDefine.h"
#include "PTScene.h"

using namespace PTMain;

int main()
{
	PT::PTRandom<float>::SetSeed(2);
	
	PTMain::PTScene Scene;
	Scene.DoPathTracing(800, 600, 64);
	
	return EXIT_SUCCESS;
}
