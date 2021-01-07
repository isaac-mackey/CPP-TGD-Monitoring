/*
 * Event.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include <string>

class Event {
public:
	std::string eventType;
	std::string eventName;
	std::vector<std::string> values;

	Event(std::string eventType, std::string eventName, std::vector<std::string>);
	virtual ~Event();
	void printEvent();
};

#endif /* EVENT_H_ */
