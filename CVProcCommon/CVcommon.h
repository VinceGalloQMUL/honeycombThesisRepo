#pragma once
#include <sstream>
//using namespace std;
using namespace System;

const float  PI_F = 3.14159265358979f;

public class vector2f {
public:
	float dx, dy;
	vector2f::vector2f() {
		dx = 0;
		dy = 0;
	}
	vector2f::vector2f(float x0, float y0) {
		dx = x0;
		dy = y0;
	}
	vector2f::vector2f(float x0, float y0, float x1, float y1) {
		dx = x1 - x0;
		dy = y1 - y0;
	}
	void vector2f::copy(vector2f srce) {
		dx = srce.dx;
		dy = srce.dy;
	}
	void vector2f::set(float x0, float y0) {
		dx = x0;
		dy = y0;
	}
	std::string vector2f::toStr() {
		std::stringstream ss;
		ss << dx << "," << dy;
		return ss.str();
	}

	void vector2f::add(vector2f addn) {
		dx += addn.dx;
		dy += addn.dy;
	}
	void vector2f::add(float x0, float y0) {
		dx += x0;
		dy += y0;
	}
	void vector2f::minus(vector2f subn) {
		dx -= subn.dx;
		dy -= subn.dy;
	}
	void vector2f::divide(float denom) {
		dx /= denom;
		dy /= denom;
	}
	float vector2f::magnitude() {
		return (float)Math::Sqrt(dx * dx + dy *dy);
	}
	float vector2f::directionDegXY() {
		if (dx == 0.0f) return (dy > 0) ? 90.0f : -90.0f;
		float tmpangle = (float)((Math::Atan(Math::Abs(dy) / Math::Abs(dx)) * 180.0f) / Math::PI);
		// dy is negated as y counts from 0 at the top
		if (dy > 0) { // upper half
			if (dx > 0) { // upper right quadrant

			}
			else { // upper left quadrant
				tmpangle = 180 - tmpangle;
			}
		}
		else { // lower half
			if (dx > 0) { // lower right quadrant
				tmpangle = -tmpangle;
			}
			else { // lower left quadrant
				tmpangle = -180 + tmpangle;
			}
		}
		return tmpangle;
	}
	void vector2f::directionV(float *vX, float *vY) {
		float mag = magnitude();
		if (mag != 0) {
			*vX = dx / mag;
			*vY = dy / mag;
		}
		else {
			*vX = *vY = 0;
		}
	}
	vector2f *vector2f::normalised() {
		vector2f *retv = new vector2f(dx, dy);
		float mag = magnitude();
		if (mag != 0) {
			retv->divide(mag);
		}
		return retv;
	}
};
public class vector3f {
public: 
	float dx, dy, dz;
	vector3f::vector3f(float x0, float y0, float z0) {
		dx = x0;
		dy = y0;
		dz = z0;
	}
	vector3f::vector3f() {
		dx = 0;
		dy = 0;
		dz = 0;
	}
	void vector3f::copy(vector3f srce) {
		dx = srce.dx;
		dy = srce.dy;
		dz = srce.dz;
	}
	void vector3f::set(float x0, float y0, float z0) {
		dx = x0;
		dy = y0;
		dz = z0;
	}
	void vector3f::add(vector3f addn) {
		dx += addn.dx;
		dy += addn.dy;
		dz += addn.dz;
	}
	void vector3f::add(float x0, float y0, float z0) {
		dx += x0;
		dy += y0;
		dz += z0;
	}
	void vector3f::minus(vector3f subn) {
		dx -= subn.dx;
		dy -= subn.dy;
		dz -= subn.dz;
	}
	void vector3f::divide(float denom) {
		dx /= denom;
		dy /= denom;
		dz /= denom;
	}
	float vector3f::magnitude() {
		return (float)Math::Sqrt(dx * dx + dy *dy + dz * dz);
	}
	float dot(vector3f refVect) { 
		return dx * refVect.dx + dy * refVect.dy + dz * refVect.dz;
	}
	//The angle between vectors cos(angle) = x dot y / mag(x) x mag(y)

	float vector3f::directionRad(vector3f refVect) {
		float cosA = dot(refVect);
		cosA /= magnitude();
		cosA /= refVect.magnitude();
		return (float)Math::Acos(cosA);
	}
	vector3f *vector3f::normalised() {
		vector3f *retv = new vector3f(dx,dy,dz);
		float mag = magnitude();
		if (mag != 0) {
			retv->divide(mag);
		}
		return retv;
	}
	// angle is standard Trig convention 0 == 3o'clock +ve is anti-clockwise
	static void vector3f::radToNormalisedVector(float rads, float *vX, float *vY, float *vZ) {
		*vX = (float)Math::Cos(rads);
		*vY = (float)Math::Sin(rads);
		*vZ = 0; // just working to convert 3d angle
	}
};
struct vector2i {
	int x;
	int y;
};
typedef struct vector2i pixelLoc;

struct beeLoc2dPxls {
	int fNum;
	float size; // also used as z value
	int x;
	int y;
	float aspectRatio;
	float ellipseMaj;
	float ellipseMin;
	float angle;
	float fTime;
};
typedef struct beeLoc2dPxls point2dPxls;

struct beeLoc3dWorking {
	int fNum;
	float size0;
	float size1;
	int x0;
	int y0;
	int x1;
	int y1;
	float aspectRatio;
	float speed;
	float directionX;
	float directionY;
	float directionZ;
	int classification;
	float Wx0;
	float Wy0;
	float Wx1;
	float Wy1;
	float RWx;
	float RWy;
	float RWz;
	float RWxSmoothed;
	float RWySmoothed;
	float RWzSmoothed;
	float RWxSmoothed2;
	float RWySmoothed2;
	float RWzSmoothed2;
};
typedef struct beeLoc3dWorking point3dWorking;
struct beeLoc3d {
	float fTime;
	float size;
	float aspectRatio;
	float RWx;
	float RWy;
	float RWz;
	float RWxSmoothed;
	float RWySmoothed;
	float RWzSmoothed;
	float speed;
	float directionX;
	float directionY;
	float directionZ;
	int classification;
};
typedef struct beeLoc3d point3dMm;

struct bee2dVector {
	point2dPxls loc;

	float speed;
	float directionX;
	float directionY;
	float directionZ;
	int classification;
};
typedef struct bee2dVector point2dVectorPxls;

struct clkStat {
	bool mousedown;
	bool rmousedown;
	bool clickedOnMenuItem;
	int menuChoice;
	int startX;
	int startY;
	int selectedZoneItem;
	clkStat() {
		mousedown = false;
		rmousedown = false;
		menuChoice = 0;
		startX = 0;
		startY = 0;
		selectedZoneItem = -1;
	}
};
typedef struct clkStat clickStatus;

