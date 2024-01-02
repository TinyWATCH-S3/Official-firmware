#pragma once

// See platformio.ini to enable different levels

template <typename T> static inline void debug_print(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
	Serial.print(txt);
#endif
}

template <typename T> static inline void debug_println(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
	Serial.println(txt);
#endif
}

static inline void debug_println()
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
	Serial.println();
#endif
}

template <typename... Args> static inline void debug_printf(const char *fmt, Args... args)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
	Serial.printf(fmt, args...);
#endif
}

template <typename T> static inline void info_print(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.print(txt);
#endif
}

template <typename T> static inline void info_print(T txt, int p)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.print(txt, p);
#endif
}

template <typename T> static inline void info_println(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.println(txt);
#endif
}

template <typename T> static inline void info_println(T txt, int p)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.println(txt, p);
#endif
}

static inline void info_println()
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.println();
#endif
}

template <typename... Args> static inline void info_printf(const char *fmt, Args... args)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
	Serial.printf(fmt, args...);
#endif
}

template <typename T> static inline void warn_print(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
	Serial.print(txt);
#endif
}

template <typename T> static inline void warn_println(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
	Serial.println(txt);
#endif
}

static inline void warn_println()
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
	Serial.println();
#endif
}

template <typename... Args> static inline void warn_printf(const char *fmt, Args... args)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_WARNING
	Serial.printf(fmt, args...);
#endif
}

template <typename T> static inline void error_print(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
	Serial.print(txt);
#endif
}

template <typename T> static inline void error_println(T txt)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
	Serial.println(txt);
#endif
}

static inline void error_println()
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
	Serial.println();
#endif
}

template <typename... Args> static inline void error_printf(const char *fmt, Args... args)
{
#if TW_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
	Serial.printf(fmt, args...);
#endif
}
