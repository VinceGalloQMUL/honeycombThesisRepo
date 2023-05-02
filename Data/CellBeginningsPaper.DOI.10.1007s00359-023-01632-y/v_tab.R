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

showControl <- 0

titleQualStr <- c("Experiment", "Control")
titleIDStr <- c("A", "B", "C", "D")


exptVtabs <- vtabs[vtabs$isRandom == 0, ]
rndVtabs <- vtabs[vtabs$isRandom == 1, ]

dataH1<-abs(exptVtabs$absDeltaBuiltToBisectDeg)
dataH1R<-abs(rndVtabs$absDeltaBuiltToBisectDeg)

############### start Fig 11 A  ###################

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
xaxisStr <- "Divergence (degrees)"
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

################ End Fig  11 A  ##################


################ Start Fig  11 B  ##################

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
xLabelStr <- "Angle of stimulus bisection (deg)"
yLabelStr <- "Angle of built wall (deg)"
cor3<-cor(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl], vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl], method="spearman") #pearson")

plot(vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl], vtabs$builtWallAngleDeg[vtabs$isRandom == showControl], xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), xlab = "", ylab = "", xaxt = 'n', yaxt = 'n')
title(titleStr, adj = 0.1, cex.main = 2.5)
linTabs <- lm(vtabs$builtWallAngleDeg[vtabs$isRandom == showControl] ~ vtabs$wallBisectedAngleDeg[vtabs$isRandom == showControl])
axis(1, seq(-90,90, 30))
axis(2, seq(-90,90, 30))
title(xlab = xLabelStr, cex.lab = 2)
title(ylab = yLabelStr, cex.lab = 2, mgp = c(2.5,0,0))
abline(linTabs, lty = 3)

################ End Fig 11 B  ##################

################ Start Fig 12 ##################
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
################ End Fig 12  ##################
