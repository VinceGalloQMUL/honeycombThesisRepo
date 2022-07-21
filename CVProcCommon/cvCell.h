#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "adjustableDisplay.h"
using namespace cv;
using namespace std;

class cvCellPoint {
public:
	 enum flexability {empty, calc, supplied, fixed};
	 flexability mFlex = empty;
	 Point2f mLoc;
};
class cvCellPointWithID {
public:
	int mId;
	cvCellPoint point;
	cvCellPointWithID(int id);
};
class cvCellDataStore
{
public:
	vector<vector<cvCellPointWithID>> mWallCentres[2];
	void setWallCentre(int cellID0, int cellID1, Point2f point, cvCellPoint::flexability flex);
	cvCellPoint::flexability getWallCentre(int cellID, int peerID, Point2f *pPoint);
	void addCell(int cellID);
	string cvCellDataStore::serialise();
	int cvCellDataStore::deserialise(std::vector<std::string>   tokenised, int tokenIdx);
	void clearWallCentres(int cellID);
};

typedef bool fnGetWallCentre(int imageID, int cellID0, int cellID1, Point2f *pPoint);
typedef  fnGetWallCentre (*pGetWallCentre);
typedef void fnSetWallCentre(int imageID, int cellID0, int cellID1, Point2f point);
typedef  fnSetWallCentre (*pSetWallCentre);

class cvCell
{
public:
	int mID;
	vector<cvCell> *mAllCells;
	vector<Point > mContours;
	Point2f  mCentre;
	vector<int> mConnectionIDs;
	vector<Point2f > mConnections;
	vector<float> mAngles;
	vector<float> mSeparations;
	vector<int> mSortedConnections;
	vector<Point2f> mWallCentres;
	vector<Point2f> mWallVectors;
	vector<Point2f> mCorners;
	float mRadius;
	float mAverageWallThickness;
	cvCell(int imageID, vector<cvCell> *allCells, vector<Point > contours, int theID, vector<Point2f> autoCellCentres, vector<vector<int >> &connections, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, vector<int> &minusCellCentres, vector<Point2i> &minusCellConnections, pGetWallCentre getWallCentre );
	cvCell(vector<cvCell> *allCells, vector<Point > contours, int theID, vector<Point2f> autoCellCentres, vector<vector<int >> &connections, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, vector<int> &minusCellCentres, vector<Point2i> &minusCellConnections, adjustableDisplay *adjDisp );
	cvCell(int imageID, vector<cvCell> *allCells, int theID, vector<Point2f> autoCellCentres, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, pGetWallCentre getWallCentre);
	cvCell(vector<cvCell> *allCells, int theID, vector<Point2f> autoCellCentres, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, adjustableDisplay *adjDisp);
	~cvCell();
	void calcWallCentres();
	bool reCalcWallCentres();
	void calcWallVectors();
	void calcWallCorners();
	void reCalcWithPeerDependencies(int imageID, int occurance, fnSetWallCentre setWallCentre);
	void setWallCentres(int imageID, fnSetWallCentre setWallCentre);
	Point2f calcContourConnIntersect(int i);
	Point2f calcCorner(int idx0, int idx1);
	void sortConnections();
	int findPeerCell(int peerID);
	Point2f getWallTowardsMe(int requester);
	float getAverageWallThickness();
	void calcAverageWallThickness();
	string serialise(float pixelsPerMM);
	static string serialisedHeadings();
	float area();

	static bool cellHasConnections(int theID, vector<vector<int >> &connections, vector<Point2i> &addCellConnections, vector<Point2i> &minusCellConnections);
	static void cvCell::removeEntry(vector<int> &toclean, int valToRemove);

	Point2f intersectLine(Point2f line_start, Point2f line_end, Point2f point);
	Point2f intersectLines(Point2f line_start0, Point2f line_vect0, Point2f line_start1, Point2f line_vect1);
};

