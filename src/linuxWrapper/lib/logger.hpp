#pragma once

#include <string>

namespace logger
{

void setupLogger();
void printInfo(std::string message);
void printError(std::string message);

} // namespace logger