int analyze_Pulse(int run=44734 ) {
  TList *list, *list2;
  TProfile *EnergyPulse[8][5];
  TProfile *EnergyPulseT0[8][5];
  list2 = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run) )->Get("output");
  list = (TList*) list2->FindObject( "energy" );

  //list->ls();
  //return 0; 
  int color[8] = {
    kBlue-3, kBlue-3,
    kGreen-3, kGreen-3,
    kOrange-3, kOrange-3,
    kBlack, kBlack };
  double sl_class[6] = {9,11,13,15,17,19};

  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      EnergyPulse[i][j] = (TProfile*) list->FindObject(Form("fEnergyPulse100300SiPM%dE_SL%d",i,j));
      EnergyPulse[i][j]->Rebin(4);
      EnergyPulse[i][j]->SetLineColor( color[i] );
      EnergyPulse[i][j]->SetMarkerColor( color[i] );
      if(i%2==0) EnergyPulse[i][j]->SetMarkerStyle( 7 );
      else EnergyPulse[i][j]->SetMarkerStyle( 24 );

      EnergyPulseT0[i][j] = (TProfile*) list->FindObject(Form("fEnergyPulseT0SiPM%dE_SL%d",i,j));
      EnergyPulseT0[i][j]->Rebin(4);
      EnergyPulseT0[i][j]->SetLineColor( color[i] );
      EnergyPulseT0[i][j]->SetMarkerColor( color[i] );
      if(i%2==0) EnergyPulseT0[i][j]->SetMarkerStyle( 7 );
      else EnergyPulseT0[i][j]->SetMarkerStyle( 24 );

    }
  }
  
  TCanvas *main2 = new TCanvas();
  TH2D *axis2 = new TH2D("axis2",";[mm];<Amp> [mV]",100,8,20,100,100,+200);
  float peak[6][5];
  float xxxx[5];
  TGraph *grMin[6];
  axis2->Draw();
  TLegend *leg2 = new TLegend(0.1,0.1,0.9,0.3);
  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      xxxx[j] = 0.5*(sl_class[j]+sl_class[j+1]);
      peak[i][j] = -EnergyPulse[i][j]->GetMinimum();
    }
    grMin[i] = new TGraph(5,xxxx,peak[i]);
    grMin[i]->SetLineColor( color[i] );
    grMin[i]->SetMarkerColor( color[i] );
    if(i%2==0) grMin[i]->SetMarkerStyle( 20 );
    else grMin[i]->SetMarkerStyle( 24 );
    grMin[i]->Draw("PLSAME");
    leg2->AddEntry( grMin[i], Form("DRS CH %d",10+i) );
  }
  leg2->SetNColumns(3);
  leg2->Draw();
  
  TCanvas *main = new TCanvas();
  TH2D *axis = new TH2D("axis",";wrt MaxTime [ns];[mV]",100,-100,+100,100,-1,+0.1);
  axis->Draw();
  TLegend *leg = new TLegend(0.1,0.1,0.3,0.5);
  for(int i=0; i!=6; ++i) {
    leg->AddEntry( EnergyPulse[i][0], Form("DRS CH %d",10+i) );
    for(int j=0; j!=5; ++j) {
      double scale = -EnergyPulse[i][j]->GetMinimum();
      EnergyPulse[i][j]->Scale(1/scale);
      EnergyPulse[i][j]->Draw("same");
    }
  }
  //leg->SetNColumns(5);
  leg->Draw();

  TCanvas *mainT = new TCanvas();
  TH2D *axisT = new TH2D("axis",";wrt T0 [ns];[mV]",100,-100,+100,100,-1,+0.1);
  axisT->Draw();
  TLegend *legT = new TLegend(0.1,0.1,0.3,0.5);
  for(int i=0; i!=6; ++i) {
    legT->AddEntry( EnergyPulseT0[i][0], Form("DRS CH %d",10+i) );
    for(int j=0; j!=5; ++j) {
      double scale = -EnergyPulseT0[i][j]->GetMinimum();
      EnergyPulseT0[i][j]->Scale(1/scale);
      EnergyPulseT0[i][j]->Draw("same");
    }
  }
  //legT->SetNColumns(5);
  legT->Draw();

  TList *listT = (TList*) list2->FindObject( "timing" );
  listT->ls();

  TH2D *TimingWalk[6][5];
  TH2D *TimingWalkC[6][5];
  TProfile *TimingWalkprof[6][5];
  TF1 *TimingWalkfit[6][5];
  TH1D *TimingWalkCProj[6][5];
  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      TimingWalk[i][j] = (TH2D*) listT->FindObject(Form("fTimingWalkSiPM%dT_SL%d",i,j));
      TimingWalkC[i][j] = (TH2D*) listT->FindObject(Form("fTimingWalkCSiPM%dT_SL%d",i,j));
      TimingWalkprof[i][j] = TimingWalk[i][j]->ProfileX( Form("fTimingWalkSiPM%dT_SL%d_PROF",i,j) );
      TimingWalkfit[i][j] = new TF1( Form("fTimingWalkSiPM%dT_SL%d_FIT",i,j), "[0]+x*[1]" );
      TimingWalkCProj[i][j] = TimingWalkC[i][j]->ProjectionY(Form("fTimingWalkCProjSiPM%dT_SL%d",i,j));
    }
  }

  TCanvas *mainAT = new TCanvas();
  mainAT->Divide(6,5);
  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      mainAT->cd(1+6*j+i);
      TimingWalk[i][j]->Draw("colz");
      TimingWalkprof[i][j]->SetLineColor( kOrange-3 );
      TimingWalkprof[i][j]->Draw("same");
      TimingWalkprof[i][j]->Fit( TimingWalkfit[i][j], "R", "", 0.0025,0.01 );
    }
  }

  cout << "double corrInter[6][5] = { " << endl;
  for(int i=0; i!=6; ++i) {
    cout << " { ";
    for(int j=0; j!=5; ++j) {
      cout << Form("%.4f, ", TimingWalkfit[i][j]->GetParameter(0) );
    }
    cout << "}, ";
    cout << endl;
  }
  cout << "}; " << endl;

  cout << "double corrSlope[6][5] = { " << endl;
  for(int i=0; i!=6; ++i) {
    cout << " { ";
    for(int j=0; j!=5; ++j) {
      cout << Form("%.4f, ", TimingWalkfit[i][j]->GetParameter(1) );
    }
    cout << "}, ";
    cout << endl;
  }
  cout << "}; " << endl;

  
  
  TCanvas *mainATC = new TCanvas();
  mainATC->Divide(6,5);
  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      mainATC->cd(1+6*j+i);
      TimingWalkC[i][j]->Draw("colz");
    }
  }

  TCanvas *mainATCP = new TCanvas();
  mainATCP->Divide(6,5);
  for(int i=0; i!=6; ++i) {
    for(int j=0; j!=5; ++j) {
      mainATCP->cd(1+6*j+i);
      TimingWalkCProj[i][j]->Draw();
    }
  }

  return 0;
}
