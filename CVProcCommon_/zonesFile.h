#pragma once
#include <list>

#include <umZone.h>
#include "zone3d.h"
#include <csvFile.h>

class zonesFile : csvFile
{
public:

	umZone *parseLineZone(std::vector<std::string>   lineArray);
	bool parseLinePerspective(std::vector<std::string>   lineArray, bool *usePerspective, int *pTLX, int *pTLY, int *pTRX, int *pTRY, int *pBLX, int *pBLY, int *pBRX, int *pBRY);
	int zonesFile::read(char *filepathname, std::vector<umZone *> *pZones, bool *usePerspective, int *pTLX, int *pTLY, int *pTRX, int *pTRY, int *pBLX, int *pBLY, int *pBRX, int *pBRY);
	int zonesFile::read(char *filepathname, std::vector<zone3dCompound *> *pZones, vector<string> *userDefinedZoneTypes);
	void write(char *filepathname, std::vector<umZone *> *pZones, bool usePerspective, int TLX, int TLY, int TRX, int TRY, int BLX, int BLY, int BRX, int BRY);
	void zonesFile::write(char *filepathname, std::vector<zone3dCompound *> *pZones, vector<string> *userDefinedZoneTypes);

	zonesFile();
	~zonesFile();
};

