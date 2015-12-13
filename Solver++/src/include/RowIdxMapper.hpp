/*
 * RowIdxMapper.hpp
 *
 *  Created on: 25 wrz 2015
 *      Author: tomasz
 */

#ifndef INCLUDE_ROWIDXMAPPER_HPP_
#define INCLUDE_ROWIDXMAPPER_HPP_

#include <list>
#include <string>

#include "Typedefs.hpp"

/** Contains all informations about rows with equations/inequalities for LP/MIP solver.
 *
 * It helps Solver class to manage LP structure allowing natural row naming at the same time.
 *
 * Example:
 *
 * Variable group x[3][4] is a group of 12 variables with base name x.
 * The goal is to create restrictions for everyone of them and create relations
 * between parameters that are stored in C-like matrix p[3][4].
 * RowIdxMapper class allow to create 12 rows named 'xp' and assign
 * the same dimensions to its base name.
 *
 * And here is a code example:
 *
 *  @code
 *
 *  s.newRows("xp[3][4]");
 *  for ( int i = 1; i <= x; i += 1 ) {
 *  	for ( int i = 1; i <= x; i += 1 ) {
 *  		s.addElement(
 *  			s.getRow("xp[][]",x,y),
 *  			s.getVariable("x[][]",x,y),
 *  			xp[x-1][y-1]
 *  		);
 *  	}
 *  }
 *
 *  @endcode
 */
class RowIdxMapper {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** Index of the first row in array/matrix or more dimensional structure.
	 *
	 * If created variable is 0-dimensional structure then beginIdx is equal to lastIdx
	 * and dimensions list is equal to null.
	 *
	 */
	RowIdx beginIdx;

	/** Every row may have multiple dimensions.
	 *
	 * List contains naturally ordered dimensional indexes of that group of rows (from the highest dimension to the lowest).
	 * If single row is created (0-dimensional row group) then dimensions list will be empty.
	 *
	 * Example:
	 *
	 * 	For given group of rows "xp[1][2][3]" dimensions list will contain elements: 1, 2 and 3.
	 *
	 */
	std::list<RowDimIdx> dimensions;

	/** Number of rows in array/matrix or more dimensional structure.
	 *
	 */
	VarIdx numberOfRows;

	/** Stores actual name of this group of rows.
	 *
	 */
	std::string groupName;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//**************************************** CONSTANT FIELDS *****************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	RowIdxMapper();

	virtual ~RowIdxMapper();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_ROWIDXMAPPER_HPP_ */
