#include "cvCell.h"
#include "geometryDesc.h""
#include "csvFile.h"

cvCellPointWithID::cvCellPointWithID(int id) {
	mId = id;
}

void cvCellDataStore::addCell(int cellID) {
	int storeIdx = 0;
	int possitivedID = cellID;
	if (cellID < 0 ) {
		possitivedID = -(cellID + 1);
		storeIdx = 1;
	}
	while (mWallCentres[storeIdx].size() <= possitivedID) {
		vector<cvCellPointWithID> *tmpPtr = new vector<cvCellPointWithID>();
		mWallCentres[storeIdx].push_back(*tmpPtr);
	}
}
void cvCellDataStore::setWallCentre(int cellID, int peerID, Point2f point, cvCellPoint::flexability flex) {
	int storeIdx = 0;
	int possitivedID = cellID;
	cvCellPointWithID *tmpPtr = nullptr;
	addCell(cellID);
	if (cellID < 0 ) {
		possitivedID = -(cellID + 1);
		storeIdx = 1;
	}
	for (int idx = 0; idx < mWallCentres[storeIdx][possitivedID].size(); idx++) {
		if (mWallCentres[storeIdx][possitivedID][idx].mId == peerID) {
			tmpPtr = &(mWallCentres[storeIdx][possitivedID][idx]);
			break;
		}
	}
	if (tmpPtr == nullptr) {
		tmpPtr = new cvCellPointWithID(peerID);
		mWallCentres[storeIdx][possitivedID].push_back(*tmpPtr);
	} 
	if ((tmpPtr->point.mFlex != cvCellPoint::flexability::fixed) || (flex == cvCellPoint::flexability::fixed)) {
		tmpPtr->point.mFlex = flex;
		tmpPtr->point.mLoc = point;
	}
}
cvCellPoint::flexability cvCellDataStore::getWallCentre(int cellID, int peerID, Point2f *pPoint) {
	cvCellPoint::flexability retv = cvCellPoint::flexability::empty;
	int storeIdx = 0;
	int possitivedID = cellID;
	cvCellPointWithID *tmpPtr = nullptr;
	if (cellID < 0 ) {
		possitivedID = -(cellID + 1);
		storeIdx = 1;
	}
	if (possitivedID < mWallCentres[storeIdx].size()) {
		for (int idx = 0; idx < mWallCentres[storeIdx][possitivedID].size(); idx++) {
			if (mWallCentres[storeIdx][possitivedID][idx].mId == peerID && mWallCentres[storeIdx][possitivedID][idx].point.mFlex != cvCellPoint::flexability::empty) {
				retv = mWallCentres[storeIdx][possitivedID][idx].point.mFlex;
				*pPoint = mWallCentres[storeIdx][possitivedID][idx].point.mLoc;
				break;
			}
		}
	}
	return retv;
}
void cvCellDataStore::clearWallCentres(int cellID) {
	int storeIdx = 0;
	int possitivedID = cellID;
	cvCellPointWithID *tmpPtr = nullptr;
	if (cellID < 0) {
		possitivedID = -(cellID + 1);
		storeIdx = 1;
	}
	mWallCentres[storeIdx][possitivedID].clear();
}

string cvCellDataStore::serialise() {
	std::stringstream ss;
	for (int storeIdx = 0; storeIdx < 2; storeIdx++) {
		ss  << ",\n" << mWallCentres[storeIdx].size();
		for (int id = 0; id < mWallCentres[storeIdx].size(); id++) {
			ss << ",\n" << id << ", " << mWallCentres[storeIdx][id].size();
			for (int peeridx = 0; peeridx < mWallCentres[storeIdx][id].size(); peeridx++) {
				ss << ", " << mWallCentres[storeIdx][id][peeridx].mId << ", " << (int)(mWallCentres[storeIdx][id][peeridx].point.mFlex) << ", " << mWallCentres[storeIdx][id][peeridx].point.mLoc.x << ", " << mWallCentres[storeIdx][id][peeridx].point.mLoc.y;
			}
		}
	}

	return ss.str();

}

int cvCellDataStore::deserialise(std::vector<std::string>   tokenised, int tokenIdx) {
	int tokenQty = tokenised.size();
	int cellQty;

	for (int storeIdx = 0; storeIdx < 2; storeIdx++) {
		if (tokenIdx < tokenQty) {
			cellQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
			while (cellQty-- > 0) {
				int cellID = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
				int peerQty = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
				vector<cvCellPointWithID> *tmpVect = new vector<cvCellPointWithID>();
				while (peerQty-- > 0) {
					int peerID = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
					int f = csvFile::parseLine1iSafely(tokenised[tokenIdx++], 0);
					float x = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
					float y = csvFile::parseLine1fSafely(tokenised[tokenIdx++], 0);
					cvCellPointWithID *tmpPtr = new cvCellPointWithID(peerID);
					tmpPtr->point.mFlex = (cvCellPoint::flexability)f;
					tmpPtr->point.mLoc = Point2f(x, y);
					tmpVect->push_back(*tmpPtr);
				}
				mWallCentres[storeIdx].push_back(*tmpVect);
			}
		}
	}
	return tokenIdx;
}

cvCell::cvCell(int imageID, vector<cvCell> *allCells, vector<Point > contours, int theID, vector<Point2f> autoCellCentres, vector<vector<int >> &connections, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, vector<int> &minusCellCentres, vector<Point2i> &minusCellConnections, pGetWallCentre getWallCentre)
{
	mAllCells = allCells;
	mID = theID;
	//std::vector<Point>::iterator itP = mContours.begin();
	//mContours.insert(itP, contours.begin(), contours.end());
	mContours = contours;
	mCentre = autoCellCentres[theID];

	mConnectionIDs.clear();

	std::vector<int>::iterator itI = mConnectionIDs.begin();
	// copy the raw connection IDs
	itI = mConnectionIDs.insert(itI, connections[theID].begin(), connections[theID].end());
	// now add the additional ones that pair with me
	for (int idx = 0; idx < addCellConnections.size(); idx++) {
		if (addCellConnections[idx].x == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].y);
		}
		if (addCellConnections[idx].y == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].x);
		}
	}
	// and the subtractive ones that pair with me
	for (int idx = 0; idx < minusCellConnections.size(); idx++) {
		if (minusCellConnections[idx].x == theID) {
			removeEntry(mConnectionIDs, minusCellConnections[idx].y);
		}
		if (minusCellConnections[idx].y == theID) {
			removeEntry(mConnectionIDs, minusCellConnections[idx].x);
		}
	}
	// now convert connection indexes into geometry
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		int tmpConn = mConnectionIDs[idx];
		if (tmpConn < 0) {
			tmpConn = (-tmpConn) - 1;
			mConnections.push_back(addCellCentres[tmpConn]);

		}
		else { // is a main item
			mConnections.push_back(autoCellCentres[tmpConn]);
		}
	}
	sortConnections();
	if (getWallCentre == nullptr) {
		calcWallCentres();
	}
	else {
		float averageRad = 0;
		Point2f tmpCentre;
		for (int idx = 0; idx < mConnections.size(); idx++) {
			float wallPoint0, wallPoint1;
			tmpCentre = calcContourConnIntersect(idx);
			// precompute a guess, now test if there is a better one - the guess will be saved if nothing else is available
			getWallCentre(imageID, mID, mConnectionIDs[idx], &tmpCentre);
			mWallCentres.push_back(tmpCentre);
			averageRad += magnitude(mWallCentres[idx] - mCentre);
		}
		mRadius = averageRad / mConnections.size();
	}
	calcWallVectors();
	calcWallCorners();
}

cvCell::cvCell(vector<cvCell> *allCells, vector<Point > contours, int theID, vector<Point2f> autoCellCentres, vector<vector<int >> &connections, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, vector<int> &minusCellCentres, vector<Point2i> &minusCellConnections, adjustableDisplay *adjDisp)
{
	mAllCells = allCells;
	mID = theID;
	//std::vector<Point>::iterator itP = mContours.begin();
	//mContours.insert(itP, contours.begin(), contours.end());
	mContours = contours;
	mCentre = autoCellCentres[theID];

	mConnectionIDs.clear();

	std::vector<int>::iterator itI = mConnectionIDs.begin();
	// copy the raw connection IDs
	itI = mConnectionIDs.insert(itI, connections[theID].begin(), connections[theID].end());
	// now add the additional ones that pair with me
	for (int idx = 0; idx < addCellConnections.size(); idx++) {
		if (addCellConnections[idx].x == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].y);
		}
		if (addCellConnections[idx].y == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].x);
		}
	}
	// and the subtractive ones that pair with me
	for (int idx = 0; idx < minusCellConnections.size(); idx++) {
		if (minusCellConnections[idx].x == theID) {
			removeEntry(mConnectionIDs, minusCellConnections[idx].y);
		}
		if (minusCellConnections[idx].y == theID) {
			removeEntry(mConnectionIDs, minusCellConnections[idx].x);
		}
	}
	// now convert connection indexes into geometry
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		int tmpConn = mConnectionIDs[idx];
		if (tmpConn < 0) {
			tmpConn = (-tmpConn) - 1;
			mConnections.push_back(addCellCentres[tmpConn]);

		}
		else { // is a main item
			mConnections.push_back(autoCellCentres[tmpConn]);
		}
	}
	sortConnections();
	if (adjDisp == nullptr) {
		calcWallCentres();
	}
	else {
		float averageRad = 0;
		Point2f newCentre = Point2f(0, 0);
		Point2f tmpCentre;
		for (int idx = 0; idx < mConnections.size(); idx++) {
			float wallPoint0, wallPoint1;
			if (adjDisp->getWallIntersection(mCentre, mConnections[idx], &wallPoint0, &wallPoint1)) {
				tmpCentre = mCentre + wallPoint0 * (mConnections[idx] - mCentre);

			}
			else {
				tmpCentre = calcContourConnIntersect(idx);
			}
			mWallCentres.push_back(tmpCentre);
			newCentre = newCentre + tmpCentre;
		}
		mCentre = newCentre / (int)(mConnections.size());
		for (int idx = 0; idx < mConnections.size(); idx++) {
			averageRad += magnitude(mWallCentres[idx] - mCentre);
		}
		mRadius = averageRad / mConnections.size();
	}
	calcWallVectors();
	calcWallCorners();
}

cvCell::cvCell(int imageID, vector<cvCell> *allCells, int theID, vector<Point2f> autoCellCentres, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, pGetWallCentre getWallCentre) {

	mAllCells = allCells;
	mID = theID;
	int possitivedID = -(mID + 1);
	//std::vector<Point>::iterator itP = mContours.begin();
	//mContours.insert(itP, contours.begin(), contours.end());
	mCentre = addCellCentres[possitivedID];

	mConnectionIDs.clear();
	// now add the additional ones that pair with me
	for (int idx = 0; idx < addCellConnections.size(); idx++) {
		if (addCellConnections[idx].x == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].y);
		}
		if (addCellConnections[idx].y == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].x);
		}
	}
	// now convert connection indexes into geometry
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		int tmpConn = mConnectionIDs[idx];
		if (tmpConn < 0) {
			tmpConn = (-tmpConn) - 1;
			mConnections.push_back(addCellCentres[tmpConn]);

		}
		else { // is a main item
			mConnections.push_back(autoCellCentres[tmpConn]);
		}
	}
	sortConnections();
	if (getWallCentre == nullptr) {
		calcWallCentres();
	}
	else {
		float averageRad = 0;
		Point2f tmpCentre;
		for (int idx = 0; idx < mConnections.size(); idx++) {
			float wallPoint0, wallPoint1;
			tmpCentre = calcContourConnIntersect(idx);
			// precompute a guess, now test if there is a better one - the guess will be saved if nothing else is available
			getWallCentre(imageID, mID, mConnectionIDs[idx], &tmpCentre);
			mWallCentres.push_back(tmpCentre);
			averageRad += magnitude(mWallCentres[idx] - mCentre);
		}
		mRadius = averageRad / mConnections.size();
	}

	calcWallVectors();
	calcWallCorners();
}

cvCell::cvCell(vector<cvCell> *allCells, int theID, vector<Point2f> autoCellCentres, vector<Point2f> &addCellCentres, vector<Point2i> &addCellConnections, adjustableDisplay *adjDisp) {

	mAllCells = allCells;
	mID = theID;
	int possitivedID = -(mID + 1);
	//std::vector<Point>::iterator itP = mContours.begin();
	//mContours.insert(itP, contours.begin(), contours.end());
	mCentre = addCellCentres[possitivedID];

	mConnectionIDs.clear();
	// now add the additional ones that pair with me
	for (int idx = 0; idx < addCellConnections.size(); idx++) {
		if (addCellConnections[idx].x == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].y);
		}
		if (addCellConnections[idx].y == theID) {
			mConnectionIDs.push_back(addCellConnections[idx].x);
		}
	}
	// now convert connection indexes into geometry
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		int tmpConn = mConnectionIDs[idx];
		if (tmpConn < 0) {
			tmpConn = (-tmpConn) - 1;
			mConnections.push_back(addCellCentres[tmpConn]);

		}
		else { // is a main item
			mConnections.push_back(autoCellCentres[tmpConn]);
		}
	}
	sortConnections();
	//	calcWallCentres();
	if (adjDisp == nullptr) {
		calcWallCentres();
	}
	else {
		float averageRad = 0;
		Point2f newCentre = Point2f(0, 0);
		Point2f tmpCentre;
		for (int idx = 0; idx < mConnections.size(); idx++) {
			float wallPoint0, wallPoint1;
			if (adjDisp->getWallIntersection(mCentre, mConnections[idx], &wallPoint0, &wallPoint1)) {
				tmpCentre = mCentre + wallPoint0 * (mConnections[idx] - mCentre);

			}
			else {
				tmpCentre = calcContourConnIntersect(idx);
			}
			mWallCentres.push_back(tmpCentre);
			newCentre = newCentre + tmpCentre;
		}
		mCentre = newCentre / (int)(mConnections.size());
		for (int idx = 0; idx < mConnections.size(); idx++) {
			averageRad += magnitude(mWallCentres[idx] - mCentre);
		}
		mRadius = averageRad / mConnections.size();

	}

	calcWallVectors();
	calcWallCorners();
}


string cvCell::serialisedHeadings() {
	std::stringstream ss;
	ss << "ID, centreX, centreY, areaMM2, wallWidthMM, qtyCorners";
	for (int idx = 0; idx < 8; idx++) {
		char idxC = '0' + idx;
		ss << ", c" << idxC << "X, c" << idxC << "Y, c" << idxC << "lenMM, a" << idxC << ", sep" << idxC;
	}
	return ss.str();
}
string cvCell::serialise(float pixelsPerMM) {
	std::stringstream ss;
	float cA = area();
	ss << mID << "," << mCentre.x / pixelsPerMM << "," << mCentre.y / pixelsPerMM << "," << cA/(pixelsPerMM * pixelsPerMM) << "," << mAverageWallThickness / (pixelsPerMM );
	ss << "," << mCorners.size();
	int crnrQty = mCorners.size();
	for (int idxC = 0; idxC < mCorners.size(); idxC++) {
		int idx = mSortedConnections[idxC];
		float lenP = magnitude(mCorners[(idxC + 1) % crnrQty] - mCorners[idxC]);
		ss << "," << (mCorners[idxC]).x / pixelsPerMM << "," << (mCorners[idxC]).y / pixelsPerMM << "," << lenP/ pixelsPerMM << "," << mAngles[idx] << "," << mSeparations[idx]/ pixelsPerMM ;
	}

	return ss.str();
}

float cvCell::area() {
	int crnrQty = mCorners.size();
	float retv = 0;
	for (int idx = 0; idx < mCorners.size(); idx++) {
		retv += triangleArea(mCorners[idx], mCorners[(idx + 1) % crnrQty], mCentre);
	}
	return retv;
}

bool cvCell::cellHasConnections(int theID, vector<vector<int >> &connections, vector<Point2i> &addCellConnections, vector<Point2i> &minusCellConnections) {
	vector<int> tmpConnections;
	std::vector<int>::iterator itI = tmpConnections.begin();
	// copy the raw connection IDs
	if (theID >= 0) {
		itI = tmpConnections.insert(itI, connections[theID].begin(), connections[theID].end());

	}
	// now add the additional ones that pair with me
	for (int idx = 0; idx < addCellConnections.size(); idx++) {
		if (addCellConnections[idx].x == theID) {
			tmpConnections.push_back(addCellConnections[idx].y);
		}
		if (addCellConnections[idx].y == theID) {
			tmpConnections.push_back(addCellConnections[idx].x);
		}
	}
	// and the subtractive ones that pair with me
	for (int idx = 0; idx < minusCellConnections.size(); idx++) {
		if (minusCellConnections[idx].x == theID) {
			removeEntry(tmpConnections, minusCellConnections[idx].y);
		}
		if (minusCellConnections[idx].y == theID) {
			removeEntry(tmpConnections, minusCellConnections[idx].x);
		}
	}
	return (tmpConnections.size() > 0);
}

void cvCell::reCalcWithPeerDependencies(int imageID, int occurance, fnSetWallCentre setWallCentre) {
	if (occurance == 0) {
		calcAverageWallThickness();
	}
	else {
		if (reCalcWallCentres()) {
			calcWallVectors();
			calcWallCorners();
			setWallCentres(imageID, setWallCentre);
		}
	}
}

void cvCell::setWallCentres(int imageID, fnSetWallCentre setWallCentre) {
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		setWallCentre(imageID, mID, mConnectionIDs[idx], mWallCentres[idx]);
	}
}

Point2f cvCell::getWallTowardsMe(int requester) {
	Point2f retv = Point2f(-1, -1);
	if (mContours.size() > 0) {
		for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
			if (mConnectionIDs[idx] == requester) {
				retv = mWallCentres[idx];
			}
		}
	}
	return retv;
}
float cvCell::getAverageWallThickness() {
	return mAverageWallThickness;
}
void cvCell::calcAverageWallThickness() {
	float tmpAverage = 0;
	int wallThicknessCount = 0;
	for (int idx = 0; idx < mConnectionIDs.size(); idx++) {
		int peerIdx = findPeerCell(mConnectionIDs[idx]);
		if (peerIdx >= 0) {
			Point2f wallLoc = mAllCells->at(peerIdx).getWallTowardsMe(mID);
			if (wallLoc.x > 0 && wallLoc.y > 0) {
				float wallThickness = magnitude(wallLoc - mWallCentres[idx]);
				tmpAverage += wallThickness;
				wallThicknessCount++;
			}
		}
	}
	mAverageWallThickness = 0;
	if (wallThicknessCount > 0) {
		mAverageWallThickness = tmpAverage / wallThicknessCount;
	}
}
int cvCell::findPeerCell(int peerID) {
	int retv = -1;
	for (int idx = 0; idx < mAllCells->size(); idx++) {
		if (mAllCells->at(idx).mID == peerID) {
			retv = idx;
			break;
		}
	}
	return retv;
}

void cvCell::calcWallVectors() {
	mWallVectors.clear();
	mSeparations.clear();
	for (int idx = 0; idx < mConnections.size(); idx++) {
		Point2f axialVect = mConnections[idx] - mCentre;
		float mag = magnitude(axialVect);
		mSeparations.push_back(mag);
		mWallVectors.push_back(Point2f(-axialVect.y/mag, axialVect.x/mag));
	}

}
void cvCell::calcWallCorners() {
	mCorners.clear();
	int crnrQty = mConnections.size();
	for (int idx = 0; idx < mConnections.size(); idx++) {
		int real_idx0 = mSortedConnections[idx];
		int real_idx1 = mSortedConnections[(idx + 1) % crnrQty];
		mCorners.push_back(calcCorner(real_idx0, real_idx1));
	}

}

Point2f cvCell::calcCorner(int idx0, int idx1) {
	Point2f retv;
	Point2f axialVect0 = mConnections[idx0] - mCentre;
	Point2f axialVect1 = mConnections[idx1] - mCentre;
	float angle0 = angleFromVector(axialVect0);
	float angle1 = angleFromVector(axialVect1);
	float interAngle = angle1 - angle0;
	if (interAngle < 0) interAngle += 2 * Math::PI;
	if (1 /*interAngle > Math::PI / 2*/) {
		Point2f tmpCorner = intersectLines(mWallCentres[idx0], mWallVectors[idx0], mWallCentres[idx1], mWallVectors[idx1]);
		Point2f axialVect = tmpCorner - mCentre;
		float axialDist = magnitude(axialVect);
		if (axialDist > 2 * mRadius) { // at the edge where some connections are not in place, the remaining connections may be close to , or beyond, 180deg apart
			// so the point may be well beyond (in either direction)...so just kludge and use the centre
			retv = mCentre + ((1.5f * mRadius) / axialDist) * axialVect;

		}
		else {
			retv = (tmpCorner + (mCentre + (mRadius / axialDist) * axialVect)) / 2;
		}

	}
	else {
		retv = intersectLines(mWallCentres[idx0], mWallVectors[idx0], mWallCentres[idx1], mWallVectors[idx1]);
	}
	return retv;
}


void cvCell::calcWallCentres() {
	float averageRad = 0;
	for (int idx = 0; idx < mConnections.size(); idx++) {
		Point2f tmpCentre = calcContourConnIntersect(idx);
		mWallCentres.push_back(tmpCentre);
		averageRad += magnitude(tmpCentre - mCentre);
	}
	mRadius = averageRad / mConnections.size();
}
bool cvCell::reCalcWallCentres() {
	bool retv = false;
	if (mID < 0) { // only do this for cells without contours
		float averageRad = 0;
		for (int idx = 0; idx < mConnections.size(); idx++) {
			int peerID = mConnectionIDs[idx];
			int peerIdx = findPeerCell(peerID);
			Point2f wallLoc = mWallCentres[idx];
			if (peerID >= 0 && peerIdx >= 0) {
				wallLoc = mAllCells->at(peerIdx).getWallTowardsMe(mID);
				float peerWallThickness = mAllCells->at(peerIdx).getAverageWallThickness();
				float wallDist = magnitude(wallLoc - mCentre) - peerWallThickness;
				if (wallDist <= 0) {
					wallLoc = mCentre;
				}
				else {
					Point2f orgVect = mWallCentres[idx] - mCentre;
					wallLoc = mCentre + orgVect * wallDist / magnitude(orgVect);
				}
				mWallCentres[idx] = wallLoc;
				retv = true; // did an update
			}
			averageRad += magnitude(wallLoc - mCentre);
		}
		mRadius = averageRad / mConnections.size();
	}
	return retv;
}

Point2f cvCell::calcContourConnIntersect(int i) {
	Point2f tmpAxialCentre = (mConnections[i] + mCentre) / 2;
	float minDist = -1;
	int minContourIdx = -1;
	Point2f axialVect = mConnections[i] - mCentre;
	float axialDist = magnitude(axialVect);
	float radius = axialDist / 2;
	if (mContours.size() > 0) {
		for (int idx = 0; idx < mContours.size(); idx++) {
			float dispSq = magnitudeSq(tmpAxialCentre - (Point2f)mContours[idx]);
			if (minDist < 0 || dispSq < minDist) {
				minDist = dispSq;
				minContourIdx = idx;
			}
		}
		radius = magnitude((Point2f)mContours[minContourIdx] - mCentre);

	}
	return mCentre + (radius / axialDist) * axialVect;
}

void cvCell::sortConnections() {
	mAngles.clear();
	for (int idx = 0; idx < mConnections.size(); idx++) {
		mSortedConnections.push_back(idx);
		mAngles.push_back(angleFromVector(mConnections[idx] - mCentre));
	}
	for (int idx = 0; idx < mConnections.size(); idx++) {
		float smallestAngle = 1000;
		int smallestIdx = -1;
		for (int idx2 = idx; idx2 < mConnections.size(); idx2++) {
			if (mAngles[mSortedConnections[idx2]] < smallestAngle) {
				smallestAngle = mAngles[mSortedConnections[idx2]];
				smallestIdx = idx2;
			}
		}
		int notSmallest = mSortedConnections[idx];
		mSortedConnections[idx] = mSortedConnections[smallestIdx];
		mSortedConnections[smallestIdx] = notSmallest;
	}

}

Point2f cvCell::intersectLine(Point2f line_start, Point2f line_end, Point2f point)
{
	cv::Point line; //direction of the line
	line.x = line_end.x - line_start.x;
	line.y = line_end.y - line_start.y;
	cv::Point point_from_start; //vector from the point to the start of the line
	point_from_start.x = point.x - line_start.x;
	point_from_start.y = point.y - line_start.y;
	int dot = point_from_start.x * line.x + point_from_start.y * line.y; //dot product of point_to_start * line
	cv::Point intersect;

	int linelength2 = line.x*line.x + line.y*line.y;//length of line squared
	intersect.x = double(dot * line.x) / linelength2 + line_start.x;
	intersect.y = double(dot * line.y) / linelength2 + line_start.y;

	return intersect;
}
Point2f cvCell::intersectLines(Point2f line_start0, Point2f line_vect0, Point2f line_start1, Point2f line_vect1)
{
	float Py = 0;
	float Px = 0;
	if (line_vect0.x == 0 || line_vect1.x == 0) {
		if (line_vect0.x == 0) {
			float m1 = line_vect1.y / line_vect1.x;
			float c1 = line_start1.y - m1 * line_start1.x;
			Px = line_start0.x;
			Py = m1 * Px + c1;

		}
		else {
			float m0 = line_vect0.y / line_vect0.x;
			float c0 = line_start0.y - m0 * line_start0.x;
			Px = line_start1.x;
			Py = m0 * Px + c0;
		}
	}
	else {
		float m0 = line_vect0.y / line_vect0.x;
		float m1 = line_vect1.y / line_vect1.x;
		float c0 = line_start0.y - m0 * line_start0.x;
		float c1 = line_start1.y - m1 * line_start1.x;
		Px = (c1 - c0) / (m0 - m1);
		Py = m0 * Px + c0;
	}
	return Point2f(Px, Py);
}

void cvCell::removeEntry(vector<int> &toclean, int valToRemove) {
	std::vector<int>::iterator itI;
	itI = find(toclean.begin(), toclean.end(), valToRemove);

	if (itI != toclean.end()) {
		toclean.erase(itI);
	}
}

cvCell::~cvCell()
{
}
