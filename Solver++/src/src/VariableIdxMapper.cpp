/*
 * VariableIdxMapper.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../include/VariableIdxMapper.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <sstream>

#include "../include/log/bundle/Bundle.hpp"
#include "../include/log/utils/LogUtils.hpp"
#include "../include/utils/MapperUtils.hpp"
#include "../include/utils/Utils.hpp"
#include "../include/Variable.hpp"

const log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("VariableIdxMapper"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//***************************************** CLASS FIELDS *******************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

Variable& VariableIdxMapper::getVariable(
		const std::list<VarDimIdx>& dimensions) {
	return *variables[MapUtils::linearizeDimensionList<VarIdx, VarDimIdx>(
			this->dimensions, dimensions)];
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

/* Friend of Variable class: setVariableName() */
VariableIdxMapper::VariableIdxMapper(const VarIdx firstIdx,
		const std::list<VarDimIdx>& dimensions, const VarIdx numberOfVariables,
		const VarValue variableLowerBound, const VarValue variableUpperBound,
		const ValueType type, const std::string& groupName) :
		beginIdx(firstIdx), dimensions(dimensions), numberOfVariables(
				numberOfVariables), type(type), groupName(groupName) {
	DEBUG(logger, BundleKey::INIT_VAR_MAPPER_CONSTRUCTOR, numberOfVariables,
			Utils::getStringDimensions(this->dimensions).c_str(), firstIdx,
			Utils::getStringValueType(type).c_str(),
			Utils::getStringBound(variableLowerBound).c_str(),
			Utils::getStringBound(variableUpperBound).c_str());
	variables = new Variable*[numberOfVariables] { };
	for (VarIdx i = 0; i < numberOfVariables; i += 1) {
		variables[i] = new Variable { variableLowerBound, variableUpperBound };
		variables[i]->setVariableName(
				MapUtils::getVariableName(i, groupName, dimensions));
	}
}

VariableIdxMapper::~VariableIdxMapper() {
	for (VarIdx i = 0; i < numberOfVariables; i += 1) {
		delete variables[i];
	}
	delete[] variables;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

std::string VariableIdxMapper::toString() {
	std::ostringstream os { };
	os << "\tName of variables:\t" << groupName
			<< "\n\tVariables' dimensions:\t"
			<< Utils::getStringDimensions(this->dimensions) << std::endl;
	return os.str();
}

VarDimIdx VariableIdxMapper::getDimensionsSize() {
	return (VarDimIdx) dimensions.size();
}

//*************************************** GETTERS & SETTERS ****************************************//

VarIdx VariableIdxMapper::getBeginIdx() const {
	return beginIdx;
}

const std::list<VarDimIdx>& VariableIdxMapper::getDimensions() const {
	return dimensions;
}

