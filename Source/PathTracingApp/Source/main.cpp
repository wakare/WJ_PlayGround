#include <cstdlib>

#include "PTBasicDefine.h"
#include "PTScene.h"
#include "OptiXTest.h"
#include "PTUtil.h"

using namespace PTMain;

int main()
{
	PT::PTRandom<float>::SetSeed(2);
	
	//PTMain::PTScene Scene;
	//Scene.DoPathTracing(800, 600, 64);

	int width = 800;
	int height = 600;

	uint32_t* DownloadBuffer = (uint32_t*) malloc (width * height * sizeof(uint32_t));

    OptiXTest Test(width, height);
    Test.Render();
    Test.Download(DownloadBuffer);

    PTUtil::WritePNGChannel("OutputOptiXResult.png", DownloadBuffer, 4, width, height);

    free (DownloadBuffer);

	return EXIT_SUCCESS;
}
