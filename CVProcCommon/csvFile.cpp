#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;

#include "CVcommon.h"
#include "csvFile.h"

using namespace System;

csvFile::csvFile() {
}

std::vector<std::string> csvFile::getNextLineAndSplitIntoTokens(std::ifstream& str)
{
	std::vector<std::string>   result;
	std::string                line;
	std::getline(str, line);

	std::stringstream          lineStream(line);
	std::string                cell;

	while (std::getline(lineStream, cell, ','))
	{
		result.push_back(cell);
	}
	// This checks for a trailing comma with no data after it.
	if (!lineStream && cell.empty())
	{
		// If there was a trailing comma then add an empty element.
		result.push_back("");
	}
	return result;
}
std::vector<std::string> csvFile::getNextLineAndSplitIntoTokensIgnoreComment(std::ifstream& str)
{
	bool aComment = false;
	std::vector<std::string>   result;
	do {
		aComment = false;
		result = getNextLineAndSplitIntoTokens(str);
		if (result.size() > 0) {
			if (result[0].substr(0, 1) == "#") aComment = true;
		}
	} while (aComment);
	return result;
}
int csvFile::parseLine1iSafely(std::string   str, int defVal) {
	int retv = defVal;
	try {
		retv = std::stoi(str);
	}
	catch (...) {

	}
	return retv;
}
int csvFile::parseLine1i(std::string   str) {
	int retv = parseLine1iSafely(str, 0);
	return retv;
}
float csvFile::parseLine1fSafely(std::string   str, float defVal) {
	float retv = defVal;
	try {
		retv = std::stof(str);
	}
	catch (...) {

	}
	return retv;

}
float csvFile::parseLine1f(std::string   str) {
	float retv = parseLine1fSafely(str, 0);
	return retv;
}
vector3f *csvFile::parseLine3f(std::vector<std::string>   lineArray) {
	vector3f *retv = NULL;
	if (lineArray.size() == 3) {
		retv = new vector3f();
		retv->dx = parseLine1fSafely(lineArray[0], 0);
		retv->dy = parseLine1fSafely(lineArray[1], 0);
		retv->dz = parseLine1fSafely(lineArray[2], 0);
	}
	return retv;
}
vector2f *csvFile::parseLine2f(std::vector<std::string>   lineArray) {
	vector2f *retv = NULL;
	if (lineArray.size() == 2) {
		retv = new vector2f();
		retv->dx = parseLine1fSafely(lineArray[0], 0);
		retv->dy = parseLine1fSafely(lineArray[1], 0);
	}
	return retv;
}
pixelLoc *csvFile::parseLine2i(std::vector<std::string>   lineArray) {
	pixelLoc *retv = NULL;
	if (lineArray.size() == 2) {
		retv = new pixelLoc();
		retv->x = parseLine1iSafely(lineArray[0], 0);
		retv->y = parseLine1iSafely(lineArray[1], 0);
	}
	return retv;

}

