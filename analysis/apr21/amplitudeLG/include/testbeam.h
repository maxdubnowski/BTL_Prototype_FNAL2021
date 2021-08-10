#ifndef TESTBEAM_H
#define TESTBEAM_H
#include <TString.h>
#include <TTree.h>
#include <TList.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>

#include "mcp.h"
#include "energy.h"
#include "timing.h"

class testbeam {
public:
  testbeam();
  ~testbeam();
  void SetInputFileName(TString);
  void SetOutputFileName(TString);
  void Process();

private:
  void Init();
  void Terminate();
  void DisplayWave(int idx, int iEvent2, DRSWAVE *A, DRSWAVE *B);
  void DisplayWave(DRSWAVE *A, DRSWAVE *B, int showfit=0);

  TTree *fTree;
  TList *fList;
  TString fOutputFileName;

  mcp *fMCP0;
  energy *fEnergy[8];
  timing *fTiming[8];

  TH1D *fEvents;
  TH1D *fTRKchi2;
  TH2D *fTRKxy0;

  TCanvas *fEventDisplayMCP;
  TCanvas *fEventDisplayEnergy[4];
  TCanvas *fEventDisplayTiming[4];

  TH2D *fTRKxyMCP0;
  TH2D *fTRKxyEnergy[8];
  TH1D *fHistThr[8];
  TProfile *fEnergyPulse[8][5];
  TProfile *fEnergyPulseT0[8][5];
  TH2D *fTimingWalk[8][5];
  TH2D *fTimingWalkC[8][5];

  TH2D *fEnergyCorrelation[4];
  TH2D *fTimingCorrelation[4];
  TH2D *fTimingCorrelationSL[4][5];

  TF1 *fQuadratic;
};

#endif
