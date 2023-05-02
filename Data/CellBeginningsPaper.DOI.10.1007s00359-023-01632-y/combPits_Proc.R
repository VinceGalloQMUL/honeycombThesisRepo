######################################################
#   Production combPits_proc.r used in chapter 3 of the thesis, now for Cell Beginnings Paper
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

exptTabMeans <- aggregate(exptRimpits$degrees, list(exptRimpits$tabinstance), FUN=mean)
colnames(exptTabMeans) <- c("tabinstance", "degrees")
rndTabMeans <- aggregate(rndRimpits$degrees, list(rndRimpits$tabinstance), FUN=mean)
colnames(rndTabMeans) <- c("tabinstance", "degrees")



showControl <- 1

titleQualStr <- c("Experiment", "Control")
titleIDStr <- c("A", "B", "C", "D")

dataH1<-rimpit[rimpit$isRandom == 0,]$degrees
dataH1R<-rimpit[rimpit$isRandom == 1,]$degrees
dataH1A<-exptTabMeans$degrees
dataH1RA<-rndTabMeans$degrees

################################## revised histogram combining the two aggregated sets into  single chart to meet Thesis updates and the paper
################### prepare Fig 9 ########################
par(mfrow = c(1,1))

#titleStr<-"A - Pit rim to wall intersection"
#aggregated = TRUE
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
xaxisStr <- "Overlap (degrees)"
# optional B/W
useColrs = c("black", "gray")

barplot (hStage1$counts, ylim = c(0, ymax * 1.25), width = c(0.5, 0.5),ylab = "Frequency", xlab = xaxisStr, col = useColrs[1], space = c(1,rep(spread + 1, nBreaks - 1)), names = as.character(commonBreaks[1:nBreaks + 1]))
barplot (hStage2$counts, width = c(0.5, 0.5), space = c(2,rep(spread + 1, nBreaks - 1)), col = useColrs[2],add = TRUE)
legend("topright", legend = c("Experiment", "Control"), col = useColrs, pch = 15, bty = "n", pt.cex = 1.0, cex = 1.0, horiz = F)
#title(titleStr, adj = 0.1)
abline(h = 0)


## copy image at 900x500
######### end of Fig 9 ###############

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
