/*
 * Event.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "Event.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Event::Event(std::string eventType, std::string eventName, vector<std::string> values) {
	// TODO Auto-generated constructor stub

	this->eventType = eventType;
	this->eventName = eventName;
	this->values = values;

}

Event::~Event() {
	// TODO Auto-generated destructor stub
}

void Event::printEvent(){

//	cout << "eventType: " << eventType << endl;
	cout << "eventName: " << this->eventName << endl;

	for (int i = 0;  i != this->values.size(); i++) {
		cout << "values["<<to_string(i)<<"]: " << values[i] << endl;
	}
	cout << endl;

}

