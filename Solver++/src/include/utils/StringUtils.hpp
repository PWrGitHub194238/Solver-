/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef STRING_UTILS_HPP_
#define STRING_UTILS_HPP_

#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "../Solver.hpp"

namespace StringUtils {

namespace Impl {

/** @brief Table of characters that specify all data definition delimiters for dimensions.
 *
 * @details This table of characters have to consist of exactly two characters:
 *  - char reserved for opening delimiter,
 *  - char reserved for closing delimiter.
 *
 *  Example:
 *  @code
 *  	Given structure (Variable/Parameter/...) definition: x[2][3]
 *  	std::cout << DIM_GROUP_DELIMETERS[0];	// will print '['
 *  	std::cout << DIM_GROUP_DELIMETERS[1];	// will print ']'
 *  @endcode
 *
 *  This characters is widely used i.e. by StringUtils::variableDimensionTokenize()
 */
const static char* DIM_GROUP_DELIMETERS { "[]" };

}

template<class T>
std::list<T> tokenize(const std::string& inputString,
		const std::string& delimiters) {
	T value { };
	std::list<T> tokens { };
	std::string::size_type lastPos = inputString.find_first_not_of(delimiters,
			0);
	std::string::size_type pos = inputString.find_first_of(delimiters, lastPos);
	std::stringstream* ss { };
	while (std::string::npos != pos || std::string::npos != lastPos) {
		ss =
				new std::stringstream { inputString.substr(lastPos,
						pos - lastPos) };
		*ss >> value;
		delete ss;
		tokens.push_back(value);
		lastPos = inputString.find_first_not_of(delimiters, pos);
		pos = inputString.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

/** Extracts from dimensional part of definition of group of variables information about its' dimensions.
 *
 * Example:
 *
 * 	variableDimensionTokenize("[1][2][3]") result in list of integer values of type T: 1, 2 and 3.
 *
 * @param inputString
 * @return
 */
template<class T>
std::list<T> variableDimensionTokenize(const char* inputString) {
	T value { };
	std::list<T> tokens { };
	std::istringstream ss(inputString);
	char left_bracket = StringUtils::Impl::DIM_GROUP_DELIMETERS[0];
	char right_bracket = StringUtils::Impl::DIM_GROUP_DELIMETERS[1];

	while (ss >> left_bracket >> value >> right_bracket) {
		tokens.push_back(value);
	}

	return tokens;

}

}

#endif /* STRING_UTILS_HPP_ */
