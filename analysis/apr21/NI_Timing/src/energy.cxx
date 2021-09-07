#include <iostream>
#include "energy.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

//=================================
energy::energy(TString name) {
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
  fSampleSum = new TH1D(Form("%s_SampleSum",name.Data()),
                        Form("%s_SampleSum;[mV]",name.Data()),
                        200,-50,1000);
  fList->Add( fSampleSum );
  fHisto_QFa = new TH1D(Form("%s_Histo_QFa",name.Data()),
                        Form("%s_Histo_QFa;[mV/ns^2]",name.Data()),
                        100,-0.2,+0.2);
  fList->Add( fHisto_QFa );
  fHisto_QFb = new TH1D(Form("%s_Histo_QFb",name.Data()),
                        Form("%s_Histo_QFb;[mV/ns]",name.Data()),
                        100,-100,+100);
  fList->Add( fHisto_QFb );
  fHisto_QFc = new TH1D(Form("%s_Histo_QFc",name.Data()),
                        Form("%s_Histo_QFc;[mV]",name.Data()),
                        100,-1000,+10000);
  fList->Add( fHisto_QFc );
  fHisto_QFx0 = new TH1D(Form("%s_Histo_QFx0",name.Data()),
                         Form("%s_Histo_QFx0;[ns]",name.Data()),
                         100,0,+200);
  fList->Add( fHisto_QFx0 );
  fHisto_QFy0 = new TH1D(Form("%s_Histo_QFy0",name.Data()),
                         Form("%s_Histo_QFy0;[mV]",name.Data()),
                         100,-10,+1000);
  fList->Add( fHisto_QFy0 );
  fHisto_QFy0rawmaxy = new TH2D(Form("%s_Histo_QFy0rawmaxy",name.Data()),
                                Form("%s_Histo_QFy0rawmaxy",name.Data()),
                                100,-10,1000,100,-10,1000);
  fList->Add( fHisto_QFy0rawmaxy );
  fHisto_AvgEneRefTime = new TH2D(Form("%s_Histo_AvgEneRefTime",name.Data()),
                                  Form("%s_Histo_AvgEneRefTime;Amplitude [mV];RefTime [ns]",name.Data()),
                                  100,0,800,100,0,200);
  fList->Add( fHisto_AvgEneRefTime );
  fHisto_RefTime = new TH2D(Form("%s_Histo_RefTime",name.Data()),
                            Form("%s_Histo_RefTime;QFx0 [ns];RefTime [ns]",name.Data()),
                            100,0,200,100,0,200);
  fList->Add( fHisto_RefTime );
}
//=================================
energy::~energy() {
  delete fList;
}
//=================================
bool energy::Process() {
  SetAmplitude(-9999);
  SetTime(-9999);

  double bl_mean, bl_sdev;
  GetBaseline( bl_mean, bl_sdev, fBase_in, fBase_en );
  fBaseline_mean->Fill( bl_mean );
  fBaseline_sdev->Fill( bl_sdev );

  ShiftWave( -bl_mean );

  double rawmaxy, rawmaxx;
  int imiddle = GetMinimum( rawmaxy, rawmaxx, 0, 1024, 0);
  
  fAmplitude_rawmaxx->Fill( rawmaxx );
  fAmplitude_rawmaxy->Fill( -rawmaxy );

  double samplesum;
  GetSampleSum( samplesum, 0, 1024 );
  fSampleSum->Fill( -samplesum/1024. );
  
  SetAmplitude( rawmaxy );
  SetTime(rawmaxx);

  // improving amplitude
  double x0, y0;
  FindMinimum_P2(y0, x0, imiddle-50, imiddle+50);
  double a = GetQFa();
  double b = GetQFb();
  double c = GetQFc();
  fHisto_QFa->Fill( a );
  fHisto_QFb->Fill( b );
  fHisto_QFc->Fill( c );
  fHisto_QFx0->Fill( x0 );
  fHisto_QFy0->Fill( -y0 );
  fHisto_QFy0rawmaxy->Fill( -rawmaxy, -y0);
  SetAmplitude( y0 );
  SetTime( x0 );

  if(GetRefTime()<-99) {
    // RefTime not set, find it.
    // Use amplitude peak to assign reference time
    SetRefTime( x0 ); // first order
  }
  /*
  if( rawmaxy < -600 ) {
    // hack for large amplitudes
    // if amplitude is greater than 600 (totally arbitrary)
    // then I change reftime to be the crossing time to 500 (also arbitrary)
    double timeX;
    FindThreshold_P1( timeX, -500, 1, 1);
    //if(GetLFrab()>-0.8) return false;
    //if(GetLFb()<0) return false;
    SetRefTime(timeX); // first order
  }
  */
  double reftime = GetRefTime();
  int xrangei = reftime/0.2 - 30/0.2;
  int xrangef = reftime/0.2 + 5/0.2;
  //int xrangei = reftime/0.2 - 20/0.2;
  //int xrangef = reftime/0.2 + 40/0.2;
  int xdelta = xrangef - xrangei;
  GetSampleSum( samplesum, xrangei, xrangef );
  SetAvgEne( -samplesum / xdelta );
  fHisto_AvgEneRefTime->Fill( GetAvgEne(), GetRefTime() );
  fHisto_RefTime->Fill( x0, GetRefTime() );

  return true;
}
