/*
 * ProcessAtom.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "ProcessAtom.h"

#include<vector>
#include<string>
#include<iostream>

using namespace std;

ProcessAtom::ProcessAtom(std::string processName, std::vector<std::string> attributes, std::vector<std::string> variables){
	// TODO Auto-generated constructor stub

	this->processName = processName;
	this->attributes = attributes;
	this->variables = variables;

}

ProcessAtom::~ProcessAtom() {
	// TODO Auto-generated destructor stub
}

void ProcessAtom::printProcessAtom(){
	string result;
	result += this->processName;
	result += "(";
	for(int i = 0; i < this->attributes.size()-1; i++){
		result += attributes[i] + " " + variables[i] + ", ";
	}
	result = result.substr(0, result.size()-2);
	result += ")";
	result += "@" + variables[attributes.size()-1];
	cout << result;
	return;
}

