#include "log.h"



void LOG(int level)
{
    switch(level)
    {

        case spdlog::level::trace:spdlog::set_level(spdlog::level::trace);break;
        case spdlog::level::debug:spdlog::set_level(spdlog::level::debug);break;
        case spdlog::level::info:spdlog::set_level(spdlog::level::info);break;
        case spdlog::level::warn:spdlog::set_level(spdlog::level::warn);break;
        case spdlog::level::err:spdlog::set_level(spdlog::level::err);break;
        case spdlog::level::critical:spdlog::set_level(spdlog::level::critical);break;
        case spdlog::level::off:spdlog::set_level(spdlog::level::off);break;
    }
}
