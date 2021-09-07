int analyze_MCP() {
  int run[15] = {44718,44719,44720,44721,44722,44724,44725,44725,44727,44728,
    44730,44731,44732,44733,44734};

  TList *list0[15];
  TList *list1[15];
  TList *listI[15];
  TH2D *TRKxyMCP0[15];
  TH2D *TRKxyMCP1[15];
  TH2D *TRKxyMCPI[15];
  TH2D *MCPta0[15];
  TH2D *MCPta1[15];
  TH1D *events[15];
  for(int irun=0; irun!=15; ++irun) {
    TList *list = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run[irun]) )->Get("output");
    events[irun] = (TH1D*) list->FindObject("fEvents");
    list0[irun] = (TList*) list->FindObject("mcp0");
    list1[irun] = (TList*) list->FindObject("mcp1");
    listI[irun] = (TList*) list->FindObject("mcpI");
    //list[irun]->ls();
    TRKxyMCP0[irun] = (TH2D*) list0[irun]->FindObject("fTRKxyMCP0");
    TRKxyMCP1[irun] = (TH2D*) list1[irun]->FindObject("fTRKxyMCP1");
    TRKxyMCPI[irun] = (TH2D*) listI[irun]->FindObject("fTRKxyMCPI");
    MCPta0[irun] = (TH2D*) listI[irun]->FindObject("fMCPta0");
    MCPta1[irun] = (TH2D*) listI[irun]->FindObject("fMCPta1");
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

  TH2D *allMCPta0 = (TH2D*) MCPta0[0]->Clone("allMCPta0");
  TH2D *allMCPta1 = (TH2D*) MCPta1[0]->Clone("allMCPta1");
  TH1D *allEvents = (TH1D*) events[0]->Clone("allEvents");
  allMCPta0->SetTitle("");
  allMCPta1->SetTitle("");
  allEvents->SetTitle("");
  for(int irun=1; irun!=15; ++irun) {
    allMCPta0->Add( MCPta0[irun] );
    allMCPta1->Add( MCPta1[irun] );
    allEvents->Add( events[irun] );
  }

  TCanvas *cMCPta0 = new TCanvas();
  allMCPta0->Draw("colz");
  TProfile *pfMCPta0 = allMCPta0->ProfileX("MCPta0profX",
                                           allMCPta0->GetYaxis()->FindBin(-0.25),
                                           allMCPta0->GetYaxis()->FindBin(-0.05),"s");
  pfMCPta0->SetLineColor(kRed-3);
  pfMCPta0->SetLineWidth(2);
  pfMCPta0->Draw("same");

  TCanvas *cMCPta1 = new TCanvas();
  allMCPta1->Draw("colz");
  TProfile *pfMCPta1 = allMCPta1->ProfileX("MCPta1profX",
                                           allMCPta1->GetYaxis()->FindBin(-0.25),
                                           allMCPta1->GetYaxis()->FindBin(-0.05),"s");
  pfMCPta1->SetLineColor(kRed-3);
  pfMCPta1->SetLineWidth(2);
  pfMCPta1->Draw("same");

  
  TH1D *hslice0[5];
  hslice0[0] = allMCPta0->ProjectionY("allMCPta0_0_1",
                                     allMCPta0->GetXaxis()->FindBin(180),
                                     allMCPta0->GetXaxis()->FindBin(222));
  hslice0[1] = allMCPta0->ProjectionY("allMCPta0_1_2",
                                     allMCPta0->GetXaxis()->FindBin(222),
                                     allMCPta0->GetXaxis()->FindBin(264));
  hslice0[2] = allMCPta0->ProjectionY("allMCPta0_2_3",
                                     allMCPta0->GetXaxis()->FindBin(264),
                                     allMCPta0->GetXaxis()->FindBin(306));
  hslice0[3] = allMCPta0->ProjectionY("allMCPta0_3_4",
                                     allMCPta0->GetXaxis()->FindBin(306),
                                     allMCPta0->GetXaxis()->FindBin(348));
  hslice0[4] = allMCPta0->ProjectionY("allMCPta0_4_5",
                                     allMCPta0->GetXaxis()->FindBin(348),
                                     allMCPta0->GetXaxis()->FindBin(390));
  TH1D *hslice1[5];
  hslice1[0] = allMCPta1->ProjectionY("allMCPta1_0_1",
                                     allMCPta1->GetXaxis()->FindBin(140),
                                     allMCPta1->GetXaxis()->FindBin(176));
  hslice1[1] = allMCPta1->ProjectionY("allMCPta1_1_2",
                                     allMCPta1->GetXaxis()->FindBin(176),
                                     allMCPta1->GetXaxis()->FindBin(212));
  hslice1[2] = allMCPta1->ProjectionY("allMCPta1_2_3",
                                     allMCPta1->GetXaxis()->FindBin(212),
                                     allMCPta1->GetXaxis()->FindBin(248));
  hslice1[3] = allMCPta1->ProjectionY("allMCPta1_3_4",
                                     allMCPta1->GetXaxis()->FindBin(248),
                                     allMCPta1->GetXaxis()->FindBin(284));
  hslice1[4] = allMCPta1->ProjectionY("allMCPta1_4_5",
                                     allMCPta1->GetXaxis()->FindBin(284),
                                     allMCPta1->GetXaxis()->FindBin(320));
  hslice0[0]->SetTitle("selection  on  mcp0  amplitude");
  hslice1[0]->SetTitle("selection  on  mcp1  amplitude");

  int col[5] = {kBlue-3, kCyan-3, kGreen-3, kOrange-3, kRed-3};
  TF1 *hfitslice0[5];
  TF1 *hfitslice1[5];
  TLegend *leg0 = new TLegend(0.1,0.7,0.9,0.9);
  leg0->SetNColumns(5);
  TLegend *leg1 = new TLegend(0.1,0.7,0.9,0.9);
  leg1->SetNColumns(5);

  TCanvas *cMCPta0_slices = new TCanvas();
  for(int i=0; i!=5; ++i) {
    hslice0[i]->Sumw2();
    hslice0[i]->SetLineColor( col[i] );
    hfitslice0[i] = new TF1( Form("fit0%d",i), "gaus" );
    hfitslice0[i]->SetLineColor( col[i] );
    hslice0[i]->Fit( hfitslice0[i] , "I");
  }
  for(int i=0; i!=5; ++i) {
    leg0->AddEntry( hslice0[i], Form("[%d-%d] mV",
                                   180+42*(i), 180+42*(i+1) ) );
  }
  for(int i=0; i!=5; ++i) {
    leg0->AddEntry( hslice0[i], Form("#mu %.1f ps",
                                   hfitslice0[i]->GetParameter(1)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    leg0->AddEntry( hslice0[i], Form("#sigma %.1f ps",
                                   hfitslice0[i]->GetParameter(2)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    if(i==0) hslice0[0]->Draw();
    else hslice0[i]->Draw("SAME");
  }
  hslice0[0]->GetXaxis()->SetRangeUser(-0.24,-0.07);
  hslice0[0]->GetYaxis()->SetRangeUser(0,1900);
  leg0->Draw();

  TCanvas *cMCPta1_slices = new TCanvas();
  for(int i=0; i!=5; ++i) {
    hslice1[i]->Sumw2();
    hslice1[i]->SetLineColor( col[i] );
    hfitslice1[i] = new TF1( Form("fit1%d",i), "gaus" );
    hfitslice1[i]->SetLineColor( col[i] );
    hslice1[i]->Fit( hfitslice1[i] , "I");
  }
  for(int i=0; i!=5; ++i) {
    leg1->AddEntry( hslice1[i], Form("[%d-%d] mV",
                                   140+36*(i), 140+36*(i+1) ) );
  }
  for(int i=0; i!=5; ++i) {
    leg1->AddEntry( hslice1[i], Form("#mu %.1f ps",
                                   hfitslice1[i]->GetParameter(1)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    leg1->AddEntry( hslice1[i], Form("#sigma %.1f ps",
                                   hfitslice1[i]->GetParameter(2)*1e3) );
  }
  for(int i=0; i!=5; ++i) {
    if(i==0) hslice1[0]->Draw();
    else hslice1[i]->Draw("SAME");
  }
  hslice1[0]->GetXaxis()->SetRangeUser(-0.24,-0.07);
  hslice1[0]->GetYaxis()->SetRangeUser(0,1900);
  leg1->Draw();

  new TCanvas();
  allEvents->Draw();
  return 0;
}
