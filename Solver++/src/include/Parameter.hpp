/*
 * parameter.h
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <string>

#include "Typedefs.hpp"

class Parameter {

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//***************************************** CLASS FIELDS *******************************************//

	/** Lower bound of this parameter.
	 *
	 * Value of this parameter cannot be less than it's LB (LB <= parameter <= UB).
	 *
	 */
	ParamValue lowerBonud;

	/** Upper bound of this parameter.
	 *
	 * Value of this parameter cannot be greater than it's LB (LB <= parameter <= UB).
	 *
	 */
	ParamValue upperBound;

	/** Value of this parameter.
	 *
	 * Value which this parameter can take is bounded by it's lower and upper bound.
	 *
	 */
	ParamValue value;

	/** Stores actual parameter full name.
	 *
	 * Example:
	 * 	p, param[2], parameter[3][4]
	 *
	 */
	std::string parameterName;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

public:

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** Creates new parameter with default value 0.
	 *
	 * @param lowerBonud value of this parameter cannot be less then this value
	 * @param upperBound value of this parameter cannot be greater then this value
	 */
	Parameter(ParamValue lowerBonud, ParamValue upperBound);

	virtual ~Parameter();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Checks if given value extends this parameter's bounds.
	 *
	 * @param value
	 * @return true if given value does extend lower or upper bound of this parameter, false if it's not.
	 */
	bool isParameterOutOfBounds(const ParamValue value);

	/** Returns string representation of this class.
	 *
	 * @return
	 */
	std::string toString();

	//*************************************** GETTERS & SETTERS ****************************************//

	void setParameterName(const std::string& parameterName);

	ParamValue getLowerBonud() const;

	ParamValue getUpperBound() const;

	const std::string& getParameterName() const;

};

#endif /* PARAMETER_HPP_ */
