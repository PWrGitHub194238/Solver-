/*
 * StringUtils.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../../../include/log/utils/LogUtils.hpp"

#include <log4cxx/xml/domconfigurator.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <cstdio>
#include <cstring>

namespace LogUtils {

void configureLog(const char* xmlFileConfig) {
	log4cxx::xml::DOMConfigurator::configure(xmlFileConfig);
}

namespace impl {

const char SPECIAL_SIGN = { '%' };

const char* getBundle(BundleKey bundleKey) {
	return LogUtils::impl::parseBundle(dictionary[bundleKey]);
}

const char* parseBundle(const char* const & bundle) {
	char* c_str { };
	std::string str(bundle);
	size_t b { 0 }, e { 0 };
	do {
		b = str.find_first_of(SPECIAL_SIGN, e);
		e = str.find_first_of(SPECIAL_SIGN, b + 1);
		if (b == std::string::npos || e == std::string::npos) {
			break;
		}
		if (e - b == 1) {
			e += 1;
			continue;
		}
		e = LogUtils::impl::insertRightFormat(str, b, e);
	} while (e != std::string::npos);
	c_str = new char[str.size() + 1] { }; // +1 for terminating NUL
	strcpy(c_str, str.c_str());
	return c_str;
}

size_t insertRightFormat(std::string& bundle, const size_t& beginIdx,
		const size_t& endIdx) {
	const size_t length = endIdx - beginIdx + 1;
	std::string subStr = bundle.substr(beginIdx, length);

	if (subStr.compare(varValueToken) == 0) {
		bundle.replace(beginIdx, length, varValueFormat);
		return beginIdx + varValueFormat.length();
	} else if (subStr.compare(varIdxToken) == 0) {
		bundle.replace(beginIdx, length, varIdxFormat);
		return beginIdx + varIdxFormat.length();
	} else if (subStr.compare(varDimIdxToken) == 0) {
		bundle.replace(beginIdx, length, varDimIdxFormat);
		return beginIdx + varDimIdxFormat.length();
	} else if (subStr.compare(rowIdxToken) == 0) {
		bundle.replace(beginIdx, length, rowIdxFormat);
		return beginIdx + rowIdxFormat.length();
	} else {
		return endIdx;
	}
}

const char* formatBundle(const char* bundle, ...) {
	va_list va { };
	static char formatted[LOG_BUFFER_SIZE] { };
	va_start(va, bundle);
	vsnprintf(formatted, LOG_BUFFER_SIZE, bundle, va);
	va_end(va);
	return formatted;
}

}

}
