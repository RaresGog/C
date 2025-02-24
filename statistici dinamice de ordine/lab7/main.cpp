#include "OST.h"

#define CATCH_CONFIG_RUNNER
#include "catch2.hpp"

#include "commandline.h"
#include "Profiler.h"

#include <cstdio>
#include <string>

using namespace lab07;

Profiler profiler("OST");

void demo(const CommandArgs& args)
{
    const int size = args.empty()? 10: atoi(args[0]);
    demonstrate(size);
}

void perf(const CommandArgs& args)
{
    const auto whichCase = args.empty()? AVERAGE: strToCase(args[0]);
    performance(profiler, whichCase);
    profiler.reset();
}

int main()
{
    const std::vector<CommandSpec> commands =
    {
        {"demo", demo, "run demo"},
        {"perf", perf, "[avg(default)|best|worst] - run performance analysis on selected case"},
    };
    return runCommandLoop(commands);
}