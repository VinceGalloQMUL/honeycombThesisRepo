#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "csvFile.h"
#include "csvAreaConfig.h"
using namespace System;

csvAreaConfig::csvAreaConfig() {
	hive = "X";
	mScaleDist = 10;
}

void csvAreaConfig::read(const char *filepathname, vector<areaDesc*>* geoDescs) {
	std::ifstream str;
	str.open(filepathname);
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	if (lineArray.size() == 8) {
		int idx = 0;
		// locFile << hive << ", "  << scaleX << ", " << scaleY << "," << offsetX << "," << offsetY << "," << rotation << "," << zoom << "," << zoomCentre. x << "," << zoomCentre.y;
		// locFile << "," << scalePt0.x << "," << scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "\n
		hive = lineArray[idx++];
		mScaleDist = stof(lineArray[idx++]);
		mScaleCorner = Point2f(stof(lineArray[idx]), stof(lineArray[idx + 1]));
		idx += 2;
		mScaleHorizontal = Point2f(stof(lineArray[idx]), stof(lineArray[idx + 1]));
		idx += 2;
		mScaleDiagonal = Point2f(stof(lineArray[idx]), stof(lineArray[idx + 1]));
		idx += 2;
	}
	lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	mobileRefs.deserialise(lineArray);
	geoDescs->clear();
	bool gotAgeo = false;
	do {
		areaDesc* tmpGeo = new areaDesc();
		lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		gotAgeo = tmpGeo->deserialise(lineArray);
		if (gotAgeo || (geoDescs->size() == 0)) { // save valid geometryDesc, or if there are none, save an empty one
			geoDescs->push_back(tmpGeo);
		}
	} while (gotAgeo);
	str.close();
}
Point resizePoint(Point2f inputLoc, perspective persp, float dispDivision) {
	Point2f retv = Point2f(((inputLoc.x)) / dispDivision, ((inputLoc.y)) / dispDivision);
	retv = persp.adjustedPerspective(retv, false, true);
	return retv;
}

void csvAreaConfig::write(const char *filepathname, vector<areaDesc*>* geoDescs, perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV, int *totals) {
	int areas[2][MENU_BROODIDX_PUPA + 1];
	areas[0][MENU_BROODIDX_EGG] = areas[0][MENU_BROODIDX_LARVA] = areas[0][MENU_BROODIDX_PUPA] = 0;
	areas[1][MENU_BROODIDX_EGG] = areas[1][MENU_BROODIDX_LARVA] = areas[1][MENU_BROODIDX_PUPA] = 0;
	ofstream locFile(filepathname, ios::out);
	locFile << hive << ", " << mScaleDist << ", " << mScaleCorner.x << "," << mScaleCorner.y << "," << mScaleHorizontal.x << "," << mScaleHorizontal.y << "," << mScaleDiagonal.x << "," << mScaleDiagonal.y << "\n";
	locFile << mobileRefs.serialise() << "\n";
	for (int idx = 0; idx < geoDescs->size(); idx++) {
		if (!geoDescs->at(idx)->isEmpty()) {
			locFile << geoDescs->at(idx)->serialise() << "\n";
		}
	}

	Point tmpPt = resizePoint(mScaleCorner, persp, dispDivision);
	locFile << "\nAdjusted\n" << tmpPt.x << "," << tmpPt.y;
	tmpPt = resizePoint(mScaleHorizontal, persp, dispDivision);
	locFile << "," << tmpPt.x << "," << tmpPt.y;
	tmpPt = resizePoint(mScaleHorizontal, persp, dispDivision);
	locFile << "," << tmpPt.x << "," << tmpPt.y << "\n";
	for (int idx = 0; idx < geoDescs->size(); idx++) {
		if (!geoDescs->at(idx)->isEmpty()) {
			int addQty = 0;
			int qty = 0;
			int addIdx = 0;
			locFile << geoDescs->at(idx)->serialiseAdjusted(persp, dispDivision, cellQty,  ref0,  refH,  refD, pxlsH, pxlsV) << "\n";
			int typeIdx = geoDescs->at(idx)->mType;
			bool isDelta = geoDescs->at(idx)->mDelta;
			int deltaIdx = (isDelta) ? 1 : 0;
			qty = geoDescs->at(idx)->mArea;
			switch (typeIdx) {
			case MENU_BROODIDX_EGG:
			case MENU_BROODIDX_LARVA:
				if (isDelta) {
					typeIdx = MENU_BROODIDX_DELTA_LARVA;
				}
				break;
			case MENU_BROODIDX_PUPA:
				if (isDelta) {
					typeIdx = MENU_BROODIDX_DELTA_PUPA;
				}
				break;
				break;
			case MENU_BROODIDX_NOT_EGG:
				typeIdx = MENU_BROODIDX_EGG;
				qty = -qty;
				break;
			case MENU_BROODIDX_NOT_LARVA:
				typeIdx = MENU_BROODIDX_LARVA;
				qty = -qty;
				break;
			case MENU_BROODIDX_NOT_PUPA:
				if (isDelta) {
					typeIdx = MENU_BROODIDX_DELTA_NOT_PUPA;
				}
				else {
					typeIdx = MENU_BROODIDX_PUPA;
					qty = -qty;

				}
				break;
			case MENU_BROODIDX_EGG_NOT_LARVA:
				addIdx = MENU_BROODIDX_EGG;
				addQty = qty;
				typeIdx = MENU_BROODIDX_LARVA;
				qty = -qty;
				break;
			case MENU_BROODIDX_LARVA_NOT_PUPA:
				addIdx = MENU_BROODIDX_LARVA;
				addQty = qty;
				typeIdx = MENU_BROODIDX_PUPA;
				qty = -qty;
				break;
			case MENU_BROODIDX_PUPA_NOT_LARVA:
				addIdx = MENU_BROODIDX_PUPA;
				addQty = qty;
				typeIdx = MENU_BROODIDX_LARVA;
				qty = -qty;
				break;
			case MENU_BROODIDX_LARVA_NOT_EGG:
				addIdx = MENU_BROODIDX_LARVA;
				addQty = qty;
				typeIdx = MENU_BROODIDX_EGG;
				qty = -qty;
				break;
			}

			areas[deltaIdx][typeIdx] += qty;
			areas[deltaIdx][addIdx] += addQty;
		}
	}
	locFile << hive ;
	for (int deltaIdx = 0; deltaIdx < 2; deltaIdx++) {
		totals[(4 * deltaIdx) + 0] = areas[deltaIdx][0];
		totals[(4 * deltaIdx) + 1] = areas[deltaIdx][1];
		totals[(4 * deltaIdx) + 2] = areas[deltaIdx][2];
		totals[(4 * deltaIdx) + 3] = areas[deltaIdx][0] + areas[deltaIdx][1] + areas[deltaIdx][2];
		locFile << ", " << totals[(4 * deltaIdx) + 0] << "," << totals[(4 * deltaIdx) + 1] << "," << totals[(4 * deltaIdx) + 2] << "," << totals[(4 * deltaIdx) + 3];

	}
	locFile << ", " << totals[8] << "," << totals[9] << "," << totals[10] << "," << totals[11];
	locFile.close();

}
int csvAreaConfig::calcSize(int whichGeo, vector<areaDesc*>* geoDescs, perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV) {
	int qty = 0;
	if (!geoDescs->at(whichGeo)->isEmpty()) {
		geoDescs->at(whichGeo)->serialiseAdjusted(persp, dispDivision, cellQty, ref0, refH, refD, pxlsH, pxlsV);
		qty = geoDescs->at(whichGeo)->mArea;
	}
	return qty;
}
