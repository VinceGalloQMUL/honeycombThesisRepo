#pragma once
#include <string>
#include <queue>
#include "CVcommon.h"

class csvFile
{
public:
	csvFile();
	static std::vector<std::string> csvFile::getNextLineAndSplitIntoTokens(std::ifstream& str);
	static std::vector<std::string> csvFile::getNextLineAndSplitIntoTokensIgnoreComment(std::ifstream& str);
	static int csvFile::parseLine1iSafely(std::string   str, int defVal);
	static int csvFile::parseLine1i(std::string   str);
	static float csvFile::parseLine1fSafely(std::string   str, float defVal);
	static float csvFile::parseLine1f(std::string   str);
	static vector3f *csvFile::parseLine3f(std::vector<std::string>   lineArray);
	static vector2f *csvFile::parseLine2f(std::vector<std::string>   lineArray);
	static pixelLoc *csvFile::parseLine2i(std::vector<std::string>   lineArray);

};

