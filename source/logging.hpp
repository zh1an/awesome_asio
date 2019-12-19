#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef USE_SPDLOG
#if USE_SPDLOG == true
#include <spdlog/spdlog.h>

#define LOGGING_INIT                                                           \
  {                                                                            \
    spdlog::set_pattern("[%L%t %H:%M:%S.%e] %^%v%$ [%s:%#]");                  \
    spdlog::set_level(spdlog::level::trace);                                   \
  }
#define LOGGING_TRACE SPDLOG_TRACE(__FUNCTION__);
#define LOGGING_DEBUG SPDLOG_DEBUG
#define LOGGING_INFO SPDLOG_INFO
#define LOGGING_WARN SPDLOG_WARN
#define LOGGING_ERROR SPDLOG_ERROR
#define LOGGING_FATAL SPDLOG_CRITICAL

#else

#define LOGGING_INIT (void)(0);
#define LOGGING_TRACE (void)(0);
#define LOGGING_DEBUG(...) (void)(0);
#define LOGGING_INFO(...) (void)(0);
#define LOGGING_WARN(...) (void)(0);
#define LOGGING_ERROR(...) (void)(0);
#define LOGGING_FATAL(...) (void)(0);

#endif
#else

#define LOGGING_INIT (void)(0);
#define LOGGING_TRACE (void)(0);
#define LOGGING_DEBUG(...) (void)(0);
#define LOGGING_INFO(...) (void)(0);
#define LOGGING_WARN(...) (void)(0);
#define LOGGING_ERROR(...) (void)(0);
#define LOGGING_FATAL(...) (void)(0);

#endif

#endif // LOGGING_HPP
