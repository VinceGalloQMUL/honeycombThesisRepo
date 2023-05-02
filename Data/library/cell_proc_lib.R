angleFromDeltas<-function(dx, dy) {
    ang<-0
    if (dy != 0) {
        ang<-atan(dy / dx)
        if (dx < 0) {
            ang<-ang + pi
        } else {
            if (dy < 0) {
                ang<-ang + 2 * pi
            }
        }
    }
    return (ang)
}

angleFromCoords<-function(p0x, p0y, p1x, p1y) {
    deltaX<-p1x - p0x
    deltaY<-p1y - p0y
    ang0<-angleFromDeltas(deltaX , deltaY)
    return (ang0)
}

angleDiffFromCoords<-function(p0x, p0y, p1x, p1y, p2x, p2y) {
    deltaX<-p1x - p0x
    delta1X<-p2x - p1x
    deltaY<-p1y - p0y
    delta1Y<-p2y - p1y
    ang0<-angleFromDeltas(deltaX , deltaY)
    ang1<-angleFromDeltas(delta1X , delta1Y)
    ang<-ang1 - ang0
    if (ang < 0) {
        ang<-ang + 2 * pi
    }
    if (ang > pi) {
        ang<-ang - pi
    }

    return (ang)
}

axisRange<-function(vals0, vals1) {
    return (c( min(min(vals0),min(vals1)), max(max(vals0),max(vals1))))
}

distSq<-function(x0,y0,x1,y1) {
    x<-x1 - x0
    y<-y1 - y0
    return (x * x + y * y)
}

processPairs<-function(vals, startMatchID) {
    imgCol<-which( colnames(vals)=="imageIdx" )
    idCol<-which( colnames(vals)=="ID" )
    cxCol<-which( colnames(vals)=="centreX" )
    cyCol<-which( colnames(vals)=="centreY" )
    vals$matchID <- -1
    matchID<-startMatchID
    for(idx in 1:length(vals$ID)) {
##        print (c(idx, vals[idx, imgCol]))
        if (vals[idx, imgCol] == 0) {
            locX<-vals[idx, cxCol]
            locY<-vals[idx, cyCol]
            minIdx<- -1
            minDist<-1000000
            for(idxo in 1:length(vals$ID)) {
                if (vals[idxo, imgCol] == 1 && vals[idxo, "matchID"] == -1) {
                    locXo<-vals[idxo, cxCol]
                    locYo<-vals[idxo, cyCol]
                    dist<-distSq(locX, locY, locXo, locYo)
                    if (dist < minDist) {
                        minDist<-dist
                        minIdx<-idxo
                    }
                }
            }
            if (minIdx >= 0 && minDist < 60) { ### ~ 3mm * 8pixels per mm ^ 2
##                print (c(idx, minIdx, minDist))
                vals[idx, "matchID"] <- matchID
                vals[minIdx, "matchID"] <- matchID
                matchID<- matchID + 1
            }
        }
    }
##    print(vals$matchID)
    return ( vals)
}


processCellWallsDedup<-function(vals, deduplicate = FALSE, pairing = FALSE) {
    cellWalls<-data.frame()
    if (pairing) { 
        cellWalls[1,1:9] <- 0

        colnames(cellWalls)<-c("id","dist","wallcnt","len", "sep", "ang", "Vang", "imageIdx", "matchID")
    } else {
        cellWalls[1,1:7] <- 0

        colnames(cellWalls)<-c("id","dist","wallcnt","len", "sep", "ang", "Vang")
    }
    wallCount<-1
    cxCol<-which( colnames(vals)=="centreX" )
    cyCol<-which( colnames(vals)=="centreY" )
    c0xCol<-which( colnames(vals)=="c0X" )
    c1xCol<-which( colnames(vals)=="c1X" )
    c0yCol<-which( colnames(vals)=="c0Y" )
    c0aCol<-which( colnames(vals)=="a0" )
    c0SCol<-which( colnames(vals)=="sep0" )
    colStep<-c1xCol - c0xCol
    c0lenMM<-which( colnames(vals)=="c0lenMM" )
    colBa<-which(colnames(vals)=="ba")
    for(idx in 1:length(vals$ID)) {
        wallcnt<-vals[idx,"qtyCorners"]
        tmpLen<-NULL
        tmpSep<-NULL
        tmpAngle<-NULL
        tmpUsedAngles <- NULL
        tmpVang<-NULL
        aveAngle<-(2 * pi)/wallcnt
        refAngle<-vals[idx,c0aCol]
        cornerAngles<-NULL
        holdWallCount<-wallCount
        layoutIsReliable<-TRUE
        boundaryAngle<-vals[idx,colBa]
        #print(boundaryAngle)
        for(idxW in 0:(wallcnt - 1)) {
            angleToAdjacentCell<-vals[idx,c0aCol + (idxW) * colStep]
            angleToBoundary<-angleToAdjacentCell - boundaryAngle
            distFromBoundary<-vals[idx,"offcentreMM"]
            if (angleToBoundary < 0) {
                angleToBoundary<-angleToBoundary + 2 * pi
            }
            #print(angleToBoundary)
            useThisWall<-TRUE
            if (deduplicate) {
#                if( (angleToAdjacentCell > pi & distFromBoundary < 0) | (angleToAdjacentCell < pi & distFromBoundary > 0)) {
#                    useThisWall<-FALSE
#                }
#            } else { ## invert the selection logic to get those in the other direction
#                if( (angleToAdjacentCell > pi & distFromBoundary > 0) | (angleToAdjacentCell < pi & distFromBoundary < 0)) {
#                    useThisWall<-FALSE
#                }
                if( (angleToBoundary > pi & distFromBoundary < 0) | (angleToBoundary < pi & distFromBoundary > 0)) {
                    useThisWall<-FALSE
                }
            } else { ## invert the selection logic to get those in the other direction
                if( (angleToBoundary > pi & distFromBoundary > 0) | (angleToBoundary < pi & distFromBoundary < 0)) {
                    useThisWall<-FALSE
                }
            }
            if(useThisWall) {
                if (pairing) { 
                    cellWalls[wallCount,"imageIdx"]<-vals[idx,"imageIdx"]
                    cellWalls[wallCount,"matchID"]<-vals[idx,"matchID"]
                }
            }
            oneWallLen<-vals[idx,c0lenMM + idxW * colStep]
            if(useThisWall) {
                cellWalls[wallCount,"id"]<-vals[idx,"ID"]
                cellWalls[wallCount,"dist"]<-distFromBoundary
                cellWalls[wallCount,"wallcnt"]<-wallcnt
                cellWalls[wallCount,"len"]<-oneWallLen
            }
            sep<-vals[idx,c0SCol + idxW * colStep]
            
            deltaX<-(vals[idx,c0xCol + (idxW) * colStep] - vals[idx,cxCol])
            deltaY<-(vals[idx,c0yCol + (idxW) * colStep] - vals[idx,cyCol])
            if (deltaY == 0) {
                print(vals[idx,"ID"])
                deltaY<-0.01 ## layoutIsReliable<-FALSE
            }
            ang<-angleFromDeltas(deltaX, deltaY)
            cornerAngles<-c(cornerAngles,ang)
            #retrieve the angle to each adjacent cell, and compute the delta from the previous
            #thus calculate the angular difference from one cell direction to the other
            if (idxW < (wallcnt - 1) ) {
                ang<-(vals[idx,c0aCol + (1 + idxW) * colStep]) - (angleToAdjacentCell)
            } else {
                ang<-(2 * pi) - (angleToAdjacentCell - vals[idx,c0aCol])
            }
            
            #cell shoudl be equispaced, this then is the error/jitter
            ang<-ang - aveAngle
            tmpSep<-c(tmpSep, sep)
            if(useThisWall) {
                cellWalls[wallCount,"sep"]<-sep
                cellWalls[wallCount,"ang"]<-ang
                tmpUsedAngles <- c(tmpUsedAngles, ang)
            }
            tmpAngle<-c(tmpAngle, ang )
            tmpLen<-c(tmpLen, oneWallLen)
            if(useThisWall) {
                wallCount<-(wallCount + 1)
            }
        }
        vals[idx,"sdLen"]<-sd(tmpLen)
        vals[idx,"sdSep"]<-sd(tmpSep)
        vals[idx,"minSep"]<-min(tmpSep)
        vals[idx,"meanSep"]<-mean(tmpSep)
        vals[idx,"maxSep"]<-max(tmpSep)
        vals[idx,"minLen"]<-min(tmpLen)
        vals[idx,"meanLen"]<-mean(tmpLen)
        vals[idx,"maxLen"]<-max(tmpLen)
        vals[idx,"totLen"]<-sum(tmpLen)
        vals[idx,"sdAngle"]<-sd(tmpAngle)
        vals[idx,"sdUsedAngle"]<-sd(tmpUsedAngles)
        sorted<-sort(cornerAngles, index.return=TRUE)
        
        for(idxW in 0:(wallcnt - 1)) {
            angleToAdjacentCell<-vals[idx,c0aCol + (idxW) * colStep]
            angleToBoundary<-angleToAdjacentCell - boundaryAngle
            #print(angleToBoundary)
            distFromBoundary<-vals[idx,"offcentreMM"]
            if (angleToBoundary < 0) {
                angleToBoundary<-angleToBoundary + 2 * pi
            }
            useThisWall<-TRUE
            if (deduplicate) {
#                if( (angleToAdjacentCell > pi & distFromBoundary < 0) | (angleToAdjacentCell < pi & distFromBoundary > 0)) {
#                    useThisWall<-FALSE
#                }
#            } else { ## invert the selection logic to get those in the other direction
#                if( (angleToAdjacentCell > pi & distFromBoundary > 0) | (angleToAdjacentCell < pi & distFromBoundary < 0)) {
#                    useThisWall<-FALSE
#                }
                if( (angleToBoundary > pi & distFromBoundary < 0) | (angleToBoundary < pi & distFromBoundary > 0)) {
                    useThisWall<-FALSE
                }
            } else { ## invert the selection logic to get those in the other direction
                if( (angleToBoundary > pi & distFromBoundary > 0) | (angleToBoundary < pi & distFromBoundary < 0)) {
                    useThisWall<-FALSE
                }
            }
            cIdx0<-sorted$ix[idxW+1] - 1;
            cIdx1<-sorted$ix[((idxW + 1) %% wallcnt) + 1] - 1
            cIdx2<-sorted$ix[((idxW + 2) %% wallcnt) + 1] - 1
            ang<-angleDiffFromCoords(vals[idx,c0xCol + (cIdx0) * colStep], vals[idx,c0yCol + (cIdx0) * colStep], vals[idx,c0xCol + (cIdx1) * colStep], vals[idx,c0yCol + (cIdx1) * colStep], vals[idx,c0xCol + (cIdx2) * colStep], vals[idx,c0yCol + (cIdx2) * colStep])
            oneVang<-0
            if (layoutIsReliable) {
                oneVang<-ang - aveAngle
            } else {
                oneVang<-0
            }
            if(useThisWall) {
                cellWalls[holdWallCount,"Vang"]<-oneVang
                holdWallCount<-holdWallCount + 1
            }
            tmpVang<-c(tmpVang, ang )
        }
         vals[idx,"sdVang"]<-sd(tmpVang)
   }
    return (list(cellWalls, vals))
}

plotPair<-function(vals0, vals1, Yname, Xname,aXlab, aYlab, bXlab, bYlab) {
    XaxisLim<-c(-20,20)
    YaxisLim<- axisRange(vals0[,Yname], vals1[,Yname])
    plot (vals0[,Yname] ~ vals0[,Xname], xlim=XaxisLim, ylim=YaxisLim , xlab = aXlab, ylab = aYlab)
    plot (vals1[,Yname] ~ vals1[,Xname], xlim=XaxisLim, ylim=YaxisLim , xlab = bXlab, ylab = bYlab)
}

plotPairByIndex<-function(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab) {
    XaxisLim<-c(-20,20)
    YaxisLim<- axisRange(vals[vals[,idxName] == 0,Yname], vals[vals[,idxName] == 1,Yname])
    plot (vals[vals[,idxName] == 0,Yname] ~ vals[vals[,idxName] == 0,Xname], xlim=XaxisLim, ylim=YaxisLim , xlab = aXlab, ylab = aYlab)
    plot (vals[vals[,idxName] == 1,Yname] ~ vals[vals[,idxName] == 1,Xname], xlim=XaxisLim, ylim=YaxisLim , xlab = bXlab, ylab = bYlab)
}
plotPairByIndexAbsX<-function(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab, absY = FALSE, maxY = -1) {
    XaxisLim<-c(0,20)
    if (absY) {
        YaxisLim<- axisRange(abs(vals[vals[,idxName] == 0,Yname]), abs(vals[vals[,idxName] == 1,Yname]))
        if (maxY >0) {
            YaxisLim[2]<-maxY
        }
        #print(YaxisLim)
        plot (abs(vals[vals[,idxName] == 0,Yname]) ~ abs(vals[vals[,idxName] == 0,Xname]), xlim=XaxisLim, ylim=YaxisLim , xlab = aXlab, ylab = aYlab)
        plot (abs(vals[vals[,idxName] == 1,Yname]) ~ abs(vals[vals[,idxName] == 1,Xname]), xlim=XaxisLim, ylim=YaxisLim , xlab = bXlab, ylab = bYlab)
    } else {
        YaxisLim<- axisRange(vals[vals[,idxName] == 0,Yname], vals[vals[,idxName] == 1,Yname])
        if (maxY >0) {
            YaxisLim[2]<-maxY
        }
        #print(YaxisLim)
        plot (vals[vals[,idxName] == 0,Yname] ~ abs(vals[vals[,idxName] == 0,Xname]), xlim=XaxisLim, ylim=YaxisLim , xlab = aXlab, ylab = aYlab)
        plot (vals[vals[,idxName] == 1,Yname] ~ abs(vals[vals[,idxName] == 1,Xname]), xlim=XaxisLim, ylim=YaxisLim , xlab = bXlab, ylab = bYlab)
    }
}
plotTriple<-function(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab, hXlab, hYlab, className, layoutLocs, absY = FALSE, maxY = -1) {
    plotPairByIndexAbsX(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab, absY, maxY)
    histSet0<-abs(vals[abs(vals[,Xname]) < 30,Yname])
    histSet1<-vals[abs(vals[,Xname]) < 30,className]
    YaxisLim<- axisRange(histSet0, histSet0)
    if (maxY >0) {
        YaxisLim[2]<-maxY
    }
    boxplot(histSet0~histSet1, at = layoutLocs, ylim=YaxisLim, xlab = hXlab, ylab = hYlab)
    probs<-NULL
    probsTime<-NULL
    qtyPlots<-(30/histWidth)
    previousTest0<-NULL
    for(setBase in 0:(qtyPlots - 1)) {
        testSet0<-abs(vals[vals[,className] == setBase,Yname])
        testSet1<-abs(vals[vals[,className] == (setBase + 0.5),Yname])
        TtestW<-wilcox.test( testSet0, testSet1)
        #TtestT<-t.test( testSet0, testSet1)
        #probmax<-max(TtestW$p.value, TtestT$p.value)
        probmax <- TtestW$p.value
        probmaxt<-0
        if (!is.null(previousTest0)) {
            TtestWt<-wilcox.test( testSet0, previousTest0)
            #TtestTt<-t.test( testSet0, previousTest0)
            #probmaxt<-max(TtestWt$p.value, TtestTt$p.value)
            #probsTime<-c(probsTime,probmaxt )
            probsTime <- c(probsTime,TtestWt$p.value )
        }
        previousTest0<-testSet0
        probs<-c(probs,probmax )
        print(c(length(testSet0), length(testSet1), probmax, probmaxt, mean(testSet0), mean(testSet1), sd(testSet0), sd(testSet1)))
    }
    for(setBase in 1:(qtyPlots)) {
        clr = "blue"
        pStyle = 15
        if (probs[setBase] > 0.05) {
            clr = "red"
            pStyle = 19
        }
        points(3 * (setBase - 1) + 1.5, YaxisLim[1] + probs[setBase] * (YaxisLim[2] - YaxisLim[1]),cex = 3, pch=pStyle, col=clr)
        if (setBase < qtyPlots)
            segments(3 * (setBase - 1) + 1.5, YaxisLim[1] + probs[setBase] * (YaxisLim[2] - YaxisLim[1]) , (setBase)*3 + 1.5, YaxisLim[1] + probs[setBase + 1] * (YaxisLim[2] - YaxisLim[1]))
    }
}
plotTwoPlus<-function(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab, hXlab, hYlab, className, layoutLocs, absY = FALSE, maxY = -1) {
    plotPairByIndexAbsX(vals, idxName, Yname, Xname,aXlab, aYlab, bXlab, bYlab, absY, maxY)
    testSet0<-abs(vals[vals[,className] == 0,Yname])
    testSet1<-abs(vals[vals[,className] == (0.5),Yname])
    YaxisLim<- axisRange(testSet0, testSet0)
    if (maxY >0) {
        YaxisLim[2]<-maxY
    }
    boxplot(testSet0 , testSet1, names = c("Stage 1", "Stage 2"), ylim=YaxisLim, xlab = hXlab, ylab = hYlab)
    TtestW<-wilcox.test( testSet0, testSet1)
    probmax <- TtestW$p.value
    print(c(length(testSet0), length(testSet1), probmax, mean(testSet0), mean(testSet1), sd(testSet0), sd(testSet1)))
}

readBoundaryVector<-function(dir, filename) {
    setwd(dir)
    boundaryVec<-read.csv(filename, sep=",", nrows=1, header=FALSE)
    #	"Scale: " << scalePt0.x << "," <<  scalePt0.y << "," << scalePt1.x << "," << scalePt1.y << "," << scaleDistanceMM << "mm, " << scaleMagnitude << "," << scale << "pixels/mm\n";

    # WW,1501,1158,1497,1453, 1765.24,1161.58, 1236.76,1154.42, 1761.24,1456.58, 1232.76,1449.42, 8.8087,
    return(c(boundaryVec[1,2], boundaryVec[1,3], boundaryVec[1,4],boundaryVec[1,5],  boundaryVec[1,14]))
}
readCellFile<-function(dir, filename) {
    setwd(dir)
    boundaryVec<-readBoundaryVector(dir, filename)
    retv<-read.csv(filename, skip=1, sep=",")
    # 	ss << mID << "," << mCentre.x / pixelsPerMM << "," << mCentre.y / pixelsPerMM << "," << cA/(pixelsPerMM * pixelsPerMM) << "," << mAverageWallThickness / (pixelsPerMM );
	#	ss << "," << (mCorners[idxC]).x / pixelsPerMM << "," << (mCorners[idxC]).y / pixelsPerMM << "," << lenP/ pixelsPerMM << "," << mAngles[idx] << "," << mSeparations[idx]/ pixelsPerMM ;

    retv$b0x<-boundaryVec[1]
    retv$b0y<-boundaryVec[2]
    retv$b1x<-boundaryVec[3]
    retv$b1y<-boundaryVec[4]
    retv$ba<-angleFromDeltas(boundaryVec[3] - boundaryVec[1], boundaryVec[2] - boundaryVec[4]) # Y inverted as coordinates are from top
    retv$scale<-boundaryVec[5]
    return (retv)
}
readCellFiles<-function(dir, filenameCollection) {
    retv<-NULL
    for(f in filenameCollection) {
        tmpCells <- readCellFile(dir, f)
        if (is.null(retv)) {
            retv <- tmpCells
        } else {
            retv <- rbind(retv, tmpCells)
       }
    }
    return(retv)
}

setDisplacementUnits <- function(vals, colname, hWidth) {
    vals$displacementUnits<-floor(abs(vals[,colname])/hWidth)
    vals$classDist<-vals$displacementUnits + (0.5* vals$imageIdx)
    return(vals)
}

