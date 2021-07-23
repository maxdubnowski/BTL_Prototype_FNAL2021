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
  void GetBaseline(double &mean, double &sdev, int irange=0, int frange=100);
  void GetMinimum(double &ymin, double &xmin, int irange=0, int frange=100, int hwindow=0);
  void FindThreshold_P1(double &x, double thr, int leftpoints=3, int rightpoints=3);
  void ShiftWave(double cte);
  void ShiftWave(double ref[1024]);
  void ShiftWave(TGraph *ref);

  double GetAmplitude() {return fAmplitude;}
  double GetTime() {return fTime;}
  void SetAmplitude(double val) {fAmplitude=val;}
  void SetTime(double val) {fTime=val;}

  double GetLFa() {return fLFa;}
  double GetLFb() {return fLFb;}
  double GetLFrab() {return fLFrab;}

private:
  void LinearFit(int ini=0, int fin=1024);
  
  double fAmplitude;
  double fTime;

  double fLFa;
  double fLFb;
  double fLFrab;

  ClassDef(DRSWAVE,0);
};

#endif
