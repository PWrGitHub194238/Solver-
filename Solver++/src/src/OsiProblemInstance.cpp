/*
 * OsiProblemInstance.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../include/OsiProblemInstance.hpp"

#include <OsiGlpkSolverInterface.hpp>

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

OsiSolverInterface* OsiProblemInstance::getNewSolverInterface(
		SolverInterface solverInterface) {
	switch (solverInterface) {
	case GLPK:
		return new OsiGlpkSolverInterface { };
	}
	return NULL;
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

OsiProblemInstance::OsiProblemInstance(SolverInterface solverInterface): numberOfColumns(0), numberOfRows(0) {
	this->si = getNewSolverInterface(solverInterface);
}

OsiProblemInstance::~OsiProblemInstance() {
	delete this->si;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void OsiProblemInstance::addNumberOfVariables(
		const VarIdx numberOfNewVariables) {
	this->numberOfColumns += numberOfNewVariables;
}

//*************************************** GETTERS & SETTERS ****************************************//

VarIdx OsiProblemInstance::getNumberOfColumns() const {
	return this->numberOfColumns;
}

OsiSolverInterface* OsiProblemInstance::getOsiSolverInterface() const {
	return this->si;
}

