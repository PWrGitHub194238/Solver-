/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 * Author: tomasz
 */

#ifndef LOG_UTILS_HPP_
#define LOG_UTILS_HPP_

#include <stddef.h>
#include <string>

#include "../bundle/Bundle.hpp"
#include "Logger.hpp"

#define TRACE(logger, bundleKey, ...) C11_LOG4CXX_TRACE( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

#define DEBUG(logger, bundleKey, ...) C11_LOG4CXX_DEBUG( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

#define INFO(logger, bundleKey, ...) C11_LOG4CXX_INFO( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

#define WARN(logger, bundleKey, ...) C11_LOG4CXX_WARN( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

#define ERROR(logger, bundleKey, ...) C11_LOG4CXX_ERROR( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

#define FATAL(logger, bundleKey, ...) C11_LOG4CXX_FATAL( \
		logger, \
		LogUtils::impl::formatBundle( \
				LogUtils::impl::getBundle(bundleKey), \
			## __VA_ARGS__) \
		)

namespace LogUtils {

const static int LOG_BUFFER_SIZE { 1024 };

/** Loads XML configuration file using DOMConfigurator
 *
 * @param xmlFileConfig path to given XML log4cxx configuration file
 */
void configureLog(const char* xmlFileConfig);

namespace impl {

/** Returns parsed (@see parseBundle()) format bundle file e.g. EN_US_Bundle.cpp
 *
 * Function gets string from extern dictionary in bundle file and returns its parsed version.
 *
 * @param bundleKey
 * @return
 */
const char* getBundle(BundleKey bundleKey);

/** Function replaces all occurrences of user-defined formatters with valid ones.
 *
 * Example with defined extern fields in bundle file e.g. EN_US_Bundle.cpp:
 *  - const char* varIdxToken = "%VarIdx%";
 *	- extern const std::string varIdxFormat("%u");
 *
 * @param bundle
 * @return
 */
const char* parseBundle(const char* const & bundle);

size_t insertRightFormat(std::string& bundle, const size_t& beginIdx,
		const size_t& length);

const char* formatBundle(const char* bundle, ...);

}

}

#endif /* LOG_UTILS_HPP_ */
