/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef VARIABLE_MODEL_EXCEPTIONS_UTILS_HPP_
#define VARIABLE_MODEL_EXCEPTIONS_UTILS_HPP_

#include <exception>

namespace VariableModelExceptions {

/** Will be thrown on attempt of creating a new variable with the same base name as already existing variable.
 *
 * Example:
 *
 * @code
 * Solver s(SolverInterface::GLPK);
 * s.newVariable("x[2]");
 * s.newVariable("x[8][4]");
 * @endcode
 *
 * will throw exception, regardless of different size and number of dimensions.
 *
 */
struct DuplicateException: public std::exception {
	const char* what() const throw () {
		return "Group of variables with the same name already exists.";
	}
};

/** Exception will be thrown on attempt of creating a new variable while invalid definition was selected.
 *
 * Valid definition of a variable is given by...
 *
 */
struct InvalidGroupDefinitionException: public std::exception {
	const char* what() const throw () {
		return "Given definition of a group of variables is invalid and cannot be interpreted.";
	}
};

struct InvalidDefinitionException: public std::exception {
	const char* what() const throw () {
		return "Given definition of a variable is invalid and cannot be interpreted.";
	}
};

struct InvalidNumberOfDimensionIndexesException: public std::exception {
	const char* what() const throw () {
		return "Given group of variables has different number of dimensions.";
	}
};

struct IndexOutOfBoundException: public std::exception {
	const char* what() const throw () {
		return "No such variable is defined (some index is to greater than definition of given group specify?)";
	}
};

struct NoSuchGroupException: public std::exception {
	const char* what() const throw () {
		return "Cannot find group of variables with given name.";
	}
};

struct ValueOutOfBoundException: public std::exception {
	const char* what() const throw () {
		return "Given value is out of variable's lower or upper bound.";
	}
};

}

#endif /* VARIABLE_MODEL_EXCEPTIONS_UTILS_HPP_ */
