#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include "AppState.h"
#include "Window.h"
#include "Common/Logger.h"

using namespace Coconut;

int main(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("%H:%M:%S.%e|%t|%l: %v");

   	debug("Starting main");
    AppState s(argc, argv);
    if (s.Init())
    {
	    return s.Run();
    }
    exit(1);
}
