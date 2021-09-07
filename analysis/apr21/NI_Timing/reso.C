void format(TGraph *a, TGraph *b, int color){
  a->SetLineColor( color );
  a->SetMarkerColor( color );
  b->SetLineColor( color );
  b->SetMarkerColor( color );
  a->SetMarkerStyle(20);
  b->SetMarkerStyle(20);
}
//==================
int reso() {
  gStyle->SetOptStat(0);
  double x[5] = {5,10,30,50,100};

  double m34[5] = {128.145,98.9457,103.23,109.061,127.513};
  double p34[5] = {84.2846,74.7181,75.8574,78.4029,84.3762};
  for(int i=0; i!=5; ++i) m34[i] /= 2;
  TGraph *grm34 = new TGraph(5,x,m34);
  TGraph *grp34 = new TGraph(5,x,p34);
  format(grm34,grp34,kBlue-3);

  double m33[5] = {110.128,74.9833,72.4495,76.5814,89.6727};
  double p33[5] = {73.8205,72.1838,66.5194,67.1155,69.9206};
  for(int i=0; i!=5; ++i) m33[i] /= 2;
  TGraph *grm33 = new TGraph(5,x,m33);
  TGraph *grp33 = new TGraph(5,x,p33);
  format(grm33,grp33,kCyan-3);

  double m31[5] = {92.5847,70.5116,66.8045,71.5032,78.9993};
  double p31[5] = {80.9279,69.9033,63.7723,63.4369,67.3793};
  for(int i=0; i!=5; ++i) m31[i] /= 2;
  TGraph *grm31 = new TGraph(5,x,m31);
  TGraph *grp31 = new TGraph(5,x,p31);
  format(grm31,grp31,kGreen-3);

  double m27[5] = {80.5899,69.839,66.5332,67.862,75.7893};
  double p27[5] = {80.9042,64.8224,61.5857,58.3342,57.9569};
  for(int i=0; i!=5; ++i) m27[i] /= 2;
  TGraph *grm27 = new TGraph(5,x,m27);
  TGraph *grp27 = new TGraph(5,x,p27);
  format(grm27,grp27,kYellow-3);

  double m25[5] = {83.059,63.9306,59.8795,60.4385,64.9335};
  double p25[5] = {82.2757,71.967,64.0389,59.5724,59.1083};
  for(int i=0; i!=5; ++i) m25[i] /= 2;
  TGraph *grm25 = new TGraph(5,x,m25);
  TGraph *grp25 = new TGraph(5,x,p25);
  format(grm25,grp25,kOrange-3);

  double m30[5] = {79.293,68.0546,61.7924,62.5198,67.8764};
  double p30[5] = {79.1361,70.6351,63.2411,65.2639,63.4254};
  for(int i=0; i!=5; ++i) m30[i] /= 2;
  TGraph *grm30 = new TGraph(5,x,m30);
  TGraph *grp30 = new TGraph(5,x,p30);
  format(grm30,grp30,kMagenta-3);

  double m28[5] = {72.276,60.0731,56.0422,57.4935,61.2038};
  double p28[5] = {80.1294,65.5924,55.2213,64.6731,62.8954};
  for(int i=0; i!=5; ++i) m28[i] /= 2;
  TGraph *grm28 = new TGraph(5,x,m28);
  TGraph *grp28 = new TGraph(5,x,p28);
  format(grm28,grp28,kRed-3);

  double m32[5] = {83.4132,64.2563,56.4775,59.8568,64.3872};
  double p32[5] = {79.2541,72.9806,66.9955,67.4072,68.707};
  for(int i=0; i!=5; ++i) m32[i] /= 2;
  TGraph *grm32 = new TGraph(5,x,m32);
  TGraph *grp32 = new TGraph(5,x,p32);
  format(grm32,grp32,kBlack);

  
  TH2D *axis1 = new TH2D("axis1","MINUS;Threshold  [mV];Resolution  [ps]",100,0,110,100,10,100);
  TH2D *axis2 = new TH2D("axis2","PLUS;Threshold  [mV];Resolution  [ps]",100,0,110,100,10,100);

  TCanvas *main = new TCanvas();
  main->Divide(2,1);
  
  main->cd(1);
  axis1->Draw();
  grm34->Draw("PLSAME");
  grm33->Draw("PLSAME");
  grm31->Draw("PLSAME");
  grm27->Draw("PLSAME");
  grm25->Draw("PLSAME");
  grm30->Draw("PLSAME");
  grm28->Draw("PLSAME");
  grm32->Draw("PLSAME");

  main->cd(2);
  axis2->Draw();
  grp34->Draw("PLSAME");
  grp33->Draw("PLSAME");
  grp31->Draw("PLSAME");
  grp27->Draw("PLSAME");
  grp25->Draw("PLSAME");
  grp30->Draw("PLSAME");
  grp28->Draw("PLSAME");
  grp32->Draw("PLSAME");

  return 0;
}
