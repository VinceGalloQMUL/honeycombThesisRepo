#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"
#include "perspective.h"

using namespace cv;
using namespace std;

#define MENU_BROODIDX_EGG	0
#define MENU_BROODIDX_LARVA	1
#define MENU_BROODIDX_PUPA	2
#define MENU_BROODIDX_NOT_EGG	3
#define MENU_BROODIDX_NOT_LARVA	4
#define MENU_BROODIDX_NOT_PUPA	5
#define MENU_BROODIDX_EGG_NOT_LARVA	6
#define MENU_BROODIDX_PUPA_NOT_LARVA	7
#define MENU_BROODIDX_LARVA_NOT_PUPA	8
#define MENU_BROODIDX_LARVA_NOT_EGG	9
#define MENU_BROODIDX_QTY	10
#define BROODIDX_OFFSET_DRONE 20
#define BROODIDX_OFFSET_DELTA 40
#define MENU_BROODIDX_DELTA_LARVA	0
#define MENU_BROODIDX_DELTA_PUPA	1
#define MENU_BROODIDX_DELTA_NOT_PUPA	2

#define FUDGEV 1.05f
#define FUDGEH 1.00f
#define NUMCELLSWIDTH_ACTUAL (61 * FUDGEH)
#define NUMCELLSHEIGHTSUPER (20.5f * FUDGEV)
#define NUMCELLSHEIGHTBROOD_ACTUAL (34.6f * FUDGEV)
#define FENCEPOSTROUNDING 1.5f
#define NUMCELLSHEIGHTBROODDISP (NUMCELLSHEIGHTBROOD_ACTUAL * 1.2f)
#define NUMCELLSWIDTHDISP (NUMCELLSWIDTH_ACTUAL * 1.2f)
public class areaDesc
{
public:
    enum shape {point, line, tri, quad};
	bool mDrone;
	bool mDelta;
	int mType;
	int mArea;
    shape mShape;
    Point2f pt0;
    Point2f pt1;
    Point2f pt2;
    Point2f pt3;
    Point2f Zpt0;
    Point2f Zpt1;
    Point2f Zpt2;
    Point2f Zpt3;
	float linearCellGapMultiplier;

    areaDesc();
    void clone(areaDesc& peer);
    std::string serialise();
	std::string serialiseAdjusted(perspective persp, float dispDivision, int cellQty, Point ref0, Point refH, Point refD, int pxlsH, int pxlsV);
    bool deserialise(std::vector<std::string>& valStrArray);
    bool isEmpty();
    bool isEmpty(Point pt);
    void emptyEntry();

	int calcArea(perspective persp, float dispDivision, int pxlsH, int pxlsV);
	int calcArea(perspective persp, float dispDivision, int pxlsH, int pxlsV, std::vector<Point2f>& outPtArray);
	int areaDesc::calcAreaLine(Point pt0, Point pt1, float pixelsPerCellHor, float pixelsPerCellDiag);
	int areaDesc::calcAreaTri(Point pt0, Point pt1, Point pt2, float pixelsPerCellWidth, float pixelsPerCellHeight);
	int areaDesc::calcAreaQuad(Point pt0, Point pt1, Point pt2, Point pt3, float pixelsPerCellWidth, float pixelsPerCellHeight);
	int areaDesc::calcAreaLine(Point pt0, Point pt1, float pixelsPerCellHor, float pixelsPerCellDiag, std::vector<Point2f>& outPtArray);
	int areaDesc::calcAreaTri(Point pt0, Point pt1, Point pt2, float pixelsPerCellWidth, float pixelsPerCellHeight, std::vector<Point2f>& outPtArray);
	int areaDesc::calcAreaQuad(Point pt0, Point pt1, Point pt2, Point pt3, float pixelsPerCellWidth, float pixelsPerCellHeight, std::vector<Point2f>& outPtArray);
	static float areaDesc::calcLengthCells(Point pt0, Point pt1, bool isDrone, float pixelsPerCellHor, float pixelsPerCellHeight, float linearMultiplier);
	float calcGradient(Point pt0, Point pt1);
	float calcLength(Point pt0, Point pt1);
	static float areaDesc::calcLengthWidth(Point pt0, Point pt1);
	static float areaDesc::calcLengthHeight(Point pt0, Point pt1);
	bool isHorizontal(Point pt0, Point pt1, float gradH, float gradD);
};

