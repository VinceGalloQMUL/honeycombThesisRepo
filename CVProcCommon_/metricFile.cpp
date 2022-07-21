#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
using namespace System;
#include "csvFile.h"
#include "metricFile.h"

using namespace System;

metricFile::metricFile() {
	framesPerSec = 40;
	scaleDistanceMM = 120;
	scaleDistancePixels = 240;
	scaleX0 = 0;
	scaleX1 = 0;
	scaleY0 = 0;
	scaleY1 = 0;
}
void metricFile::read(char *filepathname) {
	std::ifstream str;
	str.open(filepathname);
	std::vector<std::string>   lineArray = getNextLineAndSplitIntoTokensIgnoreComment(str);
	if (lineArray.size() == 6) {
		// locFile << framesPerSec << "," << scaleDistanceMM << "," << scaleX0 << "," << scaleY0 << "," << scaleX1 << "," << scaleY1 << "\n";
		framesPerSec = std::stof(lineArray[0]);
		scaleDistanceMM = std::stoi(lineArray[1]);
		scaleX0 = std::stoi(lineArray[2]);
		scaleY0 = std::stoi(lineArray[3]);
		scaleX1 = std::stoi(lineArray[4]);
		scaleY1 = std::stoi(lineArray[5]);
	}

}
void metricFile::write(char *filepathname) {
	ofstream locFile(filepathname, ios::out);
	locFile << framesPerSec << ", " << scaleDistanceMM << "," << scaleX0 << "," << scaleY0 << "," << scaleX1 << "," << scaleY1 << "\n";
	locFile.close();

}
float metricFile::getFrameRate() {
	return framesPerSec;
}

float metricFile::getPixelsPerMM() {
	return scaleDistancePixels / scaleDistanceMM;
}
float metricFile::setScaleDistance(float scaleDistance) {
	scaleDistanceMM = scaleDistance;
	if (scaleDistanceMM < 10) scaleDistanceMM = 10; // prevent divide by zero
	return getPixelsPerMM();
}
void metricFile::setScalePixels(int x0, int y0, int x1, int y1) {
	scaleX0 = x0;
	scaleY0 = y0;
	scaleX1 = x1;
	scaleY1 = y1;
	setScalePixels();
}
void metricFile::setScalePixels() {
	scaleDistancePixels = Math::Sqrt((scaleX0 - scaleX1) * (scaleX0 - scaleX1) + (scaleY0 - scaleY1) * (scaleY0 - scaleY1));
	if (scaleDistancePixels < 10) scaleDistancePixels = 10; // prevent divide by zero
}
