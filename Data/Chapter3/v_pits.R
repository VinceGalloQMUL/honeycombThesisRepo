######################################################
#   Production V_tab.r used in chapter 3 of the thesis
######################################################
#library(ggplot2)
setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
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
plot (vpits$deltaPitTangentToBisectDeg, vpits$deltaBuiltToBisectDeg)
plot (vpits$deltaPitTangentToBisectDeg, vpits$deltaBuiltToTangentDeg)

vpits$builtFractionOfTan <- vpits$deltaBuiltToBisectDeg / vpits$deltaPitTangentToBisectDeg

################### compare deviation from bisection vs common tangent
hist (vpits$Vangle, 18)
 
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

summary(vpits$deltaBuiltBisectedDeg)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00913  7.14089 13.05719 14.05207 19.67909 37.73047 
> summary(vpits$deltaBuiltToPitTangentDeg)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00001  2.48957  5.90003  8.85009 12.47461 41.46771 
 
########### start ########## dual plot wallBisectedAngleDeg) cf max(vpits$builtWallAngleDeg & pitToPitTangentDeg
########## not used ####################
par(mfrow = c(2,1))
axisMax = max(max(vpits$wallBisectedAngleDeg), max(vpits$builtWallAngleDeg))
axisMin = min(min(vpits$wallBisectedAngleDeg), min(vpits$builtWallAngleDeg))
axisMax<-max(abs(axisMin), abs(axisMax))
#plot 3
axisMax = max(max(vpits$pitToPitTangentDeg), axisMax)
axisMin = min(min(vpits$pitToPitTangentDeg), axisMin)
axisMax<-max(abs(axisMin), abs(axisMax))
titleStr <- "A - Orientation of built wall vs stimulus bisection"
xLabelStr <- "Orientation of stimulus bisection"
yLabelStr <- "Orientation of built wall"
cor4<-cor(vpits$builtWallAngleDeg, vpits$wallBisectedAngleDeg, method="pearson")
linTabs2 <- lm(vpits$builtWallAngleDeg ~ vpits$wallBisectedAngleDeg)

plot(vpits$wallBisectedAngleDeg, vpits$builtWallAngleDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
text(-axisMax/2, 7 * axisMax / 8, sprintf("Correlation = %0.3f", cor4))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs2$coefficients[2]))
abline(linTabs2)


titleStr <- "B - Orientation of built wall vs pit common tangent"
xLabelStr <- "Orientation of pit common tangent"
yLabelStr <- "Orientation of built wall"
cor5<-cor(vpits$builtWallAngleDeg, vpits$pitToPitTangentDeg, method="pearson")
linTabs3 <- lm(vpits$builtWallAngleDeg ~ vpits$pitToPitTangentDeg)

plot(vpits$pitToPitTangentDeg, vpits$builtWallAngleDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
text(-axisMax/2, 7 * axisMax / 8, sprintf("Correlation = %0.3f", cor5))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs3$coefficients[2]))
abline(linTabs3)

##### end ######## dual plot wallBisectedAngleDeg) cf max(vpits$builtWallAngleDeg & pitToPitTangentDeg


  ###### start ######## histograms of built vs composite stimuli ############# - Fig 1-14
par(mfrow = c(1,2))
 
dataH1<-abs(vpits$deltaBuiltToPitTangentDeg)
dataH2<-abs(vpits$deltaBuiltToBisectDeg)
hQty<-18
yMax<-max(hist (dataH1, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH2, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-yMax
XaxisRange<-c(0,90)
sumry<-summary(dataH1)
titleStr<-"A - Divergence of built wall\nfrom pits common tangent (with V)"
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f - %0.1f", sumry[4], sumry[2], sumry[5])
xLabelStr<-"Degrees"
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

sumry<-summary(dataH2)
titleStr<-"B - Divergence of built wall\nfrom V bisection (with pits)"
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f - %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH2, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

summary(dataH1)
d(sdataH1)
summary(dataH2)
sd(dataH2)
wilcox.test(abs(vpits$deltaBuiltToPitTangentDeg), abs(vpits$deltaBuiltToBisectDeg))

    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00001  2.48957  5.90003  8.85009 12.47461 41.46771 
[1] 8.666144
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00917  7.14077 13.05714 14.05213 19.67953 37.73099 
[1] 9.220021
	Wilcoxon rank sum test with continuity correction

data:  abs(vpits$deltaBuiltToPitTangentDeg) and abs(vpits$deltaBuiltToBisectDeg)
W = 2076, p-value = 5.505e-05

################### replicated graphs from single element stimuli #############
dataHA<-abs(pitPair$deltaangle[pitPair$isRandom == 0])
yMax<-max(hist (dataHA, breaks = seq(0,90,5), plot=FALSE)$counts)
sumry<-summary(dataHA)
summary(dataHA)
sd(dataHA)
XaxisRange<-c(0,90)

titleStr<-"C - Divergence of built wall\nfrom pits common tangent (no V)"
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f - %0.1f", sumry[3], sumry[2], sumry[5])
xLabelStr<-"Degrees"
hist (dataHA, breaks = seq(0,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, border = "blue", xlab=xLabelStr, xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))
XaxisRange<-c(0,90)

dataH1<-abs(exptVtabs$absDeltaBuiltToBisectDeg)
summary(dataH1)
sd(dataH1)

hQty<-18
yMax<-max(hist (dataH1, breaks = seq(-90,90,5), plot=FALSE)$counts)

titleStr<-"D - Divergence of built wall\nfrom V bisection (no pits)"
XaxisRange<-c(0,90)
sumry<-summary(dataH1)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, border = "blue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))

wilcox.test(abs(vpits$deltaBuiltToPitTangentDeg), abs(pitPair$deltaangle[pitPair$isRandom == 0]))
W = 2463, p-value = 0.723

wilcox.test(abs(vpits$deltaBuiltToBisectDeg), abs(exptVtabs$absDeltaBuiltToBisectDeg))
W = 3954, p-value = 0.01007


################### END replicated graphs from singel element stimuli #############

################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
par(mfrow = c(1,1))

dataTanWithV<-abs(vpits$deltaBuiltToPitTangentDeg)
dataBisectWithPits<-abs(vpits$deltaBuiltToBisectDeg)

dataTanNoV<-abs(pitPair$deltaangle[pitPair$isRandom == 0])
dataBisectNoPits<-abs(exptVtabs$absDeltaBuiltToBisectDeg)

titleStr<-"A - Divergence of built wall\nfrom pits-common-tagent"
legendStr = c("With V", "No V")
set1 <- dataTanWithV
set2 <- dataTanNoV
col1 <- 3
col2 <- 1
useColrs[1] <- colrs[col1]
useColrs[2] <- colrs[col2]
or
titleStr<-"B - Divergence of built wall\nfrom V bisection"
legendStr = c("With pits", "No pits")
set1 <- dataBisectWithPits
set2 <- dataBisectNoPits


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

xaxisStr <- "Degrees"
#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),main = titleStr, ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = legendStr, col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline(h=0)

## copy image at 700x500




################### try aggregation

vpitsAgg <- aggregate(vpits, list(vpits$ID), FUN=mean)

wilcox.test(abs(vpitsAgg$deltaBuiltToBisectDeg),abs(vpitsAgg$deltaBuiltToPitTangentDeg))
 
hist(abs(vpitsAgg$deltaBuiltToBisectDeg))
hist(abs(exptDeltaBuilt$absDeltaBuiltToBisectDeg))
wilcox.test(vpitsAgg$deltaBuiltToBisectDeg,abs(exptDeltaBuilt$absDeltaBuiltToBisectDeg))
W = 83, p-value = 0.003911

hist (abs(vpitsAgg$deltaBuiltToPitTangentDeg))
hist(abs(vpitsAgg$deltaBuiltToBisectDeg))
wilcox.test(abs(vpitsAgg$deltaBuiltToBisectDeg),abs(vpitsAgg$deltaBuiltToPitTangentDeg))
W = 112, p-value = 0.1182
 
##############################################################################
 
par(mfrow = c(2,2))
par(mfrow = c(2,1))
 
#plot 1
axisMax = max(max(vpits$deltaPitTangentToBisectDeg), max(vpits$deltaBuiltToBisectDeg))
axisMin = min(min(vpits$deltaPitTangentToBisectDeg), min(vpits$deltaBuiltToBisectDeg))
axisMax<-max(abs(axisMin), abs(axisMax))
titleStr <- "A - Deviation from bisection of built wall vs pit common tangent"
xLabelStr <- "Deviation of common tangent from bisection"
yLabelStr <- "Deviation of built from bisection"
plot(vpits$deltaPitTangentToBisectDeg, vpits$deltaBuiltToBisectDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
lin <- lm(vpits$deltaBuiltToBisectDeg ~ vpits$deltaPitTangentToBisectDeg)
abline(lin)
cor1<-cor(vpits$deltaBuiltToBisectDeg, vpits$deltaPitTangentToBisectDeg, method="pearson")
text(-axisMax/2, 3 * axisMax / 4, sprintf("Correlation = %0.3f", cor1))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", lin$coefficients[2]))
summary (lin)

#plot 2
axisMax = max(max(vpits$deltaBisectToTangentDeg), max(vpits$deltaBuiltToTangentDeg))
axisMin = min(min(vpits$deltaBisectToTangentDeg), min(vpits$deltaBuiltToTangentDeg))
axisMax<-max(abs(axisMin), abs(axisMax))
titleStr <- "B - Deviation from  pit common tangent of built wall vs bisection"
xLabelStr <- "Deviation of built from common tangent"
yLabelStr <- "Deviation of bisection from common tangent"
plot(vpits$deltaBisectToTangentDeg, vpits$deltaBuiltToTangentDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
lin2 <- lm(vpits$deltaBuiltToTangentDeg ~ vpits$deltaBisectToTangentDeg)
abline(lin2)
cor2<-cor(vpits$deltaBuiltToTangentDeg, vpits$deltaBisectToTangentDeg, method="pearson")
text(-axisMax/2, 3 * axisMax / 4, sprintf("Correlation = %0.3f", cor2))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", lin2$coefficients[2]))
summary (lin2)

############### V-Tabs ###################

vtabs<-read.csv("D:/Projects/PhD/Research/CombFormation/Stigmergy/2021/V_tab.csv", sep=",")
for(idx in 1:length(vtabs$ID)) {
    vtabs$Vangle[idx] <- angleObtuse( abs(angleDeg(vtabs$wall0Angle[idx] - vtabs$wall1Angle[idx])))
    vtabs$deltaBuiltToBisectDeg[idx]<-angleLT90(angleDeg(vtabs$builtWallAngle[idx] - vtabs$wallBisectedAngle[idx]))
    vtabs$wallBisectedAngleDeg[idx]<-angleLT90(angleDeg(vtabs$wallBisectedAngle[idx]))
    vtabs$builtWallAngleDeg[idx]<-angleLT90(angleDeg(vtabs$builtWallAngle[idx]))
    vtabs$builtWallAngleDeg[idx]<-commonDirection(vtabs$builtWallAngleDeg[idx], vtabs$wallBisectedAngleDeg[idx])
##    vtabs$wallBisectedAngleDeg[idx]<-angleLT90(vtabs$wallBisectedAngleDeg[idx])
}

                       
par(mfrow = c(2,1))
#check the distributions of V angle and orientation


#plot 1
axisMax = max(max(vtabs$wallBisectedAngleDeg), max(vtabs$builtWallAngleDeg))
axisMin = min(min(vtabs$wallBisectedAngleDeg), min(vtabs$builtWallAngleDeg))
axisMax<-max(abs(axisMin), abs(axisMax))
titleStr <- "A - Angle of built wall vs stimulus bisection"
xLabelStr <- "Angle of stimulus bisection"
yLabelStr <- "Angle of built wall"
cor3<-cor(vtabs$builtWallAngleDeg, vtabs$wallBisectedAngleDeg, method="pearson")

plot(vtabs$wallBisectedAngleDeg, vtabs$builtWallAngleDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
linTabs <- lm(vtabs$builtWallAngleDeg ~ vtabs$wallBisectedAngleDeg)
text(-axisMax/2, 3 * axisMax / 4, sprintf("Correlation = %0.3f", cor3))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs$coefficients[2]))
abline(linTabs)
summary (linTabs)

########### start ########## dual plot wallBisectedAngleDeg) cf max(vpits$builtWallAngleDeg & pitToPitTangentDeg
axisMax = max(max(vpits$wallBisectedAngleDeg), max(vpits$builtWallAngleDeg))
axisMin = min(min(vpits$wallBisectedAngleDeg), min(vpits$builtWallAngleDeg))
axisMax<-max(abs(axisMin), abs(axisMax))
#plot 3
axisMax = max(max(vpits$pitToPitTangentDeg), axisMax)
axisMin = min(min(vpits$pitToPitTangentDeg), axisMin)
axisMax<-max(abs(axisMin), abs(axisMax))
titleStr <- "A - Orientation of built wall vs stimulus bisection"
xLabelStr <- "Orientation of stimulus bisection"
yLabelStr <- "Orientation of built wall"
cor4<-cor(vpits$builtWallAngleDeg, vpits$wallBisectedAngleDeg, method="pearson")
linTabs2 <- lm(vpits$builtWallAngleDeg ~ vpits$wallBisectedAngleDeg)

plot(vpits$wallBisectedAngleDeg, vpits$builtWallAngleDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
text(-axisMax/2, 7 * axisMax / 8, sprintf("Correlation = %0.3f", cor4))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs2$coefficients[2]))
abline(linTabs2)


titleStr <- "B - Orientation of built wall vs pit common tangent"
xLabelStr <- "Orientation of pit common tangent"
yLabelStr <- "Orientation of built wall"
cor5<-cor(vpits$builtWallAngleDeg, vpits$pitToPitTangentDeg, method="pearson")
linTabs3 <- lm(vpits$builtWallAngleDeg ~ vpits$pitToPitTangentDeg)

plot(vpits$pitToPitTangentDeg, vpits$builtWallAngleDeg, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
text(-axisMax/2, 7 * axisMax / 8, sprintf("Correlation = %0.3f", cor5))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs3$coefficients[2]))
abline(linTabs3)

##### end ######## dual plot wallBisectedAngleDeg) cf max(vpits$builtWallAngleDeg & pitToPitTangentDeg

#ggplot(vtabs, aes(x=builtWallAngleDeg, y=wallBisectedAngleDeg)) + 
#geom_point(color='#2980B9', size = 4) + 
#geom_smooth(method=lm, se= TRUE, color='#2C3E50')

#ggplot(vpits, aes(x=builtWallAngleDeg, y=wallBisectedAngleDeg)) + 
#geom_point(color='#2980B9', size = 4) + 
#geom_smooth(method=lm, se= TRUE, color='#2C3E50')

boxplot(vtabs$wallBisectedAngleDeg, vpits$wallBisectedAngleDeg, vtabs$builtWallAngleDeg, vpits$builtWallAngleDeg, vpits$deltaBuiltToBisectDeg)

cor(vpits$wallBisectedAngleDeg, vtabs$builtWallAngleDeg, method="pearson")

###### histograms of V-tab splay ==========
par(mfrow = c(2,1))
titleStr<-"Angle between blades of stimuli: V-shaped"
xLabelStr<-"Angle between blades (degrees)"
hist (vtabs$Vangle, 18, xlim=c(90,180), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(90, 180, 15))
sumry<-summary(vtabs$Vangle)

abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
titleStr<-"Orientation of stimuli: V-shaped"
xLabelStr<-"Orientation (degrees)"
hist (vtabs$wallBisectedAngleDeg, 18, xlim=c(-90,90), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(-90, 90, 30))
sumry<-summary(vtabs$wallBisectedAngleDeg)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)



titleStr<-"Angle between blades of stimuli: V-shaped with pits"
hist ( vpits$Vangle, 18, xlim=c(90,180), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(90, 180, 15))
sumry<-summary(vpits$Vangle)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
titleStr<-"Orientation of stimuli: V-shaped with pits"
xLabelStr<-"Orientation (degrees)"
hist (vpits$wallBisectedAngleDeg, 18, xlim=c(-90,90), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(-90, 90, 30))
sumry<-summary(vpits$wallBisectedAngleDeg)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)

##############################


htabs<-hist (vtabs$Vangle, 18, plot=FALSE)
hpits<-hist (vpits$Vangle, 18, plot=FALSE)
histlen<-max(length(hpits$density),length(htabs$density))
tabLen<-max(length(vtabs$Vangle),length(vpits$Vangle))
combinedTab <- data.frame(stimulus = c(rep ("tabs",tabLen), rep("pits",tabLen)),
        value = c(vtabs$Vangle, rep(NA, tabLen - length(vtabs$Vangle)), vpits$Vangle,rep(NA, tabLen - length(vpits$Vangle))))
ggplot(combinedTab, aes(x=value, fill=stimulus)) + geom_histogram(alpha=0.6)+facet_grid(stimulus ~ .)
mean(  vtabs$Vangle)
mean(  vpits$Vangle)  
mean(  vtabs$Vangle)
[1] 120.9334
[1] 121.9771
min(  vtabs$Vangle)
[1] 92.42397
min(  vpits$Vangle)                        
[1] 90.0369
max(  vtabs$Vangle)
[1] 165.9638
max(  vpits$Vangle)                        
[1] 150.516
summary(vpits$Vangle)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
  90.04  112.03  124.33  121.98  133.24  150.52
  summary(vtabs$Vangle)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
  92.42  104.81  120.41  120.93  133.03  165.96 
  
  ###### start ######## histograms of built vs bisection ############# - Fig 1-11
par(mfrow = c(1,2))
#boxplot(vtabs$deltaBuiltToBisectDeg)
#### plot 1 & 2, deposit relative to bisection
dataH1<-vtabs$deltaBuiltToBisectDeg
dataH1A<-abs(vtabs$deltaBuiltToBisectDeg)
hQty<-18
yMax<-max(hist (dataH1, breaks = seq(-90,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH2, breaks = seq(-90,90,5), plot=FALSE)$counts)
yMax<-yMax
sumry<-summary(dataH1)

# absolute values
hQty<-18
yMax<-max(hist (dataH1, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH1A, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-yMax
sumry<-summary(dataH1)
titleStr<-"A - Divergence of built wall from V bisection"
XaxisRange<-c(-90,90)
xLabelStr<-sprintf("Divergence (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

titleStr<-"B - Absolute divergence of built wall from V bisection"
XaxisRange<-c(0,90)
xLabelStr<-sprintf("Divergence (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH1A, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
sumry<-summary(dataH1A)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))
summary(dataH1)
summary(dataH1A)
 ######### end ############
 
  ###### start ######## histograms of built vs composite stimuli ############# - Fig 1-14
par(mfrow = c(2,1))
 
dataH1<-abs(vpits$deltaBuiltToPitTangentDeg)
dataH2<-abs(vpits$deltaBuiltToBisectDeg)
hQty<-18
yMax<-max(hist (dataH1, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH2, breaks = seq(0,90,5), plot=FALSE)$counts)
yMax<-yMax
XaxisRange<-c(0,90)
sumry<-summary(dataH1)
titleStr<-"A - Divergence of built wall from pits common tangent (with V)"
xLabelStr<-sprintf("Deviation (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

sumry<-summary(dataH2)
titleStr<-"B - Deviation of built wall from V bisection (with pits)"
xLabelStr<-sprintf("Deviation (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH2, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

summary(dataH1)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00001  2.48957  5.90003  8.85009 12.47461 41.46771 
> summary(dataH2)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
 0.00917  7.14077 13.05714 14.05213 19.67953 37.73099 
 
 t.test(abs(vpits$deltaBuiltToBisectDeg), abs(vtabs$deltaBuiltToBisectDeg))
 
 #################### 2 pits #################################
 v2pits<-read.csv("D:/Projects/PhD/Research/CombFormation/Stigmergy/2021/twopitswall.csv", sep=",")
for(idx in 1:length(v2pits$type)) {
    v2pits$pitsangle[idx] <- angleLT90(angleFromVetorDeg(v2pits$pit1x[idx] - v2pits$pit0x[idx], v2pits$pit1y[idx] - v2pits$pit0y[idx]))
    v2pits$tangentangle[idx] <- angleLT90(v2pits$pitsangle[idx] + 90)
    v2pits$wallangle[idx] <- angleLT90(angleFromVetorDeg(v2pits$w1x[idx] - v2pits$w0x[idx], v2pits$w1y[idx] - v2pits$w0y[idx]))
    v2pits$tangentangle[idx] <- commonDirection(v2pits$tangentangle[idx], v2pits$wallangle[idx])
}
v2pits$deltaangle <- v2pits$wallangle- v2pits$tangentangle

summary (v2pits$deltaangle)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max. 
-20.73007  -6.08618  -0.59897  -0.05083   6.13187  36.99086 
summary (abs(v2pits$deltaangle))
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
  0.000   2.232   6.343   7.964  11.762  36.991 
  
titleStr <- "Angle of built wall vs pits common tangent"
xLabelStr <- "Angle of pits common tangent"
yLabelStr <- "Angle of built wall"
#cor(Y ~ x)
corpits<-cor(v2pits$wallangle, v2pits$tangentangle, method="pearson")
axisMax<- 90
#plot ( X ~ Y)
plot(v2pits$tangentangle, v2pits$wallangle, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)
#lm (Y ~ X)
linPits <- lm(v2pits$wallangle ~ v2pits$tangentangle)
text(-axisMax/2, 3 * axisMax / 4, sprintf("Sample count = %d", length(v2pits$wallangle)))
text(-axisMax/2, 5 * axisMax / 8, sprintf("Correlation = %0.3f", corpits))
text(-axisMax/2, 4 * axisMax / 8, sprintf("Coefficient = %0.3f", linPits$coefficients[2]))
abline(linPits)
summary (linPits)

plot(v2pits$tangentangle, v2pits$deltaangle, xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), main=titleStr, xlab = xLabelStr, ylab = yLabelStr)

  ###### start ######## histograms of built vs pits common tangent ############# - Fig 1-8

par(mfrow = c(1,2))

dataH1<-v2pits$deltaangle
hQty<-18
yMax<-max(hist (dataH1, breaks = seq(-90,90,5), plot=FALSE)$counts)
sumry<-summary(dataH1)
XaxisRange<-c(-90,90)

titleStr<-"A - Divergence of built wall from pits common tangent"
xLabelStr<-sprintf("Divergence (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(-90,90,10))

dataH1A<-abs(v2pits$deltaangle)
yMax<-max(hist (dataH1A, breaks = seq(0,90,5), plot=FALSE)$counts)
sumry<-summary(dataH1A)
XaxisRange<-c(0,90)

titleStr<-"B - Absolute divergence of built wall from pits common tangent"
xLabelStr<-sprintf("Divergence (degrees), median %0.1f, 1st quartile %0.1f, 3rd quartile %0.1f", sumry[4], sumry[2], sumry[5])
hist (dataH1A, breaks = seq(0,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))
################## end ##########
# ============investigation , but currently excluded ===============
looking at the wall angle vs pit angle chart. It seems to show a bias of the wall towards 90 in excess of the tangent angle
 hp<-hist (v2pits$tangentangle, 18)
 ## this appears to show bimodally skewed distribution supporting the observation...but, how well spread are the pit angles?
 hw<-hist (v2pits$wallangle, 18)
 ## answer, not very
 plot (hp$density/hw$density)
 ## so when regularised, the sew seems less apparent (non-existent?)
 
v2pits$signCorrectedDelta<-v2pits$deltaangle * sign(v2pits$tangentangle)
plot(abs(v2pits$tangentangle), v2pits$deltaangle * sign(v2pits$tangentangle))
drawMean<-function(m, midval, width) {
    segments(x0 = midval - (width - 1), y0 = m, x1 = midval + (width -1), y1 = m, lwd = 4) 
}
drawTopOrBottom<-function(m, midval, width) {
    segments(x0 = midval - (width - 2), y0 = m, x1 = midval + (width -2), y1 = m, lwd = 1) 
}
drawSide<-function(m, sideloc, updown) {
    segments(x0 = sideloc, y0 = m - updown, x1 = sideloc, y1 = m + updown, lwd = 1) 
}
drawBox<-function(m, s, midval, width) {
    drawMean(m, midval, width)
    drawTopOrBottom(m + s, midval, width)
    drawTopOrBottom(m - s, midval, width)
    drawSide(m, midval -(width - 2), s)
    drawSide(m, midval +(width - 2), s)
}
for(midval in c(45, 55, 65, 75, 85)) {
    subset<-v2pits$signCorrectedDelta[abs(v2pits$tangentangle) > (midval - 5) & abs(v2pits$tangentangle) < (midval + 5)]
    ms<-mean(subset)
    sds<-sd(subset)
    drawBox(ms, sds, midval, 5) #points(midval, ms, pch = 22)
    points(midval, ms + sds, pch = 25)
    points( midval, ms - sds, pch = 24)
}
abline(h=0, lty=2)


# ============ end of exclusion ====================

