/*
 * ProcessAtom.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef PROCESSATOM_H_
#define PROCESSATOM_H_

#include <string>
#include <vector>

using namespace std;

class ProcessAtom {
public:
	ProcessAtom(std::string processName, std::vector<std::string> attributes, std::vector<std::string> variables);

	std::string processName;
	std::vector<std::string> attributes;
	std::vector<std::string> variables;
	virtual ~ProcessAtom();
	void printProcessAtom();
};

#endif /* PROCESSATOM_H_ */
