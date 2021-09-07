int walk(int run=44734 ) {
  TList *list[5], *list2, *list3, *list3E;
  TH2D     *enetim[2][5];
  TProfile *enetimp[2][5];
  TF1      *enetimf[2][5], *enetimfext[2][5];
  list2 = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run) )->Get("output");
  list3 = (TList*) list2->FindObject( "timing" );
  list3E = (TList*) list2->FindObject( "energy" );
  for(int ithr=0; ithr!=5; ++ithr) {
    list[ithr] = (TList*) list3->FindObject( Form("thr_%d",ithr) );
    enetim[0][ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingLE_%d",ithr) );
    enetim[1][ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingRE_%d",ithr) );
    enetimp[0][ithr] = enetim[0][ithr]->ProfileX( Form("L_%d",ithr) );
    enetimp[1][ithr] = enetim[1][ithr]->ProfileX( Form("R_%d",ithr) );
    enetimf[0][ithr] = new TF1( Form("FL_%d",ithr), "[0]+[1]/x+[2]*TMath::Power(x,9)*1e-27", 100, 500 );
    enetimf[1][ithr] = new TF1( Form("FR_%d",ithr), "[0]+[1]/x+[2]*TMath::Power(x,9)*1e-27", 100, 500 );
    //enetimf[0][ithr]->SetParameter( 1, 150);
    //enetimf[1][ithr]->SetParameter( 1, 150);
    //enetimf[0][ithr]->SetParLimits( 2, 50, 350);
    //enetimf[1][ithr]->SetParLimits( 2, 50, 350);
    enetimfext[0][ithr] = new TF1( Form("FLext_%d",ithr), "[0]+[1]/x+[2]*TMath::Power(x,9)*1e-27", 0, 1000 );
    enetimfext[1][ithr] = new TF1( Form("FRext_%d",ithr), "[0]+[1]/x+[2]*TMath::Power(x,9)*1e-27", 0, 1000 );
    enetimf[0][ithr]->SetLineColor( kRed-3 );
    enetimf[1][ithr]->SetLineColor( kRed-3 );
    enetimfext[0][ithr]->SetLineColor( kBlue-3 );
    enetimfext[1][ithr]->SetLineColor( kBlue-3 );
    enetimfext[0][ithr]->SetLineStyle( 2 );
    enetimfext[1][ithr]->SetLineStyle( 2 );
  }

  TCanvas *main = new TCanvas();
  main->Divide(5,2);
  for(int ithr=0; ithr!=5; ++ithr) {
    main->cd(1+ithr);
    enetimp[0][ithr]->SetLineWidth(3);
    enetimp[0][ithr]->SetLineColor(kOrange-3);
    enetim[0][ithr]->Draw("colz");
    enetimp[0][ithr]->Draw("same");
    enetimp[0][ithr]->Fit( enetimf[0][ithr], "W" );
    enetimfext[0][ithr]->SetParameter( 0, enetimf[0][ithr]->GetParameter(0) );
    enetimfext[0][ithr]->SetParameter( 1, enetimf[0][ithr]->GetParameter(1) );
    enetimfext[0][ithr]->SetParameter( 2, enetimf[0][ithr]->GetParameter(2) );
    enetimfext[0][ithr]->Draw("same");
    main->cd(6+ithr);
    enetimp[1][ithr]->SetLineWidth(3);
    enetimp[1][ithr]->SetLineColor(kOrange-3);
    enetim[1][ithr]->Draw("colz");
    enetimp[1][ithr]->Draw("same");
    enetimp[1][ithr]->Fit( enetimf[1][ithr], "W" );
    enetimfext[1][ithr]->SetParameter( 0, enetimf[1][ithr]->GetParameter(0) );
    enetimfext[1][ithr]->SetParameter( 1, enetimf[1][ithr]->GetParameter(1) );
    enetimfext[1][ithr]->SetParameter( 2, enetimf[1][ithr]->GetParameter(2) );
    enetimfext[1][ithr]->Draw("same");
  }

  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[0][ithr]->GetParameter(0) << ", ";
  cout << endl;
  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[0][ithr]->GetParameter(1) << ", ";
  cout << endl;
  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[0][ithr]->GetParameter(2) << ", ";
  cout << endl;

  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[1][ithr]->GetParameter(0) << ", ";
  cout << endl;
  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[1][ithr]->GetParameter(1) << ", ";
  cout << endl;
  for(int ithr=0; ithr!=5; ++ithr)
    cout << enetimf[1][ithr]->GetParameter(2) << ", ";
  cout << endl;

  double xx[5] = {5,10,30,50,100};
  double p0L[5], p1L[5], p2L[5], p0R[5], p1R[5], p2R[5];
  for(int ithr=0; ithr!=5; ++ithr) {
    p0L[ithr] = enetimf[0][ithr]->GetParameter(0);
    p1L[ithr] = enetimf[0][ithr]->GetParameter(1);
    p2L[ithr] = enetimf[0][ithr]->GetParameter(2);
    p0R[ithr] = enetimf[1][ithr]->GetParameter(0);
    p1R[ithr] = enetimf[1][ithr]->GetParameter(1);
    p2R[ithr] = enetimf[1][ithr]->GetParameter(2);
  }
  
  TLatex *tex = new TLatex();
  tex->SetTextSize(0.2);
  tex->SetTextColor( kCyan-3 );
  TCanvas *main2 = new TCanvas();
  main2->SetBottomMargin(0.2);
  main2->Divide(1,3,0,0);
  main2->cd(1)->SetGridx(1);
  main2->cd(1)->SetGridy(1);
  TGraph *gp0L = new TGraph(5,xx,p0L); gp0L->SetMarkerStyle(20); gp0L->SetMarkerColor(kRed-3);
  TGraph *gp0R = new TGraph(5,xx,p0R); gp0R->SetMarkerStyle(20); gp0R->SetMarkerColor(kBlue-3);
  gp0L->Draw("AP");
  gp0L->GetYaxis()->SetRangeUser(-2,0);
  gp0L->SetTitle(";Threshold  [mV];A  [ns]");
  tex->DrawLatexNDC( 0.2, 0.8, "A + B / x + C x^{9} / 1e27" );
  tex->DrawLatexNDC( 0.8, 0.8, Form("%d",run) );
  gp0L->GetYaxis()->SetTitleSize(0.1);
  gp0L->GetYaxis()->SetTitleOffset(0.4);
  gp0L->GetYaxis()->SetLabelSize(0.1);
  gp0R->Draw("PSAME");

  main2->cd(2)->SetGridx(1);
  main2->cd(2)->SetGridy(2);
  TGraph *gp1L = new TGraph(5,xx,p1L); gp1L->SetMarkerStyle(20); gp1L->SetMarkerColor(kRed-3);
  TGraph *gp1R = new TGraph(5,xx,p1R); gp1R->SetMarkerStyle(20); gp1R->SetMarkerColor(kBlue-3);
  gp1L->Draw("AP");
  gp1L->GetYaxis()->SetRangeUser(10,130);
  gp1L->SetTitle(";Threshold  [mV];B  [ns*mV]");
  //tex->DrawLatexNDC( 0.2, 0.8, ". + B exp(-x/.)" );
  gp1L->GetYaxis()->SetTitleSize(0.1);
  gp1L->GetYaxis()->SetTitleOffset(0.4);
  gp1L->GetYaxis()->SetLabelSize(0.1);
  gp1R->Draw("PSAME");

  main2->cd(3)->SetGridx(1);
  main2->cd(3)->SetGridy(2);
  TGraph *gp2L = new TGraph(5,xx,p2L); gp2L->SetMarkerStyle(20); gp2L->SetMarkerColor(kRed-3);
  TGraph *gp2R = new TGraph(5,xx,p2R); gp2R->SetMarkerStyle(20); gp2R->SetMarkerColor(kBlue-3);
  gp2L->Draw("AP");
  gp2L->GetYaxis()->SetRangeUser(-100,-0.1);
  gp2L->SetTitle(";Threshold  [mV];C  [ns/mV^{9}]");
  //tex->DrawLatexNDC( 0.2, 0.8, ". + . exp(-x/C)" );
  gp2L->GetYaxis()->SetTitleSize(0.1);
  gp2L->GetYaxis()->SetTitleOffset(0.4);
  gp2L->GetYaxis()->SetLabelSize(0.1);
  gp2L->GetXaxis()->SetTitleSize(0.1);
  gp2L->GetXaxis()->SetTitleOffset(1.0);
  gp2L->GetXaxis()->SetLabelSize(0.1);
  gp2R->Draw("PSAME");
  main2->ls();
  
  TCanvas *mm = (TCanvas*) list3E->FindObject("ED4_Energy");
  mm->Draw();
  
  return 0;
}
