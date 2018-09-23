#pragma once

#include <string>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#define infoMessage(message, ...) logger::printInfo(message, ##__VA_ARGS__)

#define debugMessage(message, ...)                                                                                     \
    if(logger::isDebugLoggingEnabled)                                                                                  \
    logger::printDebugDetail(__FILE__, __LINE__, message, ##__VA_ARGS__)

#define errorMessage(message, ...) logger::printErrorDetail(__FILE__, __LINE__, message, ##__VA_ARGS__)

namespace logger
{
static auto console = spdlog::stdout_color_mt("console");
static bool debugEnabled = false;

void setupLogger()
{
    spdlog::set_pattern("[%T:%e][%^%L%$][t:%t][P:%P] %^%v%$");
}

void printError(std::string message)
{
    console->error(message);
}

void printDebug(std::string message)
{
    console->debug(message);
}

void enableDebugLogging()
{
    debugEnabled = true;
    spdlog::set_level(spdlog::level::debug);
}

void disableDebugLogging()
{
    debugEnabled = false;
    spdlog::set_level(spdlog::level::info);
}

bool isDebugLoggingEnabled()
{
    return debugEnabled;
}

/**
 * Prints out the log info message
 *
 * @param fmt: Pointer to print out custom message
 * @param args: Arguments that will be added to the message (fmt)
 **/
template <typename... Args>
void printInfo(const char* fmt, const Args&... args)
{
    std::stringstream ss;
    ss << fmt;
    console->info(ss.str().c_str(), args...);
}

/**
 * Prints out the debug log message
 *
 * @param current_file: The current file where the macro debugDetail is called
 * @param current_line: The current line where the macro debugDetail is called
 * @param fmt: Pointer to print out custom message
 * @param args: Arguments that will be added to the message (fmt)
 **/
template <typename... Args>
void printDebugDetail(const char* current_file, int current_line, const char* fmt, const Args&... args)
{
    std::stringstream ss;
    if(strrchr(current_file, '/') != nullptr)
    {
        current_file = strrchr(current_file, '/') + 1;
    }
    ss << current_file << " " << current_line << " " << fmt;
    console->debug(ss.str().c_str(), args...);
}

/**
 * Prints out the error log message
 *
 * @param current_file: The current file where the macro debugDetail is called
 * @param current_line: The current line where the macro debugDetail is called
 * @param fmt: Pointer to print out custom message
 * @param args: Arguments that will be added to the message (fmt)
 **/
template <typename... Args>
void printErrorDetail(const char* current_file, int current_line, const char* fmt, const Args&... args)
{
    std::stringstream ss;
    if(strrchr(current_file, '/') != nullptr)
    {
        current_file = strrchr(current_file, '/') + 1;
    }
    ss << current_file << " " << current_line << " " << fmt;
    console->error(ss.str().c_str(), args...);
}

} // namespace logger