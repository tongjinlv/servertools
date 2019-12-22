#include "log.h"



#define E(...) spdlog::error(__VA_ARGS__);
#define I(...) spdlog::info(__VA_ARGS__);
#define I(...) spdlog::info(__VA_ARGS__);


void LOG(int level)
{
    switch(level)
    {
        case spdlog::level::info:spdlog::set_level(spdlog::level::info);
        case spdlog::level::warn:spdlog::set_level(spdlog::level::warn);
    }
    
}
