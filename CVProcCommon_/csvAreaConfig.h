#pragma once
#include <csvFile.h>
#include "perspective.h"
#include "areaDesc.h"

class csvAreaConfig : csvFile
{
public:
	string hive;
	perspectiveImageReference mobileRefs;
	int mScaleDist;
	Point mScaleCorner;
	Point mScaleHorizontal;
	Point mScaleDiagonal;
	void read(const char *fname, vector<areaDesc*>* geoDescs);
	void write(const char *fname, std::vector<areaDesc*>* geoDescs, perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV, int *totals);
	int csvAreaConfig::calcSize(int whichGeo, vector<areaDesc*>* geoDescs, perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV);

	csvAreaConfig();
};

