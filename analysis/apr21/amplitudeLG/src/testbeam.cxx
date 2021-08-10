#include "testbeam.h"
#include <TFile.h>
#include <TList.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <iostream>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>

#include "mcp.h"
#include "energy.h"
#include "timing.h"

//=================================
testbeam::testbeam() {
  fList = NULL;
  fTree = NULL;
  fMCP0 = NULL;
  fEventDisplayMCP=NULL;
  fEvents = NULL;
  fTRKchi2 = NULL;
  fTRKxy0 = NULL;
  fTRKxyMCP0 = NULL;
  for(int i=0; i!=8; ++i) {
    fTRKxyEnergy[i] = NULL;
  }
  for(int i=0; i!=4; ++i) {
    fEventDisplayEnergy[i]=NULL;
  }
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
void testbeam::DisplayWave(DRSWAVE *A, DRSWAVE *B, int showfit) {
  TGraph *a, *b;
  double aplot = 999;
  if(A!=NULL) {
    a = new TGraph(1024,A->GetX(),A->GetY());
    a->SetLineColor(kBlue-3);
    a->SetMarkerColor(kBlue-3);
    a->SetMarkerStyle(24);
    aplot = a->GetMinimum();
  }
  double bplot = 999;
  if(B!=NULL) {
    b = new TGraph(1024,B->GetX(),B->GetY());
    b->SetLineColor(kRed-3);
    b->SetMarkerColor(kRed-3);
    b->SetMarkerStyle(24);
    bplot = b->GetMinimum();
  }
  if(aplot>0 && bplot>0) return;
  if(aplot>bplot)
    b->Draw("AP");
  else
    a->Draw("AP");
  if(A!=NULL) a->Draw("PSAME");
  if(B!=NULL) b->Draw("PSAME");
  if(showfit==2) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,A->GetQFa());
      fQuadratic->SetParameter(1,A->GetQFb());
      fQuadratic->SetParameter(2,A->GetQFc());
      fQuadratic->SetRange( A->GetTime()-10, A->GetTime()+10 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,B->GetQFa());
      fQuadratic->SetParameter(1,B->GetQFb());
      fQuadratic->SetParameter(2,B->GetQFc());
      fQuadratic->SetRange( B->GetTime()-10, B->GetTime()+10 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
  }
  if(showfit==1) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,A->GetLFa());
      fQuadratic->SetParameter(2,A->GetLFb());
      fQuadratic->SetRange( A->GetTime()-1, A->GetTime()+1 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,B->GetLFa());
      fQuadratic->SetParameter(2,B->GetLFb());
      fQuadratic->SetRange( B->GetTime()-1, B->GetTime()+1 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
  }
}
//=================================
void testbeam::Init() {
  fList = new TList();
  fList->SetName( "output" );
  fList->SetOwner();
  TList *listTRK = new TList();
  listTRK->SetName("tracking");
  listTRK->SetOwner();
  fEvents = new TH1D( "fEvents","fEvents",10,-0.5,9.5 );
  fList->Add( fEvents );
  fTRKchi2 = new TH1D( "fTRKchi2","fTRKchi2",100,0,100 );
  listTRK->Add( fTRKchi2 );
  fTRKxy0 = new TH2D( "fTRKxy0","fTRKxy0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listTRK->Add( fTRKxy0 );
  fList->Add(listTRK);
  //--------------
  TList *listMCP = new TList();
  listMCP->SetName("mcp");
  listMCP->SetOwner();
  fMCP0 = new mcp("MCP0");
  listMCP->Add( fMCP0->GetList() );
  fEventDisplayMCP = new TCanvas("ED_MCP");
  fEventDisplayMCP->Divide(5,5);
  listMCP->Add( fEventDisplayMCP );
  fTRKxyMCP0 = new TH2D( "fTRKxyMCP0","fTRKxyMCP0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP->Add( fTRKxyMCP0 );
  fList->Add(listMCP);
  //--------------
  TList *listENE = new TList();
  listENE->SetName("energy");
  listENE->SetOwner();
  for(int i=0; i!=8; ++i) {
    fEnergy[i] = new energy( Form("SiPM%dE",i) );
    listENE->Add( fEnergy[i]->GetList() );
    fTRKxyEnergy[i] = new TH2D(Form("fTRKxySiPM%dE",i),
                              Form("fTRKxySiPM%dE;[mm];[mm]",i),
                               100,-10,50, 100, -10,50 );
    listENE->Add( fTRKxyEnergy[i] );
    for(int isl=0; isl!=5; ++isl) {
      fEnergyPulse[i][isl] = new TProfile(Form("fEnergyPulse100300SiPM%dE_SL%d",i,isl),
                                        Form("fEnergyPulse100300SiPM%dE_SL%d;Alligned by MaxTime [ns];[mV]",i,isl),
                                        1000,-100,100 );
      listENE->Add( fEnergyPulse[i][isl] );
      fEnergyPulseT0[i][isl] = new TProfile(Form("fEnergyPulseT0SiPM%dE_SL%d",i,isl),
                                          Form("fEnergyPulseT0SiPM%dE_SL%d;Alligned by T0 [ns];[mV]",i,isl),
                                        1000,-100,100 );
      listENE->Add( fEnergyPulseT0[i][isl] );
    }
  }
  for(int i=0; i!=4; ++i) {
    fEventDisplayEnergy[i] = new TCanvas( Form("ED_SiPME_%d",i) );
    fEventDisplayEnergy[i]->Divide(5,5);
    listENE->Add( fEventDisplayEnergy[i] );
    fEnergyCorrelation[i] = new TH2D(Form("EnergyCorrelation_%d",i),
                                     Form("EnergyCorrelation_%d;[mV];[mV]",i),
                                     100,0,800,100,0,800);
    listENE->Add( fEnergyCorrelation[i] );
  }
  fList->Add(listENE);
  //--------------
  TList *listTIM = new TList();
  listTIM->SetName("timing");
  listTIM->SetOwner();
  for(int i=0; i!=8; ++i) {
    fTiming[i] = new timing( Form("SiPM%dT",i) );
    listTIM->Add( fTiming[i]->GetList() );
    for(int isl=0; isl!=5; ++isl) {
      fTimingWalk[i][isl] = new TH2D(Form("fTimingWalkSiPM%dT_SL%d",i,isl),
                                      Form("fTimingWalkSiPM%dT_SL%d;1/[mV];[ns]",i,isl),
                                      100,0.0,+0.03,100,-2,+2 );
      listTIM->Add( fTimingWalk[i][isl] );
      fTimingWalkC[i][isl] = new TH2D(Form("fTimingWalkCSiPM%dT_SL%d",i,isl),
                                      Form("fTimingWalkCSiPM%dT_SL%d;1/[mV];[ns]",i,isl),
                                      100,0.0,+0.03,100,-2,+2 );
      listTIM->Add( fTimingWalkC[i][isl] );
    }
  }
  for(int i=0; i!=4; ++i) {
    fEventDisplayTiming[i] = new TCanvas( Form("ED_SiPMT_%d",i) );
    fEventDisplayTiming[i]->Divide(5,5);
    listTIM->Add( fEventDisplayTiming[i] );
    fTimingCorrelation[i] = new TH2D(Form("TimingCorrelation_%d",i),
                                     Form("TimingCorrelation_%d;Time wrt MCP [nS]; Time wrt MCP [ns]",i),
                                     100,-2,+2,100,-2,+2);
    listTIM->Add( fTimingCorrelation[i] );
    for(int isl=0; isl!=5; ++isl) {
      fTimingCorrelationSL[i][isl] = new TH2D(Form("fTimingCorrelationSiPM%dT_SL%d",i,isl),
                                      Form("fTimingCorrelationSiPM%dT_SL%d;[ns];[ns]",i,isl),
                                      100,-2,+2,100,-2,+2 );
      listTIM->Add( fTimingCorrelationSL[i][isl] );
    }  }
  fList->Add(listTIM);
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
  TGraph *tmpEnergy[8][25];
  TGraph *tmpTiming[8][25];
  for(;iEvent!=nEvents; ++iEvent) {
    fTree->GetEntry( iEvent );
    fEvents->Fill( 0 );
    if(iEvent%1000==0) std::cout << "Events read so far: " << iEvent << std::endl;

    if(ntracks!=1) continue;
    fEvents->Fill( 1 );
    
    double xproj = xIntercept*1e-3;
    double yproj = yIntercept*1e-3;
    fTRKxy0->Fill( xproj, yproj );

    fTRKchi2->Fill( chi2 );

    fMCP0->Fill( times[0], channel[0] ); // group 0 channel 0
    bool passMCP0 =  fMCP0->Process();

    if(iEvent1<25) {
      fEventDisplayMCP->cd(iEvent1+1);
      DisplayWave(fMCP0, NULL, 1);
    }
    iEvent1++;

    if( passMCP0 ) fTRKxyMCP0->Fill( xproj, yproj );
    if( !passMCP0 ) continue;
    fEvents->Fill( 2 );

    double T0 = fMCP0->GetTime();
    
    // =================================
    // =================================
    // ========== ENERGY ===============
    // =================================
    // =================================
    fEnergy[0]->Fill( times[1], channel[10+1] ); // group 1 channel 10 0e00
    fEnergy[1]->Fill( times[1], channel[11+1] ); // group 1 channel 11 0e00
    fEnergy[2]->Fill( times[1], channel[12+1] ); // group 1 channel 12 5e13
    fEnergy[3]->Fill( times[1], channel[13+1] ); // group 1 channel 13 5e13
    fEnergy[4]->Fill( times[1], channel[14+1] ); // group 1 channel 14 2e14
    fEnergy[5]->Fill( times[1], channel[15+1] ); // group 1 channel 15 2e14
    fEnergy[6]->Fill( times[1], channel[9+1] );  // spy channel
    bool passEnergy[8] = {false,false,false,false,false,false,false,false};
    for(int i=0; i!=7; ++i)
      passEnergy[i] =  fEnergy[i]->Process();
    for(int i=0; i!=8; ++i)
      if( passEnergy[i] )
        fTRKxyEnergy[i]->Fill( xproj, yproj );

    if(passEnergy[0]&&passEnergy[1]) {
      static int iEvent2_Bar0 = 0;
      if(iEvent2_Bar0<25) {
        fEventDisplayEnergy[0]->cd(iEvent2_Bar0+1);
        DisplayWave(fEnergy[0], fEnergy[1], 2);
      }
      fEnergyCorrelation[0]->Fill(-fEnergy[0]->GetAmplitude(),
                                  -fEnergy[1]->GetAmplitude() );
      iEvent2_Bar0++;
    }
    if(passEnergy[2]&&passEnergy[3]) {
      static int iEvent2_Bar1 = 0;
      if(iEvent2_Bar1<25) {
        fEventDisplayEnergy[1]->cd(iEvent2_Bar1+1);
        DisplayWave(fEnergy[2], fEnergy[3], 2);
      }
      fEnergyCorrelation[1]->Fill(-fEnergy[2]->GetAmplitude(),
                                  -fEnergy[3]->GetAmplitude() );
      iEvent2_Bar1++;
    }
    if(passEnergy[4]&&passEnergy[5]) {
      static int iEvent2_Bar2 = 0;
      if(iEvent2_Bar2<25) {
        fEventDisplayEnergy[2]->cd(iEvent2_Bar2+1);
        DisplayWave(fEnergy[4], fEnergy[5], 2);
        
      }
      fEnergyCorrelation[2]->Fill(-fEnergy[4]->GetAmplitude(),
                                  -fEnergy[5]->GetAmplitude() );
      iEvent2_Bar2++;
    }

    double mcpCenterIx = 15.6; // valid for run 44718 - 34
    double mcpCenterIy = 25.1; // valid for run 44718 - 34
    double bar0CenterX = 14.0; // valid for run 44734 [8-20]
    double bar0CenterY = 24.3; // valid for run 44734 [22.5-26]

    if(xproj < bar0CenterX - 9) continue;
    if(xproj > bar0CenterX + 19) continue;
    if(yproj < bar0CenterY - 1) continue;
    if(yproj > bar0CenterY + 1) continue;
    fEvents->Fill( 3 );

    int sl_class;
    if(xproj>9&&xproj<11)  sl_class = 0;
    if(xproj>11&&xproj<13) sl_class = 1;
    if(xproj>13&&xproj<15) sl_class = 2;
    if(xproj>15&&xproj<17) sl_class = 3;
    if(xproj>17&&xproj<19) sl_class = 4;

    double amplitude[8];
    double time[8];
    double thr_hg[8];
    for(int i=0; i!=7; ++i) {
      amplitude[i] = -fEnergy[i]->GetAmplitude();
      time[i] = fEnergy[i]->GetTime();
      thr_hg[i] = 0.2*12.0*fEnergy[i]->GetAmplitude();
    }
    // =============
    // Filling up TProfile
    for(int j=0; j!=1024; ++j) {
      for(int i=0; i!=7; ++i) {
        if( (amplitude[i]<300)&&(amplitude[i]>100) )
          fEnergyPulse[i][sl_class]->Fill(fEnergy[i]->GetPointX(j)-time[i],
                                          fEnergy[i]->GetPointY(j));
        fEnergyPulseT0[i][sl_class]->Fill(fEnergy[i]->GetPointX(j)-T0,
                                          fEnergy[i]->GetPointY(j));
      }
    }
    // =============
    
    // =================================
    // =================================
    // =========== TIME ================
    // =================================
    // =================================

    fTiming[0]->Fill( times[0], channel[2] ); // group 0 channel 2 0e00
    fTiming[1]->Fill( times[0], channel[3] ); // group 0 channel 3 0e00
    fTiming[2]->Fill( times[0], channel[4] ); // group 0 channel 4 5e13
    fTiming[3]->Fill( times[0], channel[5] ); // group 0 channel 5 5e13
    fTiming[4]->Fill( times[0], channel[6] ); // group 0 channel 6 2e14
    fTiming[5]->Fill( times[0], channel[7] ); // group 0 channel 7 2e14

    double iT0[8] = {
      T0-37, T0-38,
      T0-37, T0-37,
      T0-37, T0-37,
      T0-37, T0-37 };
    double delay = 37;
    T0 -= delay;

    double corrInter[7][5] = {
     { -1.7748, -1.8565, -1.7907, -1.7753, -1.7965 },
     { -1.0471, -0.9803, -1.0093, -1.0678, -1.0129 },
     { -1.7439, -1.3479, -1.3935, -1.3439, -1.2810 },
     { -1.4092, -1.5089, -1.2345, -1.4312, -1.2831 },
     { -1.3050, -1.2673, -0.9635, -1.1130, -1.2864 },
      { -1.4566, -1.4389, -1.0179, -1.4351, -1.3668 },
      { 0, 0, 0, 0, 0 },
    };
    double corrSlope[7][5] = {
     { 171.0131, 177.3607, 167.8040, 166.2089, 165.1642 },
     { 189.7844, 180.1492, 189.9891, 200.3627, 192.5884 },
     { 222.7975, 177.3238, 178.3665, 169.6434, 154.1469 },
     { 150.2418, 173.1383, 118.4242, 153.2749, 132.0686 },
     { 313.3333, 299.1318, 251.6950, 290.5783, 318.4777 },
     { 356.7735, 361.5877, 261.3423, 352.1975, 344.0982 },
      { 0, 0, 0, 0, 0 },
    };
    
    
    bool passTiming[8] = {false,false,false,false,false,false,false,false};
    for(int i=0; i!=7; ++i) {
      //fTiming[i]->SetThreshold( thr_hg[i] ); // constant fraction threshold
      fTiming[i]->SetThreshold( -200 ); // 200mV threshold
      fTiming[i]->SetThreshold( -20 ); // 20mV threshold
      passTiming[i] =  fTiming[i]->Process();
      if(passTiming[i]) {
        double param = 1/amplitude[i];
        double corr = corrInter[i][sl_class] + corrSlope[i][sl_class]*param;
        fTimingWalk[i][sl_class]->Fill(param, fTiming[i]->GetTime() -iT0[i] );
        if(param>0.0025)
          fTimingWalkC[i][sl_class]->Fill(param, fTiming[i]->GetTime()-iT0[i]-corr );
      }
    }

    if(passTiming[0]&&passTiming[1]) {
      static int iEvent3_Bar0 = 0;
      if(iEvent3_Bar0<25) {
        fEventDisplayTiming[0]->cd(iEvent3_Bar0+1);
        DisplayWave(fTiming[0], fTiming[1],1);
      }
      fTimingCorrelation[0]->Fill(fTiming[0]->GetTime() -iT0[0],
                                  fTiming[1]->GetTime() -iT0[1] );
      fTimingCorrelationSL[0][sl_class]->Fill(fTiming[0]->GetTime() -iT0[0],
                                              fTiming[1]->GetTime() -iT0[1] );
      iEvent3_Bar0++;
    }
    if(passTiming[2]&&passTiming[3]) {
      static int iEvent3_Bar1 = 0;
      if(iEvent3_Bar1<25) {
        fEventDisplayTiming[1]->cd(iEvent3_Bar1+1);
        DisplayWave(fTiming[2], fTiming[3],1);
      }
      fTimingCorrelation[1]->Fill(fTiming[2]->GetTime() -iT0[2],
                                  fTiming[3]->GetTime() -iT0[3]);
      fTimingCorrelationSL[1][sl_class]->Fill(fTiming[2]->GetTime() -iT0[2],
                                              fTiming[3]->GetTime() -iT0[3] );
      iEvent3_Bar1++;
    }
    if(passTiming[4]&&passTiming[5]) {
      static int iEvent3_Bar2 = 0;
      if(iEvent3_Bar2<25) {
        fEventDisplayTiming[2]->cd(iEvent3_Bar2+1);
        DisplayWave(fTiming[4], fTiming[5],1);
      }
      fTimingCorrelation[2]->Fill(fTiming[4]->GetTime() -iT0[4],
                                  fTiming[5]->GetTime() -iT0[5]);
      fTimingCorrelationSL[2][sl_class]->Fill(fTiming[4]->GetTime() -iT0[4],
                                              fTiming[5]->GetTime() -iT0[5] );
      iEvent3_Bar2++;
    }
  }
  fTRKxyMCP0->Divide( fTRKxy0 );
  for(int i=0; i!=8; ++i)
    fTRKxyEnergy[i]->Divide( fTRKxy0 );
  Terminate();
  fTree = NULL; //don't call me again
}
