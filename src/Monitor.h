/*
 * Monitor.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include "Event.h"
#include "Rule.h"
#include "Assignment.h"

#include <string>

class Monitor {
public:
	std::string monitorName;
	std::vector<Assignment*> assignmentVector;
	std::vector<Rule*> ruleVector;
	int currentTime;

	Monitor(std::string monitorName);
	virtual ~Monitor();

	void handleProcessEvent(Event* e);
	void findMatches(Rule* r);
	void removeExpiredData(int latestEventTime);
	bool expired(Assignment * a);
	void printAssignments();
	bool isHeadAssignmentMatchforBodyAssignment(Assignment * bodyA, Assignment * headA);
	std::vector<std::string> intersection(std::vector<std::string> v1, std::vector<std::string> v2);

};

#endif /* MONITOR_H_ */
