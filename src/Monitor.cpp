/*
 * Monitor.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "Monitor.h"
#include "Event.h"
#include "Rule.h"
#include "Assignment.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;


Monitor::Monitor(std::string monitorName) {

	this->monitorName = monitorName;
	this->currentTime = 0;

}

Monitor::~Monitor() {
	// TODO Auto-generated destructor stub
}

void Monitor::handleProcessEvent(Event* e) {

	cout << "Handle process event: ";
	e->printEvent();
	cout << endl;

	std::vector<Rule*>::iterator ruleIterator, end;
	for(ruleIterator = this->ruleVector.begin(), end = this->ruleVector.end() ; ruleIterator != end; ++ruleIterator) {

		cout << "Check if event creates new body assignment for Rule "<<(*ruleIterator)->ruleName <<endl;

		//Iterate through body process atoms
		std::vector<ProcessAtom*>::iterator bodyProcessAtomIterator, end;
		for(bodyProcessAtomIterator = (*ruleIterator)->bodyProcessAtoms.begin(),
			  end = (*ruleIterator)->bodyProcessAtoms.end();
			  bodyProcessAtomIterator != end; bodyProcessAtomIterator++){

			if (e->eventName == (*bodyProcessAtomIterator)->processName){

				cout << "Create new body assignment for process atom: ";
				(*bodyProcessAtomIterator)->printProcessAtom();
				cout << endl;

			  std::map <string, bool> variablesDefinedFlags;
			  for(int i = 0; i < (*ruleIterator)->bodyVariables.size(); i++){
				  variablesDefinedFlags[(*ruleIterator)->bodyVariables[i]] = false;
			  }

			  std::map <string, string> values;
			  for(int i = 0; i < (*bodyProcessAtomIterator)->attributes.size(); i++){

				  cout << "Assign " << e->values[i] << " to "<< (*bodyProcessAtomIterator)->variables[i] << endl;

				  //get variable associated with position in process schema
				  values[(*bodyProcessAtomIterator)->variables[i]] = e->values[i];
				  variablesDefinedFlags[(*bodyProcessAtomIterator)->variables[i]] = true;

			  }

			  std::vector<ProcessAtom*> seenProcessAtoms;
			  seenProcessAtoms.push_back((*bodyProcessAtomIterator));

			  std::vector<ProcessAtom*> missingProcessAtoms = (*ruleIterator)->bodyProcessAtoms;

			  missingProcessAtoms.erase(std::remove(missingProcessAtoms.begin(), missingProcessAtoms.end(), (*bodyProcessAtomIterator)), missingProcessAtoms.end());

			  vector<Assignment*> matchingAssignments;

			  Assignment* a = new Assignment((*ruleIterator)->bodyVariables,values,
					  variablesDefinedFlags, "body", *ruleIterator, missingProcessAtoms, seenProcessAtoms,
					  matchingAssignments);

			  cout << "Newly created assignment:" << endl;
			  a->printAssignment();
			  cout << endl;

		  	  cout << "Saving new assignment to monitor" <<endl;
		  	  this->assignmentVector.push_back(a);
		  }
	  }

	  cout << endl;

	  cout << "Checking if event creates new head assignment" <<endl;

	  std::vector<ProcessAtom*>::iterator headProcessAtomIterator;
	  for(headProcessAtomIterator = (*ruleIterator)->headProcessAtoms.begin(),
			  end = (*ruleIterator)->headProcessAtoms.end();
			  headProcessAtomIterator != end; headProcessAtomIterator++){

		  if (e->eventName == (*headProcessAtomIterator)->processName){

			  std::map <string, bool> variablesDefinedFlags;
			  for(int i = 0; i < (*ruleIterator)->headVariables.size(); i++){
				  variablesDefinedFlags[(*ruleIterator)->headVariables[i]] = false;
			  }

			  // extract assignment from event values
			  std::map <string, string> values;
			  for(int i = 0; i < (*headProcessAtomIterator)->attributes.size(); i++){

				  cout << "Assign " << e->values[i] << " to "<< (*headProcessAtomIterator)->variables[i] << endl;

				  //get variable associated with attribute using process atom
				  values[(*headProcessAtomIterator)->variables[i]] = e->values[i];
				  variablesDefinedFlags[(*headProcessAtomIterator)->variables[i]] = true;

			  }

			  std::vector<ProcessAtom*> seenProcessAtoms;
			  seenProcessAtoms.push_back((*headProcessAtomIterator));

			  std::vector<ProcessAtom*> missingProcessAtoms = (*ruleIterator)->headProcessAtoms;

			  // Removes process name that creates assignment
			  missingProcessAtoms.erase(std::remove(missingProcessAtoms.begin(), missingProcessAtoms.end(), (*headProcessAtomIterator)), missingProcessAtoms.end());

			  std::vector<Assignment*> matchingAssignments;

			  Assignment* a = new Assignment((*ruleIterator)->headVariables, values, variablesDefinedFlags, "head", *ruleIterator, missingProcessAtoms, seenProcessAtoms, matchingAssignments);

			  cout << "Newly created assignment:" << endl;
			  a->printAssignment();
			  cout << endl;

		  	  cout << "Saving new assignment to monitor" <<endl;
		  	  this->assignmentVector.push_back(a);
		  }
	  }

	  cout << endl;

	  //Search for assignments to extend with this event
	  cout << "Search for assignments to extend with this event" << endl;
	  cout << endl;

	  for(int i = 0; i < this->assignmentVector.size(); i++){
		  Assignment* a = this->assignmentVector[i];

		  vector<string> aDefinedVariables;

		  for(int i = 0; i < a->variables.size(); i++){
			  if (a->variablesDefinedFlags[a->variables[i]]){
				  aDefinedVariables.push_back(a->variables[i]);
			  }
		  }

		  for(int j = 0; j < a->missingProcessAtoms.size(); j++){
			  if (e->eventName == a->missingProcessAtoms[j]->processName){
				  ProcessAtom* missingProcessAtom = a->missingProcessAtoms[j];

				  cout << "Try to extend: " << endl;
				  a->printAssignment();
				  cout << endl;

				  // extract assignment from event values
				  std::map <string, string> eventValuesForVariables;

				  for(int i = 0; i < missingProcessAtom->variables.size(); i++){
					  //get variable associated with attribute using process atom
					  eventValuesForVariables[missingProcessAtom->variables[i]] = e->values[i];
				  }

				  bool match = true;

				  cout << "Check agreement of event values with existing assignment" << endl;
				  for(int i = 0; i < missingProcessAtom->variables.size(); i++){
					  if (a->variablesDefinedFlags[missingProcessAtom->variables[i]]){
						  if (a->values[missingProcessAtom->variables[i]] != eventValuesForVariables[missingProcessAtom->variables[i]]){
							  cout << "Disagreement with variable " << missingProcessAtom->variables[i] << endl;
							  cout << endl;
							  match = false;
							  break;
						  }
					  }
				  }

				  if (!match){
					  break;
				  }
				  cout << "Event values agree with existing assignment" << endl;

				  //Check gap atoms
				  vector<GapAtom*> constraint;
				  if (a->typeOfAssignment=="body"){
					  constraint = a->rulePointer->bodyGapAtoms;
				  } else {
					  constraint = a->rulePointer->headGapAtoms;
				  }

				  std::map<string, string> mergedMap(a->values);
				  std::map<string, string>::iterator eventMapIterator = eventValuesForVariables.begin();

					// TO DO
				  while(eventMapIterator != eventValuesForVariables.end()){
					  if (not a->variablesDefinedFlags[eventMapIterator->first]){
						  aDefinedVariables.push_back(eventMapIterator->first);
						  cout << "Adding value for " << eventMapIterator->first << " to assignment" << endl;
					  }
					  mergedMap[eventMapIterator->first] = eventMapIterator->second;
					  eventMapIterator++;
				  }

				  cout << "New mapping from variables: ";
				  vector<string>::iterator definedVariableIterator = aDefinedVariables.begin();
				  while(definedVariableIterator != aDefinedVariables.end()){
					  cout << *definedVariableIterator << ": " << mergedMap[*definedVariableIterator] << ", ";
					  definedVariableIterator++;
				  }
				  cout << endl;

				  cout << "Check if new assignment is consistent with " << a->typeOfAssignment << " gap atoms" << endl;
				  bool consistentWithGapAtoms = true;
				  for(int i = 0; i < constraint.size(); i++){
					  string lhs = constraint[i]->lhs;
					  string rhs = constraint[i]->rhs;
					  bool lhsDefined, rhsDefined = false;
					  std::map<string, string>::iterator mergedMapIterator = mergedMap.begin();
					  while(mergedMapIterator != mergedMap.end()){
					  	  if (mergedMapIterator->first == lhs) lhsDefined = true;
					  	  if (mergedMapIterator->first == rhs) rhsDefined = true;
					  	  mergedMapIterator++;
					  }
					  if(lhsDefined and rhsDefined){
					  	  cout << "Check gap atom ";
					  	  constraint[i]->printGapAtom();
					  	  cout << endl;
					  	  if (not constraint[i]->checkTruthValueWithAssignment(mergedMap)) {
					  		  consistentWithGapAtoms = false;
					  	  }
					  }
				  }

				  if (consistentWithGapAtoms){
					  cout << "Create a new assignment" << endl;

					  Assignment * b = new Assignment(*a);

					  for(auto elem : mergedMap)
					  	{
					  	   b->values[elem.first] = elem.second;
					  	   b->variablesDefinedFlags[elem.first] = true;
					  	}

					  b->computeExpirationTime();

					  b->seenProcessAtoms.push_back(missingProcessAtom);

					  b->missingProcessAtoms.erase(std::remove(b->missingProcessAtoms.begin(), b->missingProcessAtoms.end(), missingProcessAtom), b->missingProcessAtoms.end());

					  if(b->missingProcessAtoms.size()==0) b->complete = true;

					  b->expirationTime = b->computeExpirationTime();

					  this->assignmentVector.push_back(b);

				  }

			  }
		  }
	  }

  }

  cout << "End of handling event" << endl;

  return;
}

void Monitor::findMatches(Rule *r){

	cout << "Looking for matches for rule " << r->ruleName << endl;

	std::vector<Assignment*>::iterator iter1;
	std::vector<Assignment*>::iterator iter2;

	// iterate over complete body assignments
	for (iter1 = this->assignmentVector.begin(); iter1 != this->assignmentVector.end(); ) {

		if ((*iter1)->rulePointer == r and (*iter1)->typeOfAssignment == "body" and not (*iter1)->complete) {

			cout << "Look for a match for: " << endl;
			(*iter1)->printAssignment();
			cout << endl;

			cout << "Iterate over complete head assignments" << endl;
			cout << endl;

			// iterate over head assignments
			for (iter2 = this->assignmentVector.begin(); iter2 != this->assignmentVector.end(); iter2++) {

				if ((*iter2)->rulePointer == r and (*iter2)->typeOfAssignment == "head"
						and (*iter1)->complete and (*iter2)->complete) {

					cout << "Check with assignment: " << endl;
					(*iter2)->printAssignment();
					cout << endl;

					//check for match between assignment iter1 and assignment iter2
					if (this->isHeadAssignmentMatchforBodyAssignment(*iter1, *iter2)){
						cout << "Found a match" << endl;
						(*iter1)->complete = true;
						(*iter1)->matchingAssignments.push_back(*iter2);
					}
				}

			}
			cout << endl;
		}
		iter1++;
	}
	return;

}

bool Monitor::expired(Assignment* a){
	return a->expirationTime < this->currentTime;
}

void Monitor::removeExpiredData(int latestEventTime){

	cout << "Removing data that expires before latest event" << std::to_string(latestEventTime) << endl;

	std::vector<Assignment *> unexpiredData;
	std::vector<Assignment *> expiredData;

	std::vector<Assignment *>::iterator assignmentIterator = this->assignmentVector.begin();
	while(assignmentIterator != this->assignmentVector.end()) {
		if ((*assignmentIterator)->expirationTime < latestEventTime){
			unexpiredData.push_back(*assignmentIterator);
			//assignmentIterator = this->assignmentVector.erase(*assignmentIterator);
			}
		else {
			expiredData.push_back(*assignmentIterator);
		}
		assignmentIterator++;
	}

	this->assignmentVector = unexpiredData;

	return;
}

void Monitor::printAssignments(){
	std::vector<Assignment*>::iterator assignmentIterator;
	for(int i = 0; i < this->assignmentVector.size(); i++){
		cout << "assignmentVector["<<to_string(i)<<"]: ";
		assignmentVector[i]->printAssignment();
		cout << endl;
	}
		cout << endl;
	return;
}

bool Monitor::isHeadAssignmentMatchforBodyAssignment(Assignment * bodyA, Assignment * headA){


	for(int i = 0; i < bodyA->variables.size(); i++){
		if (bodyA->variablesDefinedFlags[bodyA->variables[i]]){
			if (headA->variablesDefinedFlags[bodyA->variables[i]]){
				cout << "Check agreement with variable " << bodyA->variables[i] << endl;
				if (bodyA->values[bodyA->variables[i]] != headA->values[bodyA->variables[i]]){
					cout << "Found disagreement with variable " << bodyA->variables[i] << endl;
					cout << endl;
					return false;
				}
			}
		}
	}

	std::map<string, string> mergedValues(bodyA->values);

	mergedValues.insert((headA->values).begin(), (headA->values).end());

	cout << "merged values: ";
	for(auto elem : mergedValues)
	{
	   std::cout << elem.first << "->" << elem.second << ", ";
	}
	cout << endl;


	for(int i = 0; i < bodyA->rulePointer->headGapAtoms.size(); i++){
		cout << "Gap atom ";
		bodyA->rulePointer->headGapAtoms[i]->printGapAtom();
		if (not bodyA->rulePointer->headGapAtoms[i]->checkTruthValueWithAssignment(mergedValues)) {
			cout << " doesn't hold" << endl;
			return false;
		}
		cout << " holds" << endl;
	}

	return true;
}

std::vector<std::string> Monitor::intersection(std::vector<std::string> v1,
                                      std::vector<std::string> v2){
    std::vector<std::string> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}
