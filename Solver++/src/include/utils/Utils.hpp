/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <list>
#include <string>
#include <unordered_map>

#include "../Typedefs.hpp"

class Variable;

namespace Utils {

namespace impl {
void storeInfinityValues(const VarValue infinity);

/** Reconstruct list of dimensions of given variable based on its index in variables array in VariableIdxMapper class.
 *
 * Example on group of variables 'x[4][5]':
 * 	x[0] -> x[1][1]	(0*4 + 0)
 * 	x[7] -> x[2][3]	(1*5 + 2)
 *
 * @param variableIdx
 * @param groupDimensions
 * @return
 */
template<typename DimIdx, typename Idx>
const std::list<DimIdx> unlinearizeDimensionList(Idx& variableIdx,
		const std::list<DimIdx>& groupDimensions) {
	DimIdx dimensionIdx { };
	DimIdx dimensionMultiplier { 1 };
	std::list<DimIdx> variableDimensions { };
	const typename std::list<DimIdx>::const_iterator itEnd = groupDimensions.end();

	for (typename std::list<DimIdx>::const_iterator itBegin =
			++groupDimensions.begin(); itBegin != itEnd; ++itBegin) {
		dimensionMultiplier *= *itBegin;
	}
	for (typename std::list<DimIdx>::const_iterator itBegin =
			++groupDimensions.begin(); itBegin != itEnd; ++itBegin) {
		dimensionIdx = 1;
		while (variableIdx >= dimensionMultiplier) {
			dimensionIdx += 1;
			variableIdx -= dimensionMultiplier;
		}
		variableDimensions.push_back(dimensionIdx);
		dimensionMultiplier /= *itBegin;
	}
	return variableDimensions;
}

}

/** Returns string that contains pairs of row index and coefficient that is corresponding to it.
 *
 * @param rowCoefficients field of Variable class.
 * @return
 */
const std::string getStringVariableRowCoefficients(
		const std::unordered_map<RowIdx, VarValue>& rowCoefficients);

/** Return exact same string that is passed as parameter or "no dimensions" in case passed string is empty.
 *
 * If given string is empty means that it refers to dimensions of 0-dimensional variable.
 *
 * @param dimensions
 * @return
 */
const std::string getStringDimensions(const std::string& dimensions);

/** Returns string representation of dimensions.
 *
 * Returned string is in the same format as dimensional part of definition of group of variables e.g. '[2][3]'.
 * If given list is empty (single variables are 0-dimensional) then "no dimensions" string will be returned.
 * @return
 */
const std::string getStringDimensions(const std::list<VarDimIdx>& dimensions);

/** Returns string representation of given bound.
 *
 * In case given bound is referring to infinity or negative infinity values
 * then "inf" or "-inf" respectively will be returned instead of numeric value.
 * @param bound
 * @return
 */
const std::string getStringBound(VarValue bound);

/** Returns name of given type.
 *
 * @param valueType
 * @return
 */
const std::string getStringValueType(ValueType valueType);

/** Returns solver-dependent values of "lower/upper bounds" of unbounded variable.
 *
 * Before calling Solver instance with chosen OsiInterface must be initialized
 * as it's constructor calls OsiInterface for solver-dependent
 * infinity value and pass them into Utils functions.
 *
 * Calling:
 *
 * Utils::getUnbounded(LOWER) will return solver-dependent value of negative infinity,
 * Utils::getUnbounded(LOWER) - solver-dependent value of infinity.
 *
 * @param bound
 * @return -1.0 * si->getInfinity() for lower bound and si->getInfinity() for upper bound
 * where si is chosen solver OsiInterface.
 */
VarValue getUnbounded(Bounds bound);

}

#endif /* UTILS_HPP_ */
