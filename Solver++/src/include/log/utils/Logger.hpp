/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 * Author: tomasz
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

/**
 * Header with rewritten logger.h from log4cxx macros due to its out of dated nature.
 * C++0x standard (from 2014) required every variable to be initialized in different way that they were before.
 *
 * For example:
 * ::log4cxx::helpers::MessageBuffer oss_; now needs to be initialize in that way:
 * ::log4cxx::helpers::MessageBuffer oss_ { } (or ::log4cxx::helpers::MessageBuffer oss_ = NULL)
 *
 * By not doing so compiler will rise a warning: 'Un- or ill-initialized variable found'.
 */

/**
 Logs a message to a specified logger with a specified level.

 @param logger the logger to be used.
 @param level the level to log.
 @param message the message string to log.
 */
#define C11_LOG4CXX_LOG(logger, level, message) { \
        if (logger->isEnabledFor(level)) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(level, oss_.str(oss_ << message), LOG4CXX_LOCATION); } }

/**
 Logs a message to a specified logger with a specified level.

 @param logger the logger to be used.
 @param level the level to log.
 @param message the message string to log in the internal encoding.
 */
#define C11_LOG4CXX_LOGLS(logger, level, message) { \
        if (logger->isEnabledFor(level)) {\
           ::log4cxx::helpers::LogCharMessageBuffer oss_ { }; \
           logger->forcedLog(level, oss_.str(oss_ << message), LOG4CXX_LOCATION); } }

/**
 Logs a message to a specified logger with the DEBUG level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_DEBUG(logger, message) { \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a message to a specified logger with the TRACE level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_TRACE(logger, message) { \
        if (LOG4CXX_UNLIKELY(logger->isTraceEnabled())) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getTrace(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a message to a specified logger with the INFO level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_INFO(logger, message) { \
        if (logger->isInfoEnabled()) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getInfo(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a message to a specified logger with the WARN level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_WARN(logger, message) { \
        if (logger->isWarnEnabled()) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getWarn(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a message to a specified logger with the ERROR level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_ERROR(logger, message) { \
        if (logger->isErrorEnabled()) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getError(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a error if the condition is not true.

 @param logger the logger to be used.
 @param condition condition
 @param message the message string to log.
 */
#define C11_LOG4CXX_ASSERT(logger, condition, message) { \
        if (!(condition) && logger->isErrorEnabled()) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getError(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

/**
 Logs a message to a specified logger with the FATAL level.

 @param logger the logger to be used.
 @param message the message string to log.
 */
#define C11_LOG4CXX_FATAL(logger, message) { \
        if (logger->isFatalEnabled()) {\
           ::log4cxx::helpers::MessageBuffer oss_ { }; \
           logger->forcedLog(::log4cxx::Level::getFatal(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}

#endif /* LOGGER_HPP_ */
