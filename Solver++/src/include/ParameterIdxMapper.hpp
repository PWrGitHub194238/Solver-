/*
 * ParamIdxMapper.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef PARAMETERMAPPER_HPP_
#define PARAMETERMAPPER_HPP_

#include <list>
#include <string>

#include "Typedefs.hpp"

class Parameter;

class ParamIdxMapper {

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** Index of the first parameter in array/matrix or more dimensional structure.
	 *
	 */
	ParamIdx beginIdx;

	/** Every parameter may have multiple dimensions.
	 *
	 * List contains naturally ordered dimensional indexes of that parameter (from the highest dimension to the lowest).
	 * If created parameter is 0-dimensional structure then dimensions list will be empty.
	 *
	 * Example:
	 *
	 * 	For given group of parameters "p[1][2][3]" dimensions list will contain elements: 1, 2 and 3.
	 *
	 */
	std::list<ParamDimIdx> dimensions;

	/** Number of parameters in array/matrix or more dimensional structure.
	 *
	 */
	ParamIdx numberOfParameters;

	/** Type of parameter.
	 *
	 * In Osi Solver Interface, parameters are not needed,
	 * they are just passed by value to matrix of linear equations/inequalities from which LP/MIP problem is consists of.
	 * Therefore there is no need for specifying a type of parameter for OSI but Solver class
	 * can decided which lower and upper bounds the newly created parameter should have without defining them separately.
	 *
	 */
	ValueType type;

	/** Array of mapped parameters.
	 *
	 * Calling the constructor will create numberOfParameters parameters with given type and bounds.
	 * Regardless of the number of dimensions in group of parameters that was defined,
	 * all of them will be linearized and assigned to the appropriate indexes in one-dimensional array.
	 *
	 */
	Parameter** parameters;

	/** Stores actual name of this group of parameters.
	 *
	 */
	std::string groupName;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	bool ifDimensionsOutOfBounds(const std::list<ParamDimIdx>& dimensions);

	/** Returns pointer to parameter in 'this' group of parameters with given indexes.
	 *
	 * @param dimensions
	 * @return pointer to parameter with given index from parameters array of pointers
	 * where given index is calculated based on given list of dimensions and linearized afterwards.
	 */
	Parameter& getParameter(const std::list<ParamDimIdx>& dimensions);

	ParamIdx linearizeDimensionList(const std::list<ParamDimIdx>& dimensions);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** Creates parameter group with given dimensions and type.
	 *
	 * Any group of parameters is defined by string that matches regular expression (@see Solver::VAR_REGEX)
	 * and it consists of parameter base name and array-like list of group's dimensions.
	 *
	 * Example:
	 *
	 * Solver::newparameter("x[3][4]", ParamType::DOUBLE) ->
	 * ParamIdxMapper("x", beginIdx, {{3},{4}}, ParamType::DOUBLE)
	 * will result in creating of group of 12 parameters of default type (double).
	 * parameters array will be filled with parameters with given lower and upper bounds in given order:
	 * [x[1][1], x[1][2], x[1][3], ..., x[3][3], x[3][4]]
	 *
	 * BeginIdx will be calculated based on current number
	 * of existing parameters/columns (@see OsiProblemInstance::numberOfColumns).
	 *
	 * @param firstIdx number of all existing parameters increased by one, index of the first parameter in this group
	 * @param dimensions part of string-like parameters' group declaration
	 * @param numberOfParameters number of parameters in this group
	 * @param parameterLowerBound lower bound of every parameter in group
	 * @param parameterUpperBound upper bound of every parameter in group
	 * @param type type of all parameters in this group
	 * @param groupName name of group of parameters
	 *
	 */
	ParamIdxMapper(const ParamIdx firstIdx,
			const std::list<ParamDimIdx>& dimensions,
			const ParamIdx numberOfParameters,
			const VarValue parameterLowerBound,
			const VarValue parameterUpperBound, const ValueType type,
			const std::string& groupName);

	virtual ~ParamIdxMapper();

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
	ParamDimIdx getDimensionsSize();

	//*************************************** GETTERS & SETTERS ****************************************//

	/** Returns value of beginIdx field.
	 *
	 * @return
	 */
	ParamIdx getBeginIdx() const;

	/** Returns dimensions list.
	 *
	 * @return
	 */
	const std::list<ParamDimIdx>& getDimensions() const;

};

#endif /* PARAMETERMAPPER_HPP_ */
