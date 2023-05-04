######################################################
#   Production V_tab.r 
######################################################
source("library/thesis_lib.r")

vpits<-read.csv("Chapter3/V_pit.csv", sep=",")

for(idx in 1:length(vpits$ID)) {
    vpits$Vangle[idx] <- angleObtuse( abs(angleDeg(vpits$wall0Angle[idx] - vpits$wall1Angle[idx])))
    vpits$deltaBuiltToBisectDeg[idx]<-angleLT90(angleDeg(vpits$builtWallAngle[idx] - vpits$wallBisectedAngle[idx]))
    vpits$deltaPitTangentToBisectDeg[idx]<-angleLT90(angleDeg(vpits$pitToPitTangent[idx] - vpits$wallBisectedAngle[idx]))
    vpits$deltaBuiltToTangentDeg[idx]<-angleLT90(angleDeg(vpits$builtWallAngle[idx] - vpits$pitToPitTangent[idx]))
    vpits$deltaBisectToTangentDeg[idx]<-angleLT90(angleDeg(vpits$wallBisectedAngle[idx] - vpits$pitToPitTangent[idx]))
    vpits$deltaBuiltToPitTangentDeg[idx] <-180 * vpits$deltaBuiltToPitTangent[idx] / pi
    vpits$deltaBuiltBisectedDeg[idx] <-180 * vpits$deltaBuiltBisected[idx] / pi

    vpits$pitToPitTangentDeg[idx]<-angleLT90(angleDeg(vpits$pitToPitTangent[idx]))
    vpits$wallBisectedAngleDeg[idx]<-angleLT90(angleDeg(vpits$wallBisectedAngle[idx]))
    vpits$builtWallAngleDeg[idx]<-angleLT90(angleDeg(vpits$builtWallAngle[idx]))
    vpits$builtWallAngleDeg[idx]<-commonDirection(vpits$builtWallAngleDeg[idx], vpits$wallBisectedAngleDeg[idx])

}
#plot (vpits$deltaPitTangentToBisectDeg, vpits$deltaBuiltToBisectDeg)
#plot (vpits$deltaPitTangentToBisectDeg, vpits$deltaBuiltToTangentDeg)

vpits$builtFractionOfTan <- vpits$deltaBuiltToBisectDeg / vpits$deltaPitTangentToBisectDeg

################### compare deviation from bisection vs common tangent
############ Fig 13 #################### 
par(mfrow = c(1,1), cex.axis = 1.0)

#sidx<-23
#vpits$builtWallAngle[sidx] - vpits$wallBisectedAngle[sidx]
#vpits$pitToPitTangent[sidx] - vpits$wallBisectedAngle[sidx]

axisMax = max(max(vpits$deltaBuiltBisectedDeg), max(vpits$deltaBuiltToPitTangentDeg))
titleStr <- "Divergence of built wall from\nbisection vs pit's common tangent"
xLabelStr <- "Divergence (degrees) built wall from bisection"
yLabelStr <- "Divergence (degrees) built wall from pit's common tangent"
morePitish <- vpits[vpits$deltaBuiltBisectedDeg < vpits$deltaBuiltToPitTangentDeg,]
moreBisectish <- vpits[vpits$deltaBuiltBisectedDeg > vpits$deltaBuiltToPitTangentDeg,]

#plot(morePitish$deltaBuiltBisectedDeg, morePitish$deltaBuiltToPitTangentDeg, pch = 15, xaxs = "i",yaxs = "i", xlim=c(0,axisMax), ylim = c(0,axisMax), main = titleStr, xlab = xLabelStr, ylab = yLabelStr)
plot(morePitish$deltaBuiltBisectedDeg, morePitish$deltaBuiltToPitTangentDeg, pch = 15, xaxs = "i",yaxs = "i", xlim=c(0,axisMax), ylim = c(0,axisMax), xlab = xLabelStr, ylab = yLabelStr)
points(moreBisectish$deltaBuiltBisectedDeg, moreBisectish$deltaBuiltToPitTangentDeg, pch = 17)
#plot(vpits$deltaBuiltBisectedDeg, vpits$deltaBuiltToPitTangentDeg, xaxs = "i",yaxs = "i", xlim=c(0,axisMax), ylim = c(0,axisMax), main = titleStr, xlab = xLabelStr, ylab = yLabelStr)
segments(0,0,axisMax,axisMax, lty = 3)
text(axisMax/4, 3*axisMax / 4, sprintf("Closer to bisection (%d)", dim(morePitish)[1]))
text( 3 * axisMax/4, axisMax / 4, sprintf("Closer to pit's common tangent (%d)", dim(moreBisectish)[1]))
cor(vpits$deltaBuiltToPitTangentDeg, vpits$deltaBuiltBisectedDeg, method="pearson")

 
########### End of Fig 13 ##############

########### Start of Fig 14 and 15 ###############
### Thsi relies on having run v_tab.R and pitsCommonTangent.R previously in the same session as the values are redrawn in these graphs
par(mfrow = c(1,1))

doFig14 = TRUE
#doFig14 = FALSE ## to do fig 15

dataTanWithV<-abs(vpits$deltaBuiltToPitTangentDeg)
dataBisectWithPits<-abs(vpits$deltaBuiltToBisectDeg)

dataTanNoV<-abs(pitPair$deltaangle[pitPair$isRandom == 0])
dataBisectNoPits<-abs(exptVtabs$absDeltaBuiltToBisectDeg)

if (doFig14) {
    titleStr<-"A - Divergence of built wall\nfrom pits-common-tagent"
    legendStr = c("With V", "No V")
    xaxisStr <- "Divergence from pits-common-tagent (deg)"
    set1 <- dataTanWithV
    set2 <- dataTanNoV
    col1 <- 3
    col2 <- 1
    useColrs[1] <- colrs[col1]
    useColrs[2] <- colrs[col2]
} else {
    titleStr<-"B - Divergence of built wall\nfrom V bisection"
    xaxisStr <- "Divergence from V bisection (deg)"
    legendStr = c("With pits", "No pits")
    set1 <- dataBisectWithPits
    set2 <- dataBisectNoPits
}

setS1m <- mean(set1)
setS1sd <- sd(set1)
set1m <- mean(set1)
set1sd <- sd(set1)
set2m <- mean(set2)
set2sd <- sd(set2)
divsQty <- 20
barMin <- min(min(set1),min(set2)) #0
barMax <- max(max(set1),max(set2))
barDiv <- floor((barMax - barMin)/(divsQty + 1) + 1)
barMin <- floor(barMin / barDiv)* barDiv
barMax <- (floor(barMax / barDiv) + 1)* barDiv
barMax <- 60
barMin <- 0
barDiv <- 5
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(set1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(set2, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
spread <- 1
# optional B/W
useColrs = c("black", "gray")

barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5), ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
legend("topright", legend = legendStr, col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline(h=0)

## copy image at 700x500

########### End of Fig 14 and 15 ##############
