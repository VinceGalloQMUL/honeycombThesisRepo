######################################################
#   Production V_tab.r used in chapter 3 of the thesis
######################################################
#library(ggplot2)
setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")


vtabs<-read.csv("Chapter3/V_tab.csv", sep=",")
for(idx in 1:length(vtabs$ID)) {
    vtabs$Vangle[idx] <- angleObtuse( abs(angleDeg(vtabs$wall0Angle[idx] - vtabs$wall1Angle[idx])))
    vtabs$deltaBuiltToBisectDeg[idx]<-angleLT90(angleDeg(vtabs$builtWallAngle[idx] - vtabs$wallBisectedAngle[idx]))
    vtabs$wallBisectedAngleDeg[idx]<-angleLT90(angleDeg(vtabs$wallBisectedAngle[idx]))
    vtabs$builtWallAngleDeg[idx]<-angleLT90(angleDeg(vtabs$builtWallAngle[idx]))
    vtabs$builtWallAngleDeg[idx]<-commonDirection(vtabs$builtWallAngleDeg[idx], vtabs$wallBisectedAngleDeg[idx])
##    vtabs$wallBisectedAngleDeg[idx]<-angleLT90(vtabs$wallBisectedAngleDeg[idx])
}
vtabs$distFraction <- vtabs$distApexToWallmm + vtabs$distApexToWall2mm
vtabs$distFraction <- vtabs$distApexToWallmm /vtabs$distFraction
vtabs$absDeltaBuiltToBisectDeg <- abs(vtabs$deltaBuiltToBisectDeg)

showControl <- 1
# or 
showControl <- 0

titleQualStr <- c("Experiment", "Control")
titleIDStr <- c("A", "B", "C", "D")

####### start of plot showing Angle of built wall vs stimulus bisection ########
par(mfrow = c(1,1), cex.axis = 1.5)
#check the distributions of V angle and orientation
#plot 1
axisMax = max(max(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl]), max(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl]))
axisMin = min(min(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl]), min(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl]))
axisMax<-max(abs(axisMin), abs(axisMax))
axisMax <- 90
#titleStr <- "Angle of built wall vs stimulus bisection"
#titleStr <- paste(titleStr, " - ", titleQualStr[showControl + 1])
titleStr <- "b"
xLabelStr <- "Angle of stimulus bisection"
yLabelStr <- "Angle of built wall"
cor3<-cor(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl], vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl], method="spearman") #pearson")

plot(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl], vtabs$builtWallAngleDeg[vtabs$isRandom == showControl], xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), xlab = "", ylab = "", xaxt = 'n', yaxt = 'n')
title(titleStr, adj = 0.1, cex.main = 2.5)
linTabs <- lm(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl] ~ vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl])
#text(-axisMax/2, 3 * axisMax / 4, sprintf("Correlation = %0.3f", cor3))
#text(-axisMax/2, 5 * axisMax / 8, sprintf("Coefficient = %0.3f", linTabs$coefficients[2]))
axis(1, seq(-90,90, 30))
axis(2, seq(-90,90, 30))
title(xlab = xLabelStr, cex.lab = 2)
title(ylab = yLabelStr, cex.lab = 2, mgp = c(2.5,0,0))
abline(linTabs)
cor3
linTabs$coefficients[2]
summary (linTabs)
length(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl])
#points(vtabs$wallBisectedAngleDeg[vtabs$isRandom == 1], vtabs$builtWallAngleDeg[vtabs$isRandom == 1], pch = 4)

####### End of plot showing Angle of built wall vs stimulus bisection ########
wilcox.test(exptDeltaBuilt$absDeltaBuiltToBisectDeg, rndDeltaBuilt$absDeltaBuiltToBisectDeg)

######### try using frame aggregation ##############

exptVtabs <- vtabs[vtabs$isRandom == 0, ]
rndVtabs <- vtabs[vtabs$isRandom == 1, ]

exptDeltaBuilt <- aggregate(exptVtabs, list(exptVtabs$ID), FUN=mean)
rndDeltaBuilt <- aggregate(rndVtabs, list(rndVtabs$ID), FUN=mean)
titleStr <- "Distribution of angle of built wall divergence\n from stimulus bisection (aggregated)"
nDivs <- 10
# or
exptDeltaBuilt <- exptVtabs
rndDeltaBuilt <- rndVtabs
titleStr <- "Distribution of angle of built wall divergence\n from stimulus bisection"
nDivs <- 18

hist(exptDeltaBuilt$absDeltaBuiltToBisectDeg)
hist(rndDeltaBuilt$absDeltaBuiltToBisectDeg)
## show as joint bar chart
### buckets of 5, 0-50
xMax <- max(exptDeltaBuilt$absDeltaBuiltToBisectDeg, max(rndDeltaBuilt$absDeltaBuiltToBisectDeg))
xMax <- nDivs * (floor(xMax / nDivs) + 1)
commonBreaks <- seq(0, xMax, xMax/nDivs)
nBreaks <- length(commonBreaks) - 1
hExpt <- hist(exptDeltaBuilt$absDeltaBuiltToBisectDeg, breaks = commonBreaks, plot = FALSE)
hRnd <- hist(rndDeltaBuilt$absDeltaBuiltToBisectDeg, breaks = commonBreaks, plot = FALSE)
yMax <- max(max(hExpt$counts, hRnd$counts))

xaxisStr <- "Degrees"
yaxisStr <- "Frequency"
spread <- 1
barplot (hExpt$counts, ylim = c(0, yMax), width = c(0.5, 0.5),main = titleStr, ylab = yaxisStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hRnd$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, horiz = F, inset = c(0,0))

######### end try using frame aggregation ##############



  ###### start ######## histograms of built vs bisection ############# - Fig 1-11
par(mfrow = c(1,2))
#boxplot(vtabs$deltaBuiltToBisectDeg[vtabs$isRandom == showControl])
#### plot 1 & 2, deposit relative to bisection
dataH1<-abs(exptVtabs$absDeltaBuiltToBisectDeg)
dataH1R<-abs(rndVtabs$absDeltaBuiltToBisectDeg)
dataH1A<-abs(exptDeltaBuilt$absDeltaBuiltToBisectDeg)
dataH1RA<-abs(rndDeltaBuilt$absDeltaBuiltToBisectDeg)
summary(dataH1)
summary(dataH1R)
summary(dataH1A)
summary(dataH1RA)

hQty<-18
yMax<-max(hist (dataH1, breaks = seq(-90,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH1R, breaks = seq(-90,90,5), plot=FALSE)$counts)

titleStrT<-" - Divergence of built wall from\nV bisection"
titleStr <- paste(titleIDStr[1], titleStrT, " - ", titleQualStr[1])
XaxisRange<-c(0,90)
sumry<-summary(dataH1)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
xLabelStr<-"Degrees"
hist (dataH1, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))
titleStr <- paste(titleIDStr[2], titleStrT, " - ", titleQualStr[2])
XaxisRange<-c(0,90)
sumry<-summary(dataH1R)
xLabelStr<-"Degrees"
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1R, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))
########## 2nd pair...aggreagted
yMax<-max(hist (dataH1A, breaks = seq(-90,90,5), plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH1RA, breaks = seq(-90,90,5), plot=FALSE)$counts)
titleStr <- paste(titleIDStr[3], titleStrT, " - ", titleQualStr[1], " aggregate")
XaxisRange<-c(0,90)
sumry<-summary(dataH1A)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1A, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))
titleStr <- paste(titleIDStr[4], titleStrT, " - ", titleQualStr[2], " aggregate")
XaxisRange<-c(0,90)
sumry<-summary(dataH1RA)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1RA, breaks = seq(-90,90,5), ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=seq(0,90,10))

## copy image at 900x500

wilcox.test(dataH1A, dataH1RA)

################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
par(mfrow = c(1,1))

aggregate = FALSE
titleStr<-"A - Divergence of built wall from\nV bisection"
titleStr<-"a"
if (aggregate) {
    set1 <- dataH1A
    set2 <- dataH1RA
} else {
    set1 <- dataH1
    set2 <- dataH1R
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
barMax <- 90
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
xaxisStr <- "Degrees"
par(mfrow = c(1,1), cex.axis = 1.5)
# optional B/W
useColrs = c("black", "gray")

#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),ylab = "", xlab = "", col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 2.0, horiz = F)
title(titleStr, adj = 0.1, cex.main = 2.5)
title(xlab = xaxisStr, cex.lab = 2)
title(ylab = "Frequency", cex.lab = 2, mgp = c(2.5,0,0))
abline(h = 0)

## copy image at 620x500
set1m
set1sd
set2m
set2sd
wilcox.test(set1, set2)
 
>  setS1m
[1] 10.53812
> setS1sd
[1] 7.645011
> set2m
[1] 30.06473
> set2sd
[1] 21.24318
> wilcox.test(set1, set2)
W = 1215, p-value = 3.482e-11


######### end of histograms  ############


###### histograms of V-tab splay and orientation ==========
par(mfrow = c(2,1))
titleStr<-"Angle between blades of stimuli: V-shaped"
titleStr <- paste(titleStr, " - ", titleQualStr[showControl + 1])
xLabelStr<-"Angle between blades (degrees)"
hist (vtabs$Vangle[vtabs$isRandom == showControl], 18, xlim=c(90,180), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(90, 180, 15))
sumry<-summary(vtabs$Vangle[vtabs$isRandom == showControl])
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry

titleStr<-"Orientation of stimuli: V-shaped"
titleStr <- paste(titleStr, " - ", titleQualStr[showControl + 1])
xLabelStr<-"Orientation (degrees)"
hist (vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl], 18, xlim=c(-90,90), main=titleStr, xlab=xLabelStr, col="blue", xaxt="n")
axis(1, at=seq(-90, 90, 30))
sumry<-summary(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl])
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry


#################### position probs ####################

mean(vtabs$distFraction[vtabs$isRandom == showControl])
##[1] 0.1853878
sd(vtabs$distFraction[vtabs$isRandom == showControl])
##[1] 0.1293906
hist(vtabs$distFraction[vtabs$isRandom == showControl])
### shows it to be non-normal -> t-test not valid, nor ranking test...except possibly once I get a control population

par(mfrow = c(1,2))

nDivs <- 10
# population of bulge locations cf distance from a triple intersection and the distribution of slot probabilities
divRange <- 0.5
#oneBasedPopns<-rep(1 / nDivs, nDivs) ## linear prediction
oneBasedPopns<-seq(1, nDivs, by=1) ## triagular prediction
oneBasedPopn<-nDivs*(nDivs+1)/2 ## triagular prediction
breaks <- seq(0, divRange, by=divRange/nDivs)
breakMids<-seq(divRange/(nDivs*2), divRange, by=divRange/nDivs)

##xaxisStr <- "Relative distance from apex to nearest and next walls"
yaxisStr <- "Frequency"

popnRatio <- length(vtabs$distFraction[vtabs$isRandom == 0])/oneBasedPopn
predictedPopn<-oneBasedPopns * popnRatio
hv<-hist(vtabs$distFraction[vtabs$isRandom == showControl], breaks, plot=FALSE)
ymax<-floor(max(max(predictedPopn), max(hv$counts)) + 1)
popnRatio <- length(vtabs$distFraction[vtabs$isRandom == 1])/oneBasedPopn
predictedPopn<-oneBasedPopns * popnRatio
ymax<-max(max(predictedPopn), ymax)
hv<-hist(vtabs$distFraction[vtabs$isRandom == showControl], breaks, plot=FALSE)
ymax<-floor(max(ymax, max(hv$counts)) + 1)

showControl <- 0
popnRatio <- length(vtabs$distFraction[vtabs$isRandom == showControl])/oneBasedPopn
predictedPopn<-oneBasedPopns * popnRatio
hv<-hist(vtabs$distFraction[vtabs$isRandom == showControl], breaks, plot=FALSE)
sumry<-summary(vtabs$distFraction[vtabs$isRandom == showControl])
xLabelStr<-sprintf("Distance, median %0.2f, 1st & 3rd quartile %0.2f & %0.2f", sumry[3], sumry[2], sumry[5])
titleStr <- paste( titleIDStr[showControl + 1], " - Relative distance between apex\nand nearest walls", " - ", titleQualStr[showControl + 1])
plot(hv,, ylim=c(0,ymax), xlab=xLabelStr, ylab = yaxisStr, main=titleStr)
points(breakMids, predictedPopn)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry

showControl <- 1
popnRatio <- length(vtabs$distFraction[vtabs$isRandom == showControl])/oneBasedPopn
predictedPopn<-oneBasedPopns * popnRatio
hv<-hist(vtabs$distFraction[vtabs$isRandom == showControl], breaks, plot=FALSE)
sumry<-summary(vtabs$distFraction[vtabs$isRandom == showControl])
xLabelStr<-sprintf("Distance, median %0.2f, 1st & 3rd quartile %0.2f & %0.2f", sumry[3], sumry[2], sumry[5])
titleStr <- paste( titleIDStr[showControl + 1], " - Relative distance between apex\nand nearest walls", " - ", titleQualStr[showControl + 1])
plot(hv,, ylim=c(0,ymax), xlab=xLabelStr, ylab = yaxisStr, main=titleStr)
points(breakMids, predictedPopn)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry

wilcox.test(vtabs$distFraction[vtabs$isRandom == 0], vtabs$distFraction[vtabs$isRandom == 1])

#################### position probs aggregated ####################

mean(exptDeltaBuilt$distFraction)
sd(exptDeltaBuilt$distFraction)
hist(exptDeltaBuilt$distFraction)
mean(rndDeltaBuilt$distFraction)
sd(rndDeltaBuilt$distFraction)
hist(rndDeltaBuilt$distFraction)


hv<-hist(exptDeltaBuilt$distFraction, breaks, plot=FALSE)
ymax<-max(hv$counts)
hv<-hist(rndDeltaBuilt$distFraction, breaks, plot=FALSE)
ymax<-floor(max(ymax, max(hv$counts)) + 1)

showControl <- 0
hv<-hist(exptDeltaBuilt$distFraction, breaks, plot=FALSE)
sumry<-summary(exptDeltaBuilt$distFraction)
xLabelStr<-sprintf("Relative distance, median %0.2f, inter-quartile %0.2f & %0.2f", sumry[3], sumry[2], sumry[5])
titleStr <- paste( titleIDStr[3], " - Relative distance between apex\nand nearest walls", " - ", titleQualStr[showControl + 1], " aggregate")
plot(hv,, ylim=c(0,ymax), xlab=xLabelStr, ylab = yaxisStr, main=titleStr)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry

showControl <- 1
hv<-hist(rndDeltaBuilt$distFraction, breaks, plot=FALSE)
sumry<-summary(rndDeltaBuilt$distFraction)
xLabelStr<-sprintf("Relative distance, median %0.2f, inter-quartile %0.2f & %0.2f", sumry[3], sumry[2], sumry[5])
titleStr <- paste( titleIDStr[4], " - Relative distance between apex\nand nearest walls", " - ", titleQualStr[showControl + 1], " aggregate")
plot(hv,, ylim=c(0,ymax), xlab=xLabelStr, ylab = yaxisStr, main=titleStr)
abline(v=sumry[4])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
sumry

mean(exptDeltaBuilt$distFraction)
sd(exptDeltaBuilt$distFraction)
mean(rndDeltaBuilt$distFraction)
sd(rndDeltaBuilt$distFraction)
wilcox.test(exptDeltaBuilt$distFraction, rndDeltaBuilt$distFraction)
...
[1] 0.1810622
[1] 0.08280886
[1] 0.367419
[1] 0.05776821
	Wilcoxon rank sum test

data:  exptDeltaBuilt$distFraction and rndDeltaBuilt$distFraction
W = 7, p-value = 5.658e-09
alternative hypothesis: true location shift is not equal to 0

################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
par(mfrow = c(1,1))

exptDeltaBuilt <- aggregate(exptVtabs, list(exptVtabs$ID), FUN=mean)
rndDeltaBuilt <- aggregate(rndVtabs, list(rndVtabs$ID), FUN=mean)

#titleStr<-"A - Relative distance between apex\nand nearest walls"
titleStr<-"a"
aggregate = TRUE
aggregate = FALSE
if (aggregate) {
    set1 <- exptDeltaBuilt$distFraction
    set2 <- rndDeltaBuilt$distFraction
} else {
    set1 <- exptVtabs$distFraction
    set2 <- rndVtabs$distFraction
}
#set1 <- exptDeltaBuilt$distFraction
#set2 <- rndDeltaBuilt$distFraction
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
barMax <- 0.5
barMin <- 0
barDiv <- 0.05
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(set1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(set2, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
spread <- 1

par(mfrow = c(1,1), cex.axis = 1.5)
xaxisStr <- "Ratio of distance to nearest wall relative to 2nd nearest"
#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.5), width = c(0.5, 0.5), ylab = "", xlab = "", col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 2.0, horiz = F)
#title(titleStr, adj = 0.1, cex.main = 2.5)
title(xlab = xaxisStr, cex.lab = 2)
title(ylab = "Frequency", cex.lab = 2, mgp = c(2.5,0,0))
abline(h=0)

## copy image at 900x700
set1m
set1sd
set2m
set2sd
wilcox.test(set1, set2)
