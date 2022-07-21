#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "CVcommon.h"
#include "csvFile.h"

#include "locationsFile.h"

using namespace System;

locationsFile::locationsFile() {
}
locationsFile::~locationsFile() {

}

// frameNum, size_or_z, x, y,   aspectRatio, angle,     frameTime, speed, direction, classification - for 877 points
#define point2dVectorPxls_basicLen 6
#define point2dVectorPxls_enhancedLen 10
#define point2dVectorPxls_extendedLen 12

point2dVectorPxls *locationsFile::parseLine(std::vector<std::string>   lineArray) {
	point2dVectorPxls *retv = NULL;
	int idx = 0;
	if ((lineArray.size() == point2dVectorPxls_basicLen && hdr.fileType == 0) || (lineArray.size() >= point2dVectorPxls_enhancedLen && hdr.fileType == 1)) {
		retv = new point2dVectorPxls();
		retv->loc.fNum = std::stoi(lineArray[idx++]);
		retv->loc.size = std::stof(lineArray[idx++]);
		retv->loc.x = std::stoi(lineArray[idx++]);
		retv->loc.y = std::stoi(lineArray[idx++]);
		retv->loc.aspectRatio = std::stof(lineArray[idx++]);
		retv->loc.angle = std::stof(lineArray[idx++]);
		retv->loc.fTime = 0;
		retv->directionX = 0;
		retv->directionY = 0;
		retv->directionZ = 0;
		retv->speed = 0;
	}
	if (lineArray.size() == point2dVectorPxls_enhancedLen && hdr.fileType == 1) {
		retv->loc.fTime = std::stof(lineArray[idx++]);
		retv->speed = std::stof(lineArray[idx++]);
		vector3f::radToNormalisedVector(std::stof(lineArray[idx++]), &(retv->directionX), &(retv->directionY), &(retv->directionZ));
		retv->classification = std::stoi(lineArray[idx++]);
	} else if ((lineArray.size() == point2dVectorPxls_extendedLen && hdr.fileType == 1)) {
		retv->loc.fTime = std::stof(lineArray[idx++]);
		retv->speed = std::stof(lineArray[idx++]);
		retv->directionX = std::stof(lineArray[idx++]);
		retv->directionY = std::stof(lineArray[idx++]);
		retv->directionZ = std::stof(lineArray[idx++]);
		retv->classification = std::stoi(lineArray[idx++]);
	}
	return retv;
}

point3dMm *locationsFile::parseLine3dMm(std::vector<std::string>   lineArray) {
	point3dMm *retv = NULL;
	int idx = 0;
	if (lineArray.size() >= 8) {
		// " fTime, size, aspectRatio, RWx, RWy, RWz" << "\n";
		// or " fTime, size, aspectRatio, RWx, RWy, RWz, speed, direction, classification" << "\n";
		// or " fTime, size, aspectRatio, RWx, RWy, RWz, RWxSmoothed, RWySmoothed, RWzSmoothed, speed, directionX, directionY, directionZ, classification" << "\n";
		retv = new point3dMm();
		retv->fTime = std::stof(lineArray[idx++]);
		retv->size = std::stof(lineArray[idx++]);
		retv->aspectRatio = std::stof(lineArray[idx++]);
		retv->RWx = std::stof(lineArray[idx++]);
		retv->RWy = std::stof(lineArray[idx++]);
		retv->RWz = std::stof(lineArray[idx++]);
		retv->RWxSmoothed = std::stof(lineArray[idx++]);
		retv->RWySmoothed = std::stof(lineArray[idx++]);
		retv->RWzSmoothed = std::stof(lineArray[idx++]);
		if (lineArray.size() == 12) {
			retv->speed = std::stof(lineArray[idx++]);
			vector3f::radToNormalisedVector(std::stof(lineArray[idx++]), &(retv->directionX), &(retv->directionY), &(retv->directionZ));
			retv->classification = std::stoi(lineArray[idx++]);
		} else if (lineArray.size() == 14) {
			retv->speed = std::stof(lineArray[idx++]);
			retv->directionX = std::stof(lineArray[idx++]);
			retv->directionY = std::stof(lineArray[idx++]);
			retv->directionZ = std::stof(lineArray[idx++]);
			retv->classification = std::stoi(lineArray[idx++]);
		}
	}
	return retv;
}

bool locationsFile::parseHeader(std::vector<std::string>   lineArray) {
	bool retv = false;
	hdr.fileType = 0;
	hdr.originX = hdr.originY = 0;
	if (lineArray.size() == 5) {
		// 	file_type(0 = point2dPxls,1 = point2dVectorPxls), originX, originY, width, height
		hdr.fileType = std::stoi(lineArray[0]);
		hdr.originX= std::stoi(lineArray[1]);
		hdr.originY = std::stoi(lineArray[2]);
		hdr.width = std::stoi(lineArray[3]);
		hdr.height = std::stoi(lineArray[4]);
		retv = true;
	}
	return retv;
}

bool locationsFile::parseHeaderStereo(std::vector<std::string>   lineArray, float *x, float *y) {
	bool retv = false;
	*x = *y = 0;
	if (lineArray.size() == 2) {
		// 	originX, originY
		*x = std::stof(lineArray[0]);
		*y = std::stof(lineArray[1]);
		retv = true;
	}
	return retv;
}

void locationsFile::setHeader(int fileType, int orgX, int orgY, int w, int h) {
		hdr.fileType = fileType;
		hdr.originX= orgX;
		hdr.originY = orgY;
		hdr.width = w;
		hdr.height = h;
}
void locationsFile::copyTo(point2dVectorPxls *pSrce, point2dVectorPxls *pDest) {
	pDest->loc.fNum = pSrce->loc.fNum;
	pDest->loc.size = pSrce->loc.size;
	pDest->loc.x = pSrce->loc.x;
	pDest->loc.y = pSrce->loc.y;
	pDest->loc.fTime = pSrce->loc.fTime;
	pDest->loc.aspectRatio = pSrce->loc.aspectRatio;
	pDest->loc.angle = pSrce->loc.angle;
	pDest->classification = pSrce->classification;
	pDest->directionX = pSrce->directionX;
	pDest->directionY = pSrce->directionY;
	pDest->directionZ = pSrce->directionZ;
	pDest->speed = pSrce->speed;

}

vector<point2dVectorPxls> * locationsFile::read(char *filepathname, bool interpolate) {
	std::ifstream str;
	str.open(filepathname);
	int lastFrameNum = -1;
	point2dVectorPxls *lastrec;
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	bool usePreviousLineAsLoc = true;
	if (parseHeader(lineArray)) {
		usePreviousLineAsLoc = false;
	}
	vector<point2dVectorPxls> *storedLocs = new vector<point2dVectorPxls>();
	for (;;) {
		if (!usePreviousLineAsLoc) {
			lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		}
		usePreviousLineAsLoc = false; // only do the line reuse bit for the 1st line in case no header was present
		point2dVectorPxls *tmprec = parseLine(lineArray);
		if (tmprec != NULL) {
			if (!(lastFrameNum < 0 || !interpolate || tmprec->loc.fNum == lastFrameNum + 1 || tmprec->loc.fNum > lastFrameNum + 9)) { //get things started or none missing or gap too large
				int frameDiff = tmprec->loc.fNum - lastFrameNum;
				for (int frameNum = lastFrameNum + 1; frameNum < tmprec->loc.fNum; frameNum++) {
					point2dVectorPxls *infill = new point2dVectorPxls();
					infill->loc.fNum = frameNum;
					infill->loc.fTime = 0;
					infill->loc.size = lastrec->loc.size + ((frameNum - lastFrameNum) * (tmprec->loc.size - lastrec->loc.size)) / frameDiff;
					infill->loc.x = lastrec->loc.x + ((frameNum - lastFrameNum) * (tmprec->loc.x - lastrec->loc.x)) / frameDiff;
					infill->loc.y = lastrec->loc.y + ((frameNum - lastFrameNum) * (tmprec->loc.y - lastrec->loc.y)) / frameDiff;
					infill->loc.aspectRatio = lastrec->loc.aspectRatio + ((frameNum - lastFrameNum) * (tmprec->loc.aspectRatio - lastrec->loc.aspectRatio)) / frameDiff;
					infill->loc.angle = lastrec->loc.angle + ((frameNum - lastFrameNum) * (tmprec->loc.angle - lastrec->loc.angle)) / frameDiff;
					infill->speed = lastrec->speed + ((frameNum - lastFrameNum) * (tmprec->speed - lastrec->speed)) / frameDiff;
					infill->directionX = lastrec->directionX;
					infill->directionY = lastrec->directionY;
					infill->directionZ = lastrec->directionZ;
					infill->classification = lastrec->classification;
					storedLocs->push_back(*infill);
				}
			}
			lastFrameNum = tmprec->loc.fNum;
			lastrec = tmprec;
			storedLocs->push_back(*tmprec);
		}
		else {
			break;
		}
	}
	str.close();
	return storedLocs;
}

vector<point3dMm> * locationsFile::read3dMm(char *filepathname, float *BLx, float *BLy, float *TRx, float *TRy) {
	std::ifstream str;
	str.open(filepathname);
	point3dMm *lastrec;
	vector<point3dMm> *storedLocs = new vector<point3dMm>();
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	if (parseHeaderStereo(lineArray, BLx, BLy)) {
		lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
		if (parseHeaderStereo(lineArray, TRx, TRy)) {
			for (;;) {
				lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
				point3dMm *tmprec = parseLine3dMm(lineArray);
				if (tmprec != NULL) {
					recalcSpeed(tmprec, lastrec);
					lastrec = tmprec;
					storedLocs->push_back(*tmprec);
				}
				else {
					break;
				}
			}
		}
	}
	str.close();
	return storedLocs;
}
void locationsFile::recalcSpeed(point3dMm *currentRec, point3dMm *prevRec) {
	if (prevRec != nullptr && currentRec != nullptr) {
		float deltaTime = currentRec->fTime - prevRec->fTime;
		float distanceX = currentRec->RWxSmoothed - prevRec->RWxSmoothed;
		float distanceY = currentRec->RWySmoothed - prevRec->RWySmoothed;
		float distanceZ = currentRec->RWzSmoothed - prevRec->RWzSmoothed;
		float tmpSpeed = (distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
		tmpSpeed = Math::Sqrt(tmpSpeed);
		currentRec->speed =  tmpSpeed / deltaTime;
	}
}


void locationsFile::write(char *filepathname, vector<point2dPxls> *pLocs, bool halfSize, int zoneOrgX, int zoneOrgY, int width, int height) {
	ofstream locFile(filepathname, ios::out);
	int multiplier = (halfSize) ? 2 : 1;
	width *= multiplier;
	height *= multiplier;
	locFile << "# file_type(0 = point2dPxls,1 = point2dVectorPxls), originX, originY, width, height" << "\n";
	locFile << 0 << ","  << zoneOrgX << "," << zoneOrgY << "," << width << ", " << height << "\n";
	locFile << "# locations as frameNum, size_or_z, x, y - for " << pLocs->size() << " points\n";
	for (int idx = 0; idx < pLocs->size(); idx++) {
		locFile << pLocs->at(idx).fNum << "," << pLocs->at(idx).size << "," << ((pLocs->at(idx).x * multiplier) - zoneOrgX) << "," 
			<< ((pLocs->at(idx).y * multiplier) - zoneOrgY) << "," << pLocs->at(idx).aspectRatio << "," << pLocs->at(idx).angle << "\n";
	}
	locFile.close();

}
void locationsFile::write(char *filepathname, vector<point2dVectorPxls> *pLocs, bool halfSize, int zoneOrgX, int zoneOrgY, int width, int height) {
	ofstream locFile(filepathname, ios::out);
	int multiplier = (halfSize) ? 2 : 1;
	width *= multiplier;
	height *= multiplier;
	locFile << "# file_type(0 = point2dPxls,1 = point3dVectorClassified), originX, originY, width, height" << "\n";
	locFile << 1 << ","  << zoneOrgX << "," << zoneOrgY << "," << width << ", " << height << "\n";
	locFile << "# locations as frameNum, size_or_z, x, y, aspectRatio, angle, frameTime, speed, direction, classification - for " << pLocs->size() << " points\n";
	for (int idx = 0; idx < pLocs->size(); idx++) {
		locFile << pLocs->at(idx).loc.fNum << "," << pLocs->at(idx).loc.size << "," << ((pLocs->at(idx).loc.x * multiplier) - zoneOrgX) << ","
			<< ((pLocs->at(idx).loc.y * multiplier) - zoneOrgY) << "," << pLocs->at(idx).loc.aspectRatio << "," << pLocs->at(idx).loc.angle  << ","
			<< pLocs->at(idx).loc.fTime << "," << pLocs->at(idx).speed << "," 
			<< pLocs->at(idx).directionX << "," << pLocs->at(idx).directionY << "," << pLocs->at(idx).directionZ << "," << pLocs->at(idx).classification << ","
			<< pLocs->at(idx).loc.ellipseMaj << "," << pLocs->at(idx).loc.ellipseMin << "\n";
	}
	locFile.close();
}
void locationsFile::write(char *filepathname, vector<point3dMm> *pLocs, float cornerBLx, float cornerBLy, float cornerTRx, float cornerTRy) {
	ofstream locFile(filepathname, ios::out);
	locFile << "# , originBLx, origin BLy, TRx, TRy" << "\n";
	locFile << cornerBLx << "," << cornerBLy << "\n";
	locFile << cornerTRx << "," << cornerTRy << "\n";
	int numPoints = 0;
	locFile << "# fTime, size, aspectRatio, RWx, RWy, RWz, RWxSmoothed, RWySmoothed, RWzSmoothed, speed, directionX, directionY, directionZ, classification" << "\n";
	for (int idx = 0; idx < pLocs->size(); idx++) {
		// framenum/fps => elapsed seconds, RealWorldZmm, RealWorldXmm, RealWorldYmm
		locFile << pLocs->at(idx).fTime << "," << pLocs->at(idx).size << "," << pLocs->at(idx).aspectRatio << "," << pLocs->at(idx).RWx << "," << pLocs->at(idx).RWy << "," << pLocs->at(idx).RWz << ","
			<< pLocs->at(idx).RWxSmoothed << "," << pLocs->at(idx).RWySmoothed << "," << pLocs->at(idx).RWzSmoothed << "," << pLocs->at(idx).speed << ","
			<< pLocs->at(idx).directionX << "," << pLocs->at(idx).directionY << ","  << pLocs->at(idx).directionZ << "," << pLocs->at(idx).classification << "\n";
		numPoints++;
	}
	locFile.close();
}
string locationsFile::write(point3dMm *pLoc) {
	stringstream ss;

	// framenum/fps => elapsed seconds, RealWorldZmm, RealWorldXmm, RealWorldYmm
	ss << pLoc->fTime << "," << pLoc->size << "," << pLoc->aspectRatio << "," << pLoc->RWx << "," << pLoc->RWy << "," << pLoc->RWz << ","
		<< pLoc->RWxSmoothed << "," << pLoc->RWySmoothed << "," << pLoc->RWzSmoothed << "," << pLoc->speed << ","
		<< pLoc->directionX << "," << pLoc->directionY << ","  << pLoc->directionZ << "," << pLoc->classification;
	return ss.str();
}
string locationsFile::writeCaption() {
	return string("# fTime, size, aspectRatio, RWx, RWy, RWz, RWxSmoothed, RWySmoothed, RWzSmoothed, speed, directionX, directionY, directionZ, classification");
}
vector2f *locationsFile::calcFlightpath(vector<point2dVectorPxls> *pLocs, int idxS, int idxE, float pixelsPerMM, float framesPerSecond) {
	vector2f *path = new vector2f(0.0f, 0.0f);
	if (idxS < 0) idxS = 0;
	if (idxE >= pLocs->size()) idxE = (int)pLocs->size() - 1;
	if (idxE > idxS) {
		for (int idx = idxS + 1; idx < idxE; idx++) {
			path->add((float)(pLocs->at(idx).loc.x - pLocs->at(idx - 1).loc.x), (float)(pLocs->at(idx).loc.y - pLocs->at(idx - 1).loc.y));
		}
		float denominator = float(pLocs->at(idxE).loc.fNum - pLocs->at(idxS).loc.fNum); // path / this will give pixels per frame
		denominator *= pixelsPerMM; // now it will yield mm per frame
		denominator /= framesPerSecond; // and now, mm per sec
		path->divide(denominator);
	}
	return path;

}
vector3f *locationsFile::calcFlightpath(vector<point3dMm> *pLocs, int idxS, int idxE) {
	vector3f *path = new vector3f(0, 0, 0);
	if (idxS < 0) idxS = 0;
	if (idxE >= pLocs->size()) idxE = (int)pLocs->size() - 1;
	if (idxE > idxS) {
		for (int idx = idxS + 1; idx < idxE; idx++) {
			path->add(pLocs->at(idx).RWx - pLocs->at(idx - 1).RWx, pLocs->at(idx).RWy - pLocs->at(idx - 1).RWy, pLocs->at(idx).RWz - pLocs->at(idx - 1).RWz);
		}
		float denominator = pLocs->at(idxE).fTime - pLocs->at(idxS).fTime; // path / this will give pixels per frame
		path->divide(denominator);
	}
	return path;

}

void locationsFile::calcFlightpaths(vector<point2dVectorPxls> *pLocs, float pixelsPerMM, float framesPerSecond, int range, float *pMinSpeed, float *pMaxSpeed) {
	float minSpeed = -1;
	float maxSpeed = -1;
	int idxS = 0;
	int idxE = range;
	int idxMax = (int)pLocs->size();
	for (int idx = 0; idx < idxMax; idx++) {
		idxS = (idx >= range)? idx - range : 0;
		idxE = ((idx + range) < idxMax) ? idx + range : idxMax - 1;
		vector2f *tmpVector = calcFlightpath(pLocs, idxS, idxE, pixelsPerMM, framesPerSecond);
		pLocs->at(idx).loc.fTime = pLocs->at(idx).loc.fNum / framesPerSecond;
		pLocs->at(idx).speed = tmpVector->magnitude();
		vector2f *tmpVectorDir = tmpVector->normalised();
		pLocs->at(idx).directionX = tmpVectorDir->dx;
		pLocs->at(idx).directionY = tmpVectorDir->dy;
		if (minSpeed < 0 || pLocs->at(idx).speed < minSpeed) minSpeed = pLocs->at(idx).speed;
		if (maxSpeed < 0 || pLocs->at(idx).speed > maxSpeed) maxSpeed = pLocs->at(idx).speed;
		delete tmpVector;
	}
	*pMinSpeed = minSpeed;
	*pMaxSpeed = maxSpeed;

}
void locationsFile::calcFlightpaths(vector<point3dMm> *pLocs, int range, float *pMinSpeed, float *pMaxSpeed) {
	float minSpeed = -1;
	float maxSpeed = -1;
	int idxS = 0;
	int idxE = range;
	int idxMax = (int)pLocs->size();
	for (int idx = 0; idx < idxMax; idx++) {
		idxS = (idx >= range)? idx - range : 0;
		idxE = ((idx + range) < idxMax) ? idx + range : idxMax - 1;
		vector3f *tmpVector = calcFlightpath(pLocs, idxS, idxE);
		pLocs->at(idx).speed = tmpVector->magnitude();
		vector3f *tmpVectorDir = tmpVector->normalised();
		pLocs->at(idx).directionX = tmpVectorDir->dx;
		pLocs->at(idx).directionY = tmpVectorDir->dy;
		pLocs->at(idx).directionZ = tmpVectorDir->dz;
		if (minSpeed < 0 || pLocs->at(idx).speed < minSpeed) minSpeed = pLocs->at(idx).speed;
		if (maxSpeed < 0 || pLocs->at(idx).speed > maxSpeed) maxSpeed = pLocs->at(idx).speed;
		delete tmpVector;
		delete tmpVectorDir;
	}
	*pMinSpeed = minSpeed;
	*pMaxSpeed = maxSpeed;

}
int locationsFile::previousOrNextClassificationSection(int initFrameNum, vector<point2dVectorPxls> *pLocs, int plusOrMinus, bool reportCurrentSection) {
	int retv = -1;
	int frameNum = initFrameNum;
	int upperLimit = (int)pLocs->size() - 1;
	if (frameNum < 0 || frameNum >= upperLimit) {
		return retv;
	}
	int currentClass = pLocs->at(frameNum).classification;
	bool inReportableSection = reportCurrentSection; // if we want a report about this sectionn then we are already in the right one
	while (frameNum >= 0 && frameNum < upperLimit) {
		frameNum += plusOrMinus;
		if (frameNum < 0) { // edge case, at the start and going backwards
			frameNum = 0;
			break;
		}
		if (!inReportableSection) {
			if (currentClass != pLocs->at(frameNum).classification) {
				inReportableSection = true;
				currentClass = pLocs->at(frameNum).classification;
			}
		}
		else {
			if (currentClass != pLocs->at(frameNum).classification) {
				// now gone too far, so 
				retv = frameNum - plusOrMinus;
				break;
			}
		}
	}
	if (retv == -1) {
		// hit the end without finding a further change, so return the limit value
		retv = frameNum;
	}
	return retv;
}
void locationsFile::setSectionClass(int idxCurrentSectionStart, int idxCurrentSectionEnd, vector<point2dVectorPxls> *pLocs, int classType) {
	int upperLimit = (int)pLocs->size();
	if ((idxCurrentSectionStart >= 0 && idxCurrentSectionStart < upperLimit) && (idxCurrentSectionEnd >= 0 && idxCurrentSectionEnd < upperLimit)) {
		for (int idx = idxCurrentSectionStart; idx <= idxCurrentSectionEnd; idx++) {
			pLocs->at(idx).classification = classType;
		}
	}

}
void locationsFile::setSectionClass(int idxPeriodStart, int *idxPeriodEnd, int *wasClassType, int classType, int frameCount, vector<point2dVectorPxls> *pLocs) {
	int upperLimit = (int)pLocs->size();
	if ((idxPeriodStart >= 0 && idxPeriodStart < upperLimit)) {
		int fNumStart = pLocs->at(idxPeriodStart).loc.fNum;
		int fNumEnd = fNumStart + frameCount;
		int startClass = pLocs->at(idxPeriodStart).classification;
		*wasClassType = startClass;
		for (int idx = idxPeriodStart; idx < upperLimit; idx++) {
			if ((pLocs->at(idx).loc.fNum > fNumEnd) || (startClass != pLocs->at(idx).classification)) {
				break;
			}
			pLocs->at(idx).classification = classType;
			*idxPeriodEnd = idx;
		}
	}

}
void locationsFile::moveSectionEdge(int idxSectionEdge, int plusOrMinus, vector<point2dVectorPxls> *pLocs) {
	int upperLimit = (int)pLocs->size();
	int newSectionEdge = idxSectionEdge + plusOrMinus;
	if ((idxSectionEdge >= 0 && idxSectionEdge < upperLimit) && (newSectionEdge >= 0 && newSectionEdge < upperLimit)) {
		pLocs->at(newSectionEdge).classification = pLocs->at(idxSectionEdge).classification;
	}
}

int locationsFile::findFrame(int frameNum, vector<point2dVectorPxls> *pLocs, point2dVectorPxls *loc) {
	int idxS = 0;
	int idxMax = (int)pLocs->size() - 1;
	int closeEnough = 10;
	int retv = -1;
	bool dosearch = true;
	int testLoc = -1;
	if ((frameNum >= pLocs->at(0).loc.fNum) && (frameNum <= pLocs->at(idxMax).loc.fNum)) {
		while (dosearch) {
			int range = idxMax - idxS;
			if (range > closeEnough) {
				testLoc = (idxS + idxMax) / 2;
				int testFnum = pLocs->at(testLoc).loc.fNum;
				if (testFnum == frameNum) {
					dosearch = false;
					retv = testLoc;
				}
				else if (testFnum > frameNum) { // indexed item is too high
					idxMax = testLoc;
				}
				else { // indexed item is too low
					idxS = testLoc;
				}
			}
			else {
				dosearch = false;
				for (testLoc = idxS; testLoc <= idxMax; testLoc++) {
					if (pLocs->at(testLoc).loc.fNum >= frameNum) {
						retv = testLoc;
						break;
					}
				}
			}
		}
	}
	if (retv >= 0 && loc != NULL) {
		copyTo(&(pLocs->at(retv)), loc);
	}
	return retv;
}
float locationsFile::sizeAverage(vector<point2dVectorPxls> *pLocs) {
	float tmpAve = 0;
	for (int idx = 0; idx < pLocs->size(); idx++) {
		tmpAve += pLocs->at(idx).loc.size;
	}
	tmpAve = tmpAve / pLocs->size();
	return tmpAve;
}
float locationsFile::sizeAverage(vector<point3dMm> *pLocs) {
	float tmpAve = 0;
	for (int idx = 0; idx < pLocs->size(); idx++) {
		tmpAve += pLocs->at(idx).size;
	}
	tmpAve = tmpAve / pLocs->size();
	return tmpAve;
}
float locationsFile::getAdvisorySizeLimit(vector<point2dVectorPxls> *pLocs, float *minSize, float *maxSize) {
	float tmpAve = sizeAverage(pLocs);
	int histSize = ((int)pLocs->size() > 200) ? 20 : (int)pLocs->size() / 10;
	int *histogram = new int[histSize];
	float histDelta = 4 * tmpAve / histSize;
	int maxHist = 0;
	int maxHistIdx = 0;
	*minSize = *maxSize = pLocs->at(0).loc.size;
	for (int h = 0; h < histSize; h++) histogram[h] = 0;
	for (int idx = 0; idx < pLocs->size(); idx++) {
		int hidx = (int)(pLocs->at(idx).loc.size / histDelta);
		if (*minSize > pLocs->at(idx).loc.size) *minSize = pLocs->at(idx).loc.size;
		if (*maxSize < pLocs->at(idx).loc.size) *maxSize = pLocs->at(idx).loc.size;
		if (hidx >= histSize) hidx = histSize - 1;
		histogram[hidx]++;
		if (histogram[hidx] > maxHist) {
			maxHist = histogram[hidx];
			maxHistIdx = hidx;
		}
	}
	float retv = 2 * maxHistIdx * histDelta;
	if (retv < *minSize) retv = *minSize;
	if (retv > *maxSize) retv = *maxSize;
	return retv;

}

float locationsFile::getAdvisorySizeLimit(vector<point3dMm> *pLocs, float *minSize, float *maxSize) {
	float tmpAve = sizeAverage(pLocs);
	int histSize = (int)(pLocs->size() > 200) ? 20 : (int)pLocs->size() / 10;
	if (histSize < 1) histSize = 1;
	int *histogram = new int[histSize];
	float histDelta = 4 * tmpAve / histSize;
	int maxHist = 0;
	int maxHistIdx = 0;
	*minSize = *maxSize = pLocs->at(0).size;
	for (int h = 0; h < histSize; h++) histogram[h] = 0;
	for (int idx = 0; idx < pLocs->size(); idx++) {
		int hidx = (int)(pLocs->at(idx).size / histDelta);
		if (*minSize > pLocs->at(idx).size) *minSize = pLocs->at(idx).size;
		if (*maxSize < pLocs->at(idx).size) *maxSize = pLocs->at(idx).size;
		if (hidx >= histSize) hidx = histSize - 1;
		histogram[hidx]++;
		if (histogram[hidx] > maxHist) {
			maxHist = histogram[hidx];
			maxHistIdx = hidx;
		}
	}
	float retv = 2 * maxHistIdx * histDelta;
	if (retv < *minSize) retv = *minSize;
	if (retv > *maxSize) retv = *maxSize;
	return retv;
}

geometryFile::geometryFile() {
		for (int idx = 0; idx < 4; idx++) {
			haveReferenceLocs3d[idx] = false;
			haveReferenceLocs[0][idx] = haveReferenceLocs[1][idx] = false;
		}
		zScaling[0][0] = 0; // these numbers are estimated from the forward-backwards videos
		zScaling[0][1] = 200;
		zScaling[1][0] = 25;
		zScaling[1][1] = 240;
		zScaling[2][0] = 45;
		zScaling[2][1] = 280;
		zScaling[3][0] = 100;
		zScaling[3][1] = 400;
		zScaling[4][0] = 350;
		zScaling[4][1] = 3900;
		for (int idx = 0; idx < 5; idx++) {
			zScaling[idx][2] = (float)(Math::Sqrt(zScaling[idx][1]));
		}

}
bool geometryFile::read(char *geometryfilename) {
	bool retv = false;
	ifstream geometryFile(geometryfilename);
	if (geometryFile.is_open()) {
		vector3f *tmprec = parseLine3f(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
		cameraLosc3d[0].copy(*tmprec); delete tmprec;
		tmprec = parseLine3f(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
		cameraLosc3d[1].copy(*tmprec); delete tmprec;
		for (int idx = 0; idx < 4; idx++) {
			tmprec = parseLine3f(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
			referenceLocs3d[idx].copy(*tmprec); delete tmprec;
			haveReferenceLocs3d[idx] = true;

		}
		pixelLoc *tmpPixelLoc;
		for (int idx1 = 0; idx1 < 2; idx1++) {
			for (int idx2 = 0; idx2 < 4; idx2++) {
				tmpPixelLoc = parseLine2i(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
				referenceLocs[idx1][idx2].x = tmpPixelLoc->x;
				referenceLocs[idx1][idx2].y = tmpPixelLoc->y;
				delete tmpPixelLoc;
				haveReferenceLocs[idx1][idx2] = true;
			}
		}
		timeLagV2WRTV1 = parseLine1f(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile)[0]);
		for (int idx = 0; idx < ZSCALING_SIZE; idx++) {
			std::vector<std::string> lineCSV = getNextLineAndSplitIntoTokensIgnoreComment(geometryFile);
			if (lineCSV.size() != 2)
				break;
			zScaling[idx][0] = parseLine1f(lineCSV[0]);
			zScaling[idx][1] = parseLine1f(lineCSV[1]);
			zScaling[idx][2] = (float)(Math::Sqrt(zScaling[idx][1]));
		}

		geometryFile.close();
		retv = true;
	}
	return retv;
}
bool geometryFile::write(char *geometryfilename) {
	ofstream locFile(geometryfilename, ios::out);
	locFile << cameraLosc3d[0].dx << "," << cameraLosc3d[0].dy << "," << cameraLosc3d[0].dz << "\n";
	locFile << cameraLosc3d[1].dx << "," << cameraLosc3d[1].dy << "," << cameraLosc3d[1].dz << "\n";
	for (int idx = 0; idx < 4; idx++) {
		locFile << referenceLocs3d[idx].dx << "," << referenceLocs3d[idx].dy << "," << referenceLocs3d[idx].dz << "\n";
	}
	for (int idx1 = 0; idx1 < 2; idx1++) {
		for (int idx2 = 0; idx2 < 4; idx2++) {
			locFile << referenceLocs[idx1][idx2].x << "," << referenceLocs[idx1][idx2].y << "\n";
		}
	}
	locFile << timeLagV2WRTV1 << "\n";
	for (int idx = 0; idx < ZSCALING_SIZE; idx++) {
		locFile << zScaling[idx][0] << "," << zScaling[idx][1] << "\n";
	}
	locFile.close();
	return true;
}
void geometryFile::getPixelLoc(int pict0Or1, int loc0to3, int *pixelX, int *pixelY) {
	*pixelX = referenceLocs[pict0Or1][loc0to3].x;
	*pixelY = referenceLocs[pict0Or1][loc0to3].y;
}
void geometryFile::setRefLocation(int menuChoice, int img0or1, int x, int y) {
	referenceLocs[img0or1][menuChoice].x = x;
	referenceLocs[img0or1][menuChoice].y = y;
	haveReferenceLocs[img0or1][menuChoice] = true;
}

bool geometryFile::haveAllRefPoints() {
	bool retv = true;
	for (int initIdx0 = 0; initIdx0 < 2; initIdx0++) {
		for (int initIdx1 = 0; initIdx1 < 2; initIdx1++) {
			retv = retv && haveReferenceLocs[initIdx0][initIdx1];
		}
	}
	for (int initIdx1 = 0; initIdx1 < 2; initIdx1++) {
		retv = retv && haveReferenceLocs3d[initIdx1];
	}
	return retv;
}

void geometryFile::setLocations(vector3f *camera1loc, vector3f *camera2loc, vector3f *ref1loc, vector3f *ref2loc, vector3f *ref3loc, vector3f *ref4loc, float frameOffset) {
	cameraLosc3d[0].copy(*camera1loc);
	cameraLosc3d[1].copy(*camera2loc);
	referenceLocs3d[0].copy(*ref1loc);
	haveReferenceLocs3d[0] = true;
	referenceLocs3d[1].copy(*ref2loc);
	haveReferenceLocs3d[1] = true;
	referenceLocs3d[2].copy(*ref3loc);
	haveReferenceLocs3d[2] = true;
	referenceLocs3d[3].copy(*ref4loc);
	haveReferenceLocs3d[3] = true;
	timeLagV2WRTV1 = frameOffset;
}
void geometryFile::getLocations(vector3f *camera1loc, vector3f *camera2loc, vector3f *ref1loc, vector3f *ref2loc, vector3f *ref3loc, vector3f *ref4loc, float *frameOffset) {
	camera1loc->copy(cameraLosc3d[0]);
	camera2loc->copy(cameraLosc3d[1]);
	ref1loc->copy(referenceLocs3d[0]);
	ref2loc->copy(referenceLocs3d[1]);
	ref3loc->copy(referenceLocs3d[2]);
	ref4loc->copy(referenceLocs3d[3]);
	*frameOffset = timeLagV2WRTV1;
}
// ********************************************************
geometry2dFile::geometry2dFile() {
	for (int idx = 0; idx < 4; idx++) {
		haveReferenceLocs2d[idx] = false;
		haveReferenceLocs[idx] = haveReferenceLocs[idx] = false;
	}

}
bool geometry2dFile::read(char *geometryfilename, std::vector<std::vector<std::string>>   *lineArrays) {
	bool retv = false;
	ifstream geometryFile(geometryfilename);
	if (geometryFile.is_open()) {
		for (int idx = 0; idx < 4; idx++) {
			vector2f *tmprec = parseLine2f(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
			referenceLocs2d[idx].copy(*tmprec); delete tmprec;
			haveReferenceLocs2d[idx] = true;

		}
		pixelLoc *tmpPixelLoc;
		for (int idx2 = 0; idx2 < 4; idx2++) {
			tmpPixelLoc = parseLine2i(getNextLineAndSplitIntoTokensIgnoreComment(geometryFile));
			referenceLocs[idx2].x = tmpPixelLoc->x;
			referenceLocs[idx2].y = tmpPixelLoc->y;
			delete tmpPixelLoc;
			haveReferenceLocs[idx2] = true;
		}
		bool subsequentLine = true;
		do {
			vector<string> oneLine = getNextLineAndSplitIntoTokensIgnoreComment(geometryFile);
			if (oneLine.size() <= 2)
				subsequentLine = false;
			lineArrays->push_back(oneLine);
		} while (subsequentLine);

		geometryFile.close();
		retv = true;
	}
	return retv;
}
bool geometry2dFile::write(char *geometryfilename, std::vector<std::string>   *lineArray) {
	ofstream locFile(geometryfilename, ios::out);
	for (int idx = 0; idx < 4; idx++) {
		locFile << referenceLocs2d[idx].dx << "," << referenceLocs2d[idx].dy<< "\n";
	}
	for (int idx2 = 0; idx2 < 4; idx2++) {
		locFile << referenceLocs[idx2].x << "," << referenceLocs[idx2].y << "\n";
	}
	for each(string str in *lineArray) {
		locFile << str << "\n";
	}
	locFile.close();
	return true;
}
void geometry2dFile::getPixelLoc(int loc0to3, int *pixelX, int *pixelY) {
	*pixelX = referenceLocs[loc0to3].x;
	*pixelY = referenceLocs[loc0to3].y;
}
void geometry2dFile::setRefLocation(int loc0to3, int x, int y) {
	referenceLocs[loc0to3].x = x;
	referenceLocs[loc0to3].y = y;
	haveReferenceLocs[loc0to3] = true;
}

bool geometry2dFile::haveAllRefPoints() {
	bool retv = true;
	for (int initIdx1 = 0; initIdx1 < 2; initIdx1++) {
		retv = retv && haveReferenceLocs[initIdx1];
	}
	for (int initIdx1 = 0; initIdx1 < 2; initIdx1++) {
		retv = retv && haveReferenceLocs2d[initIdx1];
	}
	return retv;
}

void geometry2dFile::setLocations(vector2f *ref1loc, vector2f *ref2loc, vector2f *ref3loc, vector2f *ref4loc) {
	referenceLocs2d[0].copy(*ref1loc);
	haveReferenceLocs2d[0] = true;
	referenceLocs2d[1].copy(*ref2loc);
	haveReferenceLocs2d[1] = true;
	referenceLocs2d[2].copy(*ref3loc);
	haveReferenceLocs2d[2] = true;
	referenceLocs2d[3].copy(*ref4loc);
	haveReferenceLocs2d[3] = true;
}
void geometry2dFile::getLocations(vector2f *ref1loc, vector2f *ref2loc, vector2f *ref3loc, vector2f *ref4loc) {
	ref1loc->copy(referenceLocs2d[0]);
	ref2loc->copy(referenceLocs2d[1]);
	ref3loc->copy(referenceLocs2d[2]);
	ref4loc->copy(referenceLocs2d[3]);
}






