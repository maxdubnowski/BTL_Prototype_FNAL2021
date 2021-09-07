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

//=================================
testbeam::testbeam() {
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
  fEvent1DisplayMCP = new TCanvas("ED1_MCP");
  fEvent1DisplayMCP->Divide(5,5);
  fList->Add( fEvent1DisplayMCP );
  fEvent2DisplayMCP = new TCanvas("ED2_MCP");
  fEvent2DisplayMCP->Divide(5,5);
  fList->Add( fEvent2DisplayMCP );

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
  TList *listMCP0 = new TList();
  listMCP0->SetName("mcp0");
  listMCP0->SetOwner();
  fMCP0 = new mcp("MCP0");
  listMCP0->Add( fMCP0->GetList() );
  fTRKxyMCP0 = new TH2D( "fTRKxyMCP0","fTRKxyMCP0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP0->Add( fTRKxyMCP0 );
  fList->Add( listMCP0 );
  //--------------
  TList *listMCP1 = new TList();
  listMCP1->SetName("mcp1");
  listMCP1->SetOwner();
  fMCP1 = new mcp("MCP1");
  listMCP1->Add( fMCP1->GetList() );
  fTRKxyMCP1 = new TH2D( "fTRKxyMCP1","fTRKxyMCP1;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP1->Add( fTRKxyMCP1 );
  fList->Add( listMCP1 );
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
  fQuadratic = new TF1("fQuadratic","[0]*x*x+[1]*x+[2]");
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
   
    //fMCP0->ShiftWave( channel[10] ); // CMN channel 9
    //fMCP1->ShiftWave( channel[10] ); // CMN channel 9
    
    if(iEvent1<25) {
      fEvent1DisplayMCP->cd(iEvent1+1);
      TGraph *ret = DisplayWave(fMCP0, fMCP1, NULL, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent1++;

    bool passMCP0 =  fMCP0->Process(-100); //100mV cut
    bool passMCP1 =  fMCP1->Process(-77); //77mV cut

    if( passMCP0 ) fTRKxyMCP0->Fill( xproj, yproj );
    if( passMCP1 ) fTRKxyMCP1->Fill( xproj, yproj );

    if( !passMCP0 ) continue;
    if( !passMCP1 ) continue;
    if(-fMCP0->GetAmplitude()>420) continue;
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

    if(xproj < mcpCenterIx - 2) continue;
    if(xproj > mcpCenterIx + 2) continue;
    if(yproj < mcpCenterIy - 2) continue;
    if(yproj > mcpCenterIy + 2) continue;
    fEvents->Fill( 3 );

    fMCPcorrE->Fill( -fMCP0->GetAmplitude(), -fMCP1->GetAmplitude() );
    fMCPcorr->Fill( fMCP0->GetTime(), fMCP1->GetTime() );
    fMCPdiff->Fill( fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta0->Fill( -fMCP0->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta1->Fill( -fMCP1->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );

  }
  fTRKxyMCP0->Divide( fTRKxy0 );
  fTRKxyMCP1->Divide( fTRKxy0 );
  fTRKxyMCPI->Divide( fTRKxy0 );
  Terminate();
  fTree = NULL; //don't call me again
}
