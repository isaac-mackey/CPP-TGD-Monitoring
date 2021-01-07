/*
 * Parser.cpp
 *
 *  Created on: Oct 14, 2020
 *      Author: isaacmackey, help from Michael Zhang (April 2020 - August 2020)
 */

#include "Parser.h"
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


Parser::Parser() {
	// TODO Auto-generated constructor stub

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

vector<Event*> Parser::readEventSequenceFromCSV(string eventStreamCSVFileName){
	vector<Event*> eventSequence;
	std::ifstream data(eventStreamCSVFileName);
	std::string line, eventType, eventName;
	vector<string> eventStrings;
	while(std::getline(data,line)) {
		std::stringstream  lineStream(line);
		std::string        cell;
		while(std::getline(lineStream,cell,',')) {
			eventStrings.push_back(cell);
		}
		eventType = eventStrings[0];
		eventName = eventStrings[1];
		eventStrings.erase(eventStrings.begin());
		eventStrings.erase(eventStrings.begin());
		eventSequence.push_back(new Event(eventType, eventName, eventStrings));
		eventStrings.clear();
	}
	return eventSequence;
}

Rule* Parser::readRuleFromTxt(string ruleTxtFile){
	std::ifstream data(ruleTxtFile);
	std::string line;
	std::getline(data, line); // Consume "Rule"
	std::getline(data, line); // Get ruleName
	//cout << "Rule: " << line << endl;
	string ruleName = line;
	std::getline(data, line); // Consume "if"
	//cout << line << endl;
	vector<string> bodyProcessAtomStrings;
	vector<string> bodyGapAtomStrings;
	vector<string> headProcessAtomStrings;
	vector<string> headGapAtomStrings;
	while(std::getline(data, line)){
		if (line == "constraints") break;
		//cout << line << endl;
		bodyProcessAtomStrings.push_back(line);
	}
	while(std::getline(data, line)){
		//cout << line << endl;
		if (line ==  "then") break;
		bodyGapAtomStrings.push_back(line);
	}
	while(std::getline(data, line)){
		if (line ==  "constraints") break;
		//cout << line << endl;
		headProcessAtomStrings.push_back(line);
	}
	while(std::getline(data, line)){
		if (line ==  "end") break;
		//cout << line << endl;
		headGapAtomStrings.push_back(line);
	}

	//cout << "Reading body process atoms" << endl;
	vector<ProcessAtom*> bodyProcessAtoms;
	for(int i = 0; i < bodyProcessAtomStrings.size(); i++){
		//cout << bodyProcessAtomStrings[i] << endl;
		std::stringstream  lineStream(bodyProcessAtomStrings[i]);
		std::string        cell;
		std::getline(lineStream,cell,'(');	//get process name
		string processName = cell;
		vector<string> attributes, variables;
		std::getline(lineStream,cell,')');

		std::string delimiter = ",";
		size_t pos = 0;
		std::string token;
		while ((pos = cell.find(delimiter)) != std::string::npos) {
		    token = cell.substr(0, pos);
		    attributes.push_back(token.substr(0, token.find(" ")));  // get attribute
		    variables.push_back(token.substr(token.find(" ")+1,100));	// get variable
		    cell.erase(0, pos + delimiter.length()+1);
		}
		attributes.push_back(cell.substr(0, cell.find(" "))); // get last attribute
		variables.push_back(cell.substr(cell.find(" ")+1,100)); // get last variable

		std::getline(lineStream, cell);
		attributes.push_back("time");
		variables.push_back(cell.substr(1,100)); //get time variable

		ProcessAtom *x = new ProcessAtom(processName, attributes, variables);
		bodyProcessAtoms.push_back(x);
	}

	//cout << "Reading head process atoms" << endl;
	vector<ProcessAtom*> headProcessAtoms;
	for(int i = 0; i < headProcessAtomStrings.size(); i++){
			std::stringstream  lineStream(headProcessAtomStrings[i]);
			std::string        cell;
			std::getline(lineStream,cell,'(');	//get process name
			string processName = cell;
			vector<string> attributes, variables;
			std::getline(lineStream,cell,')');

			std::string delimiter = ",";
			size_t pos = 0;
			std::string token;
			while ((pos = cell.find(delimiter)) != std::string::npos) {
			    token = cell.substr(0, pos);
			    attributes.push_back(token.substr(0, token.find(" ")));  // get attribute
			    variables.push_back(token.substr(token.find(" ")+1,100));	// get variable
			    cell.erase(0, pos + delimiter.length()+1);
			}
			attributes.push_back(cell.substr(0, cell.find(" "))); // get last attribute
			variables.push_back(cell.substr(cell.find(" ")+1,100)); // get last variable

			std::getline(lineStream, cell);
			attributes.push_back("time");
			variables.push_back(cell.substr(1,100)); //get time variable

			ProcessAtom *x = new ProcessAtom(processName, attributes, variables);
			headProcessAtoms.push_back(x);
		}

	//cout << "Reading body gap atoms" << endl;
	vector<GapAtom*> bodyGapAtoms;
	for(int i = 0; i < bodyGapAtomStrings.size(); i++){
		//cout << bodyGapAtomStrings[i] << endl;
		string s = bodyGapAtomStrings[i];
		string lhs,rhs,direction, gap;
		gap = "0";

		int offset = 1;

		if (s.find("<=")!= string::npos){
			direction = "<=";
			offset++;
		} else if (s.find(">=") != string::npos){
			direction = ">=";
			offset++;
		} else if (s.find(">") != string::npos){
			direction = ">";
		} else if (s.find("<") != string::npos){
			direction = "<";
		} else if (s.find("=") != string::npos){
			direction = "=";
		} else {
			cout << "operator not recognized" << endl;
		}

		rhs = s.substr(s.find(direction)+offset);

		if(s.find("+")!= string::npos){
			lhs = s.substr(0,s.find("+"));
			gap = s.substr(s.find("+")+1,s.find(direction)-s.find("+")-1);
		} else {
			lhs = s.substr(0,s.find(direction));
		}

		lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
		rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());
		gap.erase(std::remove(gap.begin(), gap.end(), ' '), gap.end());

		GapAtom * x= new GapAtom(lhs, rhs, direction, "days", std::stoi(gap));
		bodyGapAtoms.push_back(x);
	}

	//cout << "Reading head gap atoms" << endl;
	vector<GapAtom*> headGapAtoms;
	for(int i = 0; i < headGapAtomStrings.size(); i++){
			//cout << headGapAtomStrings[i] << endl;
			string s = headGapAtomStrings[i];
			string lhs,rhs,direction, gap;
			gap = "0";

			int offset = 1;

			if (s.find("<=")!= string::npos){
				direction = "<=";
				offset++;
			} else if (s.find(">=") != string::npos){
				direction = ">=";
				offset++;
			} else if (s.find(">") != string::npos){
				direction = ">";
			} else if (s.find("<") != string::npos){
				direction = "<";
			} else if (s.find("=") != string::npos){
				direction = "=";
			} else {
				cout << "operator not recognized" << endl;
			}

			rhs = s.substr(s.find(direction)+offset);

			if(s.find("+")!= string::npos){
				lhs = s.substr(0,s.find("+"));
				gap = s.substr(s.find("+")+1,s.find(direction)-s.find("+")-1);
			} else {
				lhs = s.substr(0,s.find(direction));
			}

			lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
			rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());
			gap.erase(std::remove(gap.begin(), gap.end(), ' '), gap.end());

			GapAtom * x= new GapAtom(lhs, rhs, direction, "days", std::stoi(gap));
			headGapAtoms.push_back(x);
	}

	return new Rule(ruleName, bodyProcessAtoms, bodyGapAtoms, headProcessAtoms, headGapAtoms);
}



