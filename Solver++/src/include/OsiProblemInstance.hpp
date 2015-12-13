/*
 * OsiProblemInstance.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef OSIPROBLEMINSTANCE_HPP_
#define OSIPROBLEMINSTANCE_HPP_

#include "Typedefs.hpp"

class OsiSolverInterface;

/** Instance of LP/MIP optimization problem.
 *
 * Contains necessary information for creating and solving LP/MIP instance of optimization problem.
 * After calling s->solve(LP) or s->solve(MIP) (@see Solver) every Solver's data structure
 * will be mapped into proper OsiSolverInterface's structure allowing it to send data to given solver
 * and retrieve any available solution (if any).
 *
 * Example:
 *
 * This section adapted from Matt Galati's example
 * on the COIN-OR Tutorial website.
 *
 * Problem from Bertsimas, Tsitsiklis page 21:
 *
 *  optimal solution: x* = (1,1)
 *
 *  minimize -1 x0 - 1 x1
 *  s.t       1 x0 + 2 x1 <= 3		// 1st row
 *            2 x0 + 1 x1 <= 3		// 2nd row
 *              x0        >= 0
 *              x1        >= 0
 *
 * is represented as follow:
 *
 * numberOfColumns	-	2			// 2 variables
 * objective		-	{-1,-1}		// array of two coefficients: for variable x0 and x1
 * col_lb			-	{ 0, 0}		// array with lower...
 * col_ub			-	{ ∞, ∞}		// ... and upper bounds for each variable x0 and x1 respectively
 * numberOfRows		-	2			// 2 linear equations/inequalities
 * row_lb			-	{-∞,-∞}		// Lower bounds for left-hand side expressions in 1st and 2nd row
 * row_ub			-	{ 3, 3}		// Upper bounds for left-hand side expressions in 1st and 2nd row

 */
class OsiProblemInstance {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** Open Solver Interface
	 *
	 * API for communicating with given LP/MIP solvers.
	 *
	 */
	OsiSolverInterface *si;

	/** Number of columns.
	 *
	 * Number of unique variables with their coefficients
	 * in every row (and objective function) of LP problem.
	 *
	 */
	VarIdx numberOfColumns;

	/** Array of objective function coefficients.
	 *
	 * Each variable must have it's own coefficients
	 * in objective function (in case the variable has no influence
	 * on objective function's value then coefficient would be 0).
	 *
	 * After calling s->solve(LP) or s->solve(MIP) (@see Solver) each pair
	 * of variable and it's coefficient will be mapped into this array
	 * letting chosen solver to solve instance of a given problem.
	 *
	 */
	VarValue* objective;

	/** Array of variables' lower bounds.
	 *
	 * Each variable has to have it's own lower bound
	 * and it cannot take lower value than it's LB.
	 *
	 * After calling s->solve(LP) or s->solve(MIP) (@see Solver) each variable's lower bound
	 * will be mapped into this array letting chosen solver to solve instance of a given problem.
	 *
	 */
	VarValue* col_lb;

	/** Array of variables' upper bounds.
	 *
	 * Each variable has to have it's own upper bound
	 * and it cannot take greater value than it's UB.
	 *
	 * After calling s->solve(LP) or s->solve(MIP) (@see Solver) each variable's upper bound
	 * will be mapped into this array letting chosen solver to solve instance of a given problem.
	 *
	 */
	VarValue* col_ub;

	/** Number of rows.
	 *
	 * Number of linear equations/inequalities in LP problem and their bounds.
	 *
	 */
	VarIdx numberOfRows;

	/** Lower bound of equation/inequality in given row.
	 *
	 * Value of the left-hand side expression in given row must be greater or equal to it's LB.
	 * If row's lower bound is equal to it's upper bound
	 * then row's left-hand side expression along with it's bounds forms strict equality.
	 *
	 */
	VarValue* row_lb;

	/** Upper bound of equation/inequality in given row.
	 *
	 * Value of the left-hand side expression in given row must be lower or equal to it's UB.
	 * If row's lower bound is equal to it's upper bound
	 * then row's left-hand side expression along with it's bounds forms strict equality.
	 *
	 */
	VarValue* row_ub;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Returns new instance of a given solver.
	 *
	 * @param solverInterface
	 * @return new OsiXXXSolverInterface where XXX is the chosen solver.
	 */
	OsiSolverInterface* getNewSolverInterface(SolverInterface solverInterface);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	OsiProblemInstance(SolverInterface solverInterface);

	virtual ~OsiProblemInstance();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Add given number of columns (new variables) to this problem instance.
	 *
	 * @param numberOfNewVariables number of new variables to be added
	 */
	void addNumberOfVariables(const VarIdx numberOfNewVariables);

	//*************************************** GETTERS & SETTERS ****************************************//

	VarIdx getNumberOfColumns() const;

	OsiSolverInterface* getOsiSolverInterface() const;

};

#endif /* OSIPROBLEMINSTANCE_HPP_ */
