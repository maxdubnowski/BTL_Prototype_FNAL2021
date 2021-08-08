int analyze_oneMCP(int run=44718) {
  TList *list = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run) )->Get("output");
  TList *list0 = (TList*) list->FindObject("MCP0");
  TList *list1 = (TList*) list->FindObject("MCP1");
  
  TH1D *MCP0_A_rmaxY = (TH1D*) list0->FindObject("MCP0_A_rmaxy");
  TH1D *MCP1_A_rmaxY = (TH1D*) list1->FindObject("MCP1_A_rmaxy");

  TH2D *fTRKxyMCP0 = (TH2D*) list->FindObject("fTRKxyMCP0");
  TH2D *fTRKxyMCP1 = (TH2D*) list->FindObject("fTRKxyMCP1");
  TH2D *fTRKxyMCPI = (TH2D*) list->FindObject("fTRKxyMCPI");

  MCP0_A_rmaxY->SetLineColor( kBlue-3 );
  MCP0_A_rmaxY->SetFillColor( kBlue-3 );
  MCP1_A_rmaxY->SetLineColor( kBlack );

  TLegend *leg = new TLegend(0.5,0.65,0.65,0.8);
  leg->AddEntry(MCP0_A_rmaxY,"MCP0");
  leg->AddEntry(MCP1_A_rmaxY,"MCP1");

  TCanvas *main = new TCanvas();
  MCP0_A_rmaxY->Draw("b");
  MCP1_A_rmaxY->Draw("same");
  MCP0_A_rmaxY->SetTitle("Amplitude");
  leg->Draw();

  TCanvas *main2 = new TCanvas();
  main2->Divide(3,1);
  main2->cd(1); fTRKxyMCP0->Draw("colz");
  main2->cd(3); fTRKxyMCP1->Draw("colz");
  main2->cd(2); fTRKxyMCPI->Draw("colz");

  return 0;
}
