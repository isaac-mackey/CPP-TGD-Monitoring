/*
 * Parser.h
 *
 *  Created on: Oct 14, 2020
 *      Author: isaacmackey
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Event.h"
#include "Rule.h"

class Parser {
public:
	Parser();
	virtual ~Parser();

	vector<Event*> readEventSequenceFromCSV(string eventStreamCSVFileName);
	Rule* readRuleFromTxt(string ruleTxtFile);
};

#endif /* PARSER_H_ */
