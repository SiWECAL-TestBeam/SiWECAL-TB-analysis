//# Copyright 2020  Adrián Irles IJCLab (CNRS/IN2P3)

#include "TROOT.h"
#include "TFile.h"
#include "DecodedSLBAnalysis.cc"

void DummyDisplay(TString filename_in, TString output=""){


  filename_in=filename_in+".root";
  cout<<" Display of file: "<<filename_in<<endl;
  DecodedSLBAnalysis ss(filename_in);

  TString map="/home/calice/TB2020/commissioning/SiWECAL-TB-analysis-git/mapping/fev10_chip_channel_x_y_mapping.txt";

  for(int i_slboard=0; i_slboard<15; i_slboard++) {
    if(i_slboard==8 || i_slboard==12) map="/home/calice/TB2020/commissioning/SiWECAL-TB-analysis-git/mapping/fev11_cob_chip_channel_x_y_mapping.txt";
    else map="/home/calice/TB2020/commissioning/SiWECAL-TB-analysis-git/mapping/fev10_chip_channel_x_y_mapping.txt"; 
    
    ss.ReadMap(map,i_slboard);
  }

  //  ss.n_slboards=3;
  ss.QuickDisplay(output);
  gSystem->Exit(0);

}
