######################################################
#   Production BaseProcessing.r used in chapter 4 of the thesis
######################################################
library("TOSTER")

setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

measurementErrors<-read.csv("Chapter4/MeasurementError.csv", sep=",")

bases<-read.csv("Chapter4/bases.csv", sep=",") # was Stigmergy/2021/DepthGauge/guage.csv

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

forPaper = TRUE
#forPaper = FALSE


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


if (forPaper) {
    titleQualStr <- c("Stage 1 covered", "Stage 2 covered", "Stage 2 accessible", "Natural", "Flat", "undefined")
    titleIDStr <- c("a", "b", "c", "d")
    titleExptStr <- c("standard backplane", "thick backplane", "standard backplane", "thick backplane")
    colrs<-c("white","gray", "black","gray","white")
} else {
    titleQualStr <- c("Stage 1 covered", "Stage 2 covered", "Stage 2 accessible", "Natural", "Flat", "undefined")
    titleIDStr <- c("A", "B", "C", "D")
    titleExptStr <- c("standard backplane", "thick backplane", "standard backplane", "thick backplane")
    colrs<-c("red", "red","blue", "Yellow", "lightblue")
}


########## not used ####################
## Flat / Opposed cells ############################ cell optimisation... figure 1.8 flat boxplot
classNames<-c("1-stage 1 curve", "2-stage 2 covered", "3-Stage 2 accessible", "Natural", "Opposed", "undefined")

class1<-codeFlat
class2<-codeNatural
class3<-codeFacetted
measure<-"flefaeRelReg"
useColrs<-colrs[c(class1, class2, class3)]
qty1<-length(bases[(bases$class == class1), measure])
qty2<-length(bases[(bases$class == class2),measure ])
#qty3<-length(bases[(bases$class == class3), measure])
#useLegend<-classNames[c(class1, class2, class3)]
useLegend<-classNames[c(class1, class2)]
#xaxisStr <- sprintf("%s (%d), %s (%d), %s (%d)",useLegend[1], qty1,useLegend[2], qty2,useLegend[3], qty3)
xaxisStr <- sprintf("%s (%d), %s (%d)",useLegend[1], qty1,useLegend[2], qty2)
yaxisStr <- "Base flatness vs facetted"

#titleStr<-"Cell base flatness vs facetted; Opposed, natural & double sided access"
titleStr<-"Cell base flatness vs facetted; Opposed & natural"

#boxplot(bases[bases["class"] == class1,measure], bases[bases["class"] == class2,measure],bases[bases["class"] == class3,measure], col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
boxplot(bases[bases["class"] == class1,measure], bases[bases["class"] == class2,measure], col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
legend("topright", legend = useLegend, col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline( h = 0.5, lty = 2)
########## end of not used ####################
par(mfrow = c(1,2))
classNames<-c("1-stage 1 curve", "2-stage 2 covered", "3-Stage 2 accessible", "Natural", "Opposed", "undefined")

flatSum <- rep(0,length(flcols))
natSum <- flatSum
for(idx in 1:length(flcols)) {
    flatSum[idx] <- -sum(bases[bases["class"] == codeFlat,flcols[idx]]) / dim(bases[bases["class"] == codeFlat,])[1]
    natSum[idx] <- -sum(bases[bases["class"] == codeNatural,flcols[idx]]) / dim(bases[bases["class"] == codeNatural,])[1]
}
barMin <- min(-bases[bases["class"] == codeFlat,flcols])
barMin <- min(barMin,min(-bases[bases["class"] == codeNatural,flcols]))
barMax <- max(-bases[bases["class"] == codeFlat,flcols])
barMax <- max(barMax,max(-bases[bases["class"] == codeNatural,flcols]))
probeNames <- c( "P0", "P1", "P2", "P3", "P4", "P5", "P6")
class1<-codeFlat
qty1<-dim(bases[(bases$class == class1), ])[1]
xaxisStr <- ""
yaxisStr <- "Base depth (mm)"
if (forPaper) {
     titleStr <- titleIDStr[1]
} else {
    titleStr<-"Cell base profile, opposed"
    #xaxisStr <- sprintf("%s cells, %d samples",classNames[class1], qty1)
}
boxplot(bases[bases["class"] == class1,flcols[1]], -bases[bases["class"] == class1,flcols[2]], 
    -bases[bases["class"] == class1,flcols[3]], -bases[bases["class"] == class1,flcols[4]], 
    -bases[bases["class"] == class1,flcols[5]], -bases[bases["class"] == class1,flcols[6]], 
    -bases[bases["class"] == class1,flcols[7]],
    col = colrs[class1], main = titleStr,ylab = yaxisStr, xlab = xaxisStr, names = probeNames, ylim = c(barMin, barMax))
abline(h = 0, lty = 2)

class1<-codeNatural
qty1<-dim(bases[(bases$class == class1), ])[1]
if (forPaper) {
     titleStr <- titleIDStr[2]
} else {
    titleStr<-"Cell base profile, natural"
    #xaxisStr <- sprintf("%s cells, %d samples",classNames[class1], qty1)
    xaxisStr <- ""
    yaxisStr <- "Base depth"
}
boxplot(bases[bases["class"] == class1,flcols[1]], -bases[bases["class"] == class1,flcols[2]], 
    -bases[bases["class"] == class1,flcols[3]], -bases[bases["class"] == class1,flcols[4]], 
    -bases[bases["class"] == class1,flcols[5]], -bases[bases["class"] == class1,flcols[6]], 
    -bases[bases["class"] == class1,flcols[7]],
    col = colrs[class1], main = titleStr,ylab = yaxisStr, xlab = xaxisStr, names = probeNames, ylim = c(barMin, barMax))
abline(h = 0, lty = 2)

############## chapter 5 fig 1.9 ##########################
par(mfrow = c(1,1))
measure <- "maxDelta"
class1<-codeFlat
class2<-codeNatural
useColrs<-colrs[c(class1, class2)]
useLegend<-classNames[c(class1, class2)]
yaxisStr <- "Max. devation from regression line (mm)"
xaxisStr <- ""
if (forPaper) {
     titleStr <- ""
} else {
    titleStr<-"Cell base deviation from flat; Opposed & natural"
}
#xaxisStr <- sprintf("%s (mean %0.2f), %s (mean %0.2f)",useLegend[1],mean(bases[bases["class"] == class1,measure]), useLegend[2], mean(bases[bases["class"] == class2,measure]) )
#legendStr <- c(sprintf("Opposing cells (%d)", length(bases[bases["class"] == class1,measure])), sprintf("Natural (%d)", length(bases[bases["class"] == class2,measure])))
legendStr <- c("Opposing cells","Natural")
#boxplot(bases[bases["class"] == class1,measure], bases[bases["class"] == class2,measure],bases[bases["class"] == class3,measure], col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
boxplot(bases[bases["class"] == class1,measure], bases[bases["class"] == class2,measure], col=useColrs, names = useLegend, main = titleStr,ylab = yaxisStr, xlab = xaxisStr)
legend("topleft", legend = legendStr, fill=colrs, border = "black", , bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
mean(bases[bases["class"] == class1,measure])
mean(bases[bases["class"] == class2,measure])
sd(bases[bases["class"] == class1,measure])
sd(bases[bases["class"] == class2,measure])
t.test(bases[bases["class"] == class1,measure], bases[bases["class"] == class2,measure])

# signed flat base angles
setS1 <- bases[bases["class"] == class1,"flSTan"]

set1 <- c(bases[bases["class"] == class1,"flSTan1"] , bases[bases["class"] == class1,"flSTan2"]) + 90
#set1 <- bases[bases["class"] == class1,"flSTan90"]
#set1 <- abs(bases[bases["class"] == class1,"flSTanAve"])
#set1 <- bases[bases["class"] == class1,"flSTanSum"]
#set1 <- bases[bases["class"] == class1,"faTan90"]
#set1 <- bases[bases["class"] == class1,"flSTan90"]
#showset <- bases[bases["class"] == class2,"faTan90"]
showset <- abs(c(bases[bases["class"] == class2,"flSTan1"] , bases[bases["class"] == class2,"flSTan2"])) + 90
set2 <- showset
setS1m <- mean(set1)
setS1sd <- sd(set1)
set1m <- mean(set1)
set1sd <- sd(set1)
set2m <- mean(set2)
set2sd <- sd(set2)
divsQty <- 15
barMin <- min(min(set1),min(set2)) #0
barMax <- max(max(set1),max(set2))
barDiv <- floor((barMax - barMin)/(divsQty + 1) + 1)
barMin <- floor(barMin / barDiv)* barDiv
barMax <- (floor(barMax / barDiv) + 1)* barDiv
#barMax <- 132
#barMin <- 70
#barDiv <- 4
commonBreaks <- seq(barMin, barMax, barDiv)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(set1, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(set2, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
ymax <- max(hStage1$counts) #max(normalisedStage1)
ymax <- max(ymax, hStage2$counts) # normalisedStage2)
spread <- 1
if (forPaper) {
     titleStr <- ""
} else {
    titleStr <- "Distribution of cell base angle to wall"
}
#xaxisStr <- sprintf("Degrees (mean %0.2f & %0.2f)", set1m, set2m)
xaxisStr <- "Degrees"
#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),main = titleStr, ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Opposing cells", "Natural"), fill=colrs, border = "black" , bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
abline(h = 0)
length(set1)
length(set2)
set1m
set2m
set1sd
set2sd
setS1sd
setS2sd
t.test(bases[(bases$class == class1), measure], bases[bases$class == class2,measure])
TOSTone(m=mean(set1),mu=90,sd=sd(set1),n=length(set1),low_eqbound_d=-0.5, high_eqbound_d=0.5, alpha=0.05)

set1 <- c(bases[bases["class"] == class1,"flSTan1"] , bases[bases["class"] == class1,"flSTan2"]) + 90
showset <- abs(c(bases[bases["class"] == class2,"flSTan1"] , bases[bases["class"] == class2,"flSTan2"])) + 90
> set1m
[1] 90.31168
> set2m
[1] 116.2232
> set1sd
[1] 6.740627
> set2sd
[1] 7.537372
> setS1sd
[1] 6.740627
> setS2sd
Error: object 'setS2sd' not found
> t.test(bases[(bases$class == class1), measure], bases[bases$class == class2,measure])

	Welch Two Sample t-test

data:  bases[(bases$class == class1), measure] and bases[bases$class == class2, measure]
t = -22.591, df = 65.563, p-value < 2.2e-16
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
 -0.4878104 -0.4085785
sample estimates:
mean of x mean of y 
0.1281007 0.5762952 
TOST results:
t-value lower bound: 5.01 	p-value lower bound: 0.000002
t-value upper bound: -4.16 	p-value upper bound: 0.00004
degrees of freedom : 83

Equivalence bounds (Cohen's d):
low eqbound: -0.5 
high eqbound: 0.5

Equivalence bounds (raw scores):
low eqbound: -3.3703 
high eqbound: 3.3703

TOST confidence interval:
lower bound 90% CI: -0.912
upper bound 90% CI:  1.535

NHST confidence interval:
lower bound 95% CI: -1.151
upper bound 95% CI:  1.774

Equivalence Test Result:
The equivalence test was significant, t(83) = -4.159, p = 0.0000388, given equivalence bounds of -3.370 and 3.370 (on a raw scale) and an alpha of 0.05.

Null Hypothesis Test Result:
The null hypothesis test was non-significant, t(83) = 0.424, p = 0.673, given an alpha of 0.05.

Based on the equivalence test and the null-hypothesis test combined, we can conclude that the observed effect is statistically not different from zero and statistically equivalent to zero.
> 
