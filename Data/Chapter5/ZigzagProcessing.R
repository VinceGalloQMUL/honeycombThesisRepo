######################################################
#   Production BaseProcessing.r used in chapter 4 of the thesis
######################################################
library("TOSTER")
library(rstatix)
install.packages("broom"), "car"

setwd("d:/Projects/PhD/QMUL/Thesis/Stats")
source("library/thesis_lib.r")

angleshare <- read.csv("Chapter5/angleshare.csv", sep=",")

## if wall angle 0 == wall angle 1 and < 0.1? radians, then its a frame value
## if delta wall angle > .2 radians then its 360, otherwise 180 class
## but calculations will still consider angle halving, and I hope I find soem straight barriers that are nto quite straight.

angleshare$angle <- 0
for(idx in 1:length(angleshare$ZigZag)) {
    for(cidx in anglecols) {
        angle<-angleshare[idx, cidx]
        if (angle != -1) {
            angle <- angle  * 180 / pi
            if (angle < 90) angle <- (180 - angle)
            if (angleshare[idx, "stage"] == 2 & angleshare[idx, "grp"] == 2 ) {
                angle <- angle - 120
            } else {
                angle <- angle - 90
            }
            angleshare[idx, "angle"] <- abs(angle)
        }
    }
        
}

angleshare %>% group_by(stage, grp) %>%
+ get_summary_stats(angle, type = "mean_sd")
# A tibble: 6 x 6
    grp stage variable     n  mean    sd
  <int> <int> <chr>    <dbl> <dbl> <dbl>
1     1     1 angle       49 106.  12.1 
2     2     1 angle       49 106.  12.1 
3     3     1 angle       49 106.  12.1 
4     1     2 angle       45  96.1  5.74
5     2     2 angle      246 117.  10.8 
6     3     2 angle      107  93.5  3.20
> 
angleshare %>% group_by(stage, grp) %>%
shapiro_test(angle)

res.aov <- anova_test(data = angleshare, dv = angle, between = grp, within = stage, wid = id2)
Warning: The 'wid' column contains duplicate ids across between-subjects variables. Automatic unique id will be created
> 
> get_anova_table(res.aov)
ANOVA Table (type III tests)

     Effect DFn DFd      F        p p<.05   ges
1       grp   2 140  2.175 1.17e-01       0.014
2     stage   1 140 69.525 6.37e-14     * 0.208
3 grp:stage   2 140  1.868 1.58e-01       0.014


boxplot(angleshare[angleshare$grp == 1 & angleshare$stage == 1,"angle"], angleshare[angleshare$grp == 1 & angleshare$stage == 2,"angle"], angleshare[angleshare$grp == 2 & angleshare$stage == 2,"angle"], angleshare[angleshare$grp == 3 & angleshare$stage == 2,"angle"])

wilcox.test(angleshare[angleshare$grp == 1 & angleshare$stage == 1,"angle"], angleshare[angleshare$stage == 2,"angle"])
wilcox.test(angleshare[angleshare$grp == 1 & angleshare$stage == 2,"angle"], angleshare[angleshare$grp == 2 & angleshare$stage == 2,"angle"])
wilcox.test(angleshare[angleshare$grp == 1 & angleshare$stage == 2,"angle"],  angleshare[angleshare$grp == 3 & angleshare$stage == 2,"angle"])
wilcox.test(angleshare[angleshare$grp == 2 & angleshare$stage == 2,"angle"], angleshare[angleshare$grp == 3 & angleshare$stage == 2,"angle"])



orthogs<-read.csv("Chapter5/orthogonalStage1.csv", sep=",")
orthogs1<-read.csv("Chapter5/orthogonalStage2.csv", sep=",")
orthogs2<-read.csv("Chapter5/orthogonalStage3.csv", sep=",")
orthogs3<-read.csv("Chapter5/orthogonalStage4.csv", sep=",")
anglecols<-which( colnames(orthogs)=="a0" ):which( colnames(orthogs)=="a7" )
angles<-c()
for(idx in 1:length(orthogs$ZigZag)) {
    for(cidx in anglecols) {
        angle<-orthogs[idx, cidx]
        if (angle != -1) {
            angle <- angle  * 180 / pi
            if (angle < 90) angle <- (180 - angle)
            angles <- c(angles, angle)
        }
    }
}

angles1<-c()
for(idx in 1:length(orthogs1$ZigZag)) {
    for(cidx in anglecols) {
        angle<-orthogs1[idx, cidx]
        if (angle != -1) {
            angle <- angle  * 180 / pi
            if (angle < 90) angle <- (180 - angle)
            angles1 <- c(angles1, angle)
        }
    }
}
angles2<-c()
for(idx in 1:length(orthogs2$ZigZag)) {
    for(cidx in anglecols) {
        angle<-orthogs2[idx, cidx]
        if (angle != -1) {
            angle <- angle  * 180 / pi
            if (angle < 90) angle <- (180 - angle)
            angles2 <- c(angles2, angle)
        }
    }
}
angles3<-c()
for(idx in 1:length(orthogs3$ZigZag)) {
    for(cidx in anglecols) {
        angle<-orthogs3[idx, cidx]
        if (angle != -1) {
            angle <- angle  * 180 / pi
            if (angle < 90) angle <- (180 - angle)
            angles3 <- c(angles3, angle)
        }
    }
}

##fig 1.1 showing all 4 stages
xaxisStr <- c("Stage 1,Isolated", "Stage 2,Adjacent", "Stage 3,Mature", "Against Frame")

titleStr<-sprintf("Cell wall to stimulus angles (n=%d, %d, %d & %d, median=%0.1f, %0.1f, %0.1f & %0.1f)",length(angles), length(angles1), length(angles2), length(angles3), zigzagStats$stats[3], zigzagStats1$stats[3], zigzagStats2$stats[3], zigzagStats3$stats[3])
boxplot(angles, angles1, angles2, angles3, col=c("red", "lightblue", "blue", "yellow"), ylim=c(90,140), names = xaxisStr, main=titleStr)
abline( h = 90, lty = 2)
abline( h = 120, lty = 3)

angles1Abs <- abs(angles1 - 90)
angles2Abs <- abs(angles2 - 120)
angles3Abs <- abs(angles3 - 90)
xaxisStr <- c("Stage 2,Adjacent", "Stage 3,Mature", "Against Frame")
boxplot(angles1Abs, angles2Abs, angles3Abs, col=c("lightblue", "blue", "yellow"), names = xaxisStr, main=titleStr)

setS1m <- mean(set1)
setS1sd <- sd(set1)
set1m <- mean(set1)
set1sd <- sd(set1)
set2m <- mean(set2)
set2sd <- sd(set2)
TOSTone(m=mean(angles1Abs), mu = mean(angles2Abs),sd=sd(angles1Abs),n=length(angles1Abs),low_eqbound_d=-0.75, high_eqbound_d=0.75, alpha=0.05)
TOSTone(m=mean(angles2Abs), mu = mean(angles1Abs),sd=sd(angles2Abs),n=length(angles2Abs),low_eqbound_d=-0.75, high_eqbound_d=0.75, alpha=0.05)


t.test(angles, angles1)
t.test(angles, angles2)
t.test(angles, angles3)
t.test(angles1, angles2)
t.test(angles1, angles3)
t.test(angles2, angles3)
mean(angles)
median(angles)
sd (angles)
mean(angles1)
median(angles1)
sd (angles1)
mean(angles2)
median(angles2)
sd (angles2)
mean(angles3)
median(angles3)
sd (angles3)
