#include "logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace logger
{
static auto console = spdlog::stdout_color_mt("console");

void setupLogger()
{
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
}

void printInfo(std::string message)
{
    console->info(message);
}

void printError(std::string message)
{
    console->error(message);
}

} // namespace logger