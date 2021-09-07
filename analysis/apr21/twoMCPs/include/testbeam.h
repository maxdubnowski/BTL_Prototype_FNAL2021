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

class testbeam {
public:
  testbeam();
  ~testbeam();
  void SetInputFileName(TString);
  void SetOutputFileName(TString);
  void Process();

private:
  TGraph* DisplayWave(DRSWAVE *A, DRSWAVE *B, DRSWAVE *C, int showfit);
  void Init();
  void Terminate();

  TTree *fTree;
  TList *fList;
  TString fOutputFileName;

  mcp *fMCP0;
  mcp *fMCP1;

  TF1 *fQuadratic;
  
  TH1D *fEvents;
  TH1D *fTRKchi2;
  TH2D *fTRKxy0;

  TCanvas *fEvent1DisplayMCP;
  TCanvas *fEvent2DisplayMCP;

  TH2D *fMCPdiffpos;
  TH2D *fMCPcorrE;
  TH2D *fMCPcorr;
  TH1D *fMCPdiff;
  TH2D *fTRKxyMCP0;
  TH2D *fTRKxyMCP1;
  TH2D *fTRKxyMCPI;
  TH2D *fMCPta0;
  TH2D *fMCPta1;

};

#endif
