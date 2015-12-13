/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef MATH_UTILS_HPP_
#define MATH_UTILS_HPP_

#include <log4cxx/logger.h>

namespace MathUtils {

const static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("MathUtils"));

bool isInteger(double value);

}

#endif /* MATH_UTILS_HPP_ */
