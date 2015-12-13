/*
 * Variable.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../include/Variable.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <sstream>

#include "../include/exp/VariableModelExeptions.hpp"
#include "../include/log/bundle/Bundle.hpp"
#include "../include/log/utils/LogUtils.hpp"
#include "../include/utils/Utils.hpp"

const log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("Variable"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//***************************************** CLASS FIELDS *******************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

bool Variable::isValueOutOfBounds(const VarValue value) {
	return !(this->lowerBonud <= value && value <= this->upperBound);
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

Variable::Variable(VarValue lowerBonud, VarValue upperBound) :
		lowerBonud(lowerBonud), upperBound(upperBound), objectiveCoefficient(0), value(
				0), isFixed { false } {
	DEBUG(logger, BundleKey::INIT_VAR_VARIABLE_CONSTRUCTOR,
			Utils::getStringBound(lowerBonud).c_str(),
			Utils::getStringBound(upperBound).c_str());
	this->rowCoefficients = new std::unordered_map<RowIdx, VarValue> { };
}

Variable::~Variable() {
	delete this->rowCoefficients;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void Variable::fixValue(const VarValue& fixedValue) {
	if (!this->isValueOutOfBounds(fixedValue)) {
		TRACE(logger, BundleKey::SET_VAR_FIX_VALUE, variableName.c_str(),
				fixedValue);
		this->isFixed = true;
		this->value = fixedValue;
	} else {
		FATAL(logger, BundleKey::SET_VAR_FIX_VALUE_OUT_OF_BOUNDS, fixedValue,
				this->getVariableName().c_str(),
				Utils::getStringBound(this->getLowerBonud()).c_str(),
				Utils::getStringBound(this->getUpperBound()).c_str());
		throw VariableModelExceptions::ValueOutOfBoundException();
	}
}

void Variable::setNewBounds(const VarBounds& varBounds) {

}

std::string Variable::toString() const {
	std::ostringstream os { };
	os << "\tName of variables:\t\t" << variableName
			<< "\n\tVariable's lower bound\t\t:\t"
			<< Utils::getStringBound(lowerBonud)
			<< "\n\tVariable's upper bound\t\t:\t"
			<< Utils::getStringBound(upperBound)
			<< "\n\tVariable's objective coefficient:\t" << objectiveCoefficient
			<< "\n\tRow coefficients (" << rowCoefficients->size() << "):\n"
			<< Utils::getStringVariableRowCoefficients(*rowCoefficients)
			<< std::endl;
	return os.str();
}

//*************************************** GETTERS & SETTERS ****************************************//

void Variable::setVariableName(const std::string& variableName) {
	this->variableName = variableName;
}

VarValue Variable::getLowerBonud() const {
	return lowerBonud;
}

VarValue Variable::getUpperBound() const {
	return upperBound;
}

const std::string& Variable::getVariableName() const {
	return this->variableName;
}
