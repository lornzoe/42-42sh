/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 00:00:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/10 04:43:52 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// LOG_ERROR - unrecoverable / user-facing failures, always logged
// LOG_WARN  - recoverable issues, unexpected but handled states
// LOG_INFO  - high level lifecycle events (parsing started, exec'ing cmd...)
// LOG_DEBUG - internal state useful when tracking down a bug
// LOG_TRACE - very verbose, e.g. every token/AST node produced
// WARN/INFO/DEBUG/TRACE only log when the DEBUG make flag is set;

#define LOG_COLOR_RED		"\033[1;31m"
#define LOG_COLOR_YELLOW	"\033[1;33m"
#define LOG_COLOR_CYAN		"\033[1;36m"
#define LOG_COLOR_GREEN		"\033[1;32m"
#define LOG_COLOR_GREY		"\033[2;37m"
#define LOG_COLOR_RESET		"\033[0m"

#define LOG_ERROR(...) \
	do { \
		fprintf(stderr, LOG_COLOR_RED "[ERROR]" LOG_COLOR_RESET "\t"); \
		fprintf(stderr, __VA_ARGS__); \
	} while (0)

#ifdef DEBUG
#	define LOG_WARN(...) \
		do { \
			fprintf(stderr, LOG_COLOR_YELLOW "[WARN]" LOG_COLOR_RESET "\t"); \
			fprintf(stderr, __VA_ARGS__); \
		} while (0)
#	define LOG_INFO(...) \
		do { \
			printf(LOG_COLOR_CYAN "[INFO]" LOG_COLOR_RESET "\t"); \
			printf(__VA_ARGS__); \
		} while (0)
#	define LOG_DEBUG(...) \
		do { \
			printf(LOG_COLOR_GREEN "[DEBUG]" LOG_COLOR_RESET "\t"); \
			printf(__VA_ARGS__); \
		} while (0)
#	define LOG_DEBUG_CALL(expr) \
		do { \
			printf(LOG_COLOR_GREEN "[DEBUG]" LOG_COLOR_RESET "\t"); \
			expr; \
		} while (0)
#	define LOG_TRACE(...) \
		do { \
			printf(LOG_COLOR_GREY "[TRACE]" LOG_COLOR_RESET "\t"); \
			printf(__VA_ARGS__); \
		} while (0)
#	define LOG_TRACE_CALL(expr) \
		do { \
			printf(LOG_COLOR_GREY "[TRACE]" LOG_COLOR_RESET "\t"); \
			expr; \
		} while (0)
#else
#	define LOG_WARN(...)
#	define LOG_INFO(...)
#	define LOG_DEBUG(...)
#	define LOG_DEBUG_CALL(expr)
#	define LOG_TRACE(...)
#	define LOG_TRACE_CALL(expr)
#endif

#endif
