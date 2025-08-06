#pragma once

#define MZ_BIT(i) (1 << i)
#define MZ_UNUSED(x) (void)x

#define METHOD_NAME             mz::functionToLocation(std::source_location::current().function_name())

#define MZ_TRACE(fmt, ...)		mz::g_logger.trace(true, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_INFO(fmt, ...)		mz::g_logger.info(true, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_WARN(fmt, ...)		mz::g_logger.warn(true, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_ERROR(fmt, ...)		mz::g_logger.error(true, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_FATAL(fmt, ...)		mz::g_logger.fatal(true, METHOD_NAME, fmt, __VA_ARGS__)

#define MZ_TRACE_NOW(fmt, ...)	mz::g_logger.trace(false, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_INFO_NOW(fmt, ...)	mz::g_logger.info(false, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_WARN_NOW(fmt, ...)	mz::g_logger.warn(false, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_ERROR_NOW(fmt, ...)	mz::g_logger.error(false, METHOD_NAME, fmt, __VA_ARGS__)
#define MZ_FATAL_NOW(fmt, ...)	mz::g_logger.fatal(false, METHOD_NAME, fmt, __VA_ARGS__)