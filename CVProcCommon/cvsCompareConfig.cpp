#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "csvFile.h"
#include "csvCompareConfig.h"
using namespace System;

csvCompareConfig::csvCompareConfig() {
	scaleDistanceMM = 80;
	hive = "X";
	scaleX = 1;
	scaleY = 1;
	offsetX = 0;
	offsetY = 0;
	rotation = 0;
	zoom = 1;
	zoomCentre = Point(2110, 1390);
}

void csvCompareConfig::read(const char *filepathname1, const char *filepathname2, const char *filepathnameJoint, vector<geometryDesc*>* geoDescs, string &customProperties) {
	std::ifstream str;
	str.open(filepathname2);
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	if (lineArray.size() == 14) {
		int idx = 0;
		// locFile << hive << ", "  << scaleX << ", " << scaleY << "," << offsetX << "," << offsetY << "," << rotation << "," << zoom << "," << zoomCentre. x << "," << zoomCentre.y;
		// locFile << "," << scalePt0.x << "," << scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "\n
		hive = lineArray[idx++];
		scaleX = std::stof(lineArray[idx++]);
		scaleY = std::stof(lineArray[idx++]);
		offsetX = std::stof(lineArray[idx++]);
		offsetY = std::stof(lineArray[idx++]);
		rotation = std::stof(lineArray[idx++]);
		zoom = std::stof(lineArray[idx++]);
		zoomCentre = Point(std::stof(lineArray[idx]), std::stof(lineArray[idx + 1]));
		idx += 2;
		scalePt0 = Point(std::stof(lineArray[idx]), std::stof(lineArray[idx + 1]));
		idx += 2;
		scalePt1 = Point(std::stof(lineArray[idx]), std::stof(lineArray[idx + 1]));
		idx += 2;
		scaleDistanceMM = std::stof(lineArray[idx++]);
	}
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	staticRefs.deserialise(lineArray);
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	cellScaleRefs.deserialise(lineArray);
	str.close();
	str.open(filepathnameJoint);
	geoDescs->clear();
	bool gotAgeo = false;
	do {
		geometryDesc* tmpGeo = new geometryDesc();
		lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		gotAgeo = tmpGeo->deserialise(lineArray);
		if (gotAgeo || (geoDescs->size() == 0)) { // save valid geometryDesc, or if there are none, save an empty one
			geoDescs->push_back(tmpGeo);
		}
	} while (gotAgeo);

	std::string oneLine;
	std::getline(str, oneLine);
	if (oneLine == CUSTOM_FLAG) {
		std::string allCustomLines;
		while (std::getline(str, oneLine))
		{
			if (oneLine == CUSTOM_FLAG) {
				break;
			}
			allCustomLines += oneLine;
		}
		customProperties = allCustomLines;
	}

	str.close();
	str.open(filepathname1);
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str); // skip 1st line
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	mobileRefs.deserialise(lineArray);
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	staticRefs.deserialise(lineArray);
	str.close();
}
float csvCompareConfig::getScale() {
	float scaleMagnitude = magnitude(scalePt0 - scalePt1);
	return scaleMagnitude / scaleDistanceMM;
}
void csvCompareConfig::write(const char *filepathname1, const char *filepathname2, const char *filepathnameJoint, const char *experimentID, vector<geometryDesc*>* geoDescs) {
	ofstream locFile(filepathnameJoint, ios::out);
	float scaleMagnitude = magnitude(scalePt0 - scalePt1);
	float scale = scaleMagnitude / scaleDistanceMM;
	for (int idx = 0; idx < geoDescs->size(); idx++) {
		if (!geoDescs->at(idx)->isEmpty()) {
			locFile << geoDescs->at(idx)->serialise() << "\n";
		}
	}
	locFile << "\nCalculations\n";
	locFile << "Scale: " << scalePt0.x << "," <<  scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "mm, " << scaleMagnitude << "," << scale << "pixels/mm\n";
	locFile << geometryDesc::serialiseAreaCaptions();
	for (int idx = 0; idx < geoDescs->size(); idx++) {
		if (!geoDescs->at(idx)->isEmpty()) {
			locFile << geoDescs->at(idx)->serialiseCalcs(experimentID, scale) << "\n";
		}
	}
	locFile << cellScaleRefs.serialiseAreaCaptions() << "\n";
	locFile << cellScaleRefs.serialiseCalcs(scale) << "\n";
	locFile.close();
	locFile.open(filepathname1, ios::out);
	locFile << hive<< scaleX << ", " << scaleY << "," << offsetX << "," << offsetY << "," << rotation << "," << zoom << "," << zoomCentre. x << "," << zoomCentre.y;
	locFile << "," << scalePt0.x << "," << scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "\n";
	locFile << mobileRefs.serialise() << "\n";
	locFile  << cellScaleRefs.serialise() << "\n";
	locFile.close();
	locFile.open(filepathname2, ios::out);
	locFile << hive << ", " << scaleX << ", " << scaleY << "," << offsetX << "," << offsetY << "," << rotation << "," << zoom << "," << zoomCentre. x << "," << zoomCentre.y;
	locFile << "," << scalePt0.x << "," << scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "\n";
	locFile << staticRefs.serialise() << "\n";
	locFile  << cellScaleRefs.serialise() << "\n";
	locFile.close();

}

cellScaleImageReference::cellScaleImageReference() {
	cellIdx = cellWallIdx = 0;
	for (int idx = 0; idx < arraySize; idx++) {
		sampleCells0[idx].x = sampleCells0[idx].y = 0;
		sampleCells1[idx].x = sampleCells1[idx].y = 0;
		sampleCellWalls0[idx].x = sampleCellWalls0[idx].y = 0;
		sampleCellWalls1[idx].x = sampleCellWalls1[idx].y = 0;
	}
}

void cellScaleImageReference::clone(cellScaleImageReference& clonee) {
	for (int idx = 0; idx < arraySize; idx++) {
		sampleCells0[idx].x = clonee.sampleCells0[idx].x;
		sampleCells0[idx].y = clonee.sampleCells0[idx].y;
		sampleCells1[idx].x = clonee.sampleCells1[idx].x;
		sampleCells1[idx].y = clonee.sampleCells1[idx].y;
		sampleCellWalls0[idx].x = clonee.sampleCellWalls0[idx].x;
		sampleCellWalls0[idx].y = clonee.sampleCellWalls0[idx].y;
		sampleCellWalls1[idx].x = clonee.sampleCellWalls1[idx].x;
		sampleCellWalls1[idx].y = clonee.sampleCellWalls1[idx].y;
	}
}
void cellScaleImageReference::setCell(Point2f p0, Point2f p1) {
		sampleCells0[cellIdx].x = p0.x;
		sampleCells0[cellIdx].y = p0.y;
		sampleCells1[cellIdx].x = p1.x;
		sampleCells1[cellIdx].y = p1.y;
}
void cellScaleImageReference::setCellWall(Point2f p0, Point2f p1) {
		sampleCellWalls0[cellWallIdx].x = p0.x;
		sampleCellWalls0[cellWallIdx].y = p0.y;
		sampleCellWalls1[cellWallIdx].x = p1.x;
		sampleCellWalls1[cellWallIdx].y = p1.y;
}
void cellScaleImageReference::increment(bool wall) {
	if (wall) {
		cellWallIdx++;
		if (cellWallIdx == arraySize) cellWallIdx = 0;

	}
	else {
		cellIdx++;
		if (cellIdx == arraySize) cellIdx = 0;
	}
}
int cellScaleImageReference::index(bool wall) {
	return (wall)? cellWallIdx : cellIdx;
}


std::string cellScaleImageReference::serialise() {
	std::stringstream ss;
	for (int idx = 0; idx < arraySize; idx++) {
		ss << sampleCells0[idx].x  << "," <<  sampleCells0[idx].y << ",";
		ss << sampleCells1[idx].x  << "," <<  sampleCells1[idx].y << ",";
		ss << sampleCellWalls0[idx].x  << "," <<  sampleCellWalls0[idx].y << ",";
		ss << sampleCellWalls1[idx].x << "," << sampleCellWalls1[idx].y;
		if (idx != arraySize - 1) ss << ",";
	}
	return ss.str();
}

void cellScaleImageReference::deserialise(std::vector<std::string> &valStrArray) {
	if (valStrArray.size() == 8 * arraySize) {
		int idx = 0;
		for (int Cidx = 0; Cidx < arraySize; Cidx++) {
			sampleCells0[Cidx].x = stof(valStrArray[idx++]);
			sampleCells0[Cidx].y = stof(valStrArray[idx++]);
			sampleCells1[Cidx].x = stof(valStrArray[idx++]);
			sampleCells1[Cidx].y = stof(valStrArray[idx++]);
			sampleCellWalls0[Cidx].x = stof(valStrArray[idx++]);
			sampleCellWalls0[Cidx].y = stof(valStrArray[idx++]);
			sampleCellWalls1[Cidx].x = stof(valStrArray[idx++]);
			sampleCellWalls1[Cidx].y = stof(valStrArray[idx++]);
		}
	}
}
std::string cellScaleImageReference::serialiseAreaCaptions() {
	return "mag0, cell0mm, magWall0, wall0mm ... N, aveCell, mm, aveWall, mm";
}
std::string cellScaleImageReference::serialiseCalcs(float scale) {
	std::stringstream ss;
	float magnitudeSumCell = 0;
	float magnitudeSumCellWall = 0;
	float tmpMag = 0;
	for (int idx = 0; idx < arraySize; idx++) {
		tmpMag = magnitude(sampleCells0[idx] - sampleCells1[idx]);
		magnitudeSumCell += tmpMag;
		ss << tmpMag << "," << tmpMag / scale << ",";
		tmpMag = magnitude(sampleCellWalls0[idx] - sampleCellWalls1[idx]);
		magnitudeSumCellWall += tmpMag;
		ss << tmpMag << "," << tmpMag / scale << ",";
	}
	ss << magnitudeSumCell / arraySize << ", "  << magnitudeSumCell / (scale * arraySize) << ", " << magnitudeSumCellWall / arraySize << ", " << magnitudeSumCellWall / (scale * arraySize);
	return ss.str();

}
