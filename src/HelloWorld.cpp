//============================================================================
// Name        : HelloWorld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Class Descriptions
#include "Parser.h"
#include "Event.h"
#include "Monitor.h"
#include "Rule.h"
#include "ProcessAtom.h"
#include "GapAtom.h"
#include "Assignment.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

int main(int argc, char * argv[]) {
	Parser* p = new Parser();


	//string eventStreamCSVFileName = "src/examples/example4/eventstream.csv";
	string eventStreamCSVFileName = argv[2];
	cout << endl;

	vector<Event *> eventVector = p->readEventSequenceFromCSV(eventStreamCSVFileName);

	for(int i = 0; i < eventVector.size(); i++){
		cout << "Event "<<to_string(i)<<endl;
		eventVector[i]->printEvent();
	}
	cout << endl;

	Monitor* m = new Monitor("MyMonitor");
	cout << "Create monitor " << m->monitorName << endl;
	cout << endl;

	cout << "Reading rules" << endl;
	//string ruleTxtFile = "src/examples/example4/rule.txt";
	string ruleTxtFile = argv[1];
	Rule* r = p->readRuleFromTxt(ruleTxtFile);

	m->ruleVector.push_back(r);

	for(int i = 0; i < m->ruleVector.size(); i++){
		m->ruleVector[i]->printRule();
	}

	cout << endl;

	int i = 0;
	string input;

	//Debugging controls
	cout << "------------------------------------------------------------------------" << endl;
	cout << "CONTROLS" << endl;
	cout << "Press a to print saved assignments" << endl;
	cout << "Press m to find matches" << endl;
	cout << "Press n to handle next event" << endl;
	cout << "Press x to remove expired data" << endl;
	cout << "Press e to end" << endl;
	cout << endl;
	cout << "Input: ";
	cin >> input;

	//Monitoring Loop
	while (input != "e"){

		if (input == "e")
			break;

		if (input == "a"){
			m->printAssignments();
		}

		if (input == "m"){
			// Find matches for rule
			std::vector<Rule*>::iterator ruleIter, end;
			for(int i = 0; i < m->ruleVector.size();i++)
				m->findMatches(m->ruleVector[i]);
		}

		//pro
		if (input == "n"){

			if (i == eventVector.size()){
				cout << "End of sequence" << endl;
			}

			eventVector[i]->printEvent();

			if (eventVector[i]->eventType == "process"){
				m->handleProcessEvent(eventVector[i]);
			}
			i++;
		}

		//Remove data whose expiration is before most recent event time (starts at 0)
		if (input == "x")
			//m->removeExpiredData(std::stoi(eventVector[i]->values[1]));

		cout << endl;

		//Debugging controls
		cout << "------------------------------------------------------------------------" << endl;
		cout << "CONTROLS" << endl;
		cout << "Press a to print saved assignments" << endl;
		cout << "Press m to find matches" << endl;
		cout << "Press n to handle next event" << endl;
		cout << "Press x to remove expired data" << endl;
		cout << "Press e to end" << endl;
		cout << endl;
		cout << "Input: ";
		cin >> input;

	}

	cout << "SUMMARY" << endl;

	cout << "assignments:" << endl;
	for (int i = 0; i < m->assignmentVector.size(); i++){
		Assignment * a = m->assignmentVector[i];
		if (a->typeOfAssignment == "body"){
			if (a->missingProcessAtoms.size()==0){
				a->printAssignment();
				cout << endl;

				cout << "matches:" << endl;
				cout << endl;

				for(int j = 0; j < a->matchingAssignments.size(); j++){
					a->matchingAssignments[j]->printAssignment();
					cout << endl;
				}

				if (a->matchingAssignments.size()==0){
					cout << "{}" << endl;
					cout << endl;
				}

			}

		}

	}

	// on "e"

	delete &eventVector;
	delete &m;

}

