/*
 * Utils.cpp
 *
 *  Created on: 3 wrz 2015
 *      Author: tomasz
 */

#include "../../include/utils/Utils.hpp"

#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "../../include/Typedefs.hpp"

namespace Utils {

namespace impl {
VarValue INFY = 0;
VarValue NEG_INFY = 0;
void storeInfinityValues(const VarValue infinity) {
	Utils::impl::INFY = infinity;
	Utils::impl::NEG_INFY = -1.0 * infinity;
}

}

const std::string getStringVariableRowCoefficients(
		const std::unordered_map<RowIdx, VarValue>& rowCoefficients) {
	std::ostringstream os { };
	std::unordered_map<RowIdx, VarValue>::const_iterator itEnd =
			rowCoefficients.end();

	os << "\t\tRowIdx\t\tCoefficient's value\n";

	for (std::unordered_map<RowIdx, VarValue>::const_iterator itBegin =
			rowCoefficients.begin(); itBegin != itEnd; ++itBegin) {
		os << "\t\t" << itBegin->first << " \t\t" << itBegin->second << "\n";
	}
	return os.str();
}

const std::string getStringDimensions(const std::string& dimensions) {
	return dimensions.empty() ? "no dimensions" : dimensions;
}

const std::string getStringDimensions(const std::list<VarDimIdx>& dimensions) {
	std::ostringstream s { };
	std::list<VarDimIdx>::const_iterator itBegin = dimensions.begin();
	std::list<VarDimIdx>::const_iterator itEnd = dimensions.end();
	for (; itBegin != itEnd; ++itBegin) {
		s << "[" << (*itBegin) << "]";
	}
	return dimensions.empty() ? "no dimensions" : s.str();
}

const std::string getStringBound(VarValue bound) {
	if (bound == Utils::impl::INFY) {
		return "inf";
	} else if (bound == Utils::impl::NEG_INFY) {
		return "-inf";
	} else {
		return std::to_string(bound);
	}
}

const std::string getStringValueType(ValueType valueType) {
	switch (valueType) {
	case ValueType::BINARY:
		return "binary";
	case ValueType::INTEGER:
		return "integer";
	default:
		return "double";
	}
}

VarValue getUnbounded(Bounds bound) {
	switch (bound) {
	case Bounds::LOWER:
		return Utils::impl::NEG_INFY;
	case Bounds::UPPER:
		return Utils::impl::INFY;
	default:
		return 0;
	}
}

}
