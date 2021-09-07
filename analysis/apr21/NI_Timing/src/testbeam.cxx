#include "testbeam.h"

#include <iostream>

#include <TFile.h>
#include <TList.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TLine.h>

#include "DRSWAVE.h"
#include "mcp.h"
#include "energy.h"
#include "timing.h"

//=================================
testbeam::testbeam(int run) {
  fRun = run;
  fList = NULL;
  fTree = NULL;
  fMCP0 = NULL;
  fMCP1 = NULL;
  fEvent1DisplayMCP=NULL;
  fEvent2DisplayMCP=NULL;
  fEvents = NULL;
  fTRKchi2 = NULL;
  fTRKxy0 = NULL;
  fTRKxyMCP0 = NULL;
  fTRKxyMCP1 = NULL;
  fMCPcorr = NULL;
  fMCPdiff = NULL;
  fMCPta0 = NULL;
  fMCPta1 = NULL;
  fEnergyL = NULL;
  fEnergyR = NULL;
  fTimingL = NULL;
  fTimingR = NULL;
}
//=================================
testbeam::~testbeam() {
  if(fList) delete fList;
}
//=================================
void testbeam::SetInputFileName(TString val) {
  TFile *inputfile = new TFile( val.Data() );
  fTree = (TTree*) inputfile->Get("pulse");
}
//=================================
void testbeam::SetOutputFileName(TString val) {
  fOutputFileName = val;
}
//=================================
TGraph* testbeam::DisplayWave(DRSWAVE *A, DRSWAVE *B, DRSWAVE *C, int showfit) {
  TGraph *a, *b, *c;
  double tmp;
  double aplot = 999;
  if(A!=NULL) {
    a = new TGraph(1024,A->GetX(),A->GetY());
    a->SetLineColor(kBlue-3);
    a->SetMarkerColor(kBlue-3);
    a->SetMarkerStyle(24);
    A->GetMinimum(aplot, tmp, 0, 1024, 0);
  }
  double bplot = 999;
  if(B!=NULL) {
    b = new TGraph(1024,B->GetX(),B->GetY());
    b->SetLineColor(kGreen-3);
    b->SetMarkerColor(kGreen-3);
    b->SetMarkerStyle(24);
    B->GetMinimum(bplot, tmp, 0, 1024, 0);
  }
  double cplot = 999;
  if(C!=NULL) {
    c = new TGraph(1024,C->GetX(),C->GetY());
    c->SetLineColor(kRed-3);
    c->SetMarkerColor(kRed-3);
    c->SetMarkerStyle(24);
    C->GetMinimum(cplot, tmp, 0, 1024, 0);
  }

  TLatex *tex = new TLatex();
  TLine *lin = new TLine();
  TGraph *ret = NULL;
  if(aplot>990 && bplot>990 && cplot>990) return NULL;
  if(aplot<bplot&&aplot<cplot)
    ret = a;
  if(bplot<aplot&&bplot<cplot)
    ret = b;
  if(cplot<aplot&&cplot<bplot)
    ret = c;
  if(ret==NULL) return NULL;
  ret->Draw("AP");
  if(A!=NULL) a->Draw("PSAME");
  if(B!=NULL) b->Draw("PSAME");
  if(C!=NULL) c->Draw("PSAME");
  if(showfit==2) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,A->GetQFa());
      fQuadratic->SetParameter(1,A->GetQFb());
      fQuadratic->SetParameter(2,A->GetQFc());
      fQuadratic->SetRange( A->GetTime()-4, A->GetTime()+4 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
      tex->SetTextColor(kBlue-3);
      tex->DrawLatexNDC(0.65, 0.30, Form("AvgEne  %.2f",A->GetAvgEne()) );
      lin->SetLineColor(kBlue-3);
      lin->DrawLine( A->GetRefTime(),-800, A->GetRefTime(),100);
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,B->GetQFa());
      fQuadratic->SetParameter(1,B->GetQFb());
      fQuadratic->SetParameter(2,B->GetQFc());
      fQuadratic->SetRange( B->GetTime()-4, B->GetTime()+4 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
      tex->SetTextColor(kGreen-3);
      tex->DrawLatexNDC(0.65, 0.25, Form("AvgEne  %.2f",B->GetAvgEne()) );
      lin->SetLineColor(kGreen-3);
      lin->DrawLine( B->GetRefTime(),-800, B->GetRefTime(),100);
    }
    if(C!=NULL) {
      fQuadratic->SetParameter(0,C->GetQFa());
      fQuadratic->SetParameter(1,C->GetQFb());
      fQuadratic->SetParameter(2,C->GetQFc());
      fQuadratic->SetRange( C->GetTime()-4, C->GetTime()+4 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
      tex->SetTextColor(kRed-3);
      tex->DrawLatexNDC(0.65, 0.20, Form("AvgEne  %.2f",C->GetAvgEne()) );
      lin->SetLineColor(kRed-3);
      lin->DrawLine( C->GetRefTime(),-800, C->GetRefTime(),100);
    }
  }
  if(showfit==1) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,A->GetLFa());
      fQuadratic->SetParameter(2,A->GetLFb());
      fQuadratic->SetRange( A->GetTime()-0.3, A->GetTime()+0.3 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,B->GetLFa());
      fQuadratic->SetParameter(2,B->GetLFb());
      fQuadratic->SetRange( B->GetTime()-0.3, B->GetTime()+0.3 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
    if(C!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,C->GetLFa());
      fQuadratic->SetParameter(2,C->GetLFb());
      fQuadratic->SetRange( C->GetTime()-0.3, C->GetTime()+0.3 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
  }
  delete tex;
  delete lin;
  return ret;
}
//=================================
void testbeam::Init() {
  fList = new TList();
  fList->SetName( "output" );
  fList->SetOwner();
  fEvents = new TH1D( "fEvents","fEvents",10,-0.5,9.5 );
  fEvents->GetXaxis()->SetBinLabel(1,"raw");
  fEvents->GetXaxis()->SetBinLabel(2,"ntrks=1");
  fEvents->GetXaxis()->SetBinLabel(3,"passed both mcp");
  fEvents->GetXaxis()->SetBinLabel(4,"acceptance");
  fList->Add( fEvents );

  //--------------
  TList *listTRK = new TList();
  listTRK->SetName("trk");
  listTRK->SetOwner();
  fTRKchi2 = new TH1D( "fTRKchi2","fTRKchi2",100,0,100 );
  listTRK->Add( fTRKchi2 );
  fTRKxy0 = new TH2D( "fTRKxy0","fTRKxy0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listTRK->Add( fTRKxy0 );
  fList->Add( listTRK );
  //--------------
  TList *listMCP = new TList();
  listMCP->SetName("mcp");
  listMCP->SetOwner();
  fMCP0 = new mcp("MCP0");
  listMCP->Add( fMCP0->GetList() );
  fMCP1 = new mcp("MCP1");
  listMCP->Add( fMCP1->GetList() );
  fTRKxyMCP0 = new TH2D( "fTRKxyMCP0","fTRKxyMCP0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP->Add( fTRKxyMCP0 );
  fTRKxyMCP1 = new TH2D( "fTRKxyMCP1","fTRKxyMCP1;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP->Add( fTRKxyMCP1 );
  fEvent1DisplayMCP = new TCanvas("ED1_MCP");
  fEvent1DisplayMCP->Divide(5,5);
  listMCP->Add( fEvent1DisplayMCP );
  fEvent2DisplayMCP = new TCanvas("ED2_MCP");
  fEvent2DisplayMCP->Divide(5,5);
  listMCP->Add( fEvent2DisplayMCP );
  fList->Add( listMCP );
  //--------------
  TList *listMCPI = new TList();
  listMCPI->SetName("mcpI");
  listMCPI->SetOwner();
  fTRKxyMCPI = new TH2D( "fTRKxyMCPI","fTRKxyMCPI;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCPI->Add( fTRKxyMCPI );
  fMCPdiffpos = new TH2D( "fMCPdiffpos","fMCPdiffpos;Distance  from  center  [mm];MCP0 - MCP1  [ns]", 100,0,8, 100,-0.3,+0);
  listMCPI->Add( fMCPdiffpos );
  fMCPcorrE = new TH2D( "fMCPcorrE","fMCPcorrE;MCP0  [mV];MCP1  [mV]", 100,0,520, 100,0,400 );
  listMCPI->Add( fMCPcorrE );
  fMCPcorr = new TH2D( "fMCPcorrT","fMCPcorrT;MCP0  [ns];MCP1  [ns]", 100,96,120, 100,96,120 );
  listMCPI->Add( fMCPcorr );
  fMCPdiff = new TH1D( "fMCPdiff","fMCPdiff;MCP0 - MCP1  [ns]", 200,-0.3,+0);
  listMCPI->Add( fMCPdiff );
  fMCPta0 = new TH2D( "fMCPta0","fMCPta0;MCP0 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,520, 100,-0.3,0 );
  listMCPI->Add( fMCPta0 );
  fMCPta1 = new TH2D( "fMCPta1","fMCPta1;MCP1 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,400, 100,-0.3,0 );
  listMCPI->Add( fMCPta1 );
  fList->Add( listMCPI );
  //--------------
  TList *listEnergy = new TList();
  listEnergy->SetName("energy");
  listEnergy->SetOwner();
  fEnergyL = new energy("ENERGYL");
  listEnergy->Add( fEnergyL->GetList() );
  fEnergyR = new energy("ENERGYR");
  listEnergy->Add( fEnergyR->GetList() );
  fTRKxyEnergyL = new TH2D( "fTRKxyEnergyL","fTRKxyEnergyL;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyL );
  fTRKxyEnergyR = new TH2D( "fTRKxyEnergyR","fTRKxyEnergyR;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyR );
  fEvent4DisplayEnergy = new TCanvas("ED4_Energy");
  fEvent4DisplayEnergy->Divide(5,5);
  listEnergy->Add( fEvent4DisplayEnergy );
  fTRKxyEnergyLS = new TH2D( "fTRKxyEnergyLS","fTRKxyEnergyLS;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyLS );
  fTRKxyEnergyRS = new TH2D( "fTRKxyEnergyRS","fTRKxyEnergyRS;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyRS );
  fEnergyLR = new TH2D( "fEnergyLR","fEnergyLR;[mV];[mV]", 100,0,800, 100,0,800 );
  listEnergy->Add( fEnergyLR );
  fEnergyLX = new TH2D( "fEnergyLX","fEnergyLX;[mm];[mV]", 100,10,20, 100,0,800 );
  listEnergy->Add( fEnergyLX );
  fEnergyRX = new TH2D( "fEnergyRX","fEnergyRX;[mm];[mV]", 100,10,20, 100,0,800 );
  listEnergy->Add( fEnergyRX );
  fEnergyAvgLR = new TH2D( "fEnergyAvgLR","fEnergyAvgLR;[mV];[mV]", 100,0,5000, 100,0,5000 );
  listEnergy->Add( fEnergyAvgLR );
  fList->Add( listEnergy );
  //--------------
  TList *listTiming = new TList();
  listTiming->SetName("timing");
  listTiming->SetOwner();
  fTimingL = new timing("TIMINGL");
  listTiming->Add( fTimingL->GetList() );
  fTimingR = new timing("TIMINGR");
  listTiming->Add( fTimingR->GetList() );
  fEvent5DisplayTiming = new TCanvas("ED5_Timing");
  fEvent5DisplayTiming->Divide(5,5);
  listTiming->Add( fEvent5DisplayTiming );
  fTimingClipLineL = new timing("TIMINGCLIPLINEL");
  listTiming->Add( fTimingClipLineL->GetList() );
  fTimingClipLineR = new timing("TIMINGCLIPLINER");
  listTiming->Add( fTimingClipLineR->GetList() );
  fEvent5DisplayTimingCL = new TCanvas("ED5_TimingClipLine");
  fEvent5DisplayTimingCL->Divide(5,5);
  listTiming->Add( fEvent5DisplayTimingCL );
  for(int thr=0; thr!=5; ++thr) {
    TList *list = new TList();
    list->SetName( Form("thr_%d",thr) );
    list->SetOwner();
    fTimingNCvsCL[thr] = new TH2D(Form("fTimingNCvsCL_%d",thr),
                                  Form("fTimingNCvsCL_%d;[ns];[ns]",thr), 100,-3,+3, 100,-3,+3 );
    list->Add( fTimingNCvsCL[thr] );
    fTimingNCvsCR[thr] = new TH2D(Form("fTimingNCvsCR_%d",thr),
                                  Form("fTimingNCvsCR_%d;[ns];[ns]",thr), 100,-3,+3, 100,-3,+3 );
    list->Add( fTimingNCvsCR[thr] );
    fTimingLE[thr] = new TH2D(Form("fTimingLE_%d",thr),
                              Form("fTimingLE_%d;[mV];[ns]",thr), 100,0,700, 100,-3,+2 );
    list->Add( fTimingLE[thr] );
    fTimingRE[thr] = new TH2D(Form("fTimingRE_%d",thr),
                              Form("fTimingRE_%d;[mV];[ns]",thr), 100,0,700, 100,-3,+2 );
    list->Add( fTimingRE[thr] );
    fTimingLEC[thr] = new TH2D(Form("fTimingLEC_%d",thr),
                               Form("fTimingLEC_%d;[mV];[ns]",thr), 100,0,700, 100,-1,+1 );
    list->Add( fTimingLEC[thr] );
    fTimingREC[thr] = new TH2D(Form("fTimingREC_%d",thr),
                               Form("fTimingREC_%d;[mV];[ns]",thr), 100,0,700, 100,-1,+1 );
    list->Add( fTimingREC[thr] );
    fTimingLR[thr] = new TH2D(Form("fTimingLR_%d",thr),
                              Form("fTimingLR_%d;LEFT  [ns];RIGHT  [ns]",thr), 100,-0.7,+0.7, 100,-0.7,+0.7 );
    list->Add( fTimingLR[thr] );
    fTimingX[thr] = new TH2D(Form("fTimingX_%d",thr),
                             Form("fTimingX_%d;Position  [mm];LEFT - RIGHT  [ns]",thr), 100,8,24, 100,-0.7,+0.7 );
    list->Add( fTimingX[thr] );
    fTimingXC[thr] = new TH2D(Form("fTimingXC_%d",thr),
                              Form("fTimingXC_%d;Position  [mm];CORRECTED  [ns]",thr), 100,8,24, 100,-0.7,+0.7 );
    list->Add( fTimingXC[thr] );
    fTiming[thr] = new TH1D(Form("fTiming_%d",thr),
                            Form("fTiming_%d;LEFT - RIGHT  [ns]",thr), 100,-0.7,+0.7 );
    list->Add( fTiming[thr] );
    fTimingX_P[thr] = new TH2D(Form("fTimingX_P_%d",thr),
                               Form("fTimingX_P_%d;Position  [mm];LEFT + RIGHT  [ns]",thr), 100,8,24, 100,-0.7,+0.7 );
    list->Add( fTimingX_P[thr] );
    fTimingXC_P[thr] = new TH2D(Form("fTimingXC_P_%d",thr),
                                Form("fTimingXC_P_%d;Position  [mm];CORRECTED  [ns]",thr), 100,8,24, 100,-0.7,+0.7 );
    list->Add( fTimingXC_P[thr] );
    fTiming_P[thr] = new TH1D(Form("fTiming_P_%d",thr),
                              Form("fTiming_P_%d;LEFT + RIGHT  [ns]",thr), 100,-0.7,+0.7 );
    list->Add( fTiming_P[thr] );
    listTiming->Add( list );
  }
  fList->Add( listTiming );
  //--------------
  fQuadratic = new TF1("fQuadratic","[0]*x*x+[1]*x+[2]");
}
//=================================
void testbeam::CalibrateTime( double A[5], double B[5], double C[5], double X[5], double Y[5], double Z[5] ) {
  {
    double L0[5] = { -1.65243, -1.54752, -1.33236, -1.21982, -1.05165 };
    double L1[5] = { 35.7638, 41.7519, 54.8656, 65.3966, 89.4069 };
    double L2[5] = { -5.24402, -6.8977, -10.9248, -12.9603, -16.0445 };
    double R0[5] = { -1.32913, -1.21708, -1.03013, -0.927725, -0.768945 };
    double R1[5] = { 32.4291, 36.7979, 52.9871, 64.8151, 89.3923 };
    double R2[5] = { -28.8281, -33.1812, -40.8888, -46.1654, -55.0166 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  
  if(fRun==44734) {
    double L0[5] = { -1.65243, -1.54752, -1.33236, -1.21982, -1.05165 };
    double L1[5] = { 35.7638, 41.7519, 54.8656, 65.3966, 89.4069 };
    double L2[5] = { -5.24402, -6.8977, -10.9248, -12.9603, -16.0445 };
    double R0[5] = { -1.32913, -1.21708, -1.03013, -0.927725, -0.768945 };
    double R1[5] = { 32.4291, 36.7979, 52.9871, 64.8151, 89.3923 };
    double R2[5] = { -28.8281, -33.1812, -40.8888, -46.1654, -55.0166 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44733) {
    double L0[5] = { -1.64547, -1.54668, -1.34304, -1.23095, -1.05994 };
    double L1[5] = { 33.8142, 40.807, 56.0258, 67.0524, 91.1824 };
    double L2[5] = { -11.3535, -11.9435, -14.8081, -16.5593, -19.6005 };
    double R0[5] = { -1.38046, -1.27654, -1.08595, -0.982456, -0.818744 };
    double R1[5] = { 33.1155, 38.7656, 56.9024, 68.9559, 92.6243 };
    double R2[5] = { -26.3077, -29.5674, -38.9338, -44.2278, -53.9192 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44731) {
    double L0[5] = { -1.64541, -1.53755, -1.33519, -1.22158, -1.05552 };
    double L1[5] = { 29.7185, 35.4395, 51.5636, 63.0823, 88.8249 };
    double L2[5] = { -9.08292, -10.3813, -12.6482, -14.4194, -16.588 };
    double R0[5] = { -1.4289, -1.34166, -1.13512, -1.03496, -0.875409 };
    double R1[5] = { 43.1704, 51.5544, 64.2704, 77.3883, 102.482 };
    double R2[5] = { -26.1771, -28.042, -37.7475, -42.0851, -50.6637 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44727) {
    double L0[5] = { -1.59093, -1.48553, -1.28018, -1.17286, -1.01595 };
    double L1[5] = { 18.7316, 26.8965, 42.8847, 55.3067, 85.1561 };
    double L2[5] = { -12.3549, -15.25, -17.9131, -19.4332, -22.2648 };
    double R0[5] = { -1.37605, -1.33322, -1.13244, -1.03169, -0.877962 };
    double R1[5] = { 26.5491, 45.0795, 59.0508, 71.7572, 98.4953 };
    double R2[5] = { -29.9394, -22.3742, -29.6799, -33.6742, -40.9186 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44725) {
    double L0[5] = { -1.64796, -1.53949, -1.35431, -1.24108, -1.06801 };
    double L1[5] = { 135.2058, 39.84, 57.5542, 69.6659, 95.5561 };
    double L2[5] = { -11.8458, -12.7539, -10.0198, -12.3995, -16.0901 };
    double R0[5] = { -1.44535, -1.2948, -1.10349, -1.00196, -0.84767 };
    double R1[5] = { 48.4317, 45.3528, 63.1678, 75.8566, 102.379 };
    double R2[5] = { -26.9753, -40.0936, -44.3937, -48.8061, -57.0068 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44730) {
    double L0[5] = { -1.61956, -1.52283, -1.30534, -1.19561, -1.02187 };
    double L1[5] = { 27.5273, 34.4868, 47.0543, 59.3114, 86.4421 };
    double L2[5] = { -12.399, -12.5952, -15.4158, -16.932, -20.0321 };
    double R0[5] = { -1.44575, -1.34475, -1.13881, -1.03025, -0.865416 };
    double R1[5] = { 48.2708, 52.0607, 67.2919, 78.9337, 104.814 };
    double R2[5] = { -27.9649, -30.4928, -40.8568, -46.7984, -56.4907 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44728) {
    double L0[5] = { -1.65827, -1.53412, -1.34304, -1.23251, -1.06389 };
    double L1[5] = { 34.4558, 36.7775, 56.795, 69.3971, 98.1578 };
    double L2[5] = { -7.58909, -9.42325, -12.3451, -14.4664, -17.3687 };
    double R0[5] = { -1.43455, -1.3303, -1.12386, -1.02396, -0.869391 };
    double R1[5] = { 40.0935, 46.7001, 58.5319, 72.0022, 101.853 };
    double R2[5] = { -26.9647, -30.5536, -40.6746, -45.7395, -55.3549 };
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
  if(fRun==44732) {
    double L0[5] = { -1.80714, -1.6094, -1.33671, -1.22502, -1.0444};
    double L1[5] = { 67.9812, 54.8817, 54.8834, 67.8156, 94.8304};
    double L2[5] = { -4.10835, -9.38831, -14.1725, -15.3453, -18.4579};
    double R0[5] = { -1.44976, -1.3108, -1.13305, -1.01387, -0.848727};
    double R1[5] = { 48.6344, 49.1138, 68.8025, 78.1488, 105.9};
    double R2[5] = { -28.6898, -36.68, -44.037, -50.8865, -61.1966};
    for(int i=0; i!=5; ++i) {
      A[i] = L0[i]; B[i] = L1[i]; C[i] = L2[i];
      X[i] = R0[i]; Y[i] = R1[i]; Z[i] = R2[i];
    }
  }
}
//=================================
void testbeam::Terminate() {
  //std::cout << "Terminate() was called" << std::endl;
  TFile *outputfile = new TFile( fOutputFileName.Data(), "RECREATE" );
  fList->Write( fList->GetName(), TObject::kSingleKey );
  outputfile->Close();
  std::cout << "File " << fOutputFileName.Data() << " was saved." << std::endl;

}
//=================================
void testbeam::Process() {
  if(!fTree) return;
  Init();
  fTree->Print();
  int event;
  unsigned short tc[2]; // trigger counter bin
  float times[2][1024]; // calibrated time
  float channel[18][1024]; // calibrated input (in V)
  float xIntercept;
  float yIntercept;
  float xSlope;
  float ySlope;
  float chi2;
  int ntracks;
  
  fTree->SetBranchAddress("event", &event);
  fTree->SetBranchAddress("tc", tc);
  fTree->SetBranchAddress("channel", channel);
  fTree->SetBranchAddress("times", times);
  fTree->SetBranchAddress("xIntercept", &xIntercept);
  fTree->SetBranchAddress("yIntercept", &yIntercept);
  fTree->SetBranchAddress("xSlope", &xSlope);
  fTree->SetBranchAddress("ySlope", &ySlope);
  fTree->SetBranchAddress("chi2", &chi2);
  fTree->SetBranchAddress("ntracks", &ntracks);

  Long64_t iEvent = 0;
  Long64_t iEvent1 = 0;
  Long64_t iEvent2 = 0;
  Long64_t iEvent4 = 0;
  Long64_t iEvent5 = 0;
  Long64_t nEvents = fTree->GetEntries();
  //nEvents = 3;
  TGraph *tmpMCP0[25];
  TGraph *tmpMCP1[25];
  for(;iEvent!=nEvents; ++iEvent) {
    fTree->GetEntry( iEvent );
    fEvents->Fill( 0 );
    if(iEvent%2000==0) std::cout << "Events read so far: " << iEvent << std::endl;

    if(ntracks!=1) continue;
    fEvents->Fill( 1 );
    
    double xproj = xIntercept*1e-3;
    double yproj = yIntercept*1e-3;
    fTRKxy0->Fill( xproj, yproj );
    fTRKchi2->Fill( chi2 );

    fMCP0->Fill( times[0], channel[0] ); // group 0 channel 0
    fMCP1->Fill( times[0], channel[1] ); // group 0 channel 1
    fEnergyL->Fill( times[1], channel[11] );
    fEnergyR->Fill( times[1], channel[12] );
    fTimingL->Fill( times[0], channel[2]  );
    fTimingR->Fill( times[0], channel[3]  );
    fTimingClipLineL->FillClipLine( times[0], channel[2], 100 ); // 100 channel delay and invert
    fTimingClipLineR->FillClipLine( times[0], channel[3], 100 ); // which is about 20 ns

    if(iEvent1<25) {
      fEvent1DisplayMCP->cd(iEvent1+1);
      TGraph *ret = DisplayWave(fMCP0, fMCP1, NULL, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent1++;

    bool passMCP0 = fMCP0->Process(-100); //100mV cut
    bool passMCP1 = fMCP1->Process(-77); //77mV cut
    fEnergyL->SetRefTime( fMCP0->GetTime() + 35 );
    fEnergyR->SetRefTime( fMCP0->GetTime() + 18 );
    bool passENEL = fEnergyL->Process();
    bool passENER = fEnergyR->Process();

    if( passMCP0 ) fTRKxyMCP0->Fill( xproj, yproj );
    if( passMCP1 ) fTRKxyMCP1->Fill( xproj, yproj );
    if( passENEL ) fTRKxyEnergyL->Fill( xproj, yproj );
    if( passENER ) fTRKxyEnergyR->Fill( xproj, yproj );

    if( !passMCP0 ) continue;
    if(-fMCP0->GetAmplitude()>420) continue;

    /*
    if( !passMCP1 ) continue;
    if(-fMCP1->GetAmplitude()>323) continue;
    fEvents->Fill( 2 );

    if(iEvent2<25) {
      fEvent2DisplayMCP->cd(iEvent2+1);
      TGraph *ret = DisplayWave(fMCP0, fMCP1, NULL, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent2++;

    fTRKxyMCPI->Fill( xproj, yproj );

    double mcpCenterIx = 15.6; // valid for run 44718 - 34
    double mcpCenterIy = 25.1; // valid for run 44718 - 34
    double rad = 0;
    rad = TMath::Power(xproj-mcpCenterIx,2);
    rad += TMath::Power(yproj-mcpCenterIy,2);
    rad = TMath::Sqrt( rad );
    fMCPdiffpos->Fill( rad, fMCP0->GetTime() - fMCP1->GetTime() );
    */
    
    
    if( passENEL ) fTRKxyEnergyLS->Fill( xproj, yproj );
    if( passENER ) fTRKxyEnergyRS->Fill( xproj, yproj );

    /*
    if(xproj < mcpCenterIx - 2) continue;
    if(xproj > mcpCenterIx + 2) continue;
    if(yproj < mcpCenterIy - 2) continue;
    if(yproj > mcpCenterIy + 2) continue;
    fEvents->Fill( 3 );
*/
     
    fMCPcorrE->Fill( -fMCP0->GetAmplitude(), -fMCP1->GetAmplitude() );
    fMCPcorr->Fill( fMCP0->GetTime(), fMCP1->GetTime() );
    fMCPdiff->Fill( fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta0->Fill( -fMCP0->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta1->Fill( -fMCP1->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );

    if( !passENEL ) continue;
    if( !passENER ) continue;
    if( fEnergyL->GetAvgEne()<50 ) continue;
    if( fEnergyR->GetAvgEne()<80 ) continue;

    fEvents->Fill( 4 );
    if(iEvent4<25) {
      fEvent4DisplayEnergy->cd(iEvent4+1);
      TGraph *ret = DisplayWave(fEnergyL, fEnergyR, NULL, 2);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent4++;

    
    
    fEnergyLR->Fill( -fEnergyL->GetAmplitude(), -fEnergyR->GetAmplitude() );
    fEnergyAvgLR->Fill( fEnergyL->GetAvgEne(), fEnergyR->GetAvgEne() );
    fEnergyLX->Fill( xproj, -fEnergyL->GetAmplitude() );
    fEnergyRX->Fill( xproj, -fEnergyR->GetAmplitude() );

    fTimingL->SetThreshold(-100);
    fTimingR->SetThreshold(-100);
    bool passTIML = fTimingL->Process();
    bool passTIMR = fTimingR->Process();
    fTimingClipLineL->SetBaselineLimits(100,200);
    fTimingClipLineR->SetBaselineLimits(100,200);
    fTimingClipLineL->SetThreshold(-100);
    fTimingClipLineR->SetThreshold(-100);
    fTimingClipLineL->Process();
    fTimingClipLineR->Process();
    fEvents->Fill( 5 );
    if(iEvent5<25) {
      fEvent5DisplayTiming->cd(iEvent5+1);
      TGraph *ret = DisplayWave(fTimingL, fTimingR, NULL, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
      fEvent5DisplayTimingCL->cd(iEvent5+1);
      ret = DisplayWave(fTimingClipLineL, fTimingClipLineR, NULL, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent5++;

    double thresholds[5] = {-5,-10,-30,-50,-100};
    double L_P0[5];
    double L_P1[5];
    double L_P2[5];
    double R_P0[5];
    double R_P1[5];
    double R_P2[5];
    CalibrateTime( L_P0, L_P1, L_P2, R_P0, R_P1, R_P2 );
    
    //BOOKEEP ONE
    fTimingL->RefTime( thresholds[0], true );
    fTimingR->RefTime( thresholds[0], true );
    fTimingClipLineL->RefTime( thresholds[0], true );
    fTimingClipLineR->RefTime( thresholds[0], true );
    //
    for(int thr=0; thr!=5;++thr) {
      if( !fTimingL->RefTime( thresholds[thr], false ) ) continue;
      if( !fTimingR->RefTime( thresholds[thr], false ) ) continue;
      if( !fTimingClipLineL->RefTime( thresholds[thr], false ) ) continue;
      if( !fTimingClipLineR->RefTime( thresholds[thr], false ) ) continue;
      double sipmtimeLNC = fTimingL->GetRefTime() - fMCP0->GetTime() + 38.0;
      double sipmtimeRNC = fTimingR->GetRefTime() - fMCP0->GetTime() + 38.5;
      double sipmtimeL = fTimingClipLineL->GetRefTime() - fMCP0->GetTime() + 38.0;
      double sipmtimeR = fTimingClipLineR->GetRefTime() - fMCP0->GetTime() + 38.5;
      fTimingNCvsCL[thr]->Fill( sipmtimeLNC, sipmtimeL );
      fTimingNCvsCR[thr]->Fill( sipmtimeRNC, sipmtimeR );
      
      fTimingLE[thr]->Fill( fEnergyL->GetAvgEne(), sipmtimeL );
      fTimingRE[thr]->Fill( fEnergyR->GetAvgEne(), sipmtimeR );

      sipmtimeL -= L_P0[thr];
      double avgeneL = fEnergyL->GetAvgEne();
      double avgeneR = fEnergyR->GetAvgEne();
      //if( fEnergyL->GetAvgEne() < 450 )
        sipmtimeL -= L_P1[thr]/avgeneL + L_P2[thr]*TMath::Power(avgeneL,9)*1e-27;
      sipmtimeR -= R_P0[thr];
      //if( fEnergyR->GetAvgEne() < 450 )
        sipmtimeR -= R_P1[thr]/avgeneR + R_P2[thr]*TMath::Power(avgeneR,9)*1e-27;

      if( TMath::Abs(sipmtimeL) > 2 ) continue;
      if( TMath::Abs(sipmtimeR) > 2 ) continue;

      fTimingLEC[thr]->Fill( fEnergyL->GetAvgEne(), sipmtimeL );
      fTimingREC[thr]->Fill( fEnergyR->GetAvgEne(), sipmtimeR );
      fTimingLR[thr]->Fill( sipmtimeL, sipmtimeR );

      double delta = sipmtimeL-sipmtimeR;
      fTimingX[thr]->Fill( xproj, delta );
      delta -=  0.255928-0.0160116*xproj;
      fTimingXC[thr]->Fill( xproj, delta );
      fTiming[thr]->Fill( delta );

      double plus = 0.5*(sipmtimeL+sipmtimeR);
      fTimingX_P[thr]->Fill( xproj, plus );
      //delta -=  0.255928-0.0160116*xproj;
      fTimingXC_P[thr]->Fill( xproj, plus );
      fTiming_P[thr]->Fill( plus );
    }
  }
  fTRKxyMCP0->Divide( fTRKxy0 );
  fTRKxyMCP1->Divide( fTRKxy0 );
  fTRKxyMCPI->Divide( fTRKxy0 );
  Terminate();
  fTree = NULL; //don't call me again
}
