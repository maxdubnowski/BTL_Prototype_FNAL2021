#include "mcp.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

//=================================
mcp::mcp(TString name) {
  fList = new TList();
  fList->SetName( name.Data() );
  fList->SetOwner();
  fBaseline_mean = new TH1D(Form("%s_BL_mean",name.Data()),
                            Form("%s_BL_mean;[mV]",name.Data()),
                            100,-500,+500);
  fList->Add( fBaseline_mean );
  fBaseline_sdev = new TH1D(Form("%s_BL_sdev",name.Data()),
                            Form("%s_BL_sdev;[mV]",name.Data()),
                            100,0,0.1);
  fList->Add( fBaseline_sdev );
  fAmplitude_rawmaxx = new TH1D(Form("%s_A_rmaxx",name.Data()),
                                Form("%s_A_rmaxX;[ns]",name.Data()),
                                100,0,200);
  fList->Add( fAmplitude_rawmaxx );
  fAmplitude_rawmaxy = new TH1D(Form("%s_A_rmaxy",name.Data()),
                                Form("%s_A_rmaxY;[mV]",name.Data()),
                                100,0,1000);
  fList->Add( fAmplitude_rawmaxy );
  fHisto_LFa = new TH1D(Form("%s_LFa",name.Data()),
                        Form("%s_LFa;[mV/ns]",name.Data()),
                        100,-1000,+1000);
  fList->Add( fHisto_LFa );
  fHisto_LFb = new TH1D(Form("%s_LFb",name.Data()),
                        Form("%s_LFb;[mV]",name.Data()),
                        100,-1000,+100000);
  fList->Add( fHisto_LFb );
  fHisto_LFrab = new TH1D(Form("%s_LFrab",name.Data()),
                          Form("%s_fHisto_LFrab",name.Data()),
                          100,-2,+2);
  fList->Add( fHisto_LFrab );
  fTime_t500 = new TH2D(Form("%s_T_thr500",name.Data()),
                        Form("%s_T_thr500;[mV];[ns]",name.Data()),
                        100,0,1000,100,0,200);
  fList->Add( fTime_t500 );
  fTime_t501 = new TH2D(Form("%s_T_thr501",name.Data()),
                        Form("%s_T_thr501;[mV];[ns]",name.Data()),
                        100,0,1000,100,90,120);
  fList->Add( fTime_t501 );
}
//=================================
mcp::~mcp() {
  delete fList;
}
//=================================
bool mcp::Process(double noiseLevel) {
  SetAmplitude(-9999);
  SetTime(-9999);

  double bl_mean, bl_sdev;
  GetBaseline( bl_mean, bl_sdev, 0, 200 );
  fBaseline_mean->Fill( bl_mean );
  fBaseline_sdev->Fill( bl_sdev );

  ShiftWave( -bl_mean );

  double rawmaxy, rawmaxx;
  GetMinimum( rawmaxy, rawmaxx, 0, 1024, 0);
  
  fAmplitude_rawmaxx->Fill( rawmaxx );
  fAmplitude_rawmaxy->Fill( -rawmaxy );

  SetAmplitude( rawmaxy );
  SetTime(rawmaxx);

  // improving time
  
  if(rawmaxy>noiseLevel) return false; // get rid of noise
  if(rawmaxy<-700) return false; // get rid of saturating pulses
  double thr = rawmaxy*0.5;

  double timeX;
  FindThreshold_P1( timeX, thr, 1, 1);
  fHisto_LFa->Fill( GetLFa() );
  fHisto_LFb->Fill( GetLFb() );
  fHisto_LFrab->Fill( GetLFrab() );
  fTime_t500->Fill( -rawmaxy, timeX );
  if(GetLFrab()>-0.8) return false;
  if(GetLFb()<0) return false;
  fTime_t501->Fill( -rawmaxy, timeX );
  SetTime(timeX); // improved time

  return true;
}
