/*
 * Bundle.h
 *
 *  Created on: 7 wrz 2015
 *      Author: tomasz
 */

#ifndef INCLUDE_BUNDLE_H_
#define INCLUDE_BUNDLE_H_

#include <string>

enum BundleKey {

	//*********************************** Solver ***********************************//
	//**************** Trace ****************//
	INIT_VAR_GROUP,
	INIT_VAR_REGEX_MATCH,
	INIT_VAR_GET_DIM_LIST,
	INIT_VAR_ADD_COUNT,
	GET_VAR_REGEX_MATCHED,
	GET_VAR_SPLIT_DEFINITION_STATUS,
	DELETE_VARIABLE,
	//**************** Debug ****************//
	//**************** Info *****************//
	//**************** Warn *****************//
	//**************** Error ****************//
	//**************** Fatal ****************//
	INIT_VAR_GROUP_REGEX_SEARCH_FAILED,
	INIT_VAR_DUPLICATE,
	GET_VAR_DIFFERENT_DIM_SIZE,
	GET_VAR_INDEX_OUT_OF_BOUNDS,
	GET_VAR_GROUP_REGEX_SEARCH_FAILED,
	SET_VAR_FIX_VALUE_OUT_OF_BOUNDS,
	//*********************************** VariableIdxMapper ***********************************//
	//**************** Trace ****************//
	IF_DIMS_OUT_OF_BOUNDS,
	LINEARIZE_DIMENSION_LIST,
	//**************** Debug ****************//
	INIT_VAR_MAPPER_CONSTRUCTOR,
	//**************** Info *****************//
	//**************** Warn *****************//
	//**************** Error ****************//
	NEGATIVE_INDEX,
	INDEX_OUT_OF_BOUNDS,
	//**************** Fatal ****************//
	//*********************************** Variable ***********************************//
	//**************** Trace ****************//
	SET_VAR_FIX_VALUE,
	//**************** Debug ****************//
	INIT_VAR_VARIABLE_CONSTRUCTOR
	//**************** Info *****************//
	//**************** Warn *****************//
	//**************** Error ****************//
	//**************** Fatal ****************//
};

extern const char* varIdxToken;
extern const std::string varIdxFormat;

extern const char* varDimIdxToken;
extern const std::string varDimIdxFormat;

extern const char* rowIdxToken;
extern const std::string rowIdxFormat;

extern const char* varValueToken;
extern const std::string varValueFormat;

extern const char* dimIdxToken;
extern const std::string dimIdxFormat;

extern const char* dictionary[];

#endif /* INCLUDE__BUNDLE_H_ */
