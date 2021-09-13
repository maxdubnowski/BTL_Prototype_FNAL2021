#include <iostream>
#include "testbeam.h"
#include "TString.h"

int main(int argn, char **argv) {
  if(argn!=2) return 1;
  TString argRun = argv[1];
  
  //int run = 46159;
  //int run = 44718;
  int run = argRun.Atoi();
  std::cout << "Run : " << run << std::endl;

  //TString ifile = Form("/Volumes/uva/testbeam_2021_may_copied/merged/Run_%d.root",run);
  //TString ifile = Form("Run_%d.root",run);
  //TString ifile = Form("/Volumes/uva/testbeam_2021_apr/merged/Run_%d.root",run);
  TString ifile = Form("/eos/cms/store/group/dpg_mtd/comm_mtd/TB/MTDTB_FNAL_Apr2021/DRS/withTracks/v4/Run_%d.root",run);
  TString ofile = Form("AnalysisOutput_%d.root",run);

  testbeam *tb = new testbeam(run);
  tb->SetInputFileName( ifile.Data() );
  tb->SetOutputFileName( ofile.Data() );
  tb->Process();
  
  return 0;
}
