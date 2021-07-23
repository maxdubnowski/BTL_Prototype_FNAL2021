#ifndef TESTBEAM_H
#define TESTBEAM_H
#include <TString.h>
#include <TTree.h>
#include <TList.h>
#include <TCanvas.h>

#include "mcp.h"

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

  TTree *fTree;
  TList *fList;
  TString fOutputFileName;

  mcp *fMCP0;
  mcp *fMCP1;

  TH1D *fEvents;
  TH1D *fTRKchi2;
  TH2D *fTRKxy0;

  TCanvas *fEventDisplayMCP;
  TH2D *fMCPcorr;
  TH1D *fMCPdiff;
  TH2D *fTRKxyMCP0;
  TH2D *fTRKxyMCP1;
  TH2D *fTRKxyMCPI;
  TH2D *fMCPta0;
  TH2D *fMCPta1;

};

#endif
