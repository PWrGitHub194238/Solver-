/*
 * Solver.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../include/Solver.hpp"

#include <bits/functional_hash.h>
#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <OsiSolverInterface.hpp>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <stddef.h>
#include <stdexcept>	/* out_of_range*/
#include <utility>

#include "../include/log/bundle/Bundle.hpp"
#include "../include/log/utils/LogUtils.hpp"
#include "../include/OsiProblemInstance.hpp"
#include "../include/ParameterIdxMapper.hpp"
#include "../include/RowIdxMapper.hpp"
#include "../include/utils/MapperUtils.hpp"
#include "../include/utils/StringUtils.hpp"
#include "../include/utils/Utils.hpp"
#include "../include/VariableIdxMapper.hpp"

const log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("Solver"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

const std::regex Solver::VAR_REGEXP = std::regex(
		"^(\\w+)((\\[[1-9][0-9]*\\])*)$");
const int Solver::VAR_REGEX_NAME_GROUP_IDX { 1 };
const int Solver::VAR_REGEX_DIM_GROUP_IDX { 2 };

//***************************************** CLASS FIELDS *******************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void Solver::addNumberOfVariables(const VarIdx& numberOfNewVariables) {
	TRACE(logger, BundleKey::INIT_VAR_ADD_COUNT, numberOfNewVariables);
	this->problemInstance->addNumberOfVariables(numberOfNewVariables);
}

std::list<VarDimIdx> Solver::getVariableDimensionList(
		const char* variableDimensionDefinition) {
	TRACE(logger, BundleKey::INIT_VAR_GET_DIM_LIST,
			Utils::getStringDimensions(variableDimensionDefinition).c_str());
	std::list<VarDimIdx> dimensions = StringUtils::variableDimensionTokenize<
			VarDimIdx>(variableDimensionDefinition);
	return dimensions;
}

VarIdx Solver::countVariablesInGroup(const std::list<VarDimIdx>& dimensions) {
	int variablesCounter { 1 };
	for (VarDimIdx dimensionSize : dimensions) {
		variablesCounter *= dimensionSize;
	}
	return variablesCounter;
}

void Solver::insertVariable(const std::string& matchedNameOfVariable,
		const VarIdx numberOfGroupVariables,
		const std::list<VarDimIdx>& listOfDimensions, const VarValue lowerBound,
		const VarValue upperBound, const ValueType variableType)
				throw (VariableModelExceptions::DuplicateException) {
	if (!this->variables->count(matchedNameOfVariable)) {
		this->variables->insert(
				std::pair<std::string, VariableIdxMapper*>(
						matchedNameOfVariable, new VariableIdxMapper {
								getNumberOfVariables(), listOfDimensions,
								numberOfGroupVariables, lowerBound, upperBound,
								variableType, matchedNameOfVariable }));
		addNumberOfVariables(numberOfGroupVariables);
	} else {
		FATAL(logger, BundleKey::INIT_VAR_DUPLICATE,
				matchedNameOfVariable.c_str(),
				Solver::getVariableIdxMapperAt(matchedNameOfVariable)->toString().c_str());
		throw VariableModelExceptions::DuplicateException();
	}
}

VariableIdxMapper* Solver::getVariableIdxMapperAt(
		const std::string& groupOfVariablesName)
				throw (VariableModelExceptions::NoSuchGroupException) {
	try {
		return this->variables->at(groupOfVariablesName);
	} catch (const std::out_of_range& e) {
		throw VariableModelExceptions::NoSuchGroupException();
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

Solver::Solver(SolverInterface solverInterface) {
	this->problemInstance = new OsiProblemInstance { solverInterface };
	this->parameters = new std::unordered_map<std::string, ParamIdxMapper*> { };
	this->variables =
			new std::unordered_map<std::string, VariableIdxMapper*> { };
	this->rows = new std::unordered_map<std::string, RowIdxMapper*> { };
	Utils::impl::storeInfinityValues(
			this->problemInstance->getOsiSolverInterface()->getInfinity());
}

Solver::~Solver() {
	delete this->problemInstance;
	delete this->parameters;
	delete this->variables;
	delete this->rows;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void Solver::newVariable(const std::string& variableDefinition)
		throw (VariableModelExceptions::DuplicateException,
		VariableModelExceptions::InvalidGroupDefinitionException) {
	return Solver::newVariable(variableDefinition,
			Utils::getUnbounded(Bounds::LOWER),
			Utils::getUnbounded(Bounds::UPPER), ValueType::DOUBLE);
}

void Solver::newVariable(const std::string& variableDefinition,
		const ValueType variableType)
				throw (VariableModelExceptions::DuplicateException,
				VariableModelExceptions::InvalidGroupDefinitionException) {
	switch (variableType) {
	case ValueType::BINARY:
		return Solver::newVariable(variableDefinition, 0, 1, variableType);
	default:
		return Solver::newVariable(variableDefinition,
				Utils::getUnbounded(Bounds::LOWER),
				Utils::getUnbounded(Bounds::UPPER), variableType);
	}
}

void Solver::newVariable(const std::string& variableDefinition,
		const VarValue variableLowerBound, const VarValue variableUpperBound,
		const ValueType variableType)
				throw (VariableModelExceptions::DuplicateException,
				VariableModelExceptions::InvalidGroupDefinitionException) {
	std::smatch variableMatch { };
	std::list<VarDimIdx> newVariableDimensions { };

	TRACE(logger, BundleKey::INIT_VAR_GROUP, variableDefinition.c_str());

	if (std::regex_search(variableDefinition, variableMatch,
			Solver::VAR_REGEXP)) {
		TRACE(logger, BundleKey::INIT_VAR_REGEX_MATCH,
				variableMatch[Solver::VAR_REGEX_NAME_GROUP_IDX].str().c_str(),
				Utils::getStringDimensions(
						variableMatch[Solver::VAR_REGEX_DIM_GROUP_IDX].str()).c_str());
		newVariableDimensions = getVariableDimensionList(
				variableMatch[Solver::VAR_REGEX_DIM_GROUP_IDX].str().c_str());
		insertVariable(variableMatch[Solver::VAR_REGEX_NAME_GROUP_IDX].str(),
				Solver::countVariablesInGroup(newVariableDimensions),
				newVariableDimensions, variableLowerBound, variableUpperBound,
				variableType);
	} else {
		FATAL(logger, BundleKey::INIT_VAR_GROUP_REGEX_SEARCH_FAILED,
				variableDefinition.c_str());
		throw VariableModelExceptions::InvalidGroupDefinitionException();
	}
	return;
}

void Solver::deleteAllVariables() {
	std::unordered_map<std::string, VariableIdxMapper*>::const_iterator itBegin =
			this->variables->begin();
	std::unordered_map<std::string, VariableIdxMapper*>::const_iterator itEnd =
			this->variables->end();
	while (itBegin != itEnd) {
		TRACE(logger, BundleKey::DELETE_VARIABLE,
				itBegin->second->toString().c_str());
		delete itBegin->second;
		++itBegin;
	}
	this->variables->clear();
}

void Solver::deleteVariable(const char* variableName) throw (std::out_of_range) {
	TRACE(logger, BundleKey::DELETE_VARIABLE,
			Solver::getVariableIdxMapperAt(variableName)->toString().c_str());

	delete Solver::getVariableIdxMapperAt(variableName);
	this->variables->erase(variableName);
}

/* Friend of VariableIdxMapper class: getVariable() */
Variable& Solver::getVariable(const std::string& variableDefinition)
		throw (std::out_of_range,
		VariableModelExceptions::InvalidNumberOfDimensionIndexesException,
		VariableModelExceptions::IndexOutOfBoundException,
		VariableModelExceptions::InvalidDefinitionException) {
	std::smatch variableMatch { };
	std::list<VarDimIdx> dimensions { };
	VariableIdxMapper*variableIdxMapper { };

	if (std::regex_search(variableDefinition, variableMatch,
			Solver::VAR_REGEXP)) {
		variableIdxMapper = Solver::getVariableIdxMapperAt(
				variableMatch[Solver::VAR_REGEX_NAME_GROUP_IDX].str());
		dimensions = getVariableDimensionList(
				variableMatch[Solver::VAR_REGEX_DIM_GROUP_IDX].str().c_str());
		TRACE(logger, BundleKey::GET_VAR_REGEX_MATCHED,
				variableDefinition.c_str());
		if (variableIdxMapper->getDimensionsSize() == dimensions.size()
				&& !MapUtils::ifDimensionsOutOfBounds<VarDimIdx>(
						variableIdxMapper->getDimensions(), dimensions)) {
			return variableIdxMapper->getVariable(dimensions);
		} else if (variableIdxMapper->getDimensionsSize()
				!= dimensions.size()) {
			FATAL(logger, BundleKey::GET_VAR_DIFFERENT_DIM_SIZE,
					variableDefinition.c_str(), dimensions.size(),
					variableMatch[Solver::VAR_REGEX_NAME_GROUP_IDX].str().c_str(),
					variableIdxMapper->getDimensionsSize());
			throw VariableModelExceptions::InvalidNumberOfDimensionIndexesException();
		} else {
			FATAL(logger, BundleKey::GET_VAR_INDEX_OUT_OF_BOUNDS,
					variableDefinition.c_str(),
					variableMatch[Solver::VAR_REGEX_NAME_GROUP_IDX].str().c_str(),
					Utils::getStringDimensions(
							variableIdxMapper->getDimensions()).c_str());
			throw VariableModelExceptions::IndexOutOfBoundException();
		}
	} else {
		FATAL(logger, BundleKey::GET_VAR_GROUP_REGEX_SEARCH_FAILED,
				variableDefinition.c_str());
		throw VariableModelExceptions::InvalidDefinitionException();
	}
}

/* Friend of VariableIdxMapper class: getVariable() */
Variable& Solver::getVariable(const std::string& variableDefinition,
		VarDimIdx firstDimensionIndex,
		...) throw (std::out_of_range,
				VariableModelExceptions::InvalidNumberOfDimensionIndexesException,
				VariableModelExceptions::IndexOutOfBoundException) {
	std::list<VarDimIdx> dimensions { };
	VariableIdxMapper*variableIdxMapper { };
	va_list va { };

	std::string variableName { };
	VarDimIdx numberOfDimensions { };
	size_t firstDimensionBracketPos = variableDefinition.find_first_of(
			StringUtils::Impl::DIM_GROUP_DELIMETERS[0]);

	if (firstDimensionBracketPos != std::string::npos) {
		variableName = variableDefinition.substr(0, firstDimensionBracketPos);
		numberOfDimensions = (VarDimIdx) (variableDefinition.length()
				- firstDimensionBracketPos) / 2;

		TRACE(logger, BundleKey::GET_VAR_SPLIT_DEFINITION_STATUS,
				variableDefinition.c_str(), variableName.c_str(),
				numberOfDimensions);
		variableIdxMapper = Solver::getVariableIdxMapperAt(variableName);

		if (variableIdxMapper->getDimensionsSize() == numberOfDimensions) {
			va_start(va, firstDimensionIndex);
			dimensions.push_back(firstDimensionIndex);
			for (VarDimIdx i = 1; i < numberOfDimensions; i += 1) {
				dimensions.push_back(va_arg(va, VarDimIdx));
			}
			va_end(va);

			if (!MapUtils::ifDimensionsOutOfBounds<VarDimIdx>(
					variableIdxMapper->getDimensions(), dimensions)) {
				return variableIdxMapper->getVariable(dimensions);
			} else {
				FATAL(logger, BundleKey::GET_VAR_INDEX_OUT_OF_BOUNDS,
						variableDefinition.c_str(), variableName.c_str(),
						Utils::getStringDimensions(
								variableIdxMapper->getDimensions()).c_str());
				throw VariableModelExceptions::IndexOutOfBoundException();
			}
		} else {
			FATAL(logger, BundleKey::GET_VAR_DIFFERENT_DIM_SIZE,
					variableDefinition.c_str(), dimensions.size(),
					variableName.c_str(),
					variableIdxMapper->getDimensionsSize());
			throw VariableModelExceptions::InvalidNumberOfDimensionIndexesException();
		}
	} else {
		FATAL(logger, BundleKey::GET_VAR_GROUP_REGEX_SEARCH_FAILED,
				variableDefinition.c_str());
		throw VariableModelExceptions::InvalidDefinitionException();
	}
}

//*************************************** GETTERS & SETTERS ****************************************//
VarIdx Solver::getNumberOfVariables() const {
	return this->problemInstance->getNumberOfColumns();
}
