#ifndef RAW2ROOT_CC
#define RAW2ROOT_CC

#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <sstream>
#include <string>
#include "InfoChip.cc"


// .x RAW2ROOT.C+

using std::cout;
using std::endl;

class RAW2ROOT {

public:
  RAW2ROOT(){
    recordEvent = false;
    _debug=false;

    chipIds.push_back(0x0000);//chip 2
    chipIds.push_back(0x0001);//chip 4
    chipIds.push_back(0x0002);//chip 3
    chipIds.push_back(0x0003);//chip 1
    chipIds.push_back(0x0004);//chip
    chipIds.push_back(0x0005);//chip
    chipIds.push_back(0x0006);//chip
    chipIds.push_back(0x0007);//chip
    chipIds.push_back(0x0008);//chip
    chipIds.push_back(0x0009);//chip
    chipIds.push_back(0x000A);//chip
    chipIds.push_back(0x000B);//chip
    chipIds.push_back(0x000C);//chip
    chipIds.push_back(0x000D);//chip
    chipIds.push_back(0x000E);//chip
    chipIds.push_back(0x000F);//chip
	
    //       chipIds.push_back(0x0010);//chip

    /*chipIds.push_back(0x0011);//chip 4
      chipIds.push_back(0x0012);//chip 3
      chipIds.push_back(0x0013);//chip 1
      chipIds.push_back(0x0014);//chip
      chipIds.push_back(0x0015);//chip
      chipIds.push_back(0x0016);//chip
      chipIds.push_back(0x0017);//chip
      chipIds.push_back(0x0018);//chip
      chipIds.push_back(0x0019);//chip
      chipIds.push_back(0x001A);//chip
      chipIds.push_back(0x001B);//chip
      chipIds.push_back(0x001C);//chip
      chipIds.push_back(0x001D);//chip
      chipIds.push_back(0x001E);//chip
      chipIds.push_back(0x001F);//chip
      chipIds.push_back(0x0020);//chip

      chipIds.push_back(0x0021);//chip 4
      chipIds.push_back(0x0022);//chip 3
      chipIds.push_back(0x0023);//chip 1
      chipIds.push_back(0x0024);//chip
      chipIds.push_back(0x0025);//chip
      chipIds.push_back(0x0026);//chip
      chipIds.push_back(0x0027);//chip
      chipIds.push_back(0x0028);//chip
      chipIds.push_back(0x0029);//chip
      chipIds.push_back(0x002A);//chip
      chipIds.push_back(0x002B);//chip
      chipIds.push_back(0x002C);//chip
      chipIds.push_back(0x002D);//chip
      chipIds.push_back(0x002E);//chip
      chipIds.push_back(0x002F);//chip
      chipIds.push_back(0x0030);//chip

      chipIds.push_back(0x0031);//chip 4
      chipIds.push_back(0x0032);//chip 3
      chipIds.push_back(0x0033);//chip 1
      chipIds.push_back(0x0034);//chip
      chipIds.push_back(0x0035);//chip
      chipIds.push_back(0x0036);//chip
      chipIds.push_back(0x0037);//chip
      chipIds.push_back(0x0038);//chip
      chipIds.push_back(0x0039);//chip
      chipIds.push_back(0x003A);//chip
      chipIds.push_back(0x003B);//chip
      chipIds.push_back(0x003C);//chip
      chipIds.push_back(0x003D);//chip
      chipIds.push_back(0x003E);//chip
      chipIds.push_back(0x003F);//chip
      chipIds.push_back(0x0040);//chip */

    info = new InfoChip(); R2Rstate=-1;

  };
  ~RAW2ROOT(){
    delete info;
  };
  void ReadFile(TString inputFileName,  bool overwrite=false,  int maxevt=99999999);

protected:

  enum {
    MEMDEPTH=15,
    NCHANNELS=64,
    NCHIP=16,
    CHIPHEAD=4,
    CHIPENDTAG=2,
    NEGDATA_THR=10 //event with data below are tagged badbcid+=32
  };

  int R2Rstate;

  int readEvent(std::vector < unsigned short int > & eventData);
  int data_integrity(std::vector < unsigned short int > & eventData, int i, int local_offset, int nColumns);
  void Initialisation();
  void analyse_hits();
  void analyse_dataintegrity();
  void plotHistos();
  void treeInit();
  void printEvent(std::vector < unsigned short int > & eventData);
  void DebugMode(bool t){ _debug=t; }
  void searchNmax(int N, int Size, Float_t * table, Float_t * tableout ,int * ranks);
  int  GetTree(TString rootfilename);
  int  BuildTimedEvents(TString rootfilename);

  TH1F* h_hit_high[NCHIP]; //hist of all hits per chip per channel
  TH1F* h_hit_filtered[NCHIP];//hist of good hits per chip per channel
  TH1F* h_hit_filtered_histo[NCHIP];//hist of nb of good hits per chip
  TH1F* h_nCol[NCHIP];
  TH1F* h_bcid[NCHIP];
  TH1D* h_TagHist[NCHIP];
  TH2D* TagHist;

  TH2I* h_chnnb;
  TH2I* h_chipnb;

  TH2F* h2_hits;
  TH2F* h2_hits_Acq;
  TH1F* h_hits_BCID[NCHIP][NCHANNELS];
  TH1F* h_col_Acq[NCHIP];

  // data integrity mistakes counter
  TH1F* h_dataIntegrity;
  //Data Integrity
  // value = 0 --> OK
  // value = 1 --> bad data size
  // value = 2 --> more than 15 memory columns
  // value = 3 --> bad chip number 
  // value = 4 --> extra bits in BCID (>12)
  // value = 5 --> extra bits in LOW GAIN  charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 6 --> extra bits in HIGH GAIN charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 7 --> hit bit from high gain != hit bit from low gain 
  // value = 8 --> gain = 1 but negative data 
  // value = 9 --> Bad number of columns or bad number of channels

  TDirectory *dPlots ;

  TFile* fout;
  TTree* tree;

  TFile *finroot;
  TTree* fev10read;

  int bcid[NCHIP][MEMDEPTH];
  int corrected_bcid[NCHIP][MEMDEPTH];
  int badbcid[NCHIP][MEMDEPTH];
  int nhits[NCHIP][MEMDEPTH];
  int charge_low[NCHIP][MEMDEPTH][NCHANNELS];
  int charge_high[NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_low[NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_high[NCHIP][MEMDEPTH][NCHANNELS];
  int event;
  int thr;
  //int numbcid;
  int numCol[NCHIP];
  int chipID[NCHIP];
  int acqNumber;

  //int allbcid[NCHIP*MEMDEPTH];

  bool recordEvent;
  bool _debug;

  std::vector <int> chipIds;

  InfoChip * info;
};

//******************************************************************************************************************

void RAW2ROOT::Initialisation() {
  fout->cd(); R2Rstate=-1;

  tree = new TTree("fev10","fev10");

  tree->Branch("event",&event,"event/I");
  tree->Branch("thr",&thr,"thr/I");
  tree->Branch("acqNumber",&acqNumber,"acqNumber/I");

  TString name;

  name = "chipid[";
  name+=NCHIP; name+="]/I";
  tree->Branch("chipid",chipID,name);

  name = "nColumns[";
  name+=NCHIP; name+="]/I";
  tree->Branch("nColumns",numCol,name);

  name = "bcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("bcid",bcid,name);

  name = "corrected_bcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("corrected_bcid",corrected_bcid,name);

  name = "badbcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("badbcid",badbcid,name);

  name = "nhits[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("nhits",nhits,name);

  name = "lowGain[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("charge_lowGain",charge_low,name);

  name = "highGain[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("charge_hiGain",charge_high,name);

  name = "gain_hit_low[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("gain_hit_low",gain_hit_low,name);

  name = "gain_hit_high[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("gain_hit_high",gain_hit_high,name);

  h2_hits = new TH2F("Hits_XY","Hits",32,-89.5,90-0.5,32,-89.5,90-0.5);
  h2_hits_Acq = new TH2F("Hits_XY_Acq","Hits per Acq",32,-89.5,90-0.5,32,-89.5,90-0.5);

  h_dataIntegrity = new TH1F("Data_Integrity","Data Integrity",10,-0.5,9.5);

  for (int j=0; j<NCHIP; j++) {
    name = "hits_chip_";
    name+=j+1;
    h_hit_high[j] = new TH1F(name,name,NCHANNELS,0,NCHANNELS);

    name = "hits_filtered_chip_";
    name+=j+1;
    h_hit_filtered[j] = new TH1F(name,name,NCHANNELS,0,NCHANNELS);

    name = "nCol_chip_";
    name+=j+1;
    h_nCol[j] = new TH1F(name,name,MEMDEPTH+1,0,MEMDEPTH+1);

    name = "bcid_chip_";
    name+=j+1;
    h_bcid[j] = new TH1F(name,name,4096,0,4096);

    name = "col_chip_Acq";
    name+=j+1;
    h_col_Acq[j] = new TH1F(name,name,4,1,5);

  }

  dPlots->cd();
  for (int j=0; j<NCHIP; j++) {
    for (int i=0; i<NCHANNELS; i++) {
      name = "chargeHigh_chip_";
      name+=j+1;
      name+="_channel_";
      name+=i;

      name+="_withHit";
      
      name = "chargeLow_chip_";
      name+=j+1;
      name+="_channel_";
      name+=i;
      
      name+="_withHit";
      
      name = "hits_chip_";
      name+=j+1;
      name+="_channel_";
      name+=i;
      name+="_fct_BCID";
      h_hits_BCID[j][i] = new TH1F(name,name,4096,0,4096);
    }
    name = "TagHist_";
    name+=j+1;
    h_TagHist[j]  = new  TH1D(name, name,16,-0.5,15.5);
  }


  // map of channel number
  fout->cd();
  h_chnnb = new TH2I("Map_chn_nb","Map_chn_nb",32,-89.5,90-0.5,32,-89.5,90-0.5);
  h_chipnb = new TH2I("Map_chip_nb","Map_chip_nb",32,-89.5,90-0.5,32,-89.5,90-0.5);
  for(int chip = 0;chip<NCHIP;chip++){
    for(int chn = 0;chn<NCHANNELS;chn++){
      h_chnnb->Fill(info->GetX(chip,chn),info->GetY(chip,chn),chn);
      h_chipnb->Fill(info->GetX(chip,chn),info->GetY(chip,chn),chip);     
    }
  }
  // map of channel number

  fout->cd();

  cout<<"End Initialisation"<<endl;

  return;
}






//******************************************************************************************************************

void RAW2ROOT::treeInit() { //init data for a single SPILL ?

  for (int k=0; k<NCHIP; k++) {
    for (int i=0; i<MEMDEPTH; i++) {
      //allbcid[i+k*MEMDEPTH]=0;
      bcid[k][i]=-999;
      badbcid[k][i]=0;
      corrected_bcid[k][i]=-999;
      nhits[k][i]=-999;
      for (int j=0; j<NCHANNELS; j++) {
	charge_low[k][i][j]=-999;
	charge_high[k][i][j]=-999;
	gain_hit_low[k][i][j]=-999;
	gain_hit_high[k][i][j]=-999;
      }
    }

    chipID[k]=-999;
    numCol[k]=-999;
  }

  recordEvent = true;

  return;
}


//******************************************************************************************************************
void RAW2ROOT::searchNmax(int N, int Size, Float_t * table, Float_t * tableout , int * ranks) {
  //assumes positive numbers, tableout and ranks initialized to 0
  int i=0;
  for (int dat=1;dat<Size+1;dat++){
    i=-1;
    while (i<N-1) {
      if (table[dat]>tableout[i+1]) {i++;} else {break;}
    }
    if (i>0) {
      for (int j=0;j<i;j++){tableout[j]=tableout[j+1]; ranks[j]=ranks[j+1];}
      tableout[i]=table[dat]; ranks[i]=dat;
    } else if (i==0) {tableout[i]=table[dat]; ranks[i]=dat;}
  }

  return;}


void RAW2ROOT::analyse_dataintegrity() {

  cout<<" "<<endl;
  cout<< " DATA INTEGRITY SUMMARY "<<endl;
  cout<< " total number of spills = "<< h_dataIntegrity->GetEntries() << endl;
  cout<< " TOTALGOOD  "<< 100.*h_dataIntegrity->GetBinContent(1)/h_dataIntegrity->GetEntries() << " %   are spills with acceptable data"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(2)/h_dataIntegrity->GetEntries() << " %   have bad data size"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(3)/h_dataIntegrity->GetEntries() << " %   have more than 15 SCA"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(4)/h_dataIntegrity->GetEntries() << " %   have bad chip number"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(5)/h_dataIntegrity->GetEntries() << " %   have extra bits in BCID"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(6)/h_dataIntegrity->GetEntries() << " %   have extrabits in low gain."<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(7)/h_dataIntegrity->GetEntries() << " %   have extrabits in high gain"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(8)/h_dataIntegrity->GetEntries() << " %   have different hit bit for low and high gain"<<endl;
  // cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(9)/h_dataIntegrity->GetEntries() << " %   gain = 1 but charge <10"<<endl;
  cout<< " bad -- " << 100.*h_dataIntegrity->GetBinContent(9)/h_dataIntegrity->GetEntries() << " %   bad number of SCA or channels"<<endl;

}

void RAW2ROOT::analyse_hits() {
  // for each chip one look to the mean ratio of  (hits)/(total events) and (good hits)/(hits)
  // good hits are identified according to algo in readEvent function
  // total events is the cumulated sum of columns over spills
  TString name;
  cout << "ANALYSE HITS"<<endl;

  //one get total events using sum(bin x bin value) from the histo of the number of columns
  Double_t TotEvents[NCHIP]; //Double_t
  Double_t UnFilteredEvents[NCHIP]; //Double_t
  for (int nchip=0; nchip<NCHIP; nchip++) {
    TotEvents[nchip]=0;
    UnFilteredEvents[nchip]=0;
    for (int nbin=2;nbin<17;nbin++) {
      TotEvents[nchip]=TotEvents[nchip]+ (nbin-1)*h_nCol[nchip]->GetBinContent(nbin) ; //ncol=nbin-1
    }

    UnFilteredEvents[nchip] = TotEvents[nchip] - h_TagHist[nchip]->GetEntries();
    
    cout << "   get " << UnFilteredEvents[nchip] << " out of " << TotEvents[nchip] << " events for chip " << nchip ;
    cout << "  (" << UnFilteredEvents[nchip]*100/TotEvents[nchip] << "%)"<<endl;
  }
  
  //map of Tagged events, % of tagged evt per chip per column
  name = "TagHist";
  TagHist  = new  TH2D(name, name, NCHIP,-0.5,NCHIP-0.5,15,0.5,15.5);
  for (int nchip=0; nchip<NCHIP; nchip++) {
    Double_t *bins = h_TagHist[nchip]->GetArray();
    for (int col=1; col<16; col++) {//bins start at index 1.
      TagHist->SetBinContent(nchip+1,col, bins[col]*100/ TotEvents[nchip] );
    }
  }

  //map of hit per chip per channel, normalized to the total number of event
  name = "HitMapHist";
  TH2D* HitMapHist  = new  TH2D(name, name, 16,-0.5,15.5,64,-0.5,63.5);
  name = "HitMapFilteredHist";
  TH2D* HitMapFilteredHist  = new  TH2D(name, name, 16,-0.5,15.5,64,-0.5,63.5);

  for (int nchip=0; nchip<NCHIP; nchip++) {
    Float_t *bins = h_hit_high[nchip]->GetArray();
    Float_t *binsfiltered = h_hit_filtered[nchip]->GetArray();
    for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
      HitMapHist->SetBinContent(nchip+1,chn+1, bins[chn+1]);
      HitMapFilteredHist->SetBinContent(nchip+1,chn+1, binsfiltered[chn+1]);
    }
  }

  cout<<" ------------------------------- "<<endl;
  return;
}



//******************************************************************************************************************

void RAW2ROOT::plotHistos() {
  h2_hits_Acq->Scale(1./event);
  h_col_Acq[0]->Scale(1./event);
  h_col_Acq[1]->Scale(1./event);
  h_col_Acq[2]->Scale(1./event);
  h_col_Acq[3]->Scale(1./event);

  fout->cd();
  fout->Write(0);
  fout->Close();

  return;
}

//******************************************************************************************************************

void RAW2ROOT::printEvent(std::vector < unsigned short int > & eventData) {
  if (_debug) {
    cout << "printing event for debug" << endl;
    for (unsigned int i=0; i<eventData.size(); i++) {
      cout << i << " 0x" << hex << eventData[i] << " "<< dec <<int(eventData[i] & 0x0fff) << endl;
    }
  }
  return;
}

//******************************************************************************************************************

int RAW2ROOT::readEvent(std::vector < unsigned short int > & eventData) {

  // ASSUMES HIGH/LOW GAIN mode  !!!
  // ----------------------

  //Data Integrity
  // value = 0 --> OK
  // value = 1 --> bad data size
  // value = 2 --> more than 15 memory columns
  // value = 3 --> bad chip number 
  // value = 4 --> extra bits in BCID (>12)
  // value = 5 --> extra bits in LOW GAIN  charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 6 --> extra bits in HIGH GAIN charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 7 --> hit bit from high gain != hit bit from low gain 
  // value = 8 --> gain = 1 but negative data 
  // value = 9 --> Bad number of columns or bad number of channels

  // check we have a decent amount of data

  printEvent(eventData);
  
  unsigned short int last=0;
  //unsigned int previousChipEndIndex = 0;
  unsigned int chipStartIndex = 0;

  int rawDataSize=0;
  int nColumns = 0;
  int local_offset = 0; //adapt to redundant chip ID word, value can be 0 or 1
  int previousBCID = -999;

  bool isValidChip = false;

  for(unsigned int i=0;i<eventData.size();i++){

    if( eventData[i] == 0xfffd){// find start chip tag
      chipStartIndex=i+CHIPHEAD;
      if (_debug) cout << "   start chip";
    }

    if(last == 0xfffe){// find end chip tag

      if( (eventData[i] > 0xff00) & (eventData[i] < 0xffff)){// chips was fffc
	isValidChip=true;
      }

	    

      if(isValidChip){
	const int offset=1; //was 2
	rawDataSize = i-chipStartIndex-CHIPENDTAG;
	local_offset = (rawDataSize-offset)%(1+NCHANNELS*2);
	
	if(local_offset!=0){
	  cout<<"<!> WARNING <!> Additionnal data words detected"<<endl;
	  //  last=eventData[i];
	}

	nColumns = (rawDataSize-offset-local_offset)/(1+NCHANNELS*2);

	if (_debug) cout<<"Chip data: "  <<" size: "<<rawDataSize<<" col: "<<nColumns<<" evt: "<<(rawDataSize-offset)%(1+NCHANNELS*2)
			<<"   local_offset :"<< local_offset  << endl;

	if((rawDataSize-offset-local_offset)%(1+NCHANNELS*2)!=0){
	  if(_debug) cout<<"<!> ERROR <!> Bad data size"<<endl;
	  last=eventData[i];
	  return 1;
	}

	if (nColumns>MEMDEPTH){
	  if(_debug) cout << "<!> ERROR <!> Bad number of columns" <<endl;
	  last=eventData[i];
	  return 2;
	}

	if(i<2) return 9;

	//test of the chip id
	int chipid = eventData[i-2];//was 3
	//irles
	bool isGoodChipNumber=false;
	for(unsigned int iChip = 0 ; iChip<chipIds.size();iChip++){
	  if(chipid==chipIds[iChip]) isGoodChipNumber=true;
	}
	if(!isGoodChipNumber){
	  if(_debug) cout << "<!> ERROR <!> Bad Chip ID: " << chipid <<endl;
	  last=eventData[i];
	  return 3;
	}

	//data integrity 
	int data_integ=data_integrity(eventData,i,local_offset,nColumns);
	if(data_integ>0) return data_integ;

	// ----------------------
	//Fill variables of the tree only if the data is okay
	int c = info->GetASUChipNumberFromChipID(chipid);
	chipID[chipid]=chipid;
	numCol[chipid]= nColumns;

	previousBCID = -999;

	int loopBCID = 0;
	// now loop over the data, fill the charge values
	for (int ibc=0; ibc<nColumns; ibc++) {

	  //fill BCID
	  bcid[chipid][ibc]=eventData[i-3-1*local_offset-ibc] & 0x0fff ;  // !!!   index to be verified   !!!

	  if(bcid[chipid][ibc]-previousBCID < 0) loopBCID++;
	  corrected_bcid[chipid][ibc] = bcid[chipid][ibc]+loopBCID*4096;

	  h_bcid[c]->Fill(bcid[chipid][ibc]);

	  // fill the charges
	  int ichan(0);
	  // range for this column
	  int begin =  i-3 - 1*local_offset - nColumns -  ibc*NCHANNELS*2; // !!!   index to be verified   !!!
	  int end = begin - NCHANNELS;
	  for (int jj = begin; jj>end; jj--) {

	    if (ibc<MEMDEPTH && ichan<NCHANNELS){
	      charge_low[chipid][ibc][ichan]=eventData[jj] & 0x0fff;
	      gain_hit_low[chipid][ibc][ichan]= (eventData[jj] >> 12 ) & 0xf;
	    }
	    else {
	      cout << "<!> ERROR <!> Low Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
	    }
	    ichan++;
	  }

	  // analyse hits and bcids
	  begin=end;
	  end=begin - NCHANNELS;
	  ichan=0;
	  int count_hits = 0;
	  for (int jj = begin; jj>end; jj--) {

	    if (ibc<MEMDEPTH && ichan<NCHANNELS){
	      charge_high[chipid][ibc][ichan]=eventData[jj] & 0x0fff;
	      gain_hit_high[chipid][ibc][ichan] = (eventData[jj] >> 12 ) & 0xf;
	      if(gain_hit_high[chipid][ibc][ichan]==1 && charge_high[chipid][ibc][ichan]>NEGDATA_THR ){
		count_hits++;
	      }
	    }  else {
	      cout << "<!> ERROR <!> High Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
	    }
	    ichan++;
	  }

	  nhits[chipid][ibc]=count_hits;
	  previousBCID = bcid[chipid][ibc];
	}
	isValidChip=false;

	//print event info
	if(_debug) {
	  for(int ibc=0; ibc<nColumns; ibc++)
	    cout<< "bcid = "<<bcid[chipid][ibc]<<endl;
	  
	  for(int ibc=0; ibc<nColumns; ibc++) {
	    for(int ich=0; ich<NCHANNELS; ich++) {
	      cout<< chipid <<" "<< ibc <<" "<< ich << " low = "<<charge_high[chipid][ibc][ich]<< " high = "<<charge_low[chipid][ibc][ich]<<endl;
	    }
	  }
	} //

      }
    }
    // ----------------------

    last=eventData[i];
    
  }

  //add tags
  int  count_negdata=0;

  for (int k=0; k<NCHIP; k++) {
    //only for valid chips in this spill
    if (chipID[k]>=0) {
      for (int ibc=0; ibc<numCol[k]; ibc++) {

	//tag successive bcids
	if (ibc==0) {
	  badbcid[k][ibc]=0;  //first col is always ok
	  for (int ichan=0; ichan<NCHANNELS; ichan++) {
	    if(gain_hit_high[k][ibc][ichan]%2==1){
	      h_hit_filtered[k]->Fill(ichan);
	    }
	  }
	} else {
	  for (int ichan=0; ichan<NCHANNELS; ichan++) {
	    if(gain_hit_high[k][ibc][ichan]%2==1){
	      h_hit_filtered[k]->Fill(ichan);
	    }
	  }
	  //irles 
	  if( ( corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1]) > 0  &&  (corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1]) < 16 ) {
	    badbcid[k][ibc]=corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1];
	  } else {
	    badbcid[k][ibc]=0; 
	  }
	}//if ibc == 0

	//tag zero data
	count_negdata=0;
	
	for (int ichan=0; ichan<NCHANNELS; ichan++) {
	  if  (charge_high[k][ibc][ichan] < NEGDATA_THR) count_negdata++;
	}//ichan

	if (count_negdata>0) {badbcid[k][ibc]+=32;}


      }//ibc
    }//chipID
  }//k

  return 0;
}



int RAW2ROOT::data_integrity(std::vector < unsigned short int > & eventData, int i, int local_offset, int nColumns){

  if (_debug) cout << "DataIntegrity: analysing event with " << eventData.size() << " entries " << event << endl;

  // ASSUMES HIGH/LOW GAIN mode  !!!
  // ----------------------
    
  //Data Integrity
  // value = 0 --> OK
  // value = 1 --> bad data size
  // value = 2 --> more than 15 memory columns
  // value = 3 --> bad chip number 
  // value = 4 --> extra bits in BCID (>12)
  // value = 5 --> extra bits in LOW GAIN  charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 6 --> extra bits in HIGH GAIN charge/hbits --> expected 13 bits, no more. The 14th is for autogain mode --> not used
  // value = 7 --> hit bit from high gain != hit bit from low gain 
  // value = 8 --> Bad number of columns or bad number of channels
    
  int check_data;
    
  //create vectors of high/low gain hit bits for comparison 
  int gain_high[15][64], gain_low[15][64];
  for(int ibcid=0; ibcid<15; ibcid++) {
    for(int ichan=0; ichan<64; ichan++) {
      gain_high[ibcid][ichan]=-999;
      gain_low[ibcid][ichan]=-999;
    }
  }
    

  for (int ibc=0; ibc<nColumns; ibc++) {
    
    if( int(eventData[i-3-1*local_offset-ibc] & 0xf000) !=0 ) {
      check_data=4; //extra bits
      cout << "<!> DataIntegrity ERROR <!> Extra bits in BCID " << ibc <<  endl;
      return check_data;
    }
    
    // fill the charges
    int ichan(0);
    // range for this column
    int begin =  i-3 - 1*local_offset - nColumns -  ibc*NCHANNELS*2; // !!!   index to be verified   !!!
    int end = begin - NCHANNELS;

    for (int jj = begin; jj>end; jj--) {     
      if (ibc<MEMDEPTH && ichan<NCHANNELS){
	gain_low[ibc][ichan]=(eventData[jj] >> 12 ) & 0xf;
	int charge = int(eventData[jj] & 0x0fff);

	if( gain_low[ibc][ichan] > 1 ) {
	  check_data=5; //extra bits
	  cout << "<!> DataIntegrity ERROR <!> Extra bits in LOW GAIN " << ibc <<  " "<< gain_low[ibc][ichan] << endl;
	  return check_data;
	}
	/*if( gain_low[ibc][ichan] ==1 && charge < NEGDATA_THR ) {
	  check_data=8; //negative data but hit bit = 1
	  cout << "<!> DataIntegrity ERROR <!> HIT HIGH LOW ==1 but negative data" << gain_low[ibc][ichan]<<" "<< charge <<  endl;
	  return check_data;
	  }*/
      }	else {
	check_data=8;
	cout << "<!> DataIntegrity ERROR <!> Low Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
	return check_data;
      }
      ichan++;
    }
    
    // analyse hits and bcids
    begin=end;
    end=begin - NCHANNELS;
    ichan=0;
    int count_hits = 0;
    for (int jj = begin; jj>end; jj--) {
      
      if (ibc<MEMDEPTH && ichan<NCHANNELS){
	gain_high[ibc][ichan] = (eventData[jj] >> 12 ) & 0xf;
	int charge = int(eventData[jj] & 0x0fff);

	if( gain_high[ibc][ichan] > 1 ) {
	  check_data=6; //extra bits
	  cout << "<!> DataIntegrity ERROR <!> Extra bits in HIGH GAIN " << ibc << " "<< gain_high[ibc][ichan] << endl;
	  return check_data;
	}
	if( gain_high[ibc][ichan] != gain_low[ibc][ichan] ) {
	  check_data=7; //extra bits
	  cout << "<!> DataIntegrity ERROR <!> HIT HIGH GAIN != HIT LOW GAIN for ibc="<<ibc<<" ichan="<<ichan << gain_high[ibc][ichan]<<" "<< gain_low[ibc][ichan] <<  endl;
	  return check_data;
	}
	/*if( gain_high[ibc][ichan] ==1 && charge <NEGDATA_THR ) {
	  check_data=8; //negative data but hit bit = 1
	  cout << "<!> DataIntegrity ERROR <!> HIT HIGH GAIN ==1 but negative data" << gain_high[ibc][ichan]<<" "<< charge <<  endl;
	  return check_data;
	  }*/
      }  else {
	check_data=8;
	cout << "<!> DataIntegrity ERROR <!> High Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
	return check_data;
      }
      ichan++;
    }
  }

  return 0;
  // ---------------------- 
}
//******************************************************************************************************************

void RAW2ROOT::ReadFile(TString inputFileName, bool overwrite,int maxevt) {

cout <<endl;
    cout << "            **** READING FILE " << inputFileName << " ****"<<endl;

    // read threshold value from event number
    if(inputFileName.Contains("_trig")){
	thr = std::stoi(inputFileName(inputFileName.Index("_trig")+5,3));
	cout << endl << "Opened file with threshold\t" << thr << endl <<  endl;
    }

    event=0;
    acqNumber=0;

    if(!overwrite){
        fout = new TFile(inputFileName+".root","create");
        if(!fout->IsOpen()){
            cout<<"<!> ERROR <!>   File already created!"<<endl;
            return;
        }
    }
    else {
        fout = new TFile(inputFileName+".root","recreate");
    }

    dPlots = fout->mkdir("plots");

    Initialisation();
    ifstream fin;
    unsigned short int dataResult=0;


    //int rawDataSize=0;
    //int nColumns = 0;
    bool outlog = false;
    bool altTag = false;
    int countchipdata=0;
    int countchip=0;

    fin.open(inputFileName.Data(), ios_base::in|ios_base::binary);
    //int nline=0;
    std::vector < unsigned short int > packetData;
    std::vector < unsigned short int > eventData;
    unsigned short int lastTwo[2]={0};

    if(fin.is_open())
        while (fin.read((char *)&dataResult, sizeof dataResult) && event<maxevt ) {

            packetData.push_back(dataResult);countchipdata++;

            if(dataResult == 0xFFFE) {//END of CHIP
                altTag = true;
            }

            if(lastTwo[1] == 0xFFFC){
                if (outlog) cout << "   SPILL "<< lastTwo[0] << " " << dataResult << " START--->" <<endl;
                countchip=0;
            }
            if(lastTwo[1] == 0xFFFD){
                if (outlog) cout << "       CHIP "<< lastTwo[0] - 65280 << " Begin..."; countchipdata=0;
            }

            if(lastTwo[1] == 0xFFFE){
                if (outlog) cout << "...End CHIP "<< lastTwo[0] - 65280 << " with " << countchipdata-2-3 << " words" <<endl;
                if (countchipdata>100) {countchip++;};
            }

            if(lastTwo[1] == 0xFFFF){
                if (outlog) cout << "   ----->END SPILL "<< lastTwo[0] << " " << dataResult << "  with " << countchip << " chips"<< endl<<acqNumber<<endl;
                packetData.clear();
                lastTwo[0]=lastTwo[1]=dataResult=0;
                altTag = false;
            }


            if(lastTwo[1] == 0x2020 && lastTwo[0] == 0x2020 && dataResult == 0xFFFF){// SPILL END
                //cout<<"SPILL END= "<<packetData.size()<<endl;
	      if(recordEvent){
		if (countchip>0){
		  int data_int = readEvent(packetData);
		  h_dataIntegrity->Fill(data_int);
		  if(_debug) cout<<" Data Int = "<<data_int<<endl;
		  if(data_int==0) {
		    event++;
		    tree->Fill();

		    //QUICK ANALISYS HISTORGRAMS
		    for(int nc=0; nc<NCHIP; nc++) {
		      int c = info->GetASUChipNumberFromChipID(nc);
		      if(numCol[c]>0 ) {
			h_nCol[c]->Fill(numCol[c]);
			h_col_Acq[c]->Fill(numCol[c]);
		      }
		      for(int ibc=0; ibc<MEMDEPTH; ibc++) {
			for( int ichan=0; ichan<NCHANNELS; ichan++){
			  if(gain_hit_high[nc][ibc][ichan]==1 && charge_high[nc][ibc][ichan]>NEGDATA_THR ){
			    h_hit_high[c]->Fill(ichan);
			    h2_hits->Fill(info->GetX(c,ichan),info->GetY(c,ichan) );
			    h2_hits_Acq->Fill(info->GetX(c,ichan),info->GetY(c,ichan) );
			    h_hits_BCID[c][ichan]->Fill(bcid[nc][ibc]);
			  }
			}
		      }
		    } // end quick analisys
		  }// end good data block
		}
	      }
	    }
	    
            if(lastTwo[1] == 0x5053 && lastTwo[0] == 0x4C49 && dataResult == 0x2020){ //New SPILL: extract SPILL number
	      if(altTag)
		cout<<"<!> WARNING <!> New spill without end flag of the previous spill - some chips found "<<endl;
	      acqNumber= packetData[packetData.size()-5]*65536+packetData[packetData.size()-4];
	      
	      treeInit();
	      packetData.clear();
	      lastTwo[0]=lastTwo[1]=dataResult=0;
	      altTag = false;
	      
            }

            lastTwo[1] = lastTwo[0];
            lastTwo[0] = dataResult;

        }


    cout <<endl;
    cout << "           **** Finished reading file ****" << endl;
    cout << "           ****  with  "<< tree->GetEntries()<< "  entries  ****" << endl;
    cout <<endl;

    cout << " FILEINTEGRITY "<< inputFileName<< " "<< h_dataIntegrity->GetBinContent(1)/h_dataIntegrity->GetEntries()<<endl;
    analyse_dataintegrity();
    analyse_hits();
    plotHistos();

    return;
}

  //******************************************************************************************************************






  ////////////////////////  GETTREE   ////////////////////////
  int RAW2ROOT::GetTree (TString rootfilename) { //from raw2root 1st pass
    //open pre-precessed data
    finroot = new TFile(rootfilename,"read");
    if (! finroot ) {return 0;}
    fev10read = (TTree*)finroot->Get("fev10"); //rawdata

    //get data from file
    fev10read->SetBranchAddress("gain_hit_high",gain_hit_high );
    fev10read->SetBranchAddress("gain_hit_low",gain_hit_low );
    fev10read->SetBranchAddress("charge_hiGain",charge_high );
    fev10read->SetBranchAddress("charge_lowGain",charge_low );
    fev10read->SetBranchAddress("bcid", bcid);
    fev10read->SetBranchAddress("badbcid", badbcid);
    fev10read->SetBranchAddress("nhits", nhits);
    fev10read->SetBranchAddress("event", &event);
    fev10read->SetBranchAddress("thr", &thr);
    fev10read->SetBranchAddress("acqNumber", &acqNumber);
    fev10read->SetBranchAddress("corrected_bcid",corrected_bcid);
    fev10read->SetBranchAddress("nColumns",numCol);
    fev10read->SetBranchAddress("chipid",chipID);

    cout << "PlotFool::GetTree : Get tree from "<< rootfilename.Data() << endl;

    R2Rstate=1;
    return 1;

  }//method GetTree




  ////////////////////////  BUILDTIMEDEVENTS   ////////////////////////
  int RAW2ROOT::BuildTimedEvents (TString rootfilename) {
    if (R2Rstate<0) return 0;
    TTree* fev10write;

    int Wevent;
    int Wthr;
    int WacqNumber;
    int Wbcid;
    int Wsca;
    int Wcorrected_bcid;
    int Wbadbcid[NCHIP];
    int Wnhits[NCHIP];
    int Wcharge_low[NCHIP][NCHANNELS];
    int Wcharge_high[NCHIP][NCHANNELS];
    int Wgain_hit_low[NCHIP][NCHANNELS];
    int Wgain_hit_high[NCHIP][NCHANNELS];

    int REFbcid = 0;
    bool LASTbcid = false;

    TString name;


    if (! GetTree(rootfilename)) {
      cout << "RAW2ROOT ERROR: "<< rootfilename.Data() << " not found" << endl;
      return 0;
    }


    //create new file
    TFile *fout = new TFile(rootfilename.ReplaceAll(".root","_TimeEvent.root"),"recreate");

    //create new tree
    fev10write = new TTree("fev10","fev10");
    //create branches
    fev10write->Branch("event",&Wevent,"event/I");
    fev10write->Branch("thr",&Wthr,"thr/I");
    fev10write->Branch("sca",&Wsca,"sca/I");
    fev10write->Branch("bcid",&Wbcid,"bcid/I");
    fev10write->Branch("corrected_bcid",&Wcorrected_bcid,"corrected_bcid/I");
    fev10write->Branch("acqNumber",&WacqNumber,"acqNumber/I");

    name = "badbcid[";
    name+=NCHIP;  name+="]/I";
    fev10write->Branch("badbcid",Wbadbcid,name);

    name = "nhits[";
    name+=NCHIP;  name+="]/I";
    fev10write->Branch("nhits",Wnhits,name);

    name = "lowGain[";
    name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
    fev10write->Branch("charge_lowGain",Wcharge_low,name);

    name = "highGain[";
    name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
    fev10write->Branch("charge_hiGain",Wcharge_high,name);

    name = "gain_hit_low[";
    name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
    fev10write->Branch("gain_hit_low",Wgain_hit_low,name);

    name = "gain_hit_high[";
    name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
    fev10write->Branch("gain_hit_high",Wgain_hit_high,name);



    for(unsigned entry = 0 ;entry<fev10read->GetEntries();entry++){
      fev10read->GetEntry(entry);

    }//entry



    return 1;
  }//method BuildTimedEvents

#endif
