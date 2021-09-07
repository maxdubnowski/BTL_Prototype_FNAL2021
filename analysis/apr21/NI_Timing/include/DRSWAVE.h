// DRWAVE : Base class for wave analysis
// inherits all functionalities of TGraph
// author: cperez
#ifndef DRSWAVE_H
#define DRSWAVE_H

#include "TGraph.h"

class DRSWAVE : public TGraph {
public:
  DRSWAVE();
  ~DRSWAVE();
  void Fill(float x[1024], float y[1024]);
  void FillClipLine(float x[1024], float y[1024], int samples);
  void GetBaseline(double &mean, double &sdev, int irange=0, int frange=100);
  void GetSampleSum(double &sum, int irange=0, int frange=100);
  int GetMinimum(double &ymin, double &xmin, int irange=0, int frange=100, int hwindow=0);
  void FindThreshold_P1(double &x, double thr, int leftpoints=3, int rightpoints=3);
  void FindMinimum_P2(double &ymin, double &xmin, int leftpoints=3, int rightpoints=3);
  void ShiftWave(double cte);
  void ShiftWave(double ref[1024]);
  void ShiftWave(float ref[1024]);
  void ShiftWave(TGraph *ref);

  double GetAvgEne() {return fAvgEne;}
  void SetAvgEne(double val) {fAvgEne=val;}

  double GetRefTime() {return fRefTime;}
  void SetRefTime(double val) {fRefTime=val;}

  double GetAmplitude() {return fAmplitude;}
  double GetTime() {return fTime;}
  void SetAmplitude(double val) {fAmplitude=val;}
  void SetTime(double val) {fTime=val;}

  double GetLFa() {return fLFa;}
  double GetLFb() {return fLFb;}
  double GetLFrab() {return fLFrab;}

  double GetQFa() {return fQFa;}
  double GetQFb() {return fQFb;}
  double GetQFc() {return fQFc;}

  void SetBaselineLimits(int a, int b) {fBase_in=a; fBase_en=b;}
  int fBase_in;
  int fBase_en;

private:
  void LinearFit(int ini=0, int fin=1024);
  void QuadraticFit(int ini=0, int fin=1024);

  double fAvgEne;
  double fRefTime;
  
  double fAmplitude;
  double fTime;

  double fLFa;
  double fLFb;
  double fLFrab;
  double fQFa;
  double fQFb;
  double fQFc;

  ClassDef(DRSWAVE,0);
};

#endif
