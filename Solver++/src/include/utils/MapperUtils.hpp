/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef MAPPER_UTILS_HPP_
#define MAPPER_UTILS_HPP_

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <list>
#include <string>

#include "../log/bundle/Bundle.hpp"
#include "../log/utils/LogUtils.hpp"
#include "../Typedefs.hpp"
#include "Utils.hpp"

class Variable;

/** @brief Set of functions that creates simple interface for certain operations that are required by every XMapper class.
 *
 */
namespace MapUtils {

const static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("MapperUtils"));


/** Checks if given list of dimensions is valid in context of dimensions of some group of elements.
 *
 *  Function will return true only if at least one of the following conditions is met"
 *  - size of list that was passed by parameter is not equal to size
 *  of groupDimensions list (number of dimension are not equal),
 *  - one of given indexes in dimensions parameter exceeds the corresponding index
 *  in groupDimensions list (element with that set of indexes does not exists).
 *
 * @param groupDimensions
 * @param elementDimensions
 * @return
 */
template<typename DimIdx>
bool ifDimensionsOutOfBounds(const std::list<DimIdx>& groupDimensions,
		const std::list<DimIdx>& elementDimensions) {
	std::list<VarDimIdx>::const_iterator itThisBegin = groupDimensions.begin();
	std::list<VarDimIdx>::const_iterator itParamBegin =
			elementDimensions.begin();
	const std::list<VarDimIdx>::const_iterator itThisEnd =
			groupDimensions.end();

	TRACE(logger, BundleKey::IF_DIMS_OUT_OF_BOUNDS,
			Utils::getStringDimensions(elementDimensions).c_str(),
			Utils::getStringDimensions(groupDimensions).c_str());
	for (; itThisBegin != itThisEnd; ++itThisBegin, ++itParamBegin) {
		if (*itParamBegin <= 0 || *itThisBegin < *itParamBegin) {
			ERROR(logger,
					(*itParamBegin <= 0 ?
							BundleKey::NEGATIVE_INDEX :
							BundleKey::INDEX_OUT_OF_BOUNDS),
					Utils::getStringDimensions(elementDimensions).c_str(),
					Utils::getStringDimensions(groupDimensions).c_str());
			return false;
		}
	}
	return false;
}

/** Return linearized index of given element in this group based on given dimensions list.
 *
 * Note that indexes of variables/parameters/rows in LP/MIP problems are NOT 0 based.
 * Therefore all calculation have to take it into account to decrease every index by 1 while reading.
 *
 * Example:
 * 	linearizeDimensionList({},{})			->	0	(with group 'x')
 * 	linearizeDimensionList({4}, {1})		->	0	(with group 'x[4]')
 *
 * 	linearizeDimensionList({5,3}, {2,3})	->	1*3 + 2	(with group 'x[5][3]')
 *	linearizeDimensionList({5,3,7}, {2,3,4})->	1*(3*7) + 2*7 + 3	(with group 'x[5][3][7]')
 *
 *	Function will not check if indexes are valid (@see ifDimensionsOutOfBounds).
 *
 * @param groupDimensions
 * @param elementDimensions
 * @return
 */
template<typename Idx, typename DimIdx>
Idx linearizeDimensionList(const std::list<DimIdx>& groupDimensions,
		const std::list<DimIdx>& elementDimensions) {
	typename std::list<DimIdx>::const_reverse_iterator itParamEnd =
			elementDimensions.rbegin();
	typename std::list<DimIdx>::const_reverse_iterator itThisEnd =
			groupDimensions.rbegin();
	const typename std::list<DimIdx>::const_reverse_iterator itParamBegin =
			elementDimensions.rend();

	VarIdx numberOfVariablesOnPreviousDim = 1;
	VarIdx variableIndex = 0;

	TRACE(logger, BundleKey::LINEARIZE_DIMENSION_LIST,
			Utils::getStringDimensions(elementDimensions).c_str());

	for (; itParamEnd != itParamBegin; ++itParamEnd, ++itThisEnd) {
		variableIndex += numberOfVariablesOnPreviousDim * (*itParamEnd - 1);
		numberOfVariablesOnPreviousDim *= *itThisEnd;
	}
	return variableIndex;
}

/** Reconstruct list of dimensions of element based on its index in XIdxMapper class
 * and given list of bounds of dimensions of that element where "X" is either: Parameter, Variable or Row.
 *
 * Every structure has its own types and this method needs to be called with proper template parameters:
 * - MapUtils::unlinearizeDimensionList<VarIdx,VarDimIdx>(i, d) - for VariableIdxMapper,
 * - MapUtils::unlinearizeDimensionList<ParamIdx,ParamDimIdx>(i, d) - for ParameterIdxMapper,
 * - MapUtils::unlinearizeDimensionList<RowIdx,RowDimIdx>(i, d) - for RowdxMapper,
 *
 * where "i" is an index of given structure in XIdxMapper one-dimensional array field
 * that stores given element and "d" is dimension list that defines group of elements
 * to which given element belongs.
 *
 * Example on group of parameters 'p[4][5]' (list<ParamDimIdx> - {4,5}):
 * 	p[0]	->	p[1][1]	(0*5 + 0)
 * 	p[7]	->	p[2][3]	(1*5 + 2)
 *
 * Example on group of variables 'x[12][6][8]' (list<VarDimIdx> - {12,6,8}):
 * 	x[0]	->	x[1][1][1]	(0*(6*8) + 0*8 + 0)
 * 	x[73]	->	x[2][4][1]	(1*(6*8) + 3*8 + 1)
 *
 * @param idx index of given structure in XIdxMapper one-dimensional array field that stores given element
 * @param groupDimensions list of upper bounds of dimensions
 * @return
 */
template<typename Idx, typename DimIdx>
const std::list<DimIdx> unlinearizeDimensionList(Idx idx,
		const std::list<DimIdx>& groupDimensions) {
	DimIdx dimensionIdx { };
	DimIdx dimensionMultiplier { 1 };
	std::list<DimIdx> variableDimensions { };
	const typename std::list<DimIdx>::const_iterator itEnd =
			groupDimensions.end();

	for (typename std::list<DimIdx>::const_iterator itBegin =
			++groupDimensions.begin(); itBegin != itEnd; ++itBegin) {
		dimensionMultiplier *= *itBegin;
	}
	for (typename std::list<DimIdx>::const_iterator itBegin =
			++groupDimensions.begin(); itBegin != itEnd; ++itBegin) {
		dimensionIdx = 1;
		while (idx >= dimensionMultiplier) {
			dimensionIdx += 1;
			idx -= dimensionMultiplier;
		}
		variableDimensions.push_back(dimensionIdx);
		dimensionMultiplier /= *itBegin;
	}
	return variableDimensions;
}

/** Return full name of element based on its index in one-dimensional array in XIdxMapper class
 * where "X" is either: Parameter, Variable or Row and given list of bounds of dimensions of that group's element.
 *
 * Every structure has its own types and this method needs to be called with proper template parameters:
 * - MapUtils::getVariableName<VarIdx,VarDimIdx>(i, n, d) - for VariableIdxMapper,
 * - MapUtils::getVariableName<ParamIdx,ParamDimIdx>(i, n, d) - for ParameterIdxMapper,
 * - MapUtils::getVariableName<RowIdx,RowDimIdx>(i, n, d) - for RowdxMapper,
 *
 * where "i" is an index of given structure in XIdxMapper one-dimensional array field that stores given element,
 * "n" states for name of given group of elements and "d" is dimension list of element which name we want to generate.
 *
 * Example on group of parameters 'p[4][5]' (group name - 'p'; list<ParamDimIdx> - {4,5}):
 * 	p[0]	->	p[1][1]	(0*5 + 0)
 * 	p[7]	->	p[2][3]	(1*5 + 2)
 *
 * Example on group of variables 'x[12][6][8]' (group name - 'x'; list<VarDimIdx> - {12,6,8}):
 * 	x[0]	->	x[1][1][1]	(0*(6*8) + 0*8 + 0)
 * 	x[73]	->	x[2][4][1]	(1*(6*8) + 3*8 + 1)
 *
 * @param idx index of given structure in XIdxMapper one-dimensional array field that stores given element
 * @param groupName name of group to which given element belongs
 * @param groupDimensions list of upper bounds of dimensions
 * @return
 */
template<typename Idx, typename DimIdx>
const std::string getVariableName(Idx idx, const std::string& groupName,
		const std::list<DimIdx>& groupDimensions) {
	return groupName
			+ (groupDimensions.empty() ?
					"" :
					Utils::getStringDimensions(
							unlinearizeDimensionList<Idx, DimIdx>(idx,
									groupDimensions)));
}

}

#endif /* MAPPER_UTILS_HPP_ */
