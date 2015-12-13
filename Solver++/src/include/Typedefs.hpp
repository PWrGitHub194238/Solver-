/*
 * Typedefs.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef TYPEDEFS_HPP_
#define TYPEDEFS_HPP_

typedef double ParamValue;
typedef unsigned int ParamIdx;
typedef unsigned int ParamDimIdx;

typedef double VarValue;
typedef unsigned int VarIdx;
typedef unsigned int VarDimIdx;

typedef unsigned int RowIdx;
typedef unsigned int RowDimIdx;

typedef std::pair<VarValue, VarValue> VarBounds;

/**
 * Test doc
 */
enum SolverInterface {
	GLPK//!< GLPK
};

enum Bounds {
	LOWER, UPPER
};

/** Test doc
 *
 */
enum ValueType {
	DOUBLE, //!< DOUBLE
	INTEGER,//!< INTEGER
	BINARY  //!< BINARY
};

#endif /* TYPEDEFS_HPP_ */
