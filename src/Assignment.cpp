/*
 * Assignment.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "Assignment.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

Assignment::Assignment(vector<string> variables, std::map<string, string> values,
		std::map<string, bool> variablesDefinedFlags, string typeOfAssignment,
		Rule* rulePointer, vector<ProcessAtom*> missingProcessAtoms, vector<ProcessAtom*> seenProcessAtoms,
		vector<Assignment*> matchingAssignments) {

	//class fields
	this->variables = variables;
	this->variablesDefinedFlags = variablesDefinedFlags;
	this->values = values;

	this->rulePointer = rulePointer;
	this->typeOfAssignment = typeOfAssignment;

	this->missingProcessAtoms = missingProcessAtoms;
	this->seenProcessAtoms = seenProcessAtoms;
	this->matchingAssignments= matchingAssignments;

	this->complete = missingProcessAtoms.size()==0;
	this->matched = matchingAssignments.size()!=0;

	this->expirationTime = this->computeExpirationTime();


}

Assignment::~Assignment() {
	// TODO Auto-generated destructor stub
}

int Assignment::computeExpirationTime(){

	//collect relevant gap atoms for computing the bound
	vector<GapAtom*> constraint;

	if (not this->complete and this->typeOfAssignment=="body"){
		cout << "compute expiration time" << endl;
		constraint = this->rulePointer->bodyGapAtoms;
	} else if (not this->complete and this->typeOfAssignment=="head"){
		cout << "compute expiration time" << endl;
		constraint = this->rulePointer->headGapAtoms;
	} else if (this->complete and this->typeOfAssignment=="head"){
		cout << "compute expiration time" << endl;
		constraint.insert(constraint.end(), this->rulePointer->bodyGapAtoms.begin(), this->rulePointer->bodyGapAtoms.end());
		constraint.insert(constraint.end(), this->rulePointer->headGapAtoms.begin(), this->rulePointer->headGapAtoms.end());
	}
	else { // complete body assignment
		cout << "compute violation time" << endl;
		constraint.insert(constraint.end(), this->rulePointer->bodyGapAtoms.begin(), this->rulePointer->bodyGapAtoms.end());
		constraint.insert(constraint.end(), this->rulePointer->headGapAtoms.begin(), this->rulePointer->headGapAtoms.end());
	}

	//collect all time variables from relevant gap atoms
	vector<string> tVars;
	for(int i = 0; i < constraint.size(); i++){
		GapAtom* c = constraint[i];
		if (std::find(tVars.begin(), tVars.end(), c->lhs) == tVars.end())
			tVars.push_back(c->lhs); // lhs is not in timeVariables, add it
		if (std::find(tVars.begin(), tVars.end(), c->rhs) == tVars.end())
			tVars.push_back(c->rhs); // rhs is not in timeVariables, add it
	}

	map<string, int> lowest;
	map<string, int> highest;

	int currentTime = 0;

	//initialize higher and lower bounds for all variables in constraint
	for(int i = 0; i < tVars.size(); i++){
		string var = tVars[i];
		cout << "var: " << var;
		if (std::find(this->variables.begin(), this->variables.end(), var) != this->variables.end()){
			if (this->variablesDefinedFlags[var]){
				cout << "defined" << endl;
				lowest[var] = stoi(values[var]);
				highest[var] = stoi(values[var]);

				//highest value for any variable will be currentTime?
				currentTime = max(stoi(values[var]), currentTime);
			} else {
				cout << "undefined" << endl;
				lowest[var] = 0;

				//Replace with MAX int and ensure no overlap with non-time variables
				highest[var] = 1000;
			}
		} else {
			cout << "undefined" << endl;
			lowest[var] = 0;
			highest[var] = 1000;
		}
	}

	vector<string> missingVariables;

	for(int i = 0; i < tVars.size(); i++){
		if (not variablesDefinedFlags[tVars[i]]){
			missingVariables.push_back(tVars[i]);
			lowest[tVars[i]] = currentTime;

		}
	}

	cout << "2 Time variables: " << endl;
	for(int i = 0; i < tVars.size(); i++){
		cout << "var: " << tVars[i] << endl;
		cout << "highest: " << to_string(highest[tVars[i]]) << endl;
		cout << "lowest: " << to_string(lowest[tVars[i]]) << endl;
	}

	//set to MAXINT
	int earliestExpirationTime = 1000;

	for(int i = 0; i < constraint.size(); i++){
		GapAtom *a = constraint[i];

		int offset = 0;

		if (a->direction=="<=" or a->direction=="<"){

			if(a->direction=="<") offset++;

			if (lowest.count(a->lhs) and not lowest.count(a->rhs))
				lowest[a->rhs] = lowest[a->lhs]+a->gap+offset;
			if (lowest.count(a->lhs) and lowest.count(a->rhs))
				lowest[a->rhs] = max(lowest[a->rhs], lowest[a->lhs]+a->gap+offset);
			if (highest.count(a->rhs) and not highest.count(a->lhs))
				highest[a->lhs] = highest[a->rhs]-a->gap-offset;
			if (highest.count(a->rhs) and highest.count(a->lhs))
				highest[a->lhs] = min(highest[a->lhs], highest[a->rhs]-a->gap-offset);
		}

		if (a->direction==">=" or a->direction==">"){

			if(a->direction==">") offset++;

			if (highest.count(a->lhs) and not highest.count(a->rhs))
				highest[a->rhs] = stoi(this->values[a->lhs])+a->gap-offset;
			if (highest.count(a->lhs) and highest.count(a->rhs))
				highest[a->rhs] = min(highest[a->rhs], highest[a->lhs]+a->gap-offset);
			if (lowest.count(a->rhs) and not lowest.count(a->lhs))
				lowest[a->lhs] = lowest[a->rhs]-a->gap+offset;
			if (lowest.count(a->rhs) and lowest.count(a->lhs))
				lowest[a->lhs] = max(lowest[a->lhs],lowest[a->rhs]-a->gap+offset);
		}

		if (a->direction=="="){
			if (lowest.count(a->lhs) and not lowest.count(a->rhs))
				lowest[a->rhs] = lowest[a->lhs]+a->gap;
			if (highest.count(a->lhs) and not highest.count(a->rhs))
				 highest[a->rhs] = lowest[a->lhs]+a->gap;
			if (lowest.count(a->rhs) and not lowest.count(a->lhs))
				lowest[a->lhs] = lowest[a->rhs]-a->gap;
			if (highest.count(a->rhs) and not highest.count(a->lhs))
				highest[a->lhs] = lowest[a->rhs]-a->gap;
			if (lowest.count(a->lhs) and highest.count(a->lhs))
				lowest[a->rhs] = lowest[a->lhs]+a->gap; highest[a->rhs] = highest[a->lhs]+a->gap;
			if (lowest.count(a->rhs) and highest.count(a->rhs))
				lowest[a->lhs] = lowest[a->rhs]+a->gap; highest[a->lhs] = highest[a->rhs]+a->gap;
		}

		for(int i = 0; i < tVars.size(); i++){
			if (not variablesDefinedFlags[tVars[i]]){
				if (highest.count(tVars[i])){
					earliestExpirationTime = min(earliestExpirationTime, highest[tVars[i]]);
				}
			}
		}
	}

	cout << "3 Time variables: " << endl;
	for(int i = 0; i < tVars.size(); i++){
		cout << "var: " << tVars[i] << endl;
		cout << "highest: " << to_string(highest[tVars[i]]) << endl;
		cout << "lowest: " << to_string(lowest[tVars[i]]) << endl;
	}

	if(earliestExpirationTime != -1)
		cout << "Computed expiration time: " << std::to_string(earliestExpirationTime) << endl;
	else
		cout << "Computed expiration time: -1" << endl;

	return earliestExpirationTime;
}

void Assignment::printAssignment(){

	cout << "ASSIGNMENT" << endl;

	cout << "type: " << this->typeOfAssignment << ", ";
	cout << this->complete << endl;

	cout << "values: ";
	for (int i = 0;  i != this->variables.size(); i++) {
		if (this->variablesDefinedFlags[this->variables[i]]) {
			cout << this->variables[i] << ": " << this->values[this->variables[i]] << ", ";
		} else {
			cout << this->variables[i] << ": undefined, ";
		}
	}
	cout << endl;

	cout << "expiration time: " << to_string(this->expirationTime) << endl;

	cout << "rule: " << this->rulePointer->ruleName << endl;

	cout << "seenProcessAtoms: ";
	for (int i = 0;  i != this->seenProcessAtoms.size(); i++) {
		this->seenProcessAtoms[i]->printProcessAtom();
		cout << ", ";
	}

	if (this->seenProcessAtoms.size()==0) { cout << "None"; }
	cout << endl;

	cout << "missingProcessAtoms: ";
	for (int i = 0;  i != this->missingProcessAtoms.size(); i++) {
		this->missingProcessAtoms[i]->printProcessAtom();
	}

	if (this->missingProcessAtoms.size()==0) { cout << "None"; }
	cout << endl;

	cout << "matches: ";
	for (int i = 0;  i != this->matchingAssignments.size(); i++) {
		cout << this->matchingAssignments[i] << ", ";
	}

	if (this->matchingAssignments.size()==0) { cout << "None"; }
	cout << endl;

	return;
}
