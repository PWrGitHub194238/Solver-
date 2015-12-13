/*
 * EN_US_Bundle.cpp
 *
 *  Created on: 7 wrz 2015
 *      Author: tomasz
 */

#include <string>

const char* varIdxToken = "%VarIdx%";
extern const std::string varIdxFormat("%u");

const char* varDimIdxToken = "%VarDimIdx%";
extern const std::string varDimIdxFormat("%u");

const char* rowIdxToken = "%RowIdx%";
extern const std::string rowIdxFormat("%u");

const char* varValueToken = "%VarValue%";
extern const std::string varValueFormat("%f");

const char* dimIdxToken = "%DimIdx%";
extern const std::string dimIdxFormat("%u");

const char* dictionary[] = {

		//*********************************** Solver ***********************************//
		//**************** Trace ****************//
		// INIT_VAR_GROUP,
		"Creating new group of variable: %s.",
		// INIT_VAR_REGEX_MATCH
		"Definition matched.\n\tNew group's name\t:\t'%s'\n\tDimensions\t\t:\t%s",
		// INIT_VAR_GET_DIM_LIST
		"Retrieving dimensions of group of variables from definition (%s).",
		// INIT_VAR_ADD_COUNT
		"%VarIdx% variables has been added to problem instance.",
		// GET_VAR_REGEX_MATCHED
		"Trying to get variable '%s' (regex definition matched).",
		// GET_VAR_SPLIT_DEFINITION_STATUS
		"Trying to get variable from partial definition '%s' (was splitted into variable name '%s' and expected number of dimensions: %DimIdx%).",
		// DELETE_VARIABLE
		"Deleting variable:\t\n%s",

		//**************** Debug ****************//

		//**************** Info *****************//

		//**************** Warn *****************//

		//**************** Error ****************//

		//**************** Fatal ****************//
		// INIT_VAR_GROUP_REGEX_SEARCH_FAILED
		"Given definition of a group of variables '%s' is invalid and cannot be interpreted.",
		// INIT_VAR_DUPLICATE
		"Group of variables with given name ('%s') already exists. Details of this variables are listed below:\n%s",
		// GET_VAR_DIFFERENT_DIM_SIZE
		"Given variable '%s' has different number of dimensions than already defined group ('%s' is %DimIdx%-dimensional group of variables).",
		// GET_VAR_INDEX_OUT_OF_BOUNDS
		"One or more indexes in given definition of variable '%s' has value that is out of bounds of already defined group's definition ('%s' is a group of variables with dimensions: %s).",
		// GET_VAR_GROUP_REGEX_SEARCH_FAILED
		"Given definition of a variable '%s' is invalid and cannot be interpreted.",
		// SET_VAR_FIX_VALUE_OUT_OF_BOUNDS
		"Cannot assign value '%VarValue%' to variable '%s' with bounds:\n\tlower\t:\t%VarValue%,\n\tupper\t:\t%VarValue%.",

		//*********************************** VariableIdxMapper ***********************************//
		//**************** Trace ****************//
		// IF_DIMS_OUT_OF_BOUNDS
		"Comparing given dimensions '%s' with 'this' variables' dimensions '%s'.",
		// LINEARIZE_DIMENSION_LIST
		"Linearizing dimension list: '%s'...",

		//**************** Debug ****************//
		// INIT_VAR_MAPPER_CONSTRUCTOR
		"Initialize mapping to %VarIdx% variables with given parameters:\n\tDimensions\t:\t%s\n\tFirst index\t:\t%VarIdx%\n\tType\t\t:\t%s\n\tLower bound\t:\t%s\n\tUpper bound\t:\t%s",

		//**************** Info *****************//

		//**************** Warn *****************//

		//**************** Error ****************//
		// NEGATIVE_INDEX
		"Invalid definition of dimensions ('%s'). Index values have to be greater than 0. Some index is out of definition for '%s'.",
		// INDEX_OUT_OF_BOUNDS
		"One of the indexes in given definition of dimensions ('%s') is out of bounds for '%s'.",

		//**************** Fatal ****************//

		//*********************************** Variable ***********************************//
		//**************** Trace ****************//
		// SET_VAR_FIX_VALUE
		"Value of variable '%s' has been fixed to '%VarValue%' and will not be changed during LP/MIP problem solving.",

		//**************** Debug ****************//
		// INIT_VAR_VARIABLE_CONSTRUCTOR
		"Initialize variable:\n\tLower bound\t:\t%s\n\tUpper bound\t:\t%s"

		//**************** Info *****************//

		//**************** Warn *****************//

		//**************** Error ****************//

		//**************** Fatal ****************//

	};
