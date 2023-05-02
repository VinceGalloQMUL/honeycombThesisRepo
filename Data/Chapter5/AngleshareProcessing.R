######################################################
#   Production AngleShare.r used in chapter 5 of the thesis
######################################################
library("TOSTER")
library(rstatix)
install.packages("broom"), "car"

setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

angleshare <- read.csv("Chapter5/angleshare.csv", sep=",")

angleshare$class <- -1
angleshare$aveDelta <- 0

for(idx in 1:length(angleshare$ID)) {
    angleshare$Vangle[idx] <- angleNonReflex(angleABS( angleDeg(angleshare$wall0Angle[idx] - angleshare$wall1Angle[idx])))
    angleshare$Vangle1[idx] <- angleNonReflex(angleABS(angleDeg(angleshare$wall0Angle[idx] - angleshare$builtWallAngle[idx])))
    angleshare$Vangle2[idx] <- angleNonReflex(angleABS(angleDeg(angleshare$wall1Angle[idx] - angleshare$builtWallAngle[idx])))
    if (angleshare$wall0Angle[idx] == angleshare$wall1Angle[idx]) {
        angleshare$Vangle[idx] <- 180
        # mistake in the image processing code where there is no wall centre marked , ie, frame edge values
        angleshare$wallBisectedAngle[idx] <- angleshare$wallBisectedAngle[idx] + pi / 2
        angleshare[idx, "class"] <- 2
        angleshare$aveDelta[idx] <- abs(angleshare$Vangle1[idx] - 90)
    } else {
        if (angleshare$Vangle[idx] > 150) {
            angleshare[idx, "class"] <- 0
            angleshare$aveDelta[idx] <- (abs(angleshare$Vangle1[idx] - 90) + abs(angleshare$Vangle2[idx] - 90)) / 2
            #angleshare$aveDelta[idx] <- (abs(angleshare$Vangle1[idx] - angleshare$Vangle2[idx])) / 2
        } else {
            angleshare[idx, "class"] <- 1
            angleshare$aveDelta[idx] <- (abs(angleshare$Vangle[idx] - 120) + abs(angleshare$Vangle1[idx] - 120) + abs(angleshare$Vangle2[idx] - 120)) / 3
        }
    }
    angleshare$deltaBuiltToBisectDeg[idx]<-angleLT90(angleDeg(angleshare$builtWallAngle[idx] - angleshare$wallBisectedAngle[idx]))
    angleshare$wallBisectedAngleDeg[idx]<-angleLT90(angleDeg(angleshare$wallBisectedAngle[idx]))
    angleshare$builtWallAngleDeg[idx]<-angleLT90(angleDeg(angleshare$builtWallAngle[idx]))
    angleshare$builtWallAngleDeg[idx]<-commonDirection(angleshare$builtWallAngleDeg[idx], angleshare$wallBisectedAngleDeg[idx])
}




share1 <- angleshare[angleshare$class == 0,]$Vangle1
share2 <- angleshare[angleshare$class == 1,]$Vangle1
share3 <- angleshare[angleshare$class == 2,]$Vangle1

forPaper = TRUE
#forPaper = FALSE

set1m <- mean(share1)
set1sd <- sd(share1)
set2m <- mean(share2)
set2sd <- sd(share2)
set3m <- mean(share3)
set3sd <- sd(share3)
divsQty <- 20
barMin <- min(min(share1),min(share2)) #0
barMin <- min(barMin,min(share3)) #0
barMax <- max(max(share1),max(share2))
barMax <- max(barMax,max(share3))
barDiv <- floor((barMax - barMin)/(divsQty + 1) + 1)
barMin <- floor(barMin / barDiv)* barDiv
barMax <- (floor(barMax / barDiv) + 1)* barDiv
barMax <- 138
barMin <- 68
barDiv <- 4
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(share1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(share2, breaks = commonBreaks, plot = FALSE)
hStage3 <- hist(share3, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
normalisedStage3 <- hStage3$counts / sum(hStage3$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
ymax <- max(ymax, hStage3$counts) # normalisedStage3)
spread <- 2
if (forPaper) {
    titleStr<-"a"
    colrs<-c("white","gray", "black")
} else {
    titleStr <- "A - Distribution of cell corner angle"
    colrs<-c("lightblue","blue", "red")
}
#xaxisStr <- sprintf("Degrees (mean %0.2f, %0.2f & %0.2f)", set1m, set2m, set3m)
xaxisStr <- "Degrees"
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),main = titleStr, ylab = "Frequency", xlab = xaxisStr, col = colrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage3$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = colrs[2],add = TRUE)
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(3,rep(spread + 1, nBreaks - 1)), col = colrs[3],add = TRUE)
#legend("topright", legend = c(sprintf("Paired cells (%d)", length(share1)), sprintf("Frame cells (%d)", length(share3)), sprintf("Triple cells (%d)", length(share2))), col=colrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Paired cells","Frame cells","Triple cells"), fill=colrs, border = "black", bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline(h = 0)

set1m
set2m
set3m
set1sd
set2sd
set3sd
TOSTone(m=mean(share1),mu=90,sd=sd(share1),n=length(share1),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)
TOSTone(m=mean(share2),mu=120,sd=sd(share2),n=length(share2),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)
TOSTone(m=mean(share3),mu=90,sd=sd(share3),n=length(share3),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)

TOSTone(m=mean(share1),mu=90,sd=sd(share1),n=length(share1),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)
TOSTtwo(m1=mean(share1), m2=mean(share3), sd1=sd(share1), sd2 = sd(share3), n1 = length(share1), n2 = length(share3),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)

share1 <- angleshare[angleshare$class == 0,]$aveDelta
share2 <- angleshare[angleshare$class == 1,]$aveDelta
share3 <- angleshare[angleshare$class == 2,]$aveDelta
set1m <- mean(share1)
set1sd <- sd(share1)
set2m <- mean(share2)
set2sd <- sd(share2)
set3m <- mean(share3)
set3sd <- sd(share3)
barMin <- min(min(share1),min(share2)) #0
barMin <- min(barMin,min(share3)) #0
barMax <- max(max(share1),max(share2))
barMax <- max(barMax,max(share3))
barDiv <- floor((barMax - barMin)/(divsQty + 1) + 1)
barMin <- floor(barMin / barDiv)* barDiv
barMax <- (floor(barMax / barDiv) + 1)* barDiv
barMax <- 24
barMin <- 0
barDiv <- 3
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(share1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(share2, breaks = commonBreaks, plot = FALSE)
hStage3 <- hist(share3, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
normalisedStage3 <- hStage3$counts / sum(hStage3$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
ymax <- max(ymax, hStage3$counts) # normalisedStage3)
spread <- 2
if (forPaper) {
    titleStr<-"b"
    colrs<-c("white","gray", "black")
} else {
    titleStr <- "B - Deviation from prediction of cell corner angle"
    colrs<-c("lightblue","blue", "red")
}
#xaxisStr <- sprintf("Degrees (mean %0.2f, %0.2f & %0.2f)", set1m, set2m, set3m)
xaxisStr <- "Degrees"
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),main = titleStr, ylab = "Frequency", xlab = xaxisStr, col = colrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(3,rep(spread + 1, nBreaks - 1)), col = colrs[3],add = TRUE)
barplot (hStage3$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = colrs[2],add = TRUE)
legend("topright", legend = c("Paired cells cf. 90deg","Frame cells cf. 90deg", "Triple cells cf. 120deg"),  fill=colrs, border = "black", bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline(h = 0)

