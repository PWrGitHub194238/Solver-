/*
 * MathUtils.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include <cmath>

namespace MathUtils {

bool isInteger(double value) {
	return std::floor(value) == value;
}

}
