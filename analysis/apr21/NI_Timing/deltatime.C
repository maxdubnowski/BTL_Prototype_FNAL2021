int deltatime(int run=44734 ) {
  TList *list[5], *list2, *list3;
  TH2D     *LR[5];
  TH2D     *XT[5];
  TH2D     *XTC[5];
  TH1D     *T[5];
  TH2D     *XT_P[5];
  TH1D     *T_P[5];
  list2 = (TList*) TFile::Open( Form("AnalysisOutput_%d.root",run) )->Get("output");
  list3 = (TList*) list2->FindObject( "timing" );
  for(int ithr=0; ithr!=5; ++ithr) {
    list[ithr] = (TList*) list3->FindObject( Form("thr_%d",ithr) );
    LR[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingLR_%d",ithr) );
    XT[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingX_%d",ithr) );
    XTC[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingXC_%d",ithr) );
    T[ithr] = (TH1D*) list[ithr]->FindObject( Form("fTiming_%d",ithr) );
    XT_P[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingX_P_%d",ithr) );
    T_P[ithr] = (TH1D*) list[ithr]->FindObject( Form("fTiming_P_%d",ithr) );
  }

  TCanvas *main = new TCanvas();
  main->Divide(5,2);
  TLatex *tex = new TLatex();
  tex->SetTextSize(0.1);
  TF1 *fit[5];
  TF1 *fit_P[5];
  for(int ithr=0; ithr!=5; ++ithr) {
    main->cd(1+ithr);
    LR[ithr]->Draw("colz");

    main->cd(6+ithr);
    T[ithr]->SetLineColor(kRed-3);
    T_P[ithr]->GetXaxis()->SetTitle("MIP TIMING  [ns]");
    T_P[ithr]->GetXaxis()->SetTitleSize(0.06);
    T_P[ithr]->GetXaxis()->SetTitleOffset(0.8);
    T_P[ithr]->GetXaxis()->SetLabelSize(0.05);
    T_P[ithr]->SetLineColor(kBlue-3);
    T_P[ithr]->Draw();
    double xce;
    xce = T_P[ithr]->GetBinCenter( T_P[ithr]->GetMaximumBin() );
    fit_P[ithr] = new TF1( Form("fit_P_%d",ithr), "gaus" );
    fit_P[ithr]->SetLineColor(kBlue-3);
    T_P[ithr]->Fit(fit_P[ithr],"R","",-0.1+xce,+0.1+xce);

    T[ithr]->Draw("same");
    xce = T[ithr]->GetBinCenter( T[ithr]->GetMaximumBin() );
    fit[ithr] = new TF1( Form("fit_%d",ithr), "gaus" );
    fit[ithr]->SetLineColor(kRed-3);
    T[ithr]->Fit(fit[ithr],"R","",-0.1+xce,+0.1+xce);

    tex->SetTextColor(kRed-3);
    tex->DrawLatexNDC( 0.15, 0.85, Form("#sigma^{-} %.0f ps",fit[ithr]->GetParameter(2)*1e3) );
    tex->SetTextColor(kBlue-3);
    tex->DrawLatexNDC( 0.15, 0.80, Form("#sigma^{+} %.0f ps",fit_P[ithr]->GetParameter(2)*1e3) );
  }

  double minus[5];
  double plus[5];
  for(int ithr=0; ithr!=5; ++ithr) {
    minus[ithr] = fit[ithr]->GetParameter(2)*1e3;
    cout << minus[ithr] << ",";
  }
  cout << endl;
  for(int ithr=0; ithr!=5; ++ithr) {
    plus[ithr] = fit_P[ithr]->GetParameter(2)*1e3;
    cout << plus[ithr] << ",";
  }
  cout << endl;

  TCanvas *main2 = new TCanvas();
  main2->SetLeftMargin(0.25);
  main2->SetBottomMargin(0.2);
  main2->Divide(5,2,0,0);
  for(int ithr=0; ithr!=5; ++ithr) {
    XT[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT[ithr]->GetYaxis()->SetLabelSize(0.1);

    XT_P[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT_P[ithr]->GetYaxis()->SetLabelSize(0.1);

    XT_P[ithr]->GetXaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetXaxis()->SetTitleOffset(0.85);
    XT_P[ithr]->GetXaxis()->SetLabelSize(0.1);
    XT_P[ithr]->GetXaxis()->SetNdivisions(505);

    main2->cd(1+ithr);
    XT[ithr]->Draw("colz");
    main2->cd(6+ithr);
    XT_P[ithr]->Draw("colz");
  }

  TCanvas *main3 = new TCanvas();
  main3->SetLeftMargin(0.25);
  main3->SetBottomMargin(0.2);
  main3->Divide(5,2,0,0);
  for(int ithr=0; ithr!=5; ++ithr) {
    XTC[ithr]->GetYaxis()->SetTitleSize(0.1);
    XTC[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XTC[ithr]->GetYaxis()->SetLabelSize(0.1);
    XTC[ithr]->GetYaxis()->SetTitle("L - R (CORR)  [ns]");
    
    XT_P[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT_P[ithr]->GetYaxis()->SetLabelSize(0.1);

    XT_P[ithr]->GetXaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetXaxis()->SetTitleOffset(0.85);
    XT_P[ithr]->GetXaxis()->SetLabelSize(0.1);
    XT_P[ithr]->GetXaxis()->SetNdivisions(505);

    main3->cd(1+ithr);
    XTC[ithr]->Draw("colz");
    main3->cd(6+ithr);
    XT_P[ithr]->Draw("colz");
  }

  return 0;
}
