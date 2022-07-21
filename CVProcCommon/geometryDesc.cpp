#include "geometryDesc.h"

float geometryDesc::stage1curve;
int geometryDesc::curveStage;
bool geometryDesc::isFirstStageCurve;

geometryDesc::geometryDesc() {
    emptyEntry();
}
void geometryDesc::clone(geometryDesc& peer) {
    mShape = peer.mShape;
    mHeight = peer.mHeight;
    mDeposit = peer.mDeposit;
    wallEnd0 = peer.wallEnd0;
    wallEnd1 = peer.wallEnd1;
    bentCentre = peer.bentCentre;
    pit0 = peer.pit0;
    pit1 = peer.pit1;
    pit2 = peer.pit2;
    pit1Used = peer.pit1Used;
    pit2Used = peer.pit2Used;
    builtWall00 = peer.builtWall00;
    builtWall01 = peer.builtWall01;
    builtWall10 = peer.builtWall10;
    builtWall11 = peer.builtWall11;
    builtWall1Used = peer.builtWall1Used;
	mRandom = peer.mRandom;
}
std::string geometryDesc::serialise() {
    std::stringstream ss;
    ss << mShape << "," << mHeight << "," << mDeposit << "," << wallEnd0.x << "," << wallEnd0.y;
    ss << "," << wallEnd1.x << "," << wallEnd1.y;
    ss << "," << bentCentre.x << "," << bentCentre.y;
    ss << "," << pit0.x << "," << pit0.y;
    ss << "," << pit1.x << "," << pit1.y << "," << pit1Used ? "1" : "0";
    ss << "," << pit2.x << "," << pit2.y << "," << pit2Used ? "1" : "0";
    ss << "," << builtWall00.x << "," << builtWall00.y;
    ss << "," << builtWall01.x << "," << builtWall01.y;
    ss << "," << builtWall10.x << "," << builtWall10.y;
	ss << "," << builtWall11.x << "," << builtWall11.y << "," << builtWall1Used ? "1" : "0";
	ss << "," << mRandom ? "1" : "0";
	return ss.str();
}

std::string geometryDesc::serialiseCalcs(const char *exptID, float scale) {
	std::stringstream ss;
	if (mShape == geometryDesc::bent) {
		ss << mShape << ", " << exptID << ", " << serialiseBent(scale);
	}
	if (mShape == geometryDesc::bulge) {
		ss << mShape << ", " << exptID << ", " << serialiseBulgeArea();
	}
	if (mShape == geometryDesc::bulge2) {
		ss << mShape << ", " << exptID << ", " << serialiseBulge2Area();
	}
	if (mShape == geometryDesc::rim1pit) {
		ss << mShape << ", " << exptID << ", " << serialiseRim1PitArea(scale);
	}
	if (mShape == geometryDesc::rim2pits) {
		ss << mShape << ", " << exptID << ", " << serialiseRim2PitsArea(scale);
	}
	if (mShape == geometryDesc::pitcell1) {
		ss << mShape << ", " << exptID << ", " << serialisePitCellArea1(scale);
	}
	if (mShape == geometryDesc::pitcell2) {
		ss << mShape << ", " << exptID << ", " << serialisePitCellArea2(scale);
	}
	if (mShape == geometryDesc::zigzag) {
		ss << mShape << ", " << exptID << ", " << serialiseZigzagArea(scale);
	}
	if (mShape == geometryDesc::curve) {
		ss << mShape << ", " << exptID << ", " << serialiseCurveArea(scale);
	}
	return ss.str();
}

float magnitudeSq(Point2f v) {
	return (v.x * v.x + v.y * v.y);
}
float magnitude(Point2f v) {
	float magnitude = Math::Sqrt(magnitudeSq(v));
	return magnitude;
}
float normalisedDotProduct(Point2f v1, Point2f v2) {
	return v1.dot(v2) / (magnitude(v1) * magnitude(v2));
}
bool isWithin(Point2f comn, Point2f opt0, Point2f opt1, Point2f target) {
	float dotOpt0 = normalisedDotProduct((opt0 - comn), (target - comn));
	float dotOpt1 = normalisedDotProduct((opt0 - comn), (opt1 - comn));
	float dotOpt2 = normalisedDotProduct((target - comn), (opt1 - comn));
	return ((dotOpt0 > dotOpt1) && (dotOpt2 > dotOpt1));
}
float minDistPointToLine(Point2f pt, Point2f ln0, Point2f ln1) {
	float lineA;
	float lineB;
	float dist;
	if (ln0.x == ln1.x) {
		dist = (ln0.x - pt.x );

	}
	else {
		lineA = (ln0.y - ln1.y) / (ln0.x - ln1.x);
		lineB = ln0.y - lineA * ln0.x;
		dist = (lineA * pt.x - pt.y + lineB) / Math::Sqrt(lineA * lineA + 1);

	}
	return Math::Abs(dist);
}

float triangleArea(Point2f a0, Point2f a1, Point2f a2) {
	float h = minDistPointToLine(a2, a0, a1);
	float l = magnitude(a1 - a0);
	return h * l / 2;
}

#define Z_0_M01 0
#define Z_M01_1 1
#define Z_1_M12 2
#define Z_M12_2 3
#define Z_2_M20 4
#define Z_M20_0 5

float triangleDistFraction(Point2f spot, Point2f midmid, Point2f wallmid, Point2f apex, float *frctionMidMid) {
	float distSpotToMidMid = magnitude(midmid - spot);
	float distApexToMidMid = magnitude(midmid - apex);
	float tmpRatio = distSpotToMidMid / distApexToMidMid;
	*frctionMidMid = Math::Min(tmpRatio, 1.0f);
	//*likelyMidMid = (proportion * proportion);
	float distToMidline = minDistPointToLine(spot, wallmid, midmid);
	float distToApex = minDistPointToLine(apex, wallmid, midmid);
	tmpRatio = distToMidline / distToApex;
//	return (proportion - (proportion * proportion) / 2) * 2;
	return Math::Min(tmpRatio, 1.0f);
}
float triangleDistFraction(Point2f spot, Point2f midmid, Point2f p0, Point2f p1, Point2f p2) {
	float distSpotToMidMid = magnitude(midmid - spot);
	float distPtToMid = (magnitude(midmid - p0) + magnitude(midmid - p1) + magnitude(midmid - p2)) / 3.0f;
	float tmpRatio = distSpotToMidMid / distPtToMid;
	return Math::Min(tmpRatio, 1.0f);
}
float wallDistanceFraction(Point2f spot, Point2f midmid, Point2f wall0, Point2f wall1, Point2f pit, float *frctionMidMid) {
	float distToWall = minDistPointToLine(spot, midmid, wall0);
	float distToWall1 = minDistPointToLine(spot, midmid, wall1);
	distToWall = Math::Min(distToWall, distToWall);
	float distSpotToMid = magnitude(midmid - spot);
	float midToPit = magnitude(midmid - pit);
	*frctionMidMid = distSpotToMid / midToPit;
	return distToWall/midToPit;
}


float angleFromVector(Point2f vec) {
	// NB Y is from top down, my geometric convention is increasing upwards

	float angle = (vec.y > 0) ? Math::PI * 3 / 2 : Math::PI / 2;
	if (vec.x != 0) {
		angle = Math::Atan(-vec.y / vec.x);	// NB Y is from top down, my geometric convention is increasing upwards, hence minus sign
	}
	if (vec.x < 0) angle += Math::PI;
	if (angle < 0) angle += 2 * Math::PI;
	return angle;
}
float angleSubtraction(float angle0, float angle1) {
	float retv = angle0 - angle1;
	if (retv < 0) retv += 2 * Math::PI;
	return retv;
}
float anglePossitiveAcute(float angle) {
	float retv = angle;
	if (retv < 0) retv += 2 * Math::PI;
	if (retv > Math::PI) retv -= Math::PI;
	if (retv > Math::PI / 2) retv = Math::PI - retv;
	return retv;
}
float anglePossitiveObtuse(float angle) {
	float retv = angle;
	if (retv < 0) retv += 2 * Math::PI;
	if (retv > Math::PI) {
		retv = retv - Math::PI;;
	}
	else {
		retv = Math::PI - retv;
	}
	return retv;
}

std::string geometryDesc::serialiseAreaCaptions() {
	std::stringstream ss;
	ss << shape::bent << ", " << serialiseBentCaption() << "\n";
	ss << shape::bulge << ", " << serialiseBulgeAreaCaption() << "\n";
	ss << shape::bulge2 << ", " << serialiseBulge2AreaCaption() << "\n";
	ss << shape::rim1pit << ", " << serialiseRim1PitAreaCaption() << "\n";
	ss << shape::rim2pits << ", " << serialiseRim2PitsAreaCaption() << "\n";
	ss << shape::pitcell1 << ", " << serialisePitCellArea1Caption() << "\n";
	ss << shape::pitcell2 << ", " << serialisePitCellArea2Caption() << "\n";
	ss << shape::zigzag << ", " << serialiseZigzagCaption() << "\n";
	ss << shape::curve << ", " << serialiseCurveCaption() << "\n";
	return ss.str();

}

std::string geometryDesc::serialiseBulgeArea() {
    std::stringstream ss;
	Point2f mid01 = (pit0 + pit1) / 2;
	Point2f mid12 = (pit1 + pit2) / 2;
	Point2f mid20 = (pit2 + pit0) / 2;
	Point2f midmid = (pit2 + pit1 + pit0) / 3;
	if (!isEmpty(wallEnd0))
		midmid = wallEnd0;
	bool isWithin0_m01_EFA = isWithin(pit2, pit0, mid01, bentCentre);
	bool isWithin1_m12_ABC = isWithin(pit0, pit1, mid12, bentCentre);
	bool isWithin2_m20_CDE = isWithin(pit1, pit2, mid20, bentCentre);
	int zone = (isWithin0_m01_EFA) ? ((isWithin1_m12_ABC) ? Z_0_M01 : ((isWithin2_m20_CDE) ? Z_2_M20 : Z_M20_0)) : ((isWithin1_m12_ABC) ? ((isWithin2_m20_CDE) ? Z_1_M12 : Z_M01_1) : Z_M12_2);
	float wallDistFraction, midDistFraction;
	switch (zone) {
	case Z_0_M01:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid01, pit0, &midDistFraction);
		break;
	case Z_M01_1:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid01, pit1, &midDistFraction);
		break;
	case Z_1_M12:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid12, pit1, &midDistFraction);
		break;
	case Z_M12_2:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid12, pit2, &midDistFraction);
		break;
	case Z_2_M20:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid20, pit2, &midDistFraction);
		break;
	case Z_M20_0:
		wallDistFraction = triangleDistFraction(bentCentre, midmid, mid20, pit0, &midDistFraction);
		break;
	}
	//float dot01m = (pit1 - pit0).dot(midmid - pit0);
	//float dot01c = (pit1 - pit0).dot(bentCentre - pit0);
	//float dot12m = (pit2 - pit1).dot(midmid - pit1);
	//float dot12c = (pit2 - pit1).dot(bentCentre - pit1);
	//float dot20m = (pit0 - pit2).dot(midmid - pit2);
	//float dot20c = (pit0 - pit2).dot(bentCentre - pit2);
	//ss << dot01m << ", " << dot01c << ", " << dot12m << ", " << dot12c << ", " << dot20m << ", " << dot20c << ", ";
	ss << wallEnd0.x << ", " << wallEnd0.y << ", " << bentCentre.x << ", " << bentCentre.y;
	ss << ", " << pit0.x << ", " << pit0.y << ", " << pit1.x << ", " << pit1.y << ", " << pit2.x << ", " << pit2.y << ", " ;
	ss << zone << ", " << wallDistFraction << ", " << midDistFraction << ", " << triangleDistFraction(bentCentre, midmid, pit0, pit1, pit2);
	return ss.str();
}

std::string geometryDesc::serialiseBentCaption() {
	return "tag, ID, isRandom, wall0Angle, wall1Angle, pitToPitAngle, builtWallAngle, wallBisectedAngle, pitToPitTangent, deltaBuiltBisected, deltaBuiltToPitTangent, deltaBuiltToPitTangentOverSumDeltas,distApexToWallmm ,distApexToWall2mm ";

}
std::string geometryDesc::serialiseBulgeAreaCaption() {
	return "TripleIntersectX,Y ,BulgeX,Y, Cell0X, Y, Cell1X,Y, Cell2X,Y, WallDistFraction, midDistFraction, TriangDistFraction";
}
std::string geometryDesc::serialiseBulge2Area() {
	std::stringstream ss;
	Point2f mid01 = (pit0 + pit1) / 2;
	Point2f mid12 = (pit1 + pit2) / 2;
	Point2f mid20 = (pit2 + pit0) / 2;
	Point2f midmid = (pit2 + pit1 + pit0) / 3;
	if (!isEmpty(wallEnd0))
		midmid = wallEnd0;
	bool isWithin0_m01_EFA = isWithin(pit2, pit0, mid01, bentCentre);
	bool isWithin1_m12_ABC = isWithin(pit0, pit1, mid12, bentCentre);
	bool isWithin2_m20_CDE = isWithin(pit1, pit2, mid20, bentCentre);
	int zone = (isWithin0_m01_EFA) ? ((isWithin1_m12_ABC) ? Z_0_M01 : ((isWithin2_m20_CDE) ? Z_2_M20 : Z_M20_0)) : ((isWithin1_m12_ABC) ? ((isWithin2_m20_CDE) ? Z_1_M12 : Z_M01_1) : Z_M12_2);
	float wallDistFraction, midDistFraction;
	wallDistFraction = wallDistanceFraction(bentCentre, builtWall00, builtWall01, builtWall10, pit0, &midDistFraction);
	ss << zone << ", " << wallDistFraction << ", " << midDistFraction << ", " << midDistFraction;
	return ss.str();
}
std::string geometryDesc::serialiseBulge2AreaCaption() {
	return "WallDistFraction, midDistFraction, TriangDistFraction";

}
std::string geometryDesc::serialiseRim1PitAreaCaption() {
	return "isRandom, angle00, angle01,angle10, angle11,angle20, angle21,angle0, angle1,angle2, angleSum";
}

std::string geometryDesc::serialiseRim2PitsAreaCaption() {
	return "isRandom, pit0x, pit0y, pit1x, pit1y,w0x, w0y, w1x, w1y";
}

std::string geometryDesc::serialiseZigzagCaption() {
	return "ZigZag, ID, y0, x1, y1, y2, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, a0, a1, a2, a3, a4, a5, a6";
}
std::string geometryDesc::serialiseCurveCaption() {
	curveStage = 0;
	isFirstStageCurve = true;
	return "cID,ID,seq,w30,w31,w32,w3d0,w3d1,w3d,w20,w21,w22,w2d0,w2d1,w2d,w10,w11,w12,w1d0,w1d1,w1d,curve";
}



std::string geometryDesc::serialiseRim1PitArea(float scale) {
	std::stringstream ss;
	Point2f vector00 = wallEnd0 - pit0;
	Point2f vector01 = wallEnd1 - pit0;
	Point2f vector10 = builtWall00 - pit0;
	Point2f vector11 = builtWall01 - pit0;
	Point2f vector20 = builtWall10 - pit0;
	Point2f vector21 = builtWall11 - pit0;
	float angle00 = isEmpty(wallEnd0) ? 0 : (angleFromVector(vector00));
	float angle01 = isEmpty(wallEnd0) ? 0 : (angleFromVector(vector01));
	float angle10 = isEmpty(builtWall00) ? 0 : (angleFromVector(vector10));
	float angle11 = isEmpty(builtWall00) ? 0 : (angleFromVector(vector11));
	float angle20 = isEmpty(builtWall10) ? 0 : (angleFromVector(vector20));
	float angle21 = isEmpty(builtWall10) ? 0 : (angleFromVector(vector21));
	float angle0 = angleSubtraction(angle00, angle01);
	float angle1 = angleSubtraction(angle10, angle11);
	float angle2 = angleSubtraction(angle20, angle21);
	float totAngle = angle0 + angle1 + angle2;
	ss << (mRandom ? "1" : "0") << ", "  << angle00 << ", " << angle01 << ", " << angle10 << ", " << angle11 << ", " << angle20 << ", " << angle21 << ", " << angle0 << ", " << angle1 << ", " << angle2 << ", " << totAngle;
	return ss.str();

}
std::string geometryDesc::serialiseRim2PitsArea(float scale) {
	//	"isRandom, pit0x, pit0y, pit1x, pit1y,w0x, w0y, w1x, w1y";

	std::stringstream ss;
	ss << (mRandom ? "1" : "0") << ", " << pit0.x << ", " << pit0.y << ", " << pit1.x << ", " << pit1.y << ", " << builtWall00.x << ", " << builtWall00.y << ", " << builtWall01.x << ", " << builtWall01.y ;
	return ss.str();

}

std::string geometryDesc::serialisePitCellArea1(float scale) {
	//pit diagonal described by pit0 to wallEnd0, (and pit1 to wallEnd1 for pitcell2 )
	// cell diagonal described by builtWall00 to builtWall01 ( and builtWall10 to builtWall11)
	std::stringstream ss;
	Point2f pitCentre0 = (pit0 + wallEnd0) / 2;
	float pitRad0 = magnitude((pit0 - wallEnd0)) / 2;
	Point2f cellCentre0 = (builtWall00 + builtWall01) / 2;
	float cellRad0 = magnitude((builtWall00 - builtWall01)) / 2;
	float centreOffset0 = magnitude((pitCentre0 - cellCentre0));
	float wallGap0 = Math::Abs(cellRad0 - (centreOffset0 + pitRad0));
	float wallGapFraction0 = wallGap0 / cellRad0;
	float centreOffsetFraction0 = centreOffset0 / cellRad0;
	ss << pitRad0 / scale << ", " << cellRad0 / scale << ", " << centreOffset0 / scale << ", " << wallGap0 / scale << ", " << wallGapFraction0 << ", " << centreOffsetFraction0;
	return ss.str(); 
}

std::string geometryDesc::serialisePitCellArea1Caption() {
	return "pitRad0,cellRad0, centreOffset0, wallGap0, wallGapFraction0, centreOffsetFraction0";
}

std::string geometryDesc::serialiseZigzagArea(float scale) {
	std::stringstream ss;
	ss << wallEnd0.x << ", " << wallEnd0.y << ", "   << wallEnd1.x << ", " << wallEnd1.y << ", "   << bentCentre.x << ", " << bentCentre.y ;
	ss << ", " << pit0.x << ", " << pit0.y << ", "   << pit1.x << ", " << pit1.y << ", "   << pit2.x << ", " << pit2.y ;
	ss << ", " << builtWall00.x << ", " << builtWall00.y << ", "   << builtWall01.x << ", " << builtWall01.y  ;
	ss << ", " << builtWall10.x << ", " << builtWall10.y << ", "   << builtWall11.x << ", " << builtWall11.y  ;
	Point2f vector0 = wallEnd1 - wallEnd0;
	Point2f vector1 = bentCentre -wallEnd1;
	Point2f vector2 = pit0 - bentCentre;
	Point2f vector3 = pit1 - pit0;
	Point2f vector4 = pit2 - pit1;
	Point2f vector5 = builtWall00 - pit2;
	Point2f vector6 = builtWall01 - builtWall00;
	Point2f vector7 = builtWall10 - builtWall01;
	Point2f vector8 = builtWall11 - builtWall10;
	float angle01 = (angleFromVector(vector0));
	float angle12 = (angleFromVector(vector1));
	float angle23 = (angleFromVector(vector2));
	float angle34 = (angleFromVector(vector3));
	float angle45 = (angleFromVector(vector4));
	float angle56 = (angleFromVector(vector5));
	float angle67 = (angleFromVector(vector6));
	float angle78 = (angleFromVector(vector7));
	float angle89 = (angleFromVector(vector8));
	float angle0 = isEmpty(bentCentre) ? -1 : anglePossitiveObtuse(angleSubtraction(angle12, angle01));
	float angle1 = isEmpty(pit0) ? -1 : anglePossitiveObtuse(angleSubtraction(angle23, angle12));
	float angle2 = isEmpty(pit1) ? -1 : anglePossitiveObtuse(angleSubtraction(angle34, angle23));
	float angle3 = isEmpty(pit2) ? -1 : anglePossitiveObtuse(angleSubtraction(angle45, angle34));
	float angle4 = isEmpty(builtWall00) ? -1 : anglePossitiveObtuse(angleSubtraction(angle56, angle45));
	float angle5 = isEmpty(builtWall01) ? -1 : anglePossitiveObtuse(angleSubtraction(angle67, angle56));
	float angle6 = isEmpty(builtWall10) ? -1 : anglePossitiveObtuse(angleSubtraction(angle78, angle67));
	float angle7 = isEmpty(builtWall11) ? -1 : anglePossitiveObtuse(angleSubtraction(angle89, angle78));
	ss << ", " << angle0 << ", " << angle1 << ", "   << angle2 << ", " << angle3 << ", " << angle4 << ", " << angle5 << ", " << angle6 << ", " << angle7;
	return ss.str();
}

std::string geometryDesc::serialiseCurveArea(float scale) {
	//"WallAngle, HalfWallAngle0, HalfWallAngle1, DeltaKinkAngle0, DeltaKinkAngle1, DeltaKinkAngleAve..1,2 or 3 times";
		/*
#define MENU_GEO_CURVEENDSTART       wallEnd0;
#define MENU_GEO_CURVEENDEND MENU_GEO_WALLEND1 wallEnd1
#define MENU_GEO_CURVE_THIRD_OR_HALF MENU_GEO_WALLENTRE bentCentre
#define MENU_GEO_CURVE_2THIRDS MENU_GEO_PIT0
#define MENU_GEO_CURVE_2THIRDS_SKIP MENU_GEO_PIT1 pit1
#define MENU_GEO_CURVE_THIRD_WALL MENU_GEO_BUILTWALL00 builtWall00
#define MENU_GEO_CURVE_HALF_WALL MENU_GEO_BUILTWALL01 builtWall01
#define MENU_GEO_CURVE_HALF_STOP MENU_GEO_BUILTWALL10
#define MENU_GEO_CURVE_2THIRDS_WALL MENU_GEO_BUILTWALL11 builtWall11
#define MENU_GEO_CURVE_STOP MENU_GEO_DONE
*/

	std::stringstream ss;
	float deltaKinkAveAve;
	if (!isEmpty(wallEnd0) && !isEmpty(wallEnd1)) {
		if (isEmpty(bentCentre)) {
			// a single wall, so just one calc
			float wallAngle = angleFromVector(wallEnd0 - wallEnd1);
			float halfWallAngle0 = angleFromVector(wallEnd0 - builtWall01);
			float halfWallAngle1 = angleFromVector(builtWall01 - wallEnd1);
			float deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
			float deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
			float deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
			deltaKinkAveAve = deltaKinkAngleAve;
			ss << curveStage << ", , , , , , , , , , , , , " << wallAngle << ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve << ", " << deltaKinkAveAve;
		}
		else {
			if (!isEmpty(pit0)) {
				// 3 wall
				float wallAngle = angleFromVector(wallEnd0 - bentCentre);
				float halfWallAngle0 = angleFromVector(wallEnd0 - builtWall00);
				float halfWallAngle1 = angleFromVector(builtWall00 - bentCentre);
				float deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
				float deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
				float deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
				deltaKinkAveAve = deltaKinkAngleAve;
				ss << curveStage << wallAngle << ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve;
				wallAngle = angleFromVector(bentCentre - pit0);
				halfWallAngle0 = angleFromVector(bentCentre - builtWall01);
				halfWallAngle1 = angleFromVector(builtWall01 - pit0);
				deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
				deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
				deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
				deltaKinkAveAve += deltaKinkAngleAve;
				ss  << ", " << wallAngle << ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve;
				wallAngle = angleFromVector(pit0 - wallEnd1);
				halfWallAngle0 = angleFromVector(bentCentre - builtWall11);
				halfWallAngle1 = angleFromVector(builtWall11 - wallEnd1);
				deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
				deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
				deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
				deltaKinkAveAve += deltaKinkAngleAve;
				deltaKinkAveAve /= 3;
				ss  << ", " << wallAngle << ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve << ", " <<  deltaKinkAveAve;

			}
			else {
				// 2 walls
				float wallAngle = angleFromVector(wallEnd0 - bentCentre);
				float halfWallAngle0 = angleFromVector(wallEnd0 - builtWall01);
				float halfWallAngle1 = angleFromVector(builtWall01 - bentCentre);
				float deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
				float deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
				float deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
				deltaKinkAveAve = deltaKinkAngleAve;
				ss << curveStage << ", , , , , , , " <<  wallAngle <<  ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve;
				wallAngle = angleFromVector(bentCentre - wallEnd1);
				halfWallAngle0 = angleFromVector(bentCentre - builtWall11);
				halfWallAngle1 = angleFromVector(builtWall11 - wallEnd1);
				deltaKinkAngle0 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle0));
				deltaKinkAngle1 = anglePossitiveAcute(angleSubtraction(wallAngle, halfWallAngle1));
				deltaKinkAngleAve = (deltaKinkAngle0 + deltaKinkAngle1) / 2;
				deltaKinkAveAve += deltaKinkAngleAve;
				deltaKinkAveAve /= 2;
				ss  << ", " << wallAngle << ", " << halfWallAngle0 << ", " << halfWallAngle1 << ", " << deltaKinkAngle0 << ", " << deltaKinkAngle1 << ", " << deltaKinkAngleAve << ", " << deltaKinkAveAve;

			}

		}
		if (isFirstStageCurve) {
			stage1curve = deltaKinkAveAve;
			isFirstStageCurve = false;
		}
		else {
			isFirstStageCurve = true;
		}
		curveStage = (curveStage + 1) % 3;
	}
	return ss.str();
}
std::string geometryDesc::serialiseBent(float scale) {
//	"tag, ID, wall0Angle, wall1Angle, pitToPitAngle, builtWallAngle, wallBisectedAngle, pitToPitTangent, deltaBuiltBisected, deltaBuiltToPitTangent, deltaBuiltToPitTangentOverSumDeltas ";
	std::stringstream ss;
	float wallAngle0 = angleFromVector(wallEnd0 - bentCentre);
	float wallAngle1 = angleFromVector(wallEnd1 - bentCentre);
	if (isEmpty(bentCentre)) {
		wallAngle0 = wallAngle1 = angleFromVector(wallEnd1 - wallEnd0);
	}
	float pitToPitAngle = angleFromVector(pit1 - pit0);
	float pitToPitTangent = angleSubtraction(pitToPitAngle, Math::PI / 2);
	float builtWallAngle = angleFromVector(builtWall01 - builtWall00);
	float wallBisectionAngle = (wallAngle0 + wallAngle1) / 2;
	wallBisectionAngle = angleSubtraction(wallBisectionAngle, 0); // just used to coerce to 0 < angle < 2Pi
	float deltaBuiltToPitTangent = anglePossitiveAcute(angleSubtraction(pitToPitTangent, builtWallAngle));
	float deltaBuiltBisected = anglePossitiveAcute(angleSubtraction(wallBisectionAngle, builtWallAngle));
	float deltaBuiltToPitTangentOverSumDeltas = deltaBuiltToPitTangent / (deltaBuiltToPitTangent + deltaBuiltBisected);
	float wallDistance0 = magnitude(builtWall00 - bentCentre);
	float wallDistance1 = magnitude(builtWall01 - bentCentre);
	wallDistance0 = (wallDistance0 < wallDistance1)? wallDistance0 : wallDistance1;
	float wallDistance10 = 3 * wallDistance0;
	if (!isEmpty(builtWall10)) {
		wallDistance10 = magnitude(builtWall10 - bentCentre);
		float wallDistance11 = magnitude(builtWall11 - bentCentre);
		wallDistance10 = (wallDistance10 < wallDistance11) ? wallDistance10 : wallDistance11;
	}
	if (isEmpty(pit0) || isEmpty(pit1)) {
		pitToPitAngle = 0;
		pitToPitTangent = 0;
		deltaBuiltToPitTangent = 0;
		deltaBuiltToPitTangentOverSumDeltas = 0;
	}
	ss << (mRandom ? "1" : "0") << ", " << wallAngle0 << ", " << wallAngle1 << ", " << pitToPitAngle << ", " << builtWallAngle << ", " << wallBisectionAngle << ", " << pitToPitTangent << ", " << deltaBuiltBisected << ", " << deltaBuiltToPitTangent << ", " << deltaBuiltToPitTangentOverSumDeltas << ", " << wallDistance0 / scale << ", " << wallDistance10 / scale;

	return ss.str();
}

std::string geometryDesc::serialisePitCellArea2(float scale) {
	// for the pit outside of the cell, max distance from pit wall to cell wall is the distance from the pit to the centre fo the other pit
	// otherwise a different cell would have been used
	std::stringstream ss;
	Point2f pitCentre0 = (pit0 + wallEnd0) / 2;
	float pitRad0 = magnitude((pit0 - wallEnd0)) / 2;
	Point2f pitCentre1 = (pit1 + wallEnd1) / 2;
	float pitRad1 = magnitude((pit1 - wallEnd1)) / 2;
	Point2f cellCentre0 = (builtWall00 + builtWall01) / 2;
	float cellRad0 = magnitude((builtWall00 - builtWall01)) / 2;
	Point2f cellCentre1 = (builtWall10 + builtWall11) / 2;
	float cellRad1 = magnitude((builtWall10 - builtWall11)) / 2;

	return serialiseTwoPitsCellArea(scale, pitCentre0, pitRad0, pitCentre1, pitRad1, cellCentre0, cellRad0) + ", " + serialiseTwoPitsCellArea(scale, pitCentre1, pitRad1, pitCentre0, pitRad0, cellCentre1, cellRad1);
}
std::string geometryDesc::serialisePitCellArea2Caption() {
	return "rad0, rad1, cellRad, centreOffset0, wallGap0, centreOffset1, wallGap1, pitToPitDist, decilePlacement, prob, pro0, pro1, pro2, pro3, pro4, pro5, pro6, pro7, pro8, pro9";
}
std::string geometryDesc::serialiseTwoPitsCellArea(float scale, Point2f p0, float rad0, Point2f p1, float rad1, Point2f cell0, float cellRad) {
	// for the pit outside of the cell, max distance from pit wall to cell wall is the distance from the pit to the centre fo the other pit
	// otherwise a different cell would have been used
	std::stringstream ss;

	float pitToPitDist = magnitude((p0 - p1));
	float centreOffset0 = magnitude((p0 - cell0));
	float wallGap0 = Math::Abs(cellRad - (centreOffset0 + rad0));
//	float wallGapFraction0 = wallGap0 / cellRad;
	float pitCentreToWall = Math::Abs(centreOffset0 - cellRad);
//	float centreOffsetFraction0 = centreOffset0 / cellRad;
	float centreOffset1 = magnitude((p1 - cell0));
	float wallGap1 = Math::Abs((centreOffset1 - rad1) - cellRad);
//	float wallGapFraction1 = wallGap1 / pitToPitDist;
//	float centreOffsetFraction1 = centreOffset1 / pitToPitDist;

//	float cellCresentArea = cresentArea(scale, cellRad, pitToPitDist);
//	scale = 1; cellRad = 4; pitToPitDist = 6;
	float cellCresentAreaAsym = cresentAreaAsym(scale, cellRad, 0, pitToPitDist);
	float fractionalAreas[10];
	ss << rad0 / scale << ", " << rad1 / scale << ", " << cellRad / scale << ", " << centreOffset0 / scale << ", " << wallGap0 / scale  << ", " << centreOffset1 / scale << ", " << wallGap1 / scale << ", " << pitToPitDist / scale;

	float remainingArea = cellCresentAreaAsym;
	for (int inc = 1; inc <= 10; inc++) {
		float nextBandArea = cresentAreaAsym(scale, cellRad, (pitToPitDist / 10) * inc, pitToPitDist);
		fractionalAreas[inc - 1] = (remainingArea - nextBandArea) / cellCresentAreaAsym;
		remainingArea = nextBandArea;
	}
	int placement = (int)(10 * ((wallGap1 + rad1) / pitToPitDist));
	if (placement >= 10)
		placement = 9;
	if (placement < 0)
		placement = 0;
	ss   << ", " << placement;
	ss   << ", " << fractionalAreas[placement];
	for (int inc = 0; inc < 10; inc++) {
		ss << ", " << fractionalAreas[inc];
	}
	return ss.str();
}
float geometryDesc::instantCellTwoPits(float pitCentreToCellWall, float cellRad, float pitRad0, float pitSep) {
	//a = invCos(((R + d) ^ 2 + S ^ 2 – R ^ 2) / 2.(R + d).S)
	// instant probability = (R + d) x angle == (R + d).invCos(((R + d) ^ 2 + S ^ 2 – R ^ 2) / 2.(R + d).S)
	// R = CellRad, d = pitCentreToCellWall, S = pitSep
	float B = cellRad + pitCentreToCellWall;
	float cosElement = ((B * B) + pitSep * pitSep - cellRad * cellRad) / 2 * B * pitSep;
	float halfAngle = Math::Acos(cosElement);
	float arcLen = halfAngle * B;
	return arcLen;
}
float geometryDesc::integralCellTwoPits(float cellRad, float pitRad0, float pitSep) {
	float distRangeStart = pitSep - 2 * cellRad;
	if (distRangeStart < 0) distRangeStart = 0;
	float distRangeEnd = pitSep;
	float distRange = distRangeEnd - distRangeStart;
	float distDelta = distRange / 100;
	float integral = 0;
	for (int idx = 0; idx < 99; idx++) {
		float instance = instantCellTwoPits(distRangeStart + distDelta / 2 + idx * distDelta, cellRad, pitRad0, pitSep);
		integral += instance;
	}
	return integral;
}
float geometryDesc::cresentAreaAsym(float scale, float cellRad, float pitCentreToWall, float pitSeparation) {
	return cresentAreaAsym(cellRad / scale, pitCentreToWall / scale, pitSeparation / scale);
}
float geometryDesc::cresentArea(float scale, float cellRad, float pitSeparation) {
	return cresentArea(cellRad / scale, pitSeparation / scale);
}
float geometryDesc::cresentArea(float cellRad, float pitSeparation) {
	float cellRadSq = cellRad * cellRad;
	float fullCircleArea = Math::PI * cellRadSq;
	float exclusionSegmentArea = 0;
	if (2 * cellRad > pitSeparation) {
		float h = Math::Sqrt(cellRadSq - pitSeparation * pitSeparation / 4);
		float intersectAngle = Math::Acos((pitSeparation / 2) / cellRad);
		float halfSegmentArea = intersectAngle * cellRadSq / 2;
		float halfTriangleArea = h * pitSeparation / 4;
		exclusionSegmentArea = (halfSegmentArea - halfTriangleArea) * 4;
	}
	return fullCircleArea - exclusionSegmentArea;
}
float COSorSINlimit(float putative) {
	float retv = putative;
	if (putative < -1) {
		retv = -1;
	}
	else if (putative > 1) {
		retv = 1;
	}
	return retv;
}
float geometryDesc::cresentAreaAsym(float cellRad, float pitCentreToWall, float pitSeparation) {
	float cellRadSq = cellRad * cellRad;
	float fullCircleArea = Math::PI * cellRadSq;
	float B = cellRad + pitCentreToWall;
	float Bsq = B * B;
	float pitSeparationSq = pitSeparation * pitSeparation;
	float exclusionSegmentArea = 0;

	if ((B + cellRad) > pitSeparation) {
		float h = Math::Sqrt(cellRadSq - pitSeparation * pitSeparation / 4);
		float COSintersectAngle_a = COSorSINlimit((cellRadSq + pitSeparationSq - Bsq)/(2 * cellRad * pitSeparation));
		float COSintersectAngle_b = COSorSINlimit((Bsq + pitSeparationSq - cellRadSq)/(2 * B * pitSeparation));
		float intersectAngle_a = Math::Acos(COSintersectAngle_a);
		float intersectAngle_b = Math::Acos(COSintersectAngle_b);
		float E = cellRad * COSintersectAngle_a;
		float D = B * COSintersectAngle_b;
		float halfSegmentRightArea = (intersectAngle_a * cellRadSq / 2);
		float halfSegmentLeftArea =(intersectAngle_b * Bsq / 2);
		float halfTriangleRightArea = 0;
		float halfTriangleLeftArea = 0;
		float tmpSq = cellRadSq - E * E;
		if (tmpSq > 0) {
			halfTriangleRightArea = Math::Sqrt(tmpSq) * E / 2;
		}
		tmpSq = Bsq - D * D;
		if (tmpSq > 0) {
			halfTriangleLeftArea = Math::Sqrt(tmpSq) * D / 2;
		}
		exclusionSegmentArea = (halfSegmentRightArea - halfTriangleRightArea) + (halfSegmentLeftArea - halfTriangleLeftArea);
	}
	return fullCircleArea - 2 * exclusionSegmentArea;
}

bool geometryDesc::deserialise(std::vector<std::string>& valStrArray) {
    bool retv = false;
    if (valStrArray.size() >= 26) {
        int idx = 0;
        mShape = (shape)stoi(valStrArray[idx++]);
        mHeight = (shape)stoi(valStrArray[idx++]);
        mDeposit = (shape)stoi(valStrArray[idx++]);
        wallEnd0 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        wallEnd1 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        bentCentre = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        pit0 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        pit1 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        pit1Used = stoi(valStrArray[idx++]) == 1;
        pit2 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        pit2Used = stoi(valStrArray[idx++]) == 1;
        builtWall00 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        builtWall01 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        builtWall10 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        builtWall11 = Point2f(stof(valStrArray[idx]), stof(valStrArray[idx + 1]));
        idx += 2;
        builtWall1Used = stoi(valStrArray[idx++]) == 1;
		if (valStrArray.size() == 27) {
			// additional value for pitRim_random
			mRandom = stoi(valStrArray[idx++]) == 1;
		}
        retv = true;
    }
    return retv;
}
bool geometryDesc::isEmpty(Point pt) {
    return pt.x == 0 && pt.y == 0;
}
bool geometryDesc::isEmpty() {
    return isEmpty(wallEnd0) && isEmpty(bentCentre) && isEmpty(pit0);
}
Point geometryDesc::getRefLoc() {
	if ((mShape == shape::rim1pit || mShape == shape::rim2pits) && !isEmpty(pit0)) {
		return pit0;
	}
	else if (!isEmpty(wallEnd0)) {
		return wallEnd0;
	}
	else {
		return bentCentre;
	}
}

void geometryDesc::emptyEntry() {
    mShape = shape::none;
    mHeight = 0;
    mDeposit = 0;
    wallEnd0 = Point(0, 0);
    wallEnd1 = Point(0, 0);
    bentCentre = Point(0, 0);
    pit0 = Point(0, 0);
    pit1 = Point(0, 0);
    pit1Used = false;
    builtWall00 = Point(0, 0);
    builtWall01 = Point(0, 0);
    builtWall10 = Point(0, 0);
    builtWall11 = Point(0, 0);
    builtWall1Used = false;
	mRandom = false;
}

