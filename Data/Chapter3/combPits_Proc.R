######################################################
#   Production combPits_proc.r used in chapter 3 of the thesis
######################################################
setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

rimpit<-read.csv("Chapter3/rimpit/rimpit090_119.csv", sep=",", header=TRUE)
rimpit$r[rimpit$isRandom == 1]<-"R"
rimpit$r[rimpit$isRandom != 1]<-"E"
rimpit$seg<-paste(trimws(rimpit$expt),rimpit$r, sep="")
rimpit$degrees<-180*rimpit$angleSum/pi
rimpit$hiveseg<-paste(trimws(rimpit$hive),rimpit$r, sep="")
rimpit$tabseg<-paste(trimws(rimpit$tab),rimpit$r, sep="")
#rimpit$tabinstance<-paste(trimws(rimpit$expt),trimws(rimpit$tab),rimpit$r, sep="")
rimpit$tabinstance<-paste(trimws(rimpit$expt),trimws(rimpit$tab), sep="")
rimpit$tabinstanceRnd<-paste(trimws(rimpit$expt),trimws(rimpit$tab),rimpit$r, sep="")
##write.table(rimpit, "D:/Projects/PhD/Research/CombFormation/Stigmergy/rimpit/rimpit090_119Categorised.csv", sep = ",", row.names = FALSE, col.names = TRUE)

exptTabMeans <- aggregate(exptRimpits$degrees, list(exptRimpits$tabinstance), FUN=mean)
colnames(exptTabMeans) <- c("tabinstance", "degrees")
rndTabMeans <- aggregate(rndRimpits$degrees, list(rndRimpits$tabinstance), FUN=mean)
colnames(rndTabMeans) <- c("tabinstance", "degrees")



showControl <- 1
# or 
showControl <- 0

titleQualStr <- c("Experiment", "Control")
titleIDStr <- c("A", "B", "C", "D")

  ###### start ######## histograms of pitrim distribution ############# - Fig 1-11
par(mfrow = c(1,2))
#### plot 1 & 2, deposit relative to bisection
dataH1<-rimpit[rimpit$isRandom == 0,]$degrees
dataH1R<-rimpit[rimpit$isRandom == 1,]$degrees
dataH1A<-exptTabMeans$degrees
dataH1RA<-rndTabMeans$degrees
summary(dataH1)
summary(dataH1R)
summary(dataH1A)
summary(dataH1RA)


brks <- seq(0,360,20)
yMax<-max(hist (dataH1, breaks = brks, plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH1R, breaks = brks, plot=FALSE)$counts)

titleStrT<-" - Pit rim to wall intersection"
titleStr <- paste(titleIDStr[1], titleStrT, "\n", titleQualStr[1])
XaxisRange<-c(0,max(brks))
sumry<-summary(dataH1)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
xLabelStr<-"Degrees"
hist (dataH1, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brks)
titleStr <- paste(titleIDStr[2], titleStrT, "\n", titleQualStr[2])
sumry<-summary(dataH1R)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1R, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brks)
########## 2nd pair...aggreagted
brks <- seq(0,360,10)

yMax<-max(hist (dataH1A, breaks = brks, plot=FALSE)$counts)
yMax<-max(yMax, hist (dataH1RA, breaks = brks, plot=FALSE)$counts)
titleStr <- paste(titleIDStr[3], titleStrT, "\n", titleQualStr[1], " aggregate")
XaxisRange<-c(0,max(brks))
sumry<-summary(dataH1A)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
xLabelStr<-"Degrees"
hist (dataH1A, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brks)
titleStr <- paste(titleIDStr[4], titleStrT, "\n", titleQualStr[2], " aggregate")
sumry<-summary(dataH1RA)
#xLabelStr<-sprintf("Degrees, median %0.1f, inter-quartile %0.1f & %0.1f", sumry[3], sumry[2], sumry[5])
hist (dataH1RA, breaks = brks, ylim=c(0,yMax), xlim=XaxisRange, main=titleStr, xlab=xLabelStr, col="lightblue", xaxt = 'n')
abline(v=sumry[3])
abline(v=sumry[2], lty=2)
abline(v=sumry[5], lty=2)
axis(side=1, at=brks)

## copy image at 900x500

################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
par(mfrow = c(1,1))

#titleStr<-"A - Pit rim to wall intersection"
aggregated = TRUE
aggregated = FALSE

if (aggregated) {
    titleStr<-"a"
    set1 <- dataH1A
    set2 <- dataH1RA
} else {
    titleStr<-"b"
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
if (aggregated) {
    barMax <- 200
    barMin <- 70
    barDiv <- 10
} else {
    barMax <- 300
    barMin <- 0
    barDiv <- 20
}
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
# optional B/W
useColrs = c("black", "gray")

#barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
#barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
#legend("topright", legend = c(sprintf("Opposing cells (%d)", length(set1)), sprintf("Natural (%d)", length(set2))), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
title(titleStr, adj = 0.1)
abline(h = 0)

## copy image at 700x500
mean(dataH1)
sd(dataH1)
length(dataH1)
mean(dataH1R)
sd(dataH1R)
length(dataH1R)
t.test(dataH1, dataH1R,  paired = FALSE)

[1] 142.5378
> sd(dataH1)
[1] 46.02041
> length(dataH1)
[1] 229
> mean(dataH1R)
[1] 92.34349
> sd(dataH1R)
[1] 31.44447
> length(dataH1R)
[1] 229
t = 13.628, df = 402.79, p-value < 2.2e-16

mean(dataH1A)
sd(dataH1A)
length(dataH1A)
mean(dataH1RA)
sd(dataH1RA)
length(dataH1RA)
t.test(dataH1A, dataH1RA,  paired = TRUE)

######### end of histograms  ############


################# junk ######################
#rimpit<-read.csv("D:/Projects/PhD/Research/CombFormation/Stigmergy/Technique2020/090_099/rimpit093094095_withSim.csv", sep=",", header=TRUE)
rimpit<-read.csv("D:/Projects/PhD/Research/CombFormation/Stigmergy/Technique2020/090_099/rimpit093094095.csv", sep=",", header=TRUE)
rimpit$r[rimpit$isRandom == 1]<-"R"
rimpit$r[rimpit$isRandom != 1]<-"E"
rimpit$seg<-paste(trimws(rimpit$expt),rimpit$r, sep="")
rimpit$degrees<-180*rimpit$angleSum/pi

par(mfrow = c(1,2))

colorBySeg = c("red", "blue", "red", "blue", "red", "blue")
#boxplot(rimpit$degrees ~ rimpit$isRandom, col=colorBySeg, ylab = "Degrees", xlab = "Experiment vs Control/Random", main = "Pit rim intersection with deposited wax")
# box plot of all experiment vs control
boxplot(rimpit$degrees ~ rimpit$isRandom, col=colorBySeg, ylab = "Degrees", xlab = "Experiment vs Control/Random", main = "Pit rim intersection with deposited wax", names = c("Experiment", "Control"))
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))
##box plot of 3 frames, experimental compared with random/control plus simulation...not a good/useful comparison
#boxplot(rimpit$degrees ~ rimpit$seg, col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")
#legend("bottomright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = T)
boxplot(rimpit$degrees[rimpit$shape == 7] ~ rimpit$seg[rimpit$shape == 7], col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")
legend("bottomright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = T)
for(i in seq(2.5 , 4.5 , 2)){   abline(v=i,lty=1, col="grey")  }

#before t-testing, check that te samples look 'normal'
fig1.3

hist(rimpit$degrees[rimpit$isRandom == 0], breaks=seq(0,360,20))
hist(rimpit$degrees[rimpit$isRandom == 1], breaks=seq(0,360,20))
t.test(rimpit$degrees[rimpit$isRandom == 0], rimpit$degrees[rimpit$isRandom == 1])

	Welch Two Sample t-test

data:  rimpit$degrees[rimpit$isRandom == 0] and rimpit$degrees[rimpit$isRandom == 1]
t = 4.2982, df = 139.53, p-value = 3.212e-05
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
 17.97494 48.59732
sample estimates:
mean of x mean of y 
128.58950  95.30337

wilcox.test(rimpit$degrees[rimpit$isRandom == 0], rimpit$degrees[rimpit$isRandom == 1])
	Wilcoxon rank sum test with continuity correction

data:  rimpit$degrees[rimpit$isRandom == 0] and rimpit$degrees[rimpit$isRandom == 1]
W = 3223.5, p-value = 5.814e-05

length(rimpit$degrees[rimpit$isRandom == 0])
[1] 97
> length(rimpit$degrees[rimpit$isRandom == 1])
[1] 47
mean(rimpit$degrees[rimpit$isRandom == 0])
mean(rimpit$degrees[rimpit$isRandom == 1])
sd(rimpit$degrees[rimpit$isRandom == 0])
sd(rimpit$degrees[rimpit$isRandom == 1])
[1] 128.5895
[1] 95.30337
[1] 59.75225
[1] 32.99658

t.test(rimpit$degrees[rimpit$seg == "093A1E" ], rimpit$degrees[rimpit$seg == "093A1R"])

	Welch Two Sample t-test

data:  rimpit$degrees[rimpit$seg == "093A1E"] and rimpit$degrees[rimpit$seg == "093A1R"]
t = 2.5355, df = 28.049, p-value = 0.01709
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
  8.130109 76.487565
sample estimates:
mean of x mean of y 
129.28436  86.97553 

wilcox.test(rimpit$degrees[rimpit$seg == "093A1E" ], rimpit$degrees[rimpit$seg == "093A1R"])
W = 156, p-value = 0.03282

length(rimpit$degrees[rimpit$seg == "093A1E"])
[1] 21
length(rimpit$degrees[rimpit$seg == "093A1R"])
[1] 10
mean(rimpit$degrees[rimpit$seg == "093A1E"])
mean(rimpit$degrees[rimpit$seg == "093A1R"])
sd(rimpit$degrees[rimpit$seg == "093A1E"])
sd(rimpit$degrees[rimpit$seg == "093A1R"])
[1] 129.2844
[1] 86.97553
[1] 67.31202
[1] 25.03847

t.test(rimpit$degrees[rimpit$seg == "094A1E" ], rimpit$degrees[rimpit$seg == "094A1R"])

	Welch Two Sample t-test

data:  rimpit$degrees[rimpit$seg == "094A1E"] and rimpit$degrees[rimpit$seg == "094A1R"]
t = 2.4573, df = 53.816, p-value = 0.01725
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
  5.827581 57.503010
sample estimates:
mean of x mean of y 
113.65117  81.98588

wilcox.test(rimpit$degrees[rimpit$seg == "094A1E" ], rimpit$degrees[rimpit$seg == "094A1R"])
W = 457.5, p-value = 0.04115

length(rimpit$degrees[rimpit$seg == "094A1E"])
length(rimpit$degrees[rimpit$seg == "094A1R"])
40/17
mean(rimpit$degrees[rimpit$seg == "094A1E"])
mean(rimpit$degrees[rimpit$seg == "094A1R"])
sd(rimpit$degrees[rimpit$seg == "094A1E"])
sd(rimpit$degrees[rimpit$seg == "094A1R"])
1] 113.6512
[1] 81.98588
[1] 65.28895
[1] 31.80232

t.test(rimpit$degrees[rimpit$seg == "096A1E" ], rimpit$degrees[rimpit$seg == "096A1R"])

	Welch Two Sample t-test

data:  rimpit$degrees[rimpit$seg == "096A1E"] and rimpit$degrees[rimpit$seg == "096A1R"]
t = 3.3086, df = 49.693, p-value = 0.001749
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
 13.35445 54.63584
sample estimates:
mean of x mean of y 
 144.7823  110.7872 
 
wilcox.test(rimpit$degrees[rimpit$seg == "096A1E" ], rimpit$degrees[rimpit$seg == "096A1R"])
W = 522, p-value = 0.005751

length(rimpit$degrees[rimpit$seg == "096A1E"])
length(rimpit$degrees[rimpit$seg == "096A1R"])
 36/20
mean(rimpit$degrees[rimpit$seg == "096A1E"])
mean(rimpit$degrees[rimpit$seg == "096A1R"])
sd(rimpit$degrees[rimpit$seg == "096A1E"])
sd(rimpit$degrees[rimpit$seg == "096A1R"])
[1] 144.7823
[1] 110.7872
[1] 43.92027
[1] 32.24541

 one tailed T test ... can be argued that this is valid as the hypothesis was that the intersection would be greater
 but a two-tailed test is more exacting, and saves the discussion to justify the improved results...however, take advice
 t.test(rimpit$angleSum[rimpit$seg == "093A1E" ], rimpit$angleSum[rimpit$seg == "093A1R"], alternative="greater")

	Welch Two Sample t-test

data:  rimpit$angleSum[rimpit$seg == "093A1E"] and rimpit$angleSum[rimpit$seg == "093A1R"]
t = 2.5355, df = 28.049, p-value = 0.008543
alternative hypothesis: true difference in means is greater than 0
95 percent confidence interval:
 0.2430206       Inf
sample estimates:
mean of x mean of y 
 2.256438  1.518009 
 t.test(rimpit$degrees[rimpit$seg == "093A1E" ], rimpit$degrees[rimpit$seg == "093A1R"], alternative="greater")

	Welch Two Sample t-test

data:  rimpit$degrees[rimpit$seg == "093A1E"] and rimpit$degrees[rimpit$seg == "093A1R"]
t = 2.5355, df = 28.049, p-value = 0.008543
alternative hypothesis: true difference in means is greater than 0
95 percent confidence interval:
 13.92406      Inf

library(ggplot2)
ggplot(rimpit, aes(x=seg, y=degrees), color = seg) + 
geom_violin(aes(fill = seg), show.legend = FALSE)  + 
scale_fill_manual(values = c("red", "blue", "red", "blue", "red", "blue"))  + 
geom_boxplot(width=0.2, fill = "black") + 
stat_summary(fun = median, geom = "point", fill = "white", shape = 21, size = 2.5) + 
xlab("Test , Experiment or Random") + 
ylab("Rim-wax overlap in degrees")

library(ggplot2)
> ggplot(rimpit, aes(x=seg, y=degrees), color = seg) + 
+     geom_violin(aes(fill = seg), show.legend = FALSE)  + 
+     scale_fill_manual(values = c("red", "blue", "red", "blue", "red", "blue", "yelloW"))  + 
+     geom_boxplot(width=0.2, fill = "black") + 
+     stat_summary(fun = median, geom = "point", fill = "white", shape = 21, size = 2.5) + 
+     xlab("Test , Experiment or Random") + 
+     ylab("Rim-wax overlap in degrees")

###################### full data set, with hive and tab identifiers #######################
rimpit<-read.csv("D:/Projects/PhD/Research/CombFormation/Stigmergy/rimpit/rimpit090_119.csv", sep=",", header=TRUE)
rimpit$r[rimpit$isRandom == 1]<-"R"
rimpit$r[rimpit$isRandom != 1]<-"E"
rimpit$seg<-paste(trimws(rimpit$expt),rimpit$r, sep="")
rimpit$degrees<-180*rimpit$angleSum/pi
rimpit$hiveseg<-paste(trimws(rimpit$hive),rimpit$r, sep="")
rimpit$tabseg<-paste(trimws(rimpit$tab),rimpit$r, sep="")
#rimpit$tabinstance<-paste(trimws(rimpit$expt),trimws(rimpit$tab),rimpit$r, sep="")
rimpit$tabinstance<-paste(trimws(rimpit$expt),trimws(rimpit$tab), sep="")
rimpit$tabinstanceRnd<-paste(trimws(rimpit$expt),trimws(rimpit$tab),rimpit$r, sep="")
write.table(rimpit, "D:/Projects/PhD/Research/CombFormation/Stigmergy/rimpit/rimpit090_119Categorised.csv", sep = ",", row.names = FALSE, col.names = TRUE)

par(mfrow = c(1,1))
par(mfrow = c(1,2))

########### first part of fig 1.5

colorBySeg = c("red", "blue", "red", "blue", "red", "blue")
#boxplot(rimpit$degrees ~ rimpit$isRandom, col=colorBySeg, ylab = "Degrees", xlab = "Experiment vs Control/Random", main = "Pit rim intersection with deposited wax")
# box plot of all experiment vs control

qtyE <- dim(rimpit[rimpit$isRandom == 0,])[1]
qtyR <- dim(rimpit[rimpit$isRandom == 1,])[1]
xaxisStr <- sprintf("Experiment (%d) vs Control/Random (%d)", qtyE, qtyR)

boxplot(rimpit$degrees ~ rimpit$isRandom, col=colorBySeg, ylab = "Degrees", xlab = xaxisStr, main = "Pit rim intersection with deposited wax", names = c("Experiment", "Control"))
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))

##box plot of 14 frames, experimental compared with random/control plus simulation...not a good/useful comparison
boxplot(rimpit$degrees ~ rimpit$seg, col=colorBySeg, ylab = "Degrees", xaxt = "n", xlab = "Experiment/Random, results for each frame", main = "Pit rim intersection with deposited wax")
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))

########### test cases, exploring the frame/tab distributions...eventually showed nothing of interest
#legend("bottomright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = T)
boxplot(rimpit$degrees ~ rimpit$seg, col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")
legend("bottomright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = T)
for(i in seq(2.5 , 4.5 , 2)){   abline(v=i,lty=1, col="grey")  }
boxplot(rimpit$degrees ~ rimpit$hiveseg, col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")
boxplot(rimpit$degrees ~ rimpit$tabseg, col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")
boxplot(rimpit$degrees ~ rimpit$tabinstance, col=colorBySeg, ylab = "Degrees", xlab = "Test number Experiment/Random", main = "Pit rim intersection with deposited wax")

########### second part of fig 1.5
boxplot(rimpit$degrees ~ rimpit$tabinstanceRnd, col=colorBySeg, ylab = "Degrees", xaxt = "n", xlab = "Experiment/Random, results for each tab", main = "Pit rim intersection with deposited wax")
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))

histogram(rimpit[rimpit$isRandom == 0,]$degrees)

############ bar chart showing pitrim distribution ################
######### <<< 700 x 500 Section 1.3.1 	##########
######### CellBeginnings 1.7 #############
exptSet <- rimpit[rimpit$isRandom == 0,]$degrees
rndSet <- rimpit[rimpit$isRandom == 1,]$degrees
divsQty <- 18
#barMin <- min(min(exptSet), min(rndSet))
#barMax <- max(max(exptSet), max(rndSet))
commonBreaks <- seq(0, 360, 360/divsQty)
nBreaks <- length(commonBreaks) - 1
hStage1 <- hist(exptSet, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(rndSet, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
normalisedChange <- hChange$counts / sum(hChange$counts)
ymax <- max(normalisedStage1)
ymax <- max(ymax, normalisedStage2)
ymax <- max(ymax, normalisedChange)

titleStr <- "Normalised distribution of rim to wall intersection"
xaxisStr <- "Degrees"
spread <- 1 # 3
barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))


################ statistics for separate values, table 1.7
mean(rimpit[rimpit$isRandom == 0,]$degrees)
sd(rimpit[rimpit$isRandom == 0,]$degrees)
length(rimpit[rimpit$isRandom == 0,]$degrees)
mean(rimpit[rimpit$isRandom == 1,]$degrees)
sd(rimpit[rimpit$isRandom == 1,]$degrees)
length(rimpit[rimpit$isRandom == 1,]$degrees)
t.test(rimpit[rimpit$isRandom == 0,]$degrees, rimpit[rimpit$isRandom == 1,]$degrees)
wilcox.test(rimpit[rimpit$isRandom == 0,]$degrees, rimpit[rimpit$isRandom == 1,]$degrees)


################ not used
anova(aov(rimpit$degrees ~ factor(rimpit$isRandom)*factor(rimpit$expt)))
Analysis of Variance Table

Response: rimpit$degrees
                                             Df Sum Sq Mean Sq  F value  Pr(>F)    
factor(rimpit$isRandom)                       1 284999  284999 187.0730 < 2e-16 ***
factor(rimpit$expt)                          14  41418    2958   1.9419 0.02112 *  
factor(rimpit$isRandom):factor(rimpit$expt)  13  28792    2215   1.4538 0.13215    
Residuals                                   397 604815    1523  

anova(aov(rimpit$degrees ~ factor(rimpit$isRandom)*factor(rimpit$tab)*factor(rimpit$expt)))
Analysis of Variance Table

Response: rimpit$degrees
                                                                Df Sum Sq Mean Sq  F value Pr(>F)    
factor(rimpit$isRandom)                                          1 284999  284999 186.8875 <2e-16 ***
factor(rimpit$tab)                                               3   4058    1353   0.8870 0.4479    
factor(rimpit$expt)                                             13  37370    2875   1.8850 0.0303 *  
factor(rimpit$isRandom):factor(rimpit$tab)                       3  14098    4699   3.0816 0.0274 *  
factor(rimpit$isRandom):factor(rimpit$expt)                     12  21866    1822   1.1949 0.2844    
factor(rimpit$tab):factor(rimpit$expt)                           5   4938     988   0.6476 0.6635    
factor(rimpit$isRandom):factor(rimpit$tab):factor(rimpit$expt)   5   8630    1726   1.1318 0.3429    
Residuals                                                      383 584066    1525  

anova(aov(rimpit$degrees ~ factor(rimpit$isRandom)*factor(rimpit$tab)*factor(rimpit$expt)))
tmpresult1 <- lm(rimpit$degrees ~ factor(rimpit$tab)+factor(rimpit$isRandom)+factor(rimpit$tab):factor(rimpit$isRandom))
anova(tmpresult1)
Response: rimpit$degrees
                                            Df Sum Sq Mean Sq  F value  Pr(>F)    
factor(rimpit$tab)                           3   6674    2225   1.4232 0.23542    
factor(rimpit$isRandom)                      1 282383  282383 180.6341 < 2e-16 ***
factor(rimpit$tab):factor(rimpit$isRandom)   3  17515    5838   3.7346 0.01136 *  
Residuals                                  418 653453    1563                     


exptRimpits <- rimpit[rimpit$isRandom == 0,]
anova(aov(exptRimpits$degrees ~ factor(exptRimpits$tab)*factor(exptRimpits$expt)))

Response: exptRimpits$degrees
                                                  Df Sum Sq Mean Sq F value  Pr(>F)  
factor(exptRimpits$tab)                            3  16368  5456.1  2.6391 0.05048 .
factor(exptRimpits$expt)                          13  43200  3323.1  1.6074 0.08461 .
factor(exptRimpits$tab):factor(exptRimpits$expt)   5  10155  2030.9  0.9823 0.42941  
Residuals                                        216 446559  2067.4 


################ modified analysis, concern about independence, so aggregate by tab
######### fig 1.7 ###################
exptRimpits <- rimpit[rimpit$isRandom == 0,]
rndRimpits <- rimpit[rimpit$isRandom == 1,]
exptTabMeans <- aggregate(exptRimpits$degrees, list(exptRimpits$tabinstance), FUN=mean)
colnames(exptTabMeans) <- c("tabinstance", "degrees")
rndTabMeans <- aggregate(rndRimpits$degrees, list(rndRimpits$tabinstance), FUN=mean)
colnames(rndTabMeans) <- c("tabinstance", "degrees")

qtyE <- length(exptTabMeans$degrees)
qtyR <- length(rndTabMeans$degrees)
xaxisStr <- sprintf("Experiment (%d) vs Control/Random (%d), aggregated by tab", qtyE, qtyR)

colorBySeg = c("red", "blue", "red", "blue", "red", "blue")
# box plot of aggregated experiment vs control
boxplot(exptTabMeans$degrees, rndTabMeans$degrees, col=colorBySeg, ylab = "Degrees", xlab = xaxisStr, main = "Pit rim intersection with deposited wax", names = c("Experiment", "Control"), ylim = c(50,200))
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, text.col = c("blue", "red"), horiz = F, inset = c(0.1,0.1))

############ bar chart showing pitrim distribution ################
######### <<< 700 x 500 Section 1.3.1 	##########
######### CellBeginnings 1.7 #############
exptSet <- rimpit[rimpit$isRandom == 0,]$degrees
rndSet <- rimpit[rimpit$isRandom == 1,]$degrees
divsQty <- 18
#barMin <- min(min(exptTabMeans$degrees), min(rndTabMeans$degrees))
#barMax <- max(max(exptTabMeans$degrees), max(rndTabMeans$degrees))
commonBreaks <- seq(0, 360, 360/divsQty)
nBreaks <- length(commonBreaks) - 1
hStageA1 <- hist(exptTabMeans$degrees, breaks = commonBreaks, plot = FALSE)
hStageA2 <- hist(rndTabMeans$degrees, breaks = commonBreaks, plot = FALSE)
hStage1 <- hist(exptSet, breaks = commonBreaks, plot = FALSE)
hStage2 <- hist(rndSet, breaks = commonBreaks, plot = FALSE)
normalisedStage1 <- hStage1$counts / sum(hStage1$counts)
normalisedStage2 <- hStage2$counts / sum(hStage2$counts)
normalisedStageA1 <- hStageA1$counts / sum(hStageA1$counts)
normalisedStageA2 <- hStageA2$counts / sum(hStageA2$counts)
ymax <- max(normalisedStageA1)
ymax <- max(ymax, normalisedStageA2)
ymax <- max(ymax, normalisedStage1)
ymax <- max(ymax, normalisedStage2)

titleStr <- "Normalised distribution of rim to wall intersection (aggregated)"
xaxisStr <- "Degrees"
spread <- 1
barplot (normalisedStageA1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "red", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (normalisedStageA2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
legend("topright", legend = c("Control", "Experiment"), col = c("blue", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, horiz = F, inset = c(0,0))

titleStr <- "Normalised distribution of rim to wall intersection (degrees)"
xaxisStr <- "Degrees"
spread <- 3
barplot (normalisedStage1, ylim = c(0, ymax), width = c(0.5, 0.5),main = titleStr, xlab = xaxisStr, col = "#FF6060", space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (normalisedStage2, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = "lightblue",add = TRUE)
barplot (normalisedStageA1, width = c(0.5, 0.5), space = c(3,rep(spread + 1, nBreaks - 1)),col = "red", add = TRUE)
barplot (normalisedStageA2, width = c(0.5, 0.5), space = c(4,rep(spread + 1, nBreaks - 1)), col = "blue",add = TRUE)
legend("topright", legend = c("Control", "Aggregated control", "Experiment", "Aggregated experiment"), col = c("lightblue","blue","#FF6060", "red"), pch = 15, bty = "n", pt.cex = 1.5, cex = 1.0, horiz = F, inset = c(0,0))



mean(exptTabMeans$degrees)
sd(exptTabMeans$degrees)
length(exptTabMeans$degrees)
mean(rndTabMeans$degrees)
sd(rndTabMeans$degrees)
length(rndTabMeans$degrees)
t.test(exptTabMeans$degrees, rndTabMeans$degrees)
wilcox.test(exptTabMeans$degrees, rndTabMeans$degrees)

t.test(exptTabMeans$degrees, rndTabMeans$degrees)

t = 11.26, df = 34.673, p-value = 3.949e-13
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
 45.04468 64.86885
sample estimates:
mean of x mean of y 
145.20079  90.24402 
