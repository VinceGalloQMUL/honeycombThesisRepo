#pragma once
#include <csvFile.h>

class metricFile : csvFile
{
public:
	float framesPerSec;
	float scaleDistanceMM;
	float scaleDistancePixels;
	int scaleX0, scaleY0, scaleX1, scaleY1;

	float getFrameRate();

	float getPixelsPerMM();
	float setScaleDistance(float scaleDistance);
	void setScalePixels(int x0, int y0, int x1, int y1);
	void setScalePixels();
	void read(char *fname);
	void write(char *fname);

	metricFile();
};

