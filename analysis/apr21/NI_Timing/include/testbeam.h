#ifndef TESTBEAM_H
#define TESTBEAM_H
#include <TString.h>
#include <TTree.h>
#include <TList.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>

#include "DRSWAVE.h"
#include "mcp.h"
#include "energy.h"
#include "timing.h"

class testbeam {
public:
  testbeam(int run);
  ~testbeam();
  void SetInputFileName(TString);
  void SetOutputFileName(TString);
  void Process();

private:
  TGraph* DisplayWave(DRSWAVE *A, DRSWAVE *B, DRSWAVE *C, int showfit);
  void CalibrateTime(double A[5], double B[5], double C[5], double X[5], double Y[5], double Z[5]);
  void Init();
  void Terminate();

  int fRun;
  
  TTree *fTree;
  TList *fList;
  TString fOutputFileName;

  mcp *fMCP0;
  mcp *fMCP1;
  energy *fEnergyL;
  energy *fEnergyR;
  timing *fTimingL;
  timing *fTimingR;
  timing *fTimingClipLineL;
  timing *fTimingClipLineR;

  TF1 *fQuadratic;
  
  TH1D *fEvents;
  TH1D *fTRKchi2;
  TH2D *fTRKxy0;

  TCanvas *fEvent1DisplayMCP;
  TCanvas *fEvent2DisplayMCP;
  TCanvas *fEvent4DisplayEnergy;
  TCanvas *fEvent5DisplayTiming;
  TCanvas *fEvent5DisplayTimingCL;

  TH2D *fMCPdiffpos;
  TH2D *fMCPcorrE;
  TH2D *fMCPcorr;
  TH1D *fMCPdiff;
  TH2D *fTRKxyMCP0;
  TH2D *fTRKxyMCP1;
  TH2D *fTRKxyMCPI;
  TH2D *fTRKxyEnergyL;
  TH2D *fTRKxyEnergyLS;
  TH2D *fTRKxyEnergyR;
  TH2D *fTRKxyEnergyRS;

  TH2D *fMCPta0;
  TH2D *fMCPta1;

  TH2D *fEnergyLX;
  TH2D *fEnergyRX;
  TH2D *fEnergyLR;
  TH2D *fEnergyAvgLR;

  TH2D *fTimingNCvsCL[5];
  TH2D *fTimingNCvsCR[5];
  TH2D *fTimingLE[5];
  TH2D *fTimingRE[5];
  TH2D *fTimingLEC[5];
  TH2D *fTimingREC[5];
  TH2D *fTimingLR[5];
  TH2D *fTimingX[5];
  TH2D *fTimingXC[5];
  TH1D *fTiming[5];
  TH2D *fTimingX_P[5];
  TH2D *fTimingXC_P[5];
  TH1D *fTiming_P[5];

};

#endif
