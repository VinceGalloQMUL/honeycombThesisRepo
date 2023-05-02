######################################################
#   Production curvedWalls.R used in chapter 4 of the thesis
######################################################
library("TOSTER")
setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

#====================== curved wall processing ==========================
########### wall plasticity chapter ##################
############ fig 1.4, 1.5 & ... #######################
curves<-read.csv("Chapter4/curvedWalls.csv", sep=",")
expt <- 1
## or
### not used
curves<-read.csv("Chapter4/curvedWallsTongues.csv", sep=",")
expt <- 2
## or
curvesThick<-read.csv("Chapter4/curvedWallsThickWall.csv", sep=",")
expt <- 3

titleQualStr <- c("Isolated", "Shared", "Control")
titleIDStr <- c("A", "B", "C", "D")
titleExptStr <- c("progressive growth", "tongue merger", "thick walls")

codeCurveOrg<-0
codeCurve2nd<-1
codeCurveCtrl<-2

curveStart<-curves[curves["seq"] == codeCurveOrg,"curve"]
curveEnd<-curves[curves["seq"] == codeCurve2nd,"curve"]
curveCtrl<-curves[curves["seq"] == codeCurveCtrl,"curve"]
curveThickStart<-curvesThick[curvesThick["seq"] == codeCurveOrg,"curve"]
curveThickEnd<-curvesThick[curvesThick["seq"] == codeCurve2nd,"curve"]
curveThickCtrl<-curvesThick[curvesThick["seq"] == codeCurveCtrl,"curve"]

length(unique(curves$ID))
  ###### start ######## histograms of curves ############# - Fig 1-8

histStart <- curveStart
histCtrl <- curveCtrl
histEnd <- curveEnd
expt <- 1
### or 
histStart <- curveThickStart
histCtrl <- curveThickCtrl
histEnd <- curveThickEnd
expt <- 3


############# start cf control #############
xAxisSprintf <- "Radians, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f"
par(mfrow = c(1,3))
brks <- seq(0,0.5,0.025)
brksX <- seq(0,0.5,0.05)
yMax<-max(hist (histStart, breaks = brks, plot=FALSE)$counts)
yMax<-max(yMax, hist (histCtrl, breaks = brks, plot=FALSE)$counts)
yMax<-max(yMax, hist (histEnd, breaks = brks, plot=FALSE)$counts)
sumry<-summary(histStart)
XaxisRange<-c(0,0.5)
num <- length(histStart)

titleStrT<-" - Wall curvature"
titleStr <- paste(titleIDStr[1], titleStrT, "\n" , titleExptStr[expt], " - ", titleQualStr[1])
xLabelStr<-sprintf(xAxisSprintf,num, sumry[3], sumry[2], sumry[5])
xLabelStr <- "Cell wall curvature (Radians)"
hist (histStart, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
abline(v=0.262, lty=4, col = "red")
axis(side=1, at=brksX)


sumry<-summary(histCtrl)
num <- length(histCtrl)

titleStr <- paste(titleIDStr[2], titleStrT, "\n " ,  titleQualStr[3])
#xLabelStr<-sprintf(xAxisSprintf,num, sumry[3], sumry[2], sumry[5])
hist (histCtrl, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brksX)

sumry<-summary(histEnd)
num <- length(histEnd)

titleStr <- paste(titleIDStr[3], titleStrT, "\n " , titleExptStr[expt], " - ",  titleQualStr[2])
#xLabelStr<-sprintf(xAxisSprintf,num, sumry[3], sumry[2], sumry[5])
hist (histEnd, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brksX)

mean(histStart)
sd(histStart)
length(histStart)
mean(histCtrl)
sd(histCtrl)
length(histCtrl)
mean(histEnd)
sd(histEnd)
length(histEnd)

wilcox.test(histStart, histCtrl)
wilcox.test(histStart, histEnd, paired = TRUE)


############ combined plot showing box and lines connecting before and after ################
## fig 1.12 , 1.15 & 1.18 ###################
######### Peripheral walls in wallPlasticity >>>>> #############
par(mfrow = c(1,1))
titleStrSubscript <- "with a cell to one side or both"
titleStr <- paste("Cell Wall curvature changes for each wall\n", titleStrSubscript, " - ", titleExptStr[expt])
ledgendStr <- c("Isolated", "Shared")
colrs <- c("lightskyblue1", "lightskyblue3")
yaxisStr <- "Wall curvature - Radians"

#boxplot(curveStart, curveEnd, col=colrs, names = ledgendStr[c(1,2)], main = titleStr,ylab = yaxisStr, add=TRUE)
boxplot(curveStart, curveEnd, col=colrs, names = ledgendStr[c(1,2)], main = titleStr,ylab = yaxisStr)
legend("topright", legend = ledgendStr[c(1,2)], col = colrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
segments(1, curveStart,2, curveEnd, lty="dashed")


# ===================== compare all classes ==============
######### <<< Section 1.4.2.5 fig 1-23	#########
######### walls in wallPlasticity >>>>> #############

curveStart<-curves[curves["seq"] == codeCurveOrg,"curve"]
curveEnd<-curves[curves["seq"] == codeCurve2nd,"curve"]
curveCtrl<-curves[curves["seq"] == codeCurveCtrl,"curve"]
curveThickStart<-curvesThick[curvesThick["seq"] == codeCurveOrg,"curve"]
curveThickEnd<-curvesThick[curvesThick["seq"] == codeCurve2nd,"curve"]
curveThickCtrl<-curvesThick[curvesThick["seq"] == codeCurveCtrl,"curve"]

par(mfrow = c(1,1))
yaxisStr <- "Wall curvature (Radians)"

titleStr<-"A - Cell wall curvature isolated, shared, thick edge, eroded and Control"


useColrs<-c("red", "yellow","blue", "red", "blue")
useLegend<-c("4i stage 1", "Control", "4i stage 2", "4ii stage 1", "4ii stage 2" )
qty1<-length(curveStart)
qty2<-length(curveEnd)
qty3<-length(curveThickStart)
qty4<-length(curveThickEnd)
qty5<-length(curveCtrl)
xaxisStr <- sprintf("%s (%d), %s (%d), %s (%d), %s (%d), %s (%d)",useLegend[1], qty1,useLegend[2], qty2,useLegend[3], qty3,useLegend[4], qty4,useLegend[5], qty5)
boxplot(curveStart, curveCtrl, curveEnd, curveThickStart, curveThickEnd,
    col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr) #, xlab = xaxisStr)
#legend("topright", legend = c("Single side", "Shared", "Natural"), col = c(colrs[class1], colrs[class3], colrs[class4]), pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)


################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
############ not used, more confusing than clarifying
par(mfrow = c(1,1))

curveStart<-curves[curves["seq"] == codeCurveOrg,"curve"]
curveEnd<-curves[curves["seq"] == codeCurve2nd,"curve"]
curveCtrl<-curves[curves["seq"] == codeCurveCtrl,"curve"]

set1 <- curveStart
set2 <- curveEnd
set3 <- curveCtrl
setS1m <- mean(set1)
setS1sd <- sd(set1)
set1m <- mean(set1)
set1sd <- sd(set1)
set2m <- mean(set2)
set2sd <- sd(set2)
set3sd <- sd(set3)
set3m <- mean(set3)
set3sd <- sd(set3)
divsQty <- 20
barMin <- min(min(set1),min(set2)) #0
barMax <- max(max(set1),max(set2))
barMin <- min(min(set3),barMin) #0
barMax <- max(max(set3),barMax)
barDiv <- (barMax - barMin)/(divsQty + 1)
barMin <- floor(barMin / barDiv)* barDiv
barMax <- (floor(barMax / barDiv) + 1)* barDiv
#barMax <- 90
#barMin <- 0
#barDiv <- 5
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(set1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(set2, breaks = commonBreaks, plot = FALSE)
hStage3 <- hist(set3, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
normalisedStage3 <- hStage3$counts / sum(hStage3$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
ymax <- max(ymax, hStage3$counts) # normalisedStage3)
spread <- 2
xaxisStr <- "Degrees"
#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),main = titleStr, ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.3, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
barplot (hStage3$counts, width = c(0.67, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[3],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)

## copy image at 700x500


###################################
# Equivalence testing
# equivalence of expt 5iii uncovered cf natural
showEquivalence <- function () {
    print(c("means, Qty", mean1, mean2, qty1, qty2))
    print(c("std", std1,std2))
    print(c("AveSD/SESOI", aveSD, SESOI))
    print(c("delta m, deltam/SD", mean2 - mean1, (mean2 - mean1) / aveSD))
    print(meanmean)
    print(meanmean-SESOI)
    print(meanmean+SESOI)
}
######### shared cf natural
equ1 <- curves[curves["seq"] == codeCurve2nd,"curve"]
equ2 <- curves[curves["seq"] == codeCurveCtrl,"curve"]
#### or ########
######### thick thinned cf natural

equ1 <- curvesThick[curvesThick["seq"] == codeCurve2nd,"curve"]
equ2 <- curves[curves["seq"] == codeCurveCtrl,"curve"]

######### shared cf thick thinned
equ1 <- curves[curves["seq"] == codeCurve2nd,"curve"]
equ2 <- curvesThick[curvesThick["seq"] == codeCurve2nd,"curve"]

std1<-sd(equ1)
std2<-sd(equ2)
mean1<-mean(equ1)
mean2<-mean(equ2)
qty1<-length(equ1)
qty2<-length(equ2)

cohenD<-0.5
aveSD<-(std1 + std2)/2
SESOI<- cohenD * aveSD # measurementSd # cohenD * aveSD
meanmean<-0 # (mea222n1 - mean2)/2
showEquivalence()

TOSTtwo.raw(m1=mean1,m2=mean2,sd1=std1,sd2=std2,n1=qty1,n2=qty2,low_eqbound=meanmean-SESOI, high_eqbound=meanmean + SESOI, alpha = 0.05, var.equal=TRUE)

############ isolated cf hypothetical arc 0.262rad
export as 700x300
equ1<-curves[curves["seq"] == codeCurveOrg,"curve"]
or
equ1<-curvesThick[curvesThick["seq"] == codeCurveOrg,"curve"]

std1<-sd(equ1)
mean1<-mean(equ1)
qty1<-length(equ1)
mean2 <- 0.262
cohenD<-0.5
SESOI<- cohenD * std1
TOSTone(m=0,mu=mean2 - mean1,sd=std1,n=qty1,low_eqbound_d=-1, high_eqbound_d =1, alpha = 0.05)

