#pragma once
#include <csvFile.h>
#include "perspective.h"
#include "geometryDesc.h"

#define CUSTOM_FLAG "CustomData"

public class cellScaleImageReference {
public:
	int arraySize = 3;
	int cellIdx;
	int cellWallIdx;
	Point2f sampleCells0[3];
	Point2f sampleCells1[3];
	Point2f sampleCellWalls0[3];
	Point2f sampleCellWalls1[3];
	cellScaleImageReference();
	void clone(cellScaleImageReference& clonee);
	std::string serialise();
	std::string serialiseAreaCaptions();
	std::string serialiseCalcs(float scale);
	void deserialise(std::vector<std::string> &valStrArray);
	void cellScaleImageReference::setCell(Point2f p0, Point2f p1);
	void cellScaleImageReference::setCellWall(Point2f p0, Point2f p1);
	void increment(bool wall);
	int index(bool wall);
};

class csvCompareConfig : csvFile
{
public:
	string hive;
	float scaleX;
	float scaleY;
	float offsetX;
	float offsetY;
	float rotation;
	float zoom;
	Point zoomCentre;
	Point2f scalePt0;
	Point2f scalePt1;
	float scaleDistanceMM;
	perspectiveImageReference staticRefs;
	perspectiveImageReference mobileRefs;
	cellScaleImageReference cellScaleRefs;

	void read(const char *filepathname1, const char *filepathname2, const char *filepathnameJoint, vector<geometryDesc*>* geoDescs, string &customProperties);
	void write(const char *filepathname1, const char *filepathname2, const char *filepathnameJoint, const char *experientID, std::vector<geometryDesc*>* geoDescs);
	float getScale();

	csvCompareConfig();
};

