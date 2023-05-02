######################################################
#   Production BaseProcessing.r used in chapter 4 of the thesis
######################################################
library("TOSTER")

setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

measurementErrors<-read.csv("Chapter4/MeasurementError.csv", sep=",")

bases<-read.csv("Chapter4/bases.csv", sep=",") # was Stigmergy/2021/DepthGauge/guage.csv
expt <- 1
or
expt <- 3

basesThick<-read.csv("Chapter4/basesthickwax.csv", sep=",") # was Stigmergy/2021/DepthGauge/basesthickwax.csv
expt <- 2

cecols<-which( colnames(bases)=="c0" ):which( colnames(bases)=="c6" )
facols<-which( colnames(bases)=="fa0" ):which( colnames(bases)=="fa6" )
flcols<-which( colnames(bases)=="fl0" ):which( colnames(bases)=="fl6" )
rawcols<-which( colnames(bases)=="h0" ):which( colnames(bases)=="h6" )

for(idx in 1:length(bases$class)) {
    ce<-bases[idx,cecols]
    fa<-bases[idx,facols]
    fl<-bases[idx,flcols]
    rawh<-bases[idx,rawcols]
    bases$ceReg[idx]<-calcRegression(as.numeric(ce))
    bases$faReg[idx]<-calcRegression(as.numeric(fa))
    bases$flReg[idx]<-calcRegression(as.numeric(fl))
    bases$maxDelta[idx]<-calcRegressionMaxDelta(as.numeric(fl))
    bases$flTan[idx]<-calcRegressionTan(as.numeric(rawh))
    bases$flSTan90[idx]<-90 + calcRegressionSignedTan(as.numeric(rawh))
    bases$flSTan1[idx]<-calcRegressionSignedTan(as.numeric(rawh[1:4]))
    bases$flSTan2[idx]<-calcRegressionSignedTan(as.numeric(rawh[4:7]))
    bases$faTan1[idx]<-calcRegressionTan(as.numeric(rawh[1:4]))
    bases$faTan2[idx]<-calcRegressionTan(as.numeric(rawh[4:7]))
    bases$faTan90[idx]<- 90 + (bases$faTan1[idx] + bases$faTan2[idx])/2
}
for(idx in 1:length(bases$class)) {
    bases$flSTanSum[idx]<- bases$flSTan1[idx] + sign(bases$flSTan1[idx]) * bases$flSTan2[idx]
    bases$flSTanAve[idx]<- (bases$flSTan1[idx] + bases$flSTan2[idx])/2
}

for(idx in 1:length(measurementErrors$class)) {
    ce<-measurementErrors[idx,cecols]
    fa<-measurementErrors[idx,facols]
    fl<-measurementErrors[idx,flcols]
    measurementErrors$ceReg[idx]<-calcRegression(as.numeric(ce))
    measurementErrors$faReg[idx]<-calcRegression(as.numeric(fa))
    measurementErrors$flReg[idx]<-calcRegression(as.numeric(fl))
}

for(idx in 1:length(basesThick$class)) {
    ce<-basesThick[idx,cecols]
    fa<-basesThick[idx,facols]
    basesThick$ceReg[idx]<-calcRegression(as.numeric(ce))
    basesThick$faReg[idx]<-calcRegression(as.numeric(fa))
}

bases$cAndFaProb<-bases$fae + bases$ce
bases$faeceRel<-1 - (bases$fae  / bases$cAndFaProb)
bases$cefaeRel<-1 - (bases$ce  / bases$cAndFaProb)
bases$cAndFaProbReg<-bases$faReg + bases$ceReg
bases$faeceRelReg<-1 - (bases$faReg  / bases$cAndFaProbReg)
bases$cefaeRelReg<-1 - (bases$ceReg  / bases$cAndFaProbReg)
bases$flAndFaProbReg<-bases$faReg + bases$flReg
bases$flefaeRelReg<-1 - (bases$flReg  / bases$flAndFaProbReg)

measurementErrors$cAndFaProb<-measurementErrors$fae + measurementErrors$ce
measurementErrors$faeceRel<-1 - (measurementErrors$fae  / measurementErrors$cAndFaProb)
measurementErrors$cefaeRel<-1 - (measurementErrors$ce  / measurementErrors$cAndFaProb)
measurementErrors$cAndFaProbReg<-measurementErrors$faReg + measurementErrors$ceReg
measurementErrors$faeceRelReg<-1 - (measurementErrors$faReg  / measurementErrors$cAndFaProbReg)
measurementErrors$cefaeRelReg<-1 - (measurementErrors$ceReg  / measurementErrors$cAndFaProbReg)
measurementErrors$flAndFaProbReg<-measurementErrors$faReg + measurementErrors$flReg
measurementErrors$flefaeRelReg<-1 - (measurementErrors$flReg  / measurementErrors$flAndFaProbReg)

basesThick$cAndFaProb<-basesThick$fae + basesThick$ce
basesThick$faeceRel<-1 - (basesThick$fae  / basesThick$cAndFaProb)
basesThick$cefaeRel<-1 - (basesThick$ce  / basesThick$cAndFaProb)
basesThick$cAndFaProbReg<-basesThick$faReg + basesThick$ceReg
basesThick$faeceRelReg<-1 - (basesThick$faReg  / basesThick$cAndFaProbReg)
basesThick$cefaeRelReg<-1 - (basesThick$ceReg  / basesThick$cAndFaProbReg)



codeCurvedOrg<-1
codeCurved2nd<-2
codeFacetted<-3
codeNatural<-4
codeFlat<-5
codeName<-6
codeBack<-7
classNames<-c("stage 1 covered", "2-stage 2 covered", "3-Stage 2 accessible", "4-Natural", "5-Flat", "undefined")

bases$Stage<-classNames[bases$class]
bases$Stage <- as.factor(bases$Stage)
basesThick$Stage<-classNames[basesThick$class]
basesThick$Stage <- as.factor(basesThick$Stage)

colrs<-c("red", "red","blue", "Yellow", "lightblue")

titleQualStr <- c("Stage 1 covered", "Stage 2 covered", "Stage 2 accessible", "Natural", "Flat", "undefined")
titleIDStr <- c("A", "B", "C", "D")
titleExptStr <- c("standard backplane", "thick backplane", "standard backplane", "thick backplane")

## Prediction 4 first, just orgcurved and natural
######### section 1##############
######### <<< 900 x 500 Section 1.4.1.4 fig 1.8	Peripheral walls in wallPlasticity >>>>> #############

 ###### start ######## histograms of curves ############# - Fig 1-8
############# start cf control #############

CorD <- 0

if (expt == 1) {
    class1<-codeCurvedOrg
    class2<-codeNatural
    baseStage1 <- bases[bases["class"] == class1,"cefaeRelReg"]
    baseControl <- bases[bases["class"] == codeNatural,"cefaeRelReg"]
    baseStage2 <- bases[bases["class"] == class2,"cefaeRelReg"]
}
if (expt == 3) {
    class1 <- codeCurved2nd
    class2 <- codeFacetted
    baseStage1 <- bases[bases["class"] == class1,"cefaeRelReg"]
    baseControl <- bases[bases["class"] == codeNatural,"cefaeRelReg"]
    baseStage2a <- bases[bases["class"] == class2,"cefaeRelReg"]
}


par(mfrow = c(1,2))
brks <- seq(0,1,0.05)
brksX <- seq(0,1,0.1)
yMax<-max(hist (baseStage1, breaks = brks, plot=FALSE)$counts)
sumry<-summary(baseStage1)
XaxisRange<-c(0,1)
num <- length(baseStage1)

titleStrT<-" - Cell base curve"
titleStr <- paste(titleIDStr[1 + CorD], titleStrT, "\n" , titleExptStr[expt], " - ", titleQualStr[class1])
if (expt == 2 | expt == 4) {
    titleStr <- paste(titleIDStr[1 + CorD], titleStrT, "\n" , titleExptStr[expt], " - Stage 1")
}
xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
xLabelStr <- "Base curve factor"
hist (baseStage1, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brksX)

if (expt == 1) {
    stats1 <- baseStage1
    sumry<-summary(baseControl)
    num <- length(baseControl)
    hist2 <- baseControl
    stats2 <- baseControl
    titleStr <- paste(titleIDStr[2 + CorD], titleStrT, "\n ", titleQualStr[codeNatural])
    #xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
}
if (expt == 3) {
    stats1 <- baseStage1
    sumry<-summary(baseStage2a)
    num <- length(baseStage2a)
    hist2 <- baseStage2a
    stats2 <- baseStage2a
    yMax<-max(hist (hist2, breaks = brks, plot=FALSE)$counts)
    titleStr <- paste(titleIDStr[2 + CorD], titleStrT, "\n ", titleExptStr[expt], " - ", titleQualStr[class2])
    #xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
}
hist (hist2, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brksX)

mean(stats1)
sd(stats1)
length(stats1)
mean(stats2)
sd(stats2)
length(stats2)

wilcox.test(stats1, stats2)
#################
################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
par(mfrow = c(1,1))

titleStrT<-" - Cell base curve"
titleStr <- paste(titleIDStr[1 + CorD], titleStrT, "\n" , titleExptStr[expt], " - ", titleQualStr[class1])
if (expt == 2 | expt == 4) {
    titleStr <- paste(titleIDStr[1 + CorD], titleStrT, "\n" , titleExptStr[expt], " - Stage 1")
}
xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
xLabelStr <- "Base curve factor"
##

set1 <- baseStage1
set2 <- baseControl
useColrs <- c("red", "blue")
titleStr<-"A"

set2 <- baseStage2a
useColrs <- c( "yellow", "red")
titleStr<-"B"

setS1m <- mean(set1)
setS1sd <- sd(set1)
set1m <- mean(set1)
set1sd <- sd(set1)
set2m <- mean(set2)
set2sd <- sd(set2)
divsQty <- 20
#barMin <- min(min(set1),min(set2)) #0
#barMax <- max(max(set1),max(set2))
#barDiv <- floor((barMax - barMin)/(divsQty + 1) + 1)
#barMin <- floor(barMin / barDiv)* barDiv
#barMax <- (floor(barMax / barDiv) + 1)* barDiv
barMax <- 1.0
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
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5), ylab = "Frequency", xlab = xLabelStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
if (expt == 3) {
    legend("topright", legend = c("Covered", "Accessible"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
} else {
    legend("topright", legend = c("Covered", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
}
title(titleStr, adj = 0.1, cex.main = 2.5)

## copy image at 700x500


############# thick backplane #############
## expt 4
    par(mfrow = c(1,3))
    class1 <- codeCurvedOrg
    class2 <- codeFacetted
    baseStage1 <- basesThick[basesThick["class"] == class1,"cefaeRelReg"]
    baseControl <- bases[bases["class"] == codeNatural,"cefaeRelReg"]
    baseStage2 <- basesThick[basesThick["class"] == class2,"cefaeRelReg"]

    brks <- seq(0,1,0.05)
    brksX <- seq(0,1,0.1)
    yMax<-max(hist (baseStage1, breaks = brks, plot=FALSE)$counts)
    yMax<-max(yMax, hist (baseControl, breaks = brks, plot=FALSE)$counts)
    yMax<-max(yMax, hist (baseStage2, breaks = brks, plot=FALSE)$counts)

    sumry<-summary(baseStage1)
    XaxisRange<-c(0,1)
    num <- length(baseStage1)

    titleStrT<-" - Cell base curve"
    titleStr <- paste(titleIDStr[1], titleStrT, "\n" , "Thick backplane stage 1")
    xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
    xLabelStr <- "Base curve factor"
    hist (baseStage1, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
    abline(v=sumry[3])
    abline(v=sumry[2], lty=2)
    abline(v=sumry[5], lty=2)

    axis(side=1, at=brksX)
    stats1 <- baseStage1
    sumry<-summary(baseControl)
    num <- length(baseControl)
    hist2 <- baseControl
    stats2 <- baseControl
    titleStr <- paste(titleIDStr[2], titleStrT, "\n ", "Control")
    #xLabelStr<-sprintf("Curve factor, n=%d, median %0.2f, inter-quartile %0.2f - %0.2f", num, sumry[3], sumry[2], sumry[5])
    hist (hist2, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
    abline(v=sumry[3])
    abline(v=sumry[2], lty=2)
    abline(v=sumry[5], lty=2)
    axis(side=1, at=brksX)

    axis(side=1, at=brksX)
    stats1 <- baseStage1
    sumry<-summary(baseStage2)
    num <- length(baseStage2)
    hist2 <- baseStage2
    stats2 <- baseStage2
    titleStr <- paste(titleIDStr[3], titleStrT, "\n ", "Thick backplane stage 2")
    hist (hist2, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
    abline(v=sumry[3])
    abline(v=sumry[2], lty=2)
    abline(v=sumry[5], lty=2)
    axis(side=1, at=brksX)


mean(baseStage1)
sd(baseStage1)
length(baseStage1)
mean(baseControl)
sd(baseControl)
length(baseControl)
mean(baseStage2)
sd(baseStage2)
length(baseStage2)

wilcox.test(baseStage1, baseControl)
wilcox.test(baseStage1, baseStage2)

# ===================== compare all classes ==============
######### <<< Section 1.4.2.5 fig 1-23	#########
######### bases in wallPlasticity >>>>> #############
par(mfrow = c(1,1))

titleStr<-"B - Cell base curved vs facetted\nCovered, natural, accessible, thick backplane stage 1 & 2"
class1<-codeCurvedOrg
class2<-codeCurved2nd
class3<-codeFacetted
class4<-codeNatural

## refresh classNames
classNames<-c("Stage 1", "Covered", "Accessible", "Natural", "undefined")
ledgendStrThick<-c("Thick stage 1", "Thick stage 2")

#useColrs<-c(colrs[class1], colrs[class2], colrs[class3], colrs[class1], colrs[class3], colrs[class4])
useColrs<-c(colrs[class1], colrs[class4], colrs[class3], colrs[class1], colrs[class3])
#useLegend<-c(classNames[class1], classNames[class2], classNames[class3], ledgendStrThick[1], ledgendStrThick[2], classNames[class4] )
useLegend<-c("4iii covered", "Natural", "4iii uncovered", "4iv stage 1", "4iv stage 2")
qty1<-length(bases[(bases$class == class1), ]$cefaeRelReg)
#qty2<-length(bases[(bases$class == class2), ]$cefaeRelReg)
qty3<-length(bases[(bases$class == class3), ]$cefaeRelReg)
qty4<-length(basesThick[(basesThick$class == class1), ]$cefaeRelReg)
qty5<-length(basesThick[(basesThick$class == class3), ]$cefaeRelReg)
qty6<-length(bases[(bases$class == class4), ]$cefaeRelReg)
yaxisStr <- "Curve factor"
#xaxisStr <- sprintf("%s (%d), %s (%d), %s (%d), %s (%d), %s (%d), %s (%d)",useLegend[1], qty1,useLegend[2], qty2,useLegend[3], qty3,useLegend[4], qty4,useLegend[5], qty5,useLegend[6], qty6)
xaxisStr <- sprintf("%s (%d), %s (%d), %s (%d), %s (%d), %s (%d)",useLegend[1], qty1,useLegend[2], qty2,useLegend[3], qty3,useLegend[4], qty4,useLegend[5], qty5)
boxplot(bases[bases["class"] == class1,"cefaeRelReg"], 
    bases[bases["class"] == class4,"cefaeRelReg"], 
    #bases[bases["class"] == class2,"cefaeRelReg"], 
    bases[bases["class"] == class3,"cefaeRelReg"], 
    basesThick[basesThick["class"] == class1,"cefaeRelReg"],
    basesThick[basesThick["class"] == class3,"cefaeRelReg"],
    col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr) #, xlab = xaxisStr)
#legend("topright", inset = c(-0.1,0), legend = c("Single side", "Accessible two sides", "Natural"), col = c(colrs[class1], colrs[class3], colrs[class4]), pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline( h = 0.5, lty = 2)

###################### not so coloured experimental chart
########## not used ###################
boxplot(bases[bases["class"] == class1,"cefaeRelReg"], 
    bases[bases["class"] == class2,"cefaeRelReg"], 
    bases[bases["class"] == class3,"cefaeRelReg"], 
    basesThick[basesThick["class"] == class1,"cefaeRelReg"],
    basesThick[basesThick["class"] == class3,"cefaeRelReg"],
    bases[bases["class"] == class4,"cefaeRelReg"], 
    col="lightblue", names = useLegend, main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
boxdata <- bases[bases["class"] == class1,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(0.6, sumry[2], 1.4, sumry[5], density=12, angle=45)
boxdata <- bases[bases["class"] == class2,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(1.6, sumry[2], 2.4, sumry[5], density=12, angle=45)
boxdata <- bases[bases["class"] == class3,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(2.6, sumry[2], 3.4, sumry[5], density=12, angle=-45)
boxdata <- basesThick[basesThick["class"] == class1,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(3.6, sumry[2], 4.4, sumry[5], density=12, angle=45)
boxdata <- basesThick[basesThick["class"] == class3,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(4.6, sumry[2], 5.4, sumry[5], density=12, angle=45)
boxdata <- bases[bases["class"] == class4,"cefaeRelReg"]
sumry <- summary(boxdata)
rect(5.6, sumry[2], 6.4, sumry[5], density=12, angle=45)
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
######### exposed cf natural
equ1 <- bases[(bases$class == codeFacetted), ]$cefaeRelReg
equ2 <- bases[(bases$class == codeNatural), ]$cefaeRelReg
#### or ########
######### cover cf remained covered

equ1 <- basesThick[(basesThick$class == codeFacetted), ]$cefaeRelReg
equ2 <- bases[(bases$class == codeNatural), ]$cefaeRelReg

equ1 <- basesThick[(basesThick$class == codeFacetted), ]$cefaeRelReg
equ2 <- bases[(bases$class == codeFacetted), ]$cefaeRelReg

std1<-sd(equ1)
std2<-sd(equ2)
mean1<-mean(equ1)
mean2<-mean(equ2)
qty1<-length(equ1)
qty2<-length(equ2)

cohenD<-0.5
aveSD<-(std1 + std2)/2
SESOI<- measurementSd # cohenD * aveSD
meanmean<-0 # (mea222n1 - mean2)/2
showEquivalence()

TOSTtwo.raw(m1=mean1,m2=mean2,sd1=std1,sd2=std2,n1=qty1,n2=qty2,low_eqbound=meanmean-SESOI, high_eqbound=meanmean + SESOI, alpha = 0.05, var.equal=TRUE)

