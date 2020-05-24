#pragma once

#include <stdarg.h>
#include <stddef.h>

// logger_t represents an active logging object that generates lines of output
// to a file descriptor. A Logger can be used simultaneously from multiple
// threads; it guarantees to serialize access to the file descriptor.
typedef struct loguin_logger loguin_logger_t;

// loguin_default is a fully initalized logger using stderr as its file
// descriptor. It has no prefix.
extern loguin_logger_t loguin_default;

// loguin_new creates a new heap-allocated logging object that writes to `fd`,
// with a `prefix` of size `len`. The prefix may be NULL, in which case it is
// omitted during printing.
loguin_logger_t *loguin_new(int fd, const char *prefix, size_t len);

// loguin_free frees all memory associated with the logging object.
//
// Calling loguin_free on loguin_default does nothing.
void loguin_free(loguin_logger_t *log);

// loguin_printf writes to the underlying fd akin to the following
// printf("%s",prefix)
// printf(format, ...)
int loguin_printf(loguin_logger_t *log, const char *format, ...);

// loguin_logf prepends "[INFO]" to the prefix and calls loguin_printf.
int loguin_logf(loguin_logger_t *log, const char *format, ...);

// loguin_warnf prepends "[WARN]" to the prefix and calls loguin_printf.
int loguin_warnf(loguin_logger_t *log, const char *format, ...);

// loguin_fatalf prepends "[FATA]" to the prefix, calls loguin_printf, and
// calls exit(status).
//
// fatal_f will call free() on the underlying log object.
int loguin_fatalf(loguin_logger_t *log, int status, const char *format, ...);

// loguin_vprintf is the variadic verion of loguin_printf.
int loguin_vprintf(loguin_logger_t *log, const char *format, va_list args);

// loguin_vlogf is the variadic verion of loguin_logf.
int loguin_vlogf(loguin_logger_t *log, const char *format, va_list args);

// loguin_vwarnf is the variadic verion of loguin_warnf.
int loguin_vwarnf(loguin_logger_t *log, const char *format, va_list args);

// loguin_vfatalf is the variadic verion of loguin_fatalf.
int loguin_vfatalf(loguin_logger_t *log, int status, const char *format, va_list args);
