#include "Log.h"

#include <cstdarg>
#ifdef __ANDROID__

#include <android/log.h>

#else

#include <iostream>
#include <string>
#endif

void Log::error(const char* format, ...)
{

    
	va_list args;
	va_start(args, format);

#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_ERROR, "BeanEngine", format, args);
	__android_log_print(ANDROID_LOG_ERROR, "BeanEngine", "\n");
#else
    std::string f = std::string("<error>") + std::string(format) + std::string("\n");
    vprintf(f.c_str(), args);
#endif    
    va_end(args);
}

void Log::debug(const char* format, ...)
{
    
#ifdef __BEAN_DEBUG__
	va_list args;
	va_start(args, format);
#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_DEBUG, "BeanEngine", format, args);
	__android_log_print(ANDROID_LOG_DEBUG, "BeanEngine", "\n");
#else
    std::string f = std::string("<debug> ") + std::string(format) + std::string("\n");
    vprintf(f.c_str(), args);
#endif    
	va_end(args);
#endif

}

void Log::warn(const char* format, ...)
{
    
	va_list args;
	va_start(args, format);
#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_WARN, "BeanEngine", format, args);
	__android_log_print(ANDROID_LOG_WARN, "BeanEngine", "\n");
#else
    std::string f = std::string("<warning> ") + std::string(format) + std::string("\n");
    vprintf(f.c_str(), args);
#endif   
	va_end(args);
}

void Log::wtf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_UNKNOWN, "BeanEngine", format, args);
	__android_log_print(ANDROID_LOG_UNKNOWN, "BeanEngine", "\n");
#else
    std::string f = std::string("<wtf> ") + std::string(format) + std::string("\n");
    vprintf(f.c_str(), args);
#endif   
	va_end(args);
}

void Log::info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_INFO, "BeanEngine", format, args);
	__android_log_print(ANDROID_LOG_INFO, "BeanEngine", "\n");
#else
    std::string f = std::string("<info> ") + std::string(format) + std::string("\n");
    vprintf(f.c_str(), args);
#endif   
	va_end(args);
}
