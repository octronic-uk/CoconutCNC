#include "AppState.h"
#include "Window.h"
#include "Logger.h"

using namespace Coconut;

int main(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::off);
   	debug("Starting main");
    AppState s(argc, argv);
    if (s.Init())
    {
	    return s.Run();
    }
    exit(1);
}
