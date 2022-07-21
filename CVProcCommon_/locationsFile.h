#pragma once
#include <queue>
#include "CVcommon.h"

#include <csvFile.h>
using namespace std;

struct locHeader {
	// 	file_type(0 = point2dPxls,1 = point2dVectorPxls), originX, originY, width, height
	int fileType;
	int originX;
	int originY;
	int width;
	int height;
};
class locationsFile : csvFile
{
public:
	struct locHeader hdr;
	bool locationsFile::parseHeader(std::vector<std::string>   lineArray);
	bool locationsFile::parseHeaderStereo(std::vector<std::string>   lineArray, float *x, float *y);
	void locationsFile::setHeader(int fileType, int orgX, int orgY, int w, int h);
	point2dVectorPxls *parseLine(std::vector<std::string>   lineArray);
	point3dMm *parseLine3dMm(std::vector<std::string>   lineArray);
	vector<point2dVectorPxls> * read(char *filepathname, bool interpolate);
	vector<point3dMm> * read3dMm(char *filepathname, float *BLx, float *BLy, float *TRx, float *TRy);
//	vector<point2dPxls> * readAsPoints(char *filepathname, bool interpolate);
	static void copyTo(point2dVectorPxls *pSrce, point2dVectorPxls *pDest);
	static void write(char *filepathname, vector<point2dPxls> *pLocs, bool halfSize, int zoneOrgX = 0, int zoneOrgY = 0, int width = 0, int height = 0);
	static void write(char *filepathname, vector<point2dVectorPxls> *pLocs, bool halfSize, int zoneOrgX, int zoneOrgY, int width, int height);
	void locationsFile::write(char *filepathname, vector<point3dMm> *pLocs, float cornerBLx, float cornerBLy, float cornerTRx, float cornerTRy);
	static string locationsFile::write(point3dMm *pLoc);
	static string locationsFile::writeCaption();
	vector2f *calcFlightpath(vector<point2dVectorPxls> *pLocs, int idxS, int idxE, float pixelsPerMM, float framesPerSecond);
	vector3f *locationsFile::calcFlightpath(vector<point3dMm> *pLocs, int idxS, int idxE);
	void calcFlightpaths(vector<point2dVectorPxls> *pLocs, float pixelsPerMM, float framesPerSecond, int range, float *minSpeed, float *maxSPeed);
	void calcFlightpaths(vector<point3dMm> *pLocs, int range, float *minSpeed, float *maxSPeed);
	static int findFrame(int frameNum, vector<point2dVectorPxls> *pLocs, point2dVectorPxls *loc);
	static int previousOrNextClassificationSection(int frameNum, vector<point2dVectorPxls> *pLocs, int plusOrMinus, bool reportCurrentSection);
	static void setSectionClass(int idxCurrentSectionStart, int idxCurrentSectionEnd, vector<point2dVectorPxls> *pLocs, int classType);
	static void moveSectionEdge(int idxSectionEdge, int plusOrMinus, vector<point2dVectorPxls> *pLocs);
	static void setSectionClass(int idxPeriodStart, int *idxPeriodEnd, int *wasClassType, int classType, int frameCount, vector<point2dVectorPxls> *pLocs);
	static float sizeAverage(vector<point2dVectorPxls> *pLocs);
	static float sizeAverage(vector<point3dMm> *pLocs);
	static float getAdvisorySizeLimit(vector<point2dVectorPxls> *pLocs, float *minSize, float *maxSize);
	static float getAdvisorySizeLimit(vector<point3dMm> *pLocs, float *minSize, float *maxSize);
	void recalcSpeed(point3dMm *currentRec, point3dMm *prevRec);

	locationsFile();
	~locationsFile();
};

#define ZSCALING_SIZE 5
class geometryFile : csvFile
{
public:
	vector3f cameraLosc3d[2];
	vector3f referenceLocs3d[4];
	pixelLoc referenceLocs[2][4];
	bool haveReferenceLocs[2][4];
	bool haveReferenceLocs3d[4];
	float zScaling[ZSCALING_SIZE][3];
	float timeLagV2WRTV1;
	bool read(char *filepathname);
	bool write(char *filepathname);
	void geometryFile::getPixelLoc(int pict0Or1, int loc0to3, int *pixelX, int *pixelY);
	void geometryFile::setRefLocation(int menuChoice, int img0or1, int x, int y);
	bool geometryFile::haveAllRefPoints();
	void geometryFile::setLocations(vector3f *camera1loc, vector3f *camera2loc, vector3f *ref1loc, vector3f *ref2loc, vector3f *ref3loc, vector3f *ref4loc, float frameOffset);
	void geometryFile::getLocations(vector3f *camera1loc, vector3f *camera2loc, vector3f *ref1loc, vector3f *ref2loc, vector3f *ref3loc, vector3f *ref4loc, float *frameOffset);

	geometryFile();
};

class geometry2dFile : csvFile
{
public:
	vector2f referenceLocs2d[4];
	pixelLoc referenceLocs[4];
	bool haveReferenceLocs[4];
	bool haveReferenceLocs2d[4];
	bool read(char *filepathname, std::vector<std::vector<std::string>>   *lineArrays);
	bool write(char *filepathname, std::vector<std::string>   *lineArray);
	void geometry2dFile::getPixelLoc(int loc0to3, int *pixelX, int *pixelY);
	void geometry2dFile::setRefLocation(int loc0to3, int x, int y);
	bool geometry2dFile::haveAllRefPoints();
	void geometry2dFile::setLocations(vector2f *ref1loc, vector2f *ref2loc, vector2f *ref3loc, vector2f *ref4loc);
	void geometry2dFile::getLocations(vector2f *ref1loc, vector2f *ref2loc, vector2f *ref3loc, vector2f *ref4loc);

	geometry2dFile();
};
