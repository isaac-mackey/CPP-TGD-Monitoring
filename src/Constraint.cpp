/*
 * Constraint.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "Constraint.h"

using namespace std;

Constraint::Constraint(string lhs, string rhs, string numerality, string units, int gap){
    this->lhs = lhs;
    this->rhs = rhs;
    this->numerality = numerality;
    this->units = units;
    this->gap = gap;
}

Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

