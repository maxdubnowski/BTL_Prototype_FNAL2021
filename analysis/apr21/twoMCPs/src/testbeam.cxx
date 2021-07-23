#include "testbeam.h"
#include <TFile.h>
#include <TList.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <iostream>
#include <TMath.h>

//=================================
testbeam::testbeam() {
  fList = NULL;
  fTree = NULL;
  fMCP0 = NULL;
  fMCP1 = NULL;
  fEventDisplayMCP=NULL;
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
void testbeam::Init() {
  fList = new TList();
  fList->SetName( "output" );
  fList->SetOwner();
  fEventDisplayMCP = new TCanvas("ED_MCP");
  fEventDisplayMCP->Divide(5,5);
  fList->Add( fEventDisplayMCP );
  fEvents = new TH1D( "fEvents","fEvents",10,-0.5,9.5 );
  fList->Add( fEvents );
  fTRKchi2 = new TH1D( "fTRKchi2","fTRKchi2",100,0,100 );
  fList->Add( fTRKchi2 );
  fTRKxy0 = new TH2D( "fTRKxy0","fTRKxy0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  fList->Add( fTRKxy0 );
  fTRKxyMCP0 = new TH2D( "fTRKxyMCP0","fTRKxyMCP0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  fList->Add( fTRKxyMCP0 );
  fTRKxyMCP1 = new TH2D( "fTRKxyMCP1","fTRKxyMCP1;[mm];[mm]", 100,-10,50, 100, -10,50 );
  fList->Add( fTRKxyMCP1 );
  fTRKxyMCPI = new TH2D( "fTRKxyMCPI","fTRKxyMCPI;[mm];[mm]", 100,-10,50, 100, -10,50 );
  fList->Add( fTRKxyMCPI );
  fMCPcorr = new TH2D( "fMCPcorr","fMCPcorr;MCP0  [ns];MCP1  [ns]", 100,0,200, 100,0,200 );
  fList->Add( fMCPcorr );
  fMCPdiff = new TH1D( "fMCPdiff","fMCPdiff;MCP0 - MCP1  [ns]", 200,-0.3,+0.3);
  fList->Add( fMCPdiff );
  fMCPta0 = new TH2D( "fMCPta0","fMCPta0;MCP0 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,800, 100,-0.3,+0.3 );
  fList->Add( fMCPta0 );
  fMCPta1 = new TH2D( "fMCPta1","fMCPta0;MCP1 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,800, 100,-0.3,+0.3 );
  fList->Add( fMCPta1 );
  //--------------
  fMCP0 = new mcp("MCP0");
  fList->Add( fMCP0->GetList() );
  fMCP1 = new mcp("MCP1");
  fList->Add( fMCP1->GetList() );
  //--------------
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
  Long64_t nEvents = fTree->GetEntries();
  //nEvents = 3;
  TGraph *tmpMCP0[25];
  TGraph *tmpMCP1[25];
  for(;iEvent!=nEvents; ++iEvent) {
    fTree->GetEntry( iEvent );
    fEvents->Fill( 0 );
    if(iEvent%500==0) std::cout << "Events read so far: " << iEvent << std::endl;

    if(ntracks!=1) continue;
    fEvents->Fill( 1 );
    
    double xproj = xIntercept*1e-3;
    double yproj = yIntercept*1e-3;
    fTRKxy0->Fill( xproj, yproj );

    fTRKchi2->Fill( chi2 );

    fMCP0->Fill( times[0], channel[0] ); // group 0 channel 0
    fMCP1->Fill( times[0], channel[1] ); // group 0 channel 1
    bool passMCP0 =  fMCP0->Process();
    bool passMCP1 =  fMCP1->Process();

    if(iEvent1<25) {
      fEventDisplayMCP->cd(iEvent1+1);
      tmpMCP0[iEvent1] = new TGraph(1024,fMCP0->GetX(),fMCP0->GetY());
      tmpMCP1[iEvent1] = new TGraph(1024,fMCP1->GetX(),fMCP1->GetY());
      tmpMCP0[iEvent1]->SetLineColor(kBlue-3);
      tmpMCP0[iEvent1]->SetMarkerColor(kBlue-3);
      tmpMCP1[iEvent1]->SetLineColor(kRed-3);
      tmpMCP1[iEvent1]->SetMarkerColor(kRed-3);
      tmpMCP0[iEvent1]->Draw("A*L");
      tmpMCP1[iEvent1]->Draw("*LSAME");
    }
    iEvent1++;

    if( passMCP0 ) fTRKxyMCP0->Fill( xproj, yproj );
    if( passMCP1 ) fTRKxyMCP1->Fill( xproj, yproj );

    if( !passMCP0 ) continue;
    if( !passMCP1 ) continue;
    fEvents->Fill( 2 );

    fTRKxyMCPI->Fill( xproj, yproj );

    double mcpCenterIx = 15.6; // valid for run 44718 - 34
    double mcpCenterIy = 25.1; // valid for run 44718 - 34

    if(xproj < mcpCenterIx - 2) continue;
    if(xproj > mcpCenterIx + 2) continue;
    if(yproj < mcpCenterIy - 2) continue;
    if(yproj > mcpCenterIy + 2) continue;
    
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
