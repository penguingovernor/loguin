#include "loguin.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

#define LOG_PREFIX GREEN "INFO" RESET "[%04lx] "
#define WARN_PREFIX YELLOW "WARN" RESET "[%04lx] "
#define FATAL_PREFIX RED "FATA" RESET "[%04lx] "

struct loguin_logger {
  pthread_mutex_t mu;
  char *prefix;
  int writer;
  size_t log_count;
  size_t warn_count;
  size_t fatal_count;
};

loguin_logger_t loguin_default = {
  .mu = PTHREAD_MUTEX_INITIALIZER,
  .prefix = "",
  .writer = STDERR_FILENO,
  .log_count = 0,
  .warn_count = 0,
  .fatal_count = 0,
};

// print_prefix prints the loggers prefix if it has one.
static int print_prefix(loguin_logger_t *l)
{
  int result = 0;
  if(l->prefix != NULL) {
    result += dprintf(l->writer, "%s", l->prefix);
  }
  return result;
}

loguin_logger_t *loguin_new(int fd, const char *prefix, size_t len)
{
  // Allocate some space for the struct.
  loguin_logger_t *log = malloc(sizeof(loguin_logger_t));
  if(log == NULL) {
    return NULL;
  }

  // Set the fields.
  log->writer = fd;
  log->log_count = 0;
  log->warn_count = 0;
  log->fatal_count = 0;
  log->mu = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  log->prefix = NULL;

  // If the length  is greather than 0 and there's a
  // valid prefix.
  if(len > 0 && prefix != NULL) {
    // ALlocate space for the prefix.
    log->prefix = malloc(sizeof(char) * (len + 1));

    // If there was an error allocated space for
    // the prefix we free the log and bail.
    if(log->prefix == NULL) {
      free(log);
      return NULL;
    }

    // Strcpy len bytes to the struct and null
    // terminate.
    strncpy(log->prefix, prefix, len);
    log->prefix[len] = '\0';
  }

  return log;
}

void loguin_free(loguin_logger_t *log)
{
  // Don't free the stack-allocated
  // loguin_default logger.
  if(log == &loguin_default) {
    return;
  }

  // Free the prefix if applicable
  // and free the struct.
  if(log->prefix != NULL) {
    free(log->prefix);
    log->prefix = NULL;
  }
  free(log);
}

int loguin_vprintf(loguin_logger_t *log, const char *format, va_list args)
{
  pthread_mutex_lock(&log->mu);
  int result = print_prefix(log);
  result += vdprintf(log->writer, format, args);
  pthread_mutex_unlock(&log->mu);
  return result;
}

int loguin_printf(loguin_logger_t *log, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int result = loguin_vprintf(log, format, args);
  va_end(args);
  return result;
}

int loguin_vlogf(loguin_logger_t *log, const char *format, va_list args)
{
  pthread_mutex_lock(&log->mu);
  int result = 0;
  result += dprintf(log->writer, LOG_PREFIX, log->log_count);
  result += print_prefix(log);
  result += vdprintf(log->writer, format, args);
  log->log_count++;
  pthread_mutex_unlock(&log->mu);
  return result;
}

int loguin_logf(loguin_logger_t *log, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int result = loguin_vlogf(log, format, args);
  va_end(args);
  return result;
}

int loguin_vwarnf(loguin_logger_t *log, const char *format, va_list args)
{
  pthread_mutex_lock(&log->mu);
  int result = 0;
  result += dprintf(log->writer, WARN_PREFIX, log->warn_count);
  result += vdprintf(log->writer, format, args);
  log->warn_count++;
  pthread_mutex_unlock(&log->mu);
  return result;
}

int loguin_warnf(loguin_logger_t *log, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int result = loguin_vwarnf(log, format, args);
  va_end(args);
  return result;
}

int loguin_vfatalf(loguin_logger_t *log, int status, const char *format, va_list args)
{
  pthread_mutex_lock(&log->mu);
  int result = 0;
  result += dprintf(log->writer, FATAL_PREFIX, log->fatal_count);
  result += print_prefix(log);
  result += vdprintf(log->writer, format, args);
  log->fatal_count++;
  pthread_mutex_unlock(&log->mu);
  loguin_free(log);
  exit(status);
  return result;
}

int loguin_fatalf(loguin_logger_t *log, int status, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int result = loguin_vfatalf(log, status, format, args);
  va_end(args);
  return result;
}
