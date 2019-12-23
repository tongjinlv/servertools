#ifndef _LOG_H_
#define _LOG_H_

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"


#define T(...) spdlog::trace(__VA_ARGS__);
#define D(...) spdlog::debug(__VA_ARGS__);
#define I(...) spdlog::info(__VA_ARGS__);
#define W(...) spdlog::warn(__VA_ARGS__);
#define E(...) spdlog::error(__VA_ARGS__);
#define C(...) spdlog::critical(__VA_ARGS__);

void LOG(int level);

#endif
