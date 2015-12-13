/*
 * Variable.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <list>
#include <string>
#include <unordered_map>

#include "Solver.hpp"
#include "Typedefs.hpp"
#include "VariableIdxMapper.hpp"

/** @brief Defines variable in LP/MIP optimization problem.
 *
 * @details Each variable has it's own lower or/and upper bounds and coefficients -
 * both in objective function and in set of linear equations/inequalities.
 *
 * Some of variable's properties are stored in @see VariableIdxMapper as each variable
 * is a part variable group with it's own, more general properties like variable type and name.
 *
 * Example:
 * @code
 * 	This section adapted from Matt Galati's example
 * 	on the COIN-OR Tutorial website.
 *
 * 	Problem from Bertsimas, Tsitsiklis page 21:
 *
 *	optimal solution: x* = (1,1)
 *
 *	minimize -1 x0 - 1 x1
 *	s.t       1 x0 + 2 x1 <= 3		// 1st row
 *            2 x0 + 1 x1 <= 3		// 2nd row
 *              x0        >= 0
 *              x1        >= 0
 * @endcode
 *
 * Problem is defined by single one-dimensional group of variables, namely x[2].
 * Group consist of two variables named x with default type double.
 * Each of variables in this group (x0 and x1) has the same lower
 * and upper bounds (0 <= x0 <= ∞, 0 <= x1 <= ∞) and objective coefficient (-1).
 *
 * Row coefficients for variable x0:	{1, 2}.
 *
 * Row coefficients for variable x1:	{2, 1}.
 *
 * NOTE:
 *
 * groups of variables are 1-based indexed so:
 *
 * @code
 * 	Solver s(SolverInterface::GLPK);
 * 	s.newVariable("x[2]");
 * 	s.getVariable("x[1]");	// x0
 * 	s.getVariable("x[2]");	// x1
 * @endcode
 *
 * NOT
 *
 * @code
 * 	Solver s(SolverInterface::GLPK);
 * 	s.newVariable("x[2]");
 * 	s.getVariable("x[0]");	// undefined variable
 * @endcode
 *
 */
class Variable {

	friend VariableIdxMapper::VariableIdxMapper(const VarIdx firstIdx,
			const std::list<VarDimIdx>& dimensions,
			const VarIdx numberOfVariables, const VarValue variableLowerBound,
			const VarValue variableUpperBound, const ValueType type,
			const std::string& groupName);

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** @brief Lower bound of this variable.
	 *
	 * @details Value of this variable cannot be less than it's LB (LB <= variable <= UB).
	 *
	 */
	VarValue lowerBonud;

	/** @brief Upper bound of this variable.
	 *
	 * @details Value of this variable cannot be greater than it's LB (LB <= variable <= UB).
	 *
	 */
	VarValue upperBound;

	/** @brief Coefficient value corresponding to this variable in objective function.
	 *
	 * @details Variable's coefficient determinate how much influence value
	 * of this variable has on final solution (objective function's value)
	 *
	 */
	VarValue objectiveCoefficient;

	/** @brief Map of coefficients associated to this variable in given row.
	 *
	 * @details In general form of LP/MIP optimization problem every row's left-hand side expression
	 * consists of every defined variable (some with coefficient equal to 0 if given variable
	 * has no influence on particular row's expression) so variable has number
	 * of coefficients equal to number of rows (and objective coefficient - @see objectiveCoefficient).
	 *
	 */
	std::unordered_map<RowIdx, VarValue>* rowCoefficients;

	/** @brief Value of this variable.
	 *
	 * @details Value which this variable can take is bounded by it's lower and upper bound.
	 *
	 */
	VarValue value;

	/** @brief True if fixValue() function was called.
	 *
	 * @details After declaration of variable there is possible to fix its value during solving LP/MIP problem.
	 * It causes to ignore current lower and upper bounds of this variable
	 * and pass current value of variable as bounds to OsiSolverInterface instead.
	 *
	 * If false then field value will be initialize
	 * during solving LP/MIP problem (not during model creation by user).
	 *
	 * @see fixValue()
	 *
	 */
	bool isFixed;

	/** @brief Stores actual variable full name.
	 *
	 * @details Full name of particular variable consists of is't base name (as it is a part of some bigger group of variables)
	 * and list of indexes that creates a unique name for this variable.
	 *
	 * Examples:
	 * @code
	 * 	x - single variable named "x";
	 * 	y[2]- two variables in group named "y";
	 * 	z[3][4] - 12 variables organized in two-dimensional structure with name "z";
	 * @endcode
	 *
	 */
	std::string variableName;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	bool isValueOutOfBounds( const VarValue value);

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

public:

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** @brief Creates new variable with given parameters.
	 *
	 * @param lowerBonud value of this variable cannot be less then this value
	 * @param upperBound value of this variable cannot be greater then this value
	 */
	Variable(VarValue lowerBonud, VarValue upperBound);

	virtual ~Variable();

	//*************************************** PUBLIC FUNCTIONS *****************************************//


	/** @brief Fix the value of this variable.
	 *
	 * @details Execution of this function will force lower and upper bounds of this variable to be temporary replaced by given fixedValue.
	 * It will not just simply add another constraint to the model
	 *
	 * Example:
	 *
	 * With given variable 'x' and with lower and upper bound of this variable set to -10 and 10 respectively.
	 * Set of operations:
	 *
	 * @code
	 *  Solver s(SolverInterface::GLPK);
	 *  s.newVariable("x");
	 *  s.fixVariableValue(s.getVariable("x"), 0);
	 * @endcode
	 *
	 * will result in constraints:
	 *
	 * @code
	 *  0 <= x <= 0
	 * @endcode
	 *
	 * instead of:
	 *
	 * @code
	 *  x = 0
	 *  -10 <= x <= 10
	 * @endcode
	 *
	 * @param fixedValue
	 */
	void fixValue(const VarValue& fixedValue);

	/** @brief Redefine upper and lower bound of this variable.
	 *
	 * @details Allows to redefine bounds of this variable. If new bounds is incorrect for this variable's type
	 * then type of variable will be extended to valid one:
	 *
	 * Examples:
	 * @code
	 *  Solver s(SolverInterface::GLPK);
	 *  s.newVariable("x");	// ValueType::DOUBLE by default
	 *  s.getVariable("x").setNewBounds(VarBounds(0,0));	// no change
	 *
	 *  s.newVariable("y",ValueType::INTEGER);
	 *	s.getVariable("y").setNewBounds(VarBounds(0,3.5));	// will change type to ValueType::DOUBLE
	 *
	 *	s.newVariable("z",ValueType::BINARY);
	 *	s.getVariable("z").setNewBounds(VarBounds(0,3));	// will change type to ValueType::INTEGER
	 * @endcode
	 *
	 * will result in creation of an unbounded variable 'x' which bounds are then adjust to 0, forcing inequalities 0 <= x <= 0 to hold.
	 *
	 * Following codes are equivalent:
	 * @code
	 *  Solver s(SolverInterface::GLPK);
	 *  s.newVariable("x");
	 *  s.getVariable("x").setNewBounds(VarBounds(0,0));
	 * @endcode
	 *
	 * @code
	 *  Solver s(SolverInterface::GLPK);
	 *  s.newVariable("x[2]", 0, 0, ValueType::DOUBLE);
	 * @endcode
	 *
	 * @param varBounds
	 */
	void setNewBounds(const VarBounds& varBounds);

	/** @brief return string representation of this variable
	 *
	 * @return
	 */
	std::string toString() const;

	//*************************************** GETTERS & SETTERS ****************************************//

private:

	/** @brief s
	 *
	 * @param variableName
	 */
	void setVariableName(const std::string& variableName);

public:

	VarValue getLowerBonud() const;

	VarValue getUpperBound() const;

	const std::string& getVariableName() const;

};

#endif /* VARIABLE_HPP_ */
