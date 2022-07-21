#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <list>

#include "CVcommon.h"

using namespace cv;
using namespace std;

float magnitudeSq(Point2f v);
float magnitude(Point2f v);
float normalisedDotProduct(Point2f v1, Point2f v2);
bool isWithin(Point2f comn, Point2f opt0, Point2f opt1, Point2f target);
float minDistPointToLine(Point2f pt, Point2f ln0, Point2f ln1);
float triangleDistFraction(Point2f spot, Point2f midmid, Point2f wallmid, Point2f apex, float *frctionMidMid);
float triangleDistFraction(Point2f spot, Point2f midmid, Point2f p0, Point2f p1, Point2f p2);
float wallDistanceFraction(Point2f spot, Point2f midmid, Point2f wall0, Point2f wall1, Point2f pit, float *frctionMidMid);
float angleFromVector(Point2f vec);
float angleSubtraction(float angle0, float angle1);
float anglePossitiveAcute(float angle);
float anglePossitiveObtuse(float angle);
float triangleArea(Point2f a0, Point2f a1, Point2f a2);
public class geometryDesc
{
public:
    enum shape {none, wallEnd, straight, bent, pitgap, bulge, bulge2, rim1pit, rim2pits, pitcell1, pitcell2, zigzag, curve, SHAPE_QTY};
	// rim1pit , centre of pit and up to 3 intersections start/end in wallEnd0-wallEnd1, builtWall00-01, 10-11 .. each assumed to run clockwise
    shape mShape;
    int mHeight;
    int mDeposit;
    Point2f wallEnd0;
    Point2f wallEnd1;
    Point2f bentCentre;
    Point2f pit0; //  wallend may have only one of these set
    Point2f pit1;
    bool pit1Used;
    Point2f pit2;
    bool pit2Used;
    Point2f builtWall00;
    Point2f builtWall01;
    Point2f builtWall10; // only used for wallend second wall of fork, and 2nd wall of bend
    Point2f builtWall11;
    bool builtWall1Used;
	bool mRandom;

    geometryDesc();
	static std::string geometryDesc::serialiseAreaCaptions();
    void clone(geometryDesc& peer);
    std::string serialiseCalcs(const char *exptID, float scale);
    std::string serialise();
    bool deserialise(std::vector<std::string>& valStrArray);
	static std::string geometryDesc::serialiseBentCaption();
	static std::string geometryDesc::serialiseBulgeAreaCaption();
	static std::string geometryDesc::serialiseBulge2AreaCaption();
	std::string geometryDesc::serialiseBulgeArea();
	std::string geometryDesc::serialiseBulge2Area();
	static std::string geometryDesc::serialisePitCellArea1Caption();
	static std::string geometryDesc::serialisePitCellArea2Caption();
	static std::string geometryDesc::serialiseRim1PitAreaCaption();
	static std::string geometryDesc::serialiseRim2PitsAreaCaption();
	static std::string geometryDesc::serialiseZigzagCaption();
	static std::string geometryDesc::serialiseCurveCaption();
	std::string geometryDesc::serialiseBent(float scale);
	std::string geometryDesc::serialisePitCellArea1(float scale);
	std::string geometryDesc::serialisePitCellArea2(float scale);
	std::string geometryDesc::serialiseTwoPitsCellArea(float scale, Point2f p0, float rad0, Point2f p1, float rad1, Point2f cell0, float cellRad);
	std::string geometryDesc::serialiseRim1PitArea(float scale);
	std::string geometryDesc::serialiseRim2PitsArea(float scale);
	std::string geometryDesc::serialiseZigzagArea(float scale);
	std::string geometryDesc::serialiseCurveArea(float scale);
	bool isEmpty();
    bool isEmpty(Point pt);
	Point getRefLoc();
    void emptyEntry();
	static float instantCellTwoPits(float pitCentreToCellWall, float cellRad, float pitRad0, float pitSep);
	static float integralCellTwoPits(float cellRad, float pitRad0, float pitSep);
	//float cresentArea(float scale, Point2f pit0, Point2f pit1, float cellRad, float pitCentreToWall, float pitSeparation);
	static float cresentAreaAsym(float scale, float cellRad, float pitCentreToWall, float pitSeparation);
	static float cresentArea(float scale, float cellRad, float pitSeparation);
	static float cresentAreaAsym(float cellRad, float pitCentreToWall, float pitSeparation);
	static float cresentArea(float cellRad, float pitSeparation);
	//float angleFromVector(Point2f vec);
	//float geometryDesc::angleSubtraction(float angle0, float angle1);
	//float geometryDesc::anglePossitiveAcute(float angle);
	//float geometryDesc::anglePossitiveObtuse(float angle);
private:
	static float stage1curve;
	static int curveStage;
	static bool isFirstStageCurve;
};

