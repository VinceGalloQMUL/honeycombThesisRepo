######################################################
#   Production TingueProc.r used in chapter 5 of the thesis
######################################################

setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

fileList016A8_B<-c("016A8_B_WW_1_.csv", "016A8_B_WW_2_.csv")
fileList016B9_C<-c("016B9_C_WW_1_.csv", "016B9_C_WW_2_.csv", "016B9_C_WW_3_.csv")
fileList017A6_A<-c("017A6_A_WW_1_.csv", "017A6_A_WW_2_.csv", "017A6_A_WW_3_.csv")
fileList017B6_A<-c("017B6_A_WW_1_.csv", "017B6_A_WW_2_.csv", "017B6_A_WW_3_.csv")
fileList021A3_6<-c("021A3_6_WW_1_.csv", "021A3_6_WW_2_.csv", "021A3_6_WW_3_.csv")
fileList021B4_6<-c("021B4_6_WW_1_.csv", "021B4_6_WW_2_.csv")
fileList026A7_9<-c("026A7_9_WW_1_.csv", "026A7_9_WW_2_.csv")
fileList026B7_9<-c("026B7_9_WW_1_.csv")


cellspair016A8_B<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList016A8_B)
cellspair016B9_C<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList016B9_C)
cellspair017A6_A<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList017A6_A)
cellspair017B6_A<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList017B6_A)
cellspair021A3_6<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList021A3_6)
cellspair021B4_6<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList021B4_6)
cellspair026A7_9<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList026A7_9)
cellspair026B7_9<-readCellFiles("d:/Projects/PhD/QMUL/Thesis/Stats/Chapter5/Paired", fileList026B7_9)
# cant processPairs (which matches cells by location) over more than one side of one frame
# so need to read frame side at a time, process pairs in each , then concatenate
matchID <- 1
cellspairm016A8_B<-processPairs(cellspair016A8_B, matchID)
matchID <- matchID + nrow(cellspair016A8_B)
cellspairm016B9_C<-processPairs(cellspair016B9_C, matchID)
matchID <- matchID + nrow(cellspair016B9_C)
cellspairm017A6_A<-processPairs(cellspair017A6_A, matchID)
matchID <- matchID + nrow(cellspair017A6_A)
cellspairm017B6_A<-processPairs(cellspair017B6_A, matchID)
matchID <- matchID + nrow(cellspair017B6_A)
cellspairm021A3_6<-processPairs(cellspair021A3_6, matchID)
matchID <- matchID + nrow(cellspair021A3_6)
cellspairm021B4_6<-processPairs(cellspair021B4_6, matchID)
matchID <- matchID + nrow(cellspair021B4_6)
cellspairm026A7_9<-processPairs(cellspair026A7_9, matchID)
matchID <- matchID + nrow(cellspair026A7_9)
cellspairm026B7_9<-processPairs(cellspair026B7_9, matchID)

cellspairm<-rbind(cellspairm016A8_B, cellspairm016B9_C)
cellspairm<-rbind(cellspairm, cellspairm017A6_A)
cellspairm<-rbind(cellspairm, cellspairm017B6_A)
cellspairm<-rbind(cellspairm, cellspairm021A3_6)
cellspairm<-rbind(cellspairm, cellspairm021B4_6)
cellspairm<-rbind(cellspairm, cellspairm026A7_9)
cellspairm<-rbind(cellspairm, cellspairm026B7_9)

processedWalls0<-processCellWallsDedup(cellspairm, TRUE, TRUE)
cellWallsPair<-processedWalls0[[1]]
cellsPair<-processedWalls0[[2]]
processedWalls0<-processCellWallsDedup(cellspairm, FALSE, TRUE)
cellWallsPairOpposite<-processedWalls0[[1]]
cellsPairOpposite<-processedWalls0[[2]]


forPaper = TRUE
#forPaper = FALSE


##############################

histWidth<-10
atLocs = c(1,2,4,5,7,8)

boxCols <- c("lightblue", "yellow")
cellWallsPair <- setDisplacementUnits(cellWallsPair, "dist", histWidth)
cellWallsPairOpposite <- setDisplacementUnits(cellWallsPairOpposite, "dist", histWidth)
cellsPair <- setDisplacementUnits(cellsPair, "offcentreMM", histWidth)
cellsPairOpposite <- setDisplacementUnits(cellsPairOpposite, "offcentreMM", histWidth)

dim(cellsPair)
dim(cellsPair[cellsPair$imageIdx == 0, ])
dim(cellsPair[cellsPair$imageIdx == 1, ])
dim(cellsPair[cellsPair$displacementUnits == 0, ])
dim(cellsPair[cellsPair$classDist == 0, ])
dim(cellsPair[cellsPair$classDist == 0.5, ])
[1] 2587   70
[1] 1315   70
> [1] 1272   70
[1] 887  70
[1] 458  70
[1] 429  70

par(mfrow = c(1,3))
layout(mat = matrix(c(1, 2,3), nrow = 1, ncol = 3))

if (forPaper) {
    boxCols<-c("white","gray")
    bandedStr <- ""
} else {
    boxCols <- c("lightblue", "yellow")
    bandedStr <- sprintf("Within %dmm of junction", histWidth)
}
stage1DistStr <- "Stage 1\nDistance from junction"
stage2DistStr <- "Stage 2\nDistance from junction"
boxplotYstr <- "Change from stage 1 to stage 2"

layoutLocs <- "classDist"

## plot all forms of metrics, to find which ones to discuss
## fig 1.6
measureStr <- "Distance to neighbouring cell"
valNames <- c("sep", "sdSep")
dispAbs <- TRUE
baseVal <- 0
SDmeasureStr <- sprintf("Sigma %s", tolower(measureStr))

## fig 1.2
measureStr <- "Corner angle"
valNames <- c("ang", "sdAngle")
dispAbs <- TRUE
baseVal <- 0
SDmeasureStr <- sprintf("Sigma %s", tolower(measureStr))


#valNames <- c("ang", "sdUsedAngle")
## fig 1.8
measureStr <- "Wall length"
valNames <- c("len", "sdLen")
dispAbs <- TRUE
baseVal <- 0
SDmeasureStr <- sprintf("Sigma %s", tolower(measureStr))

## fig 1.9
measureStr <- "cell area (mm2)"
valNames <- c("", "areaMM2")
SDmeasureStr <- "Cell area (mm2)"
dispAbs <- FALSE
testSet4<-abs(cellsPair[cellsPair[,"classDist"] == 2,valNames[2]])
testSet5<-abs(cellsPair[cellsPair[,"classDist"] == (2.5),valNames[2]])
meanAreaAtDistance = mean(c(testSet4 , testSet5))
baseVal <- meanAreaAtDistance
SDmeasureStr <- sprintf("Delta %s", tolower(measureStr))

#plotTwoPlus(cellWallsPair, "imageIdx", valNames[1], "dist", stage1DistStr, measureStr, stage2DistStr, measureStr, bandedStr, boxplotYstr, layoutLocs, atLocs, TRUE)
#plotTwoPlus(cellsPair, "imageIdx", valNames[2], "offcentreMM", stage1DistStr, SDmeasureStr, stage2DistStr, SDmeasureStr, bandedStr, boxplotYstr,  layoutLocs, atLocs, TRUE, -1, cols = boxCols, limitDist = histWidth)
plotTwoPlus(cellsPair, "imageIdx", valNames[2], "offcentreMM", stage1DistStr, SDmeasureStr, stage2DistStr, "", bandedStr, "",  layoutLocs, atLocs, dispAbs, -1, cols = boxCols, limitDist = histWidth, baseVal, baseVal)
## 700 x 400


abs(cellsPair[cellsPair[,"imageIdx"] == 0,"areaMM2"])

testSet0<-abs(cellsPair[cellsPair[,"classDist"] == 0,"areaMM2"])
testSet1<-abs(cellsPair[cellsPair[,"classDist"] == (0.5),"areaMM2"])
testSet2<-abs(cellsPair[cellsPair[,"classDist"] == 1,"areaMM2"])
testSet3<-abs(cellsPair[cellsPair[,"classDist"] == (1.5),"areaMM2"])
testSet4<-abs(cellsPair[cellsPair[,"classDist"] == 2,"areaMM2"])
testSet5<-abs(cellsPair[cellsPair[,"classDist"] == (2.5),"areaMM2"])
testDelta0 <- testSet0 - meanAreaAtDistance
testDelta1 <- testSet1 - meanAreaAtDistance

> mean(testSet4)
[1] 24.18677
> mean(testSet5)
[1] 24.15876
> sd(testSet4)
[1] 4.001234
> sd(testSet5)
[1] 3.035266
mean(testDelta0)
sd(testDelta0)
mean(testDelta1)
sd(testDelta1)
t.test(testDelta0, testDelta1)

##################### cell by cell change
par(mfrow = c(1,1))

classNames<-c("Stage 1", "Stage 2", "Change")
if (forPaper) {
    colrs<-c("lightgrey", "lightgrey", "lightgrey")
} else {
    colrs<-c("lightblue", "Yellow", "lightgrey", "white")
}

distLim <- 10

## for fig 1.6 & 1.7, use area
colAttribute<-"areaMM2"
yaxisStr <- "Cell area mm2"
titleStrPre <- "Cell area"
useYlimitForArea <- TRUE

## for fig 1.8 & 1.9, use sdAngle
colAttribute<-"sdAngle"
yaxisStr <- "StdDev. per cell corner angles (rad)"
titleStrPre <- "Corner angle deviation"
useYlimitForArea <- FALSE

setstart<-cellsPair[cellsPair$matchID > 0 & cellsPair$imageIdx == 0 & abs(cellsPair$offcentreMM) < distLim,c("matchID", colAttribute)]
setend<-cellsPair[cellsPair$matchID > 0 & cellsPair$imageIdx == 1 & abs(cellsPair$offcentreMM) < distLim,c("matchID", colAttribute)]
bothMatchID<-c(unique(setstart$matchID),unique(setend$matchID))
commonIDs<-bothMatchID[duplicated(bothMatchID)]

## sanity check , not needed in dissertation
#percentage of cells matched stage 1 and stage 2
length(unique(setstart$matchID))
length(unique(setend$matchID))
length(commonIDs)

sdStart<-NULL
sdEnd<-NULL
for(commonID in commonIDs) {
    tmpSD<-setstart[setstart$matchID == commonID, colAttribute]
    sdStart<-c(sdStart, tmpSD)
    tmpSD<-setend[setend$matchID == commonID, colAttribute]
    sdEnd<-c(sdEnd, tmpSD)
}
sdChange <- sdEnd - sdStart
titleStr<-sprintf("%s %s", titleStrPre, "at stage 1 or 2")

################# start, end, change and towards mean #######################
if (forPaper) {
    par(mfrow = c(1,2))
} else {
    par(mfrow = c(1,1))
}

stage1mean<-mean(sdStart)
stage2mean<-mean(sdEnd)

startdiff<-sdStart - stage1mean
startdiff2<-sdStart - stage2mean
endDiff<-sdEnd - stage2mean
cSmaller <- length(sdChange[sdChange < 0])
cLarger <- length(sdChange[sdChange >= 0])
sdChangeToAv <- (startdiff - endDiff) * sign(startdiff)
cNeg <- length(sdChangeToAv[sdChangeToAv < 0])
cPos <- length(sdChangeToAv[sdChangeToAv >= 0])
print(c(cSmaller, cLarger, cNeg, cPos))
xaxisStr <- sprintf("%d cells measured at %s and %s (%d/%d closer to mean)", length(commonIDs), classNames[1], classNames[2], cPos, (cNeg + cPos))
if (forPaper) {
    if(useYlimitForArea) {
        ymax <- max(max(sdStart), max(sdEnd), max(sdChange))
        ymin <- -10 # min(min(sdStart), min(sdEnd), min(sdChange))
        boxplot(sdStart, sdEnd, sdChange, col=colrs, names = classNames, main = "a",ylab = yaxisStr, ylim = c(ymin, ymax))
    } else {
        boxplot(sdStart, sdEnd, sdChange, col=colrs, names = classNames, main = "a",ylab = yaxisStr)
    }
} else {
    boxplot(sdStart, sdEnd, sdChange, sdChangeToAv, col=colrs[c(1,2,3,3)], names = c(classNames, "Toward mean"), main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
    legend("topright", legend = classNames, col = colrs[c(1,2,3,3)], pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
}
segments(1, sdStart[seq(1,length(sdStart),10)], 2, sdEnd[seq(1,length(sdEnd),10)], lty="dashed")
abline( h = 0, lty = 2)

################# start, end, change and towards mean #######################
################# plot of change and towards mean #######################
titleStr<-sprintf("Change of %s %s", titleStrPre, "\nrelative to the value at stage 1")

## produce figs 1.12
s2change<- (sdEnd - sdStart)
#xmax <- max(sdStart)
xmax <- max(startdiff2)
xmin <- min(startdiff2) # 0
ymax <- max(s2change)
ymin <- min(s2change) # 0
titleStr<-sprintf("%s %s", yaxisStr, "change related to deviation from mean")
if (forPaper) {
    xmax <- max(sdStart)
    xmin <- min(sdStart) # 0
    if(useYlimitForArea) {
        ymax <- max(max(sdStart), max(sdEnd), max(sdChange))
        ymin <- -10 # min(min(sdStart), min(sdEnd), min(sdChange))
        plot (sdStart,s2change,main = "b",ylab = sprintf("Change in %s", yaxisStr), xlab = sprintf("%s %s", yaxisStr, "at stage 1"), ylim = c(ymin, ymax), xlim = c(xmin, xmax), xaxs = 'i')
    } else {
        plot (sdStart,s2change,main = "b",ylab = sprintf("Change in %s", yaxisStr), xlab = sprintf("%s %s", yaxisStr, "at stage 1"), xlim = c(xmin, xmax), xaxs = 'i')
    }
    trend<-lm(s2change ~ sdStart)
    #abline( v = stage2mean, lty = 2)
} else {
    plot (startdiff2,s2change,ylab = sprintf("Change in %s", yaxisStr), xlab = sprintf("%s %s", yaxisStr, "at stage 1 relative to Stage 2"), xlim = c(xmin, xmax))
    legend("topright", legend = classNames, col = colrs[c(1,2,3,3)], pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
    trend<-lm(s2change ~ startdiff2)
    abline( v = 0, lty = 2)
}
summary(trend)
abline(trend, lty = 3)
abline( h = 0, lty = 2)
##for paper, export 1000x500

cor1<-cor(startdiff2,s2change, method="spearman") # pearson")
text(xmax/2, 7 * ymax / 8, sprintf("Correlation = %0.3f", cor1))
text(xmax/2, 5 * ymax / 8, sprintf("Coefficient = %0.3f", trend$coefficients[2]))

################# plot of change and towards mean #######################


