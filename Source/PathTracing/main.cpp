#include <cstdlib>

#include "PTRandom.h"
#include "PTScene.h"

using namespace PTMain;

int main()
{
	PT::PTRandom<float>::SetSeed(2);
	
	PTMain::PTScene Scene;
	Scene.DoPathTracing(1024, 768, 256);
	
	return EXIT_SUCCESS;
}
