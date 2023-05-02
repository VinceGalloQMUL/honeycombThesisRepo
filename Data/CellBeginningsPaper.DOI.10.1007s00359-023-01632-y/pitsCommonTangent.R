######################################################
#   Production pitsCommonTangent.r used in chapter 3 of the thesis and now Cell beginnings paper
######################################################
setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")
source("library/cell_proc_lib.r")

pitPair<-read.csv("Chapter3/twopitswall.csv", sep=",")
for(idx in 1:length(pitPair$ID)) {
    pitPair$pitsangle[idx] <- angleLT90(angleFromVetorDeg(pitPair$pit1x[idx] - pitPair$pit0x[idx], pitPair$pit1y[idx] - pitPair$pit0y[idx]))
    pitPair$tangentangle[idx] <- angleLT90(pitPair$pitsangle[idx] + 90)
    pitPair$wallangle[idx] <- angleLT90(angleFromVetorDeg(pitPair$w1x[idx] - pitPair$w0x[idx], pitPair$w1y[idx] - pitPair$w0y[idx]))
    pitPair$tangentangle[idx] <- commonDirection(pitPair$tangentangle[idx], pitPair$wallangle[idx])
    pitPair$pitMidX[idx] <- (pitPair$pit1x[idx] + pitPair$pit0x[idx])/2
    pitPair$pitMidY[idx] <- (pitPair$pit1y[idx] + pitPair$pit0y[idx])/2
    pitPair$pitMidLen[idx] <- distPointToPoint(pitPair$pit0x[idx], pitPair$pit0y[idx], pitPair$pit1x[idx], pitPair$pit1y[idx])
    tmpIntersect <- intersection(pitPair$pit0x[idx], pitPair$pit0y[idx],pitPair$pit1x[idx], pitPair$pit1y[idx],pitPair$w0x[idx], pitPair$w0y[idx],pitPair$w1x[idx], pitPair$w1y[idx])
    pitPair$intersectX[idx] <- tmpIntersect[1]
    pitPair$intersectY[idx] <- tmpIntersect[2]
    pitPair$distMidToIntersectFraction[idx] <- distPointToPoint(pitPair$pitMidX[idx], pitPair$pitMidY[idx], pitPair$intersectX[idx], pitPair$intersectY[idx]) / pitPair$pitMidLen[idx]
    pitPair$wallMidX[idx] <- (pitPair$w1x[idx] - pitPair$w0x[idx]) / 2
    pitPair$wallMidY[idx] <- (pitPair$w1y[idx] - pitPair$w0y[idx]) / 2
    dp0 <- distPointToPoint(pitPair$wallMidX[idx], pitPair$wallMidY[idx], pitPair$pit0x[idx], pitPair$pit0y[idx])
    dp1 <- distPointToPoint(pitPair$wallMidX[idx], pitPair$wallMidY[idx], pitPair$pit1x[idx], pitPair$pit1y[idx])
    pitPair$wallMidRelativeDistance[idx] <- dp0 / (dp0 + dp1)
    tmpDist0 <- distPointToPoint(pitPair$intersectX[idx], pitPair$intersectY[idx], pitPair$pit0x[idx], pitPair$pit0y[idx])
    tmpDist1 <- distPointToPoint(pitPair$intersectX[idx], pitPair$intersectY[idx], pitPair$pit1x[idx], pitPair$pit1y[idx])
    if (max(tmpDist0, tmpDist1) >= pitPair$pitMidLen[idx]) pitPair$isRandom[idx] <- 2 + pitPair$isRandom[idx]
}
pitPair$deltaangle <- pitPair$wallangle- pitPair$tangentangle
pitPair$absDeltaBuiltToTanDeg <- abs(pitPair$deltaangle)


showControl <- 0

titleQualStr <- c("Experiment", "Control")
titleIDStr <- c("A", "B", "C", "D")


############## Start Fig 10 B ################

par(mfrow = c(1,1))
  
titleStr <- "Orientation of built wall vs pits common tangent"
titleStr <- "b"
xLabelStr <- "Orientation of pits common tangent (deg)"
yLabelStr <- "Orientation of built wall (deg)"
corpits<-cor(pitPair$wallangle[pitPair$isRandom == showControl], pitPair$tangentangle[pitPair$isRandom == showControl], method="spearman") #pearson")
axisMax<- 90
#plot ( X ~ Y)
plot(pitPair$tangentangle[pitPair$isRandom == showControl], pitPair$wallangle[pitPair$isRandom == showControl], xaxt = "none", yaxt = "none", xlim=c(-axisMax,axisMax), ylim = c(-axisMax,axisMax), xlab = "", ylab = "")
title(titleStr, adj = 0.1, cex.main = 2.5)
axis(1, seq(-90,90, 30), cex.axis = 1.5)
title(xlab = xLabelStr, cex.lab = 2)
axis(2, seq(-90,90, 30), cex.axis = 1.5)
title(ylab = yLabelStr, cex.lab = 2, mgp = c(2.5,0,0))
#lm (Y ~ X)
linPits <- lm(pitPair$wallangle[pitPair$isRandom == showControl] ~ pitPair$tangentangle[pitPair$isRandom == showControl])
abline(linPits, lty = 3)

############## end Fig 10 B ################

############## Start Fig 10 A ################

par(mfrow = c(1,1))

#titleStr<-"A - Divergence of built wall from\ncommon tangent"
##
titleStr<-"a"

#dataHA<-abs(exptDeltaBuilt$absDeltaBuiltToTanDeg)
#dataHRA<-abs(rndDeltaBuilt$absDeltaBuiltToTanDeg)
dataHA<-abs(pitPair$deltaangle[pitPair$isRandom == 0])
dataHRA<-abs(pitPair$deltaangle[pitPair$isRandom == 1])

set1 <- dataHA
set2 <- dataHRA
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
#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5), ylab = "", xlab = "", col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]), cex.axis = 1.5)
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 2.0, horiz = F)
title(titleStr, adj = 0.1, cex.main = 2.5)
title(xlab = xaxisStr, cex.lab = 2)
#axis(1, commonBreaks, cex.axis = 1.5)
title(ylab = "Frequency", cex.lab = 2, mgp = c(2.5,0,0))
abline(h=0)
## copy image at 700x700
####################### end of Fig 19 #################
