/*
 * Constraint.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <string>

class Constraint  {
public:
	Constraint(std::string lhs, std::string rhs, std::string numerality, std::string units, int gap);

	std::string lhs, rhs, numerality, units;
	int gap;

	virtual ~Constraint();
};

#endif /* CONSTRAINT_H_ */
