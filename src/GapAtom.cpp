/*
 * GapAtom.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: isaacmackey
 */

#include "GapAtom.h"

#include <string>
#include<iostream>

using namespace std;

GapAtom::GapAtom(std::string lhs, std::string rhs, std::string direction, std::string units, int gap){
    this->lhs = lhs;
    this->rhs = rhs;
    this->direction = direction;	// possible directions: <, <=, =, >=, >
    this->units = units;
    this->gap = gap;

}


GapAtom::~GapAtom() {
	// TODO Auto-generated destructor stub
}

void GapAtom::printGapAtom(){
	cout << this->lhs <<" + "<< std::to_string(this->gap)<<" "<< this->direction<<" "<< this->rhs;
	return;
}

bool GapAtom::checkTruthValueWithAssignment(std::map<string, string> assignment){

	if (this->direction == "<") {
		return (std::stoi(assignment[this->lhs])+this->gap < std::stoi(assignment[this->rhs]));
	} else if (this->direction == "<=") {
		return (std::stoi(assignment[this->lhs])+this->gap <= std::stoi(assignment[this->rhs]));
	} else if (this->direction ==  "=") {
		return (std::stoi(assignment[this->lhs])+this->gap == std::stoi(assignment[this->rhs]));
	} else if (this->direction ==  ">=") {
		return (std::stoi(assignment[this->lhs])+this->gap >= std::stoi(assignment[this->rhs]));
	} else if (this->direction ==  ">") {
		return (std::stoi(assignment[this->lhs])+this->gap > std::stoi(assignment[this->rhs]));
	} else {
		 cout << "ERROR: gap atom direction operator not recognized";
	}

	return false;
}

