int analyze_MCP() {
  int run[15] = {44718,44719,44720,44721,44722,44724,44725,44725,44727,44728,
    44730,44731,44732,44733,44734};

  TList *list[15];
  TH2D *TRKxyMCP0[15];
  TH2D *TRKxyMCP1[15];
  TH2D *TRKxyMCPI[15];
  TH2D *MCPta0[15];
  for(int irun=0; irun!=15; ++irun) {
    list[irun] = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run[irun]) )->Get("output");
    //list[irun]->ls();
    TRKxyMCP0[irun] = (TH2D*) list[irun]->FindObject("fTRKxyMCP0");
    TRKxyMCP1[irun] = (TH2D*) list[irun]->FindObject("fTRKxyMCP1");
    TRKxyMCPI[irun] = (TH2D*) list[irun]->FindObject("fTRKxyMCPI");
    MCPta0[irun] = (TH2D*) list[irun]->FindObject("fMCPta0");
  }

  TCanvas *cTRKxyMCPI = new TCanvas();
  cTRKxyMCPI->Divide(4,4);
  for(int irun=0; irun!=15; ++irun) {
    cTRKxyMCPI->cd(irun+1)->SetGridx(1);
    cTRKxyMCPI->cd(irun+1)->SetGridy(1);
    TRKxyMCPI[irun]->Draw("colz");
    TRKxyMCPI[irun]->GetXaxis()->SetRangeUser(10,21);
    TRKxyMCPI[irun]->GetYaxis()->SetRangeUser(20,30);
  }

  TCanvas *cTRKxyMCP0 = new TCanvas();
  cTRKxyMCP0->Divide(4,4);
  for(int irun=0; irun!=15; ++irun) {
    cTRKxyMCP0->cd(irun+1)->SetGridx(1);
    cTRKxyMCP0->cd(irun+1)->SetGridy(1);
    TRKxyMCP0[irun]->Draw("colz");
    TRKxyMCP0[irun]->GetXaxis()->SetRangeUser(5,25);
    TRKxyMCP0[irun]->GetYaxis()->SetRangeUser(15,35);
  }

  TCanvas *cTRKxyMCP1 = new TCanvas();
  cTRKxyMCP1->Divide(4,4);
  for(int irun=0; irun!=15; ++irun) {
    cTRKxyMCP1->cd(irun+1)->SetGridx(1);
    cTRKxyMCP1->cd(irun+1)->SetGridy(1);
    TRKxyMCP1[irun]->Draw("colz");
    TRKxyMCP1[irun]->GetXaxis()->SetRangeUser(5,25);
    TRKxyMCP1[irun]->GetYaxis()->SetRangeUser(15,35);
  }

  TCanvas *cMCPta0 = new TCanvas();
  TH2D *allMCPta0 = (TH2D*) MCPta0[0]->Clone("allMCPta0");
  for(int irun=1; irun!=15; ++irun) {
    allMCPta0->Add( MCPta0[irun] );
  }
  allMCPta0->Draw("colz");
  TProfile *pfMCPta0 = allMCPta0->ProfileX("MCPta0profX",
                                           allMCPta0->GetYaxis()->FindBin(-0.25),
                                           allMCPta0->GetYaxis()->FindBin(-0.05),"s");
  pfMCPta0->SetLineColor(kRed-3);
  pfMCPta0->SetLineWidth(2);
  pfMCPta0->Draw("same");

  TH1D *hslice[5];
  hslice[0] = allMCPta0->ProjectionY("allMCPta0_100_200",
                                     allMCPta0->GetXaxis()->FindBin(100),
                                     allMCPta0->GetXaxis()->FindBin(200));
  hslice[1] = allMCPta0->ProjectionY("allMCPta0_200_300",
                                     allMCPta0->GetXaxis()->FindBin(200),
                                     allMCPta0->GetXaxis()->FindBin(300));
  hslice[2] = allMCPta0->ProjectionY("allMCPta0_300_400",
                                     allMCPta0->GetXaxis()->FindBin(300),
                                     allMCPta0->GetXaxis()->FindBin(400));
  hslice[3] = allMCPta0->ProjectionY("allMCPta0_400_500",
                                     allMCPta0->GetXaxis()->FindBin(400),
                                     allMCPta0->GetXaxis()->FindBin(500));
  hslice[4] = allMCPta0->ProjectionY("allMCPta0_500_600",
                                     allMCPta0->GetXaxis()->FindBin(500),
                                     allMCPta0->GetXaxis()->FindBin(600));
  TCanvas *cMCPta0_slices = new TCanvas();
  int col[5] = {kBlue-3, kCyan-3, kGreen-3, kOrange-3, kRed-3};
  TF1 *hfitslice[5];
  TLegend *leg = new TLegend(0.1,0.7,0.9,0.9);
  leg->SetNColumns(5);
  for(int i=0; i!=5; ++i) {
    hslice[i]->Sumw2();
    hslice[i]->Scale(1/hslice[i]->GetMaximum());
    hslice[i]->SetLineColor( col[i] );
    hfitslice[i] = new TF1( Form("fit%d",i), "gaus" );
    hfitslice[i]->SetLineColor( col[i] );
    hslice[i]->Fit( hfitslice[i] , "I");
  }
/*
  for(int i=0; i!=5; ++i) {
    leg->AddEntry( hslice[i], Form("AMP [%d-%d] #mu %.0f ps  #sigma %.0f ps",
                                   100*(i+1), 100*(i+2),
                                   hfitslice[i]->GetParameter(1)*1e3,
                                   hfitslice[i]->GetParameter(2)*1e3) );
  }
 */
  for(int i=0; i!=5; ++i) {
    leg->AddEntry( hslice[i], Form("AMP [%d-%d]",
                                   100*(i+1), 100*(i+2) ) );
  }
  for(int i=0; i!=5; ++i) {
    leg->AddEntry( hslice[i], Form("#mu %.1f ps",
                                   hfitslice[i]->GetParameter(1)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    leg->AddEntry( hslice[i], Form("#sigma %.1f ps",
                                   hfitslice[i]->GetParameter(2)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    if(i==0) hslice[0]->Draw();
    else hslice[i]->Draw("SAME");
  }
  hslice[0]->GetXaxis()->SetRangeUser(-0.24,-0.07);
  hslice[0]->GetYaxis()->SetRangeUser(0,1.4);
  leg->Draw();

  return 0;
}
