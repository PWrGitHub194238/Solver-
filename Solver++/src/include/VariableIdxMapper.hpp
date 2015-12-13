/*
 * VariableIdxMapper.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef VARIABLEIDXMAPPER_HPP_
#define VARIABLEIDXMAPPER_HPP_

#include <list>
#include <string>

#include "Solver.hpp"
#include "Typedefs.hpp"

class Variable;

/** @brief Contains all informations about grouped variables.
 *
 * @details Every variable group has to have it's own base name
 * which is a unique key to unordered map of variable's groups
 * stored in Solver class (@see Solver::variables).
 *
 * Before resolving LP/MIP problem every variable has to be mapped into 1-dimensional array.
 *
 * Example:
 * @code
 * 	Variable group x[3][4] is a group of 12 variables with base name x.
 * 	VariableIdxMapper will map this matrix (or more dimensional structure) into simple
 * 	array of variables (for LP/MIP solver processing) allowing to refer to them
 * 	in more natural way using indexes in the same time.
 * @endcode
 *
 */
class VariableIdxMapper {

	friend Variable& Solver::getVariable(const std::string& variableDefinition)
			throw (std::out_of_range,
					VariableModelExceptions::InvalidNumberOfDimensionIndexesException,
					VariableModelExceptions::IndexOutOfBoundException,
					VariableModelExceptions::InvalidGroupDefinitionException);

	friend Variable& Solver::getVariable(const std::string& variableDefinition,
			VarDimIdx firstDimensionIndex, ...) throw (std::out_of_range,
					VariableModelExceptions::InvalidNumberOfDimensionIndexesException,
					VariableModelExceptions::IndexOutOfBoundException,
					VariableModelExceptions::InvalidGroupDefinitionException);

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** @brief Index of the first variable in array/matrix or more dimensional structure.
	 *
	 * @brief During extracting model, every group of variables will be joined into one array of variables
	 * when this value is an index of the first variable of this group of variables.
	 *
	 * Example:
	 * @code
	 * 	Solver s(SolverInterface::GLPK);
	 *	s.newVariable("x");
	 *	s.newVariable("y[2]");
	 *	s.newVariable("z[3]");
	 * @endcode
	 *
	 * will result in passing below array to model:
	 *
	 * @code
	 * 	["x","y[1]","y[2]","z[1]","z[2]","z[3]"]
	 * @endcode
	 *
	 */
	VarIdx beginIdx;

	/** @brief Every variable may have multiple dimensions.
	 *
	 * List contains naturally ordered dimensional indexes of that variable (from the highest dimension to the lowest).
	 * If created variable is 0-dimensional structure then dimensions list will be empty.
	 *
	 * Example:
	 *
	 * 	For given group of variables "x[1][2][3]" dimensions list will contain elements: 1, 2 and 3.
	 *
	 */
	std::list<VarDimIdx> dimensions;

	/** @brief Number of variables in array/matrix or more dimensional structure.
	 *
	 */
	VarIdx numberOfVariables;

	/** @brief Type of variable.
	 *
	 * Due to MIP solver specification every variable with any type but double will be created
	 * as double - only after successfully finding of first LP solution (first upper/lower bound)
	 * solver will turn these variables into given type in order to find MIP solution of given problem.
	 *
	 */
	ValueType type;

	/** @brief Array of mapped variables.
	 *
	 * Calling the constructor will create numberOfVariables variables with given type and bounds.
	 * Regardless of the number of dimensions in group of variables that was defined,
	 * all of them will be linearized and assigned to the appropriate indexes in one-dimensional array.
	 *
	 */
	Variable** variables;

	/** @brief Stores actual name of this group of variables.
	 *
	 */
	std::string groupName;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Returns pointer to variable in 'this' group of variables with given indexes.
	 *
	 * @param dimensions
	 * @return pointer to variable with given index from variables array of pointers
	 * where given index is calculated based on given list of dimensions and linearized afterwards.
	 */
	Variable& getVariable(const std::list<VarDimIdx>& dimensions);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** Creates variable group with given dimensions and type.
	 *
	 * Any group of variables is defined by string that matches regular expression (@see Solver::VAR_REGEX)
	 * and it consists of variable base name and array-like list of group's dimensions.
	 *
	 * Example:
	 *
	 * Solver::newVariable("x[3][4]", ValueType::DOUBLE) ->
	 * VariableIdxMapper("x", beginIdx, {{3},{4}}, ValueType::DOUBLE)
	 * will result in creating of group of 12 variables of default type (double).
	 * Variables array will be filled with variables with given lower and upper bounds in given order:
	 * [x[1][1], x[1][2], x[1][3], ..., x[3][3], x[3][4]]
	 *
	 * BeginIdx will be calculated based on current number
	 * of existing variables/columns (@see OsiProblemInstance::numberOfColumns).
	 *
	 * @param firstIdx number of all existing variables increased by one, index of the first variable in this group
	 * @param dimensions part of string-like variables' group declaration
	 * @param numberOfVariables number of variables in this group
	 * @param variableLowerBound lower bound of every variable in group
	 * @param variableUpperBound upper bound of every variable in group
	 * @param type type of all variables in this group
	 * @param groupName name of group of variables
	 *
	 */
	VariableIdxMapper(const VarIdx firstIdx,
			const std::list<VarDimIdx>& dimensions,
			const VarIdx numberOfVariables, const VarValue variableLowerBound,
			const VarValue variableUpperBound, const ValueType type,
			const std::string& groupName);

	virtual ~VariableIdxMapper();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Returns string representation of this class.
	 *
	 * @return
	 */
	std::string toString();

	/** Returns number of dimensions of this group.
	 *
	 * @return
	 */
	VarDimIdx getDimensionsSize();

	//*************************************** GETTERS & SETTERS ****************************************//

	/** Returns value of beginIdx field.
	 *
	 * @return
	 */
	VarIdx getBeginIdx() const;

	/** Returns dimensions list.
	 *
	 * @return
	 */
	const std::list<VarDimIdx>& getDimensions() const;

};

#endif /* VARIABLEIDXMAPPER_HPP_ */
