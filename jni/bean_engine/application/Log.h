#pragma once

#ifdef __BEAN_DEBUG__

#define B_ASSERT(x) \
	if (!(x)) { \
		Log::error("Assert: '%s' failed on line %i in %s", #x, __LINE__, __FILE__); \
	}

#else

#define B_ASSERT(x)

#endif


class Log {
public:
	static void debug(const char* format, ...);
	static void error(const char* format, ...);
	static void warn(const char* format, ...);
	static void info(const char* format, ...);
	static void wtf(const char* format, ...);
};
