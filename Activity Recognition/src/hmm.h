#ifndef HMM_H
#define HMM_H

void HMMTrain(int cvnum);
void HMMTest(int cvnum);
void calculateEmission();
void normalizeTransitionProb();
void updateLikelihood(int *event);
void saveHMM(FILE *fp);
void readHMM(FILE *fp);
int  getMax();

#endif
