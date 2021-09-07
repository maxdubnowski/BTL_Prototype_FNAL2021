TGraphErrors* CreateRandom(int Npoints) {
  // generate points randomly from a line
  // y = 0.5 x + 3
  TGraphErrors *gr = new TGraphErrors( Npoints );
  double a_gen = 5; //slope
  double b_gen = 200; // intercept
  for(int i=0; i!=Npoints; ++i) {
    double xi = 5 + gRandom->Rndm()*10;
    double yi = a_gen * xi + b_gen;
    gr->SetPointX( i, xi );
    gr->SetPointY( i, gRandom->Gaus(yi,0.2) );
    gr->SetPointError( i, 0.0, 0.2 );
  }
  return gr;
}

int regresion() {
  int Npoints = 1000;
  TGraphErrors *gr = CreateRandom(Npoints);
  gr->Draw("A*");
  //===========================
  //===========================
  //===========================

  double Sx=0;
  double Sy=0;
  double Sxx=0;
  double Sxy=0;
  double S=0;
  for(int i=0; i!=Npoints; ++i) {
    double x = gr->GetPointX( i );
    double y = gr->GetPointY( i );
    double dx = gr->GetErrorX( i );
    double dy = gr->GetErrorY( i );
    S += 1/dy;
    Sx += x/dy;
    Sy += y/dy;
    Sxx += x*x/dy;
    Sxy += x*y/dy;
  }
  double den = Sx*Sx - S*Sxx;
  double intercept = Sxy*Sx - Sy*Sxx;
  double slope = Sy*Sx - Sxy*S;
  slope /= den;
  intercept /= den;

  double var_a = S / den;
  double var_b = Sxx / den;
  double covar = -Sx / den;
  double rab = covar / TMath::Sqrt( var_a*var_b );
  cout << slope << "+-" << var_a << endl;
  cout << intercept << "+-" << var_b << endl;
  cout << covar << endl;
  cout << rab << endl;
  return 0;
}
