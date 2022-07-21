#pragma once
#include <list>

#include <beeTagLoc.h>
#include <csvFile.h>

class beeTagLocFile : csvFile
{
public:

	beeTagLoc *parseLine(std::vector<std::string>   lineArray);
	int beeTagLocFile::read(char *filepathname, std::vector<beeTagLoc *> *pTags);
	static void write(char *filepathname, std::vector<beeTagLoc *> *pTags);
	static void append(char *filepathname, std::vector<beeTagLoc *> *pTags);

	beeTagLocFile();
	~beeTagLocFile();
};

