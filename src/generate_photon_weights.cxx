#ifndef __photon_analyze_cxx__
#define __photon_analyze_cxx__

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TLine.h>
#include <TProfile.h>
#include <TLorentzVector.h>

#include "fastjet/ClusterSequence.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <math.h>

#include <AtlasUtils.h>

#include <GlobalParams.h>
#include <Utilities.h>

#include "header.h"

using namespace PythiaCoMStudy;




TH1D* GetPhotonSpectrum (TTree* inTree, const char* name) {
  float photon_pt = 0;
  const int nEvents = inTree->GetEntries ();
  inTree->SetBranchAddress ("photon_pt",   &photon_pt);

  const int nPtGBins = 60;
  const double* pTGBins = logspace (60, 300, nPtGBins);
  TH1D* h_g_pt_yield = new TH1D (Form ("h_g_pt_yield_%s", name), ";#it{p}_{T}^{#gamma} [GeV]", nPtGBins, pTGBins);
  h_g_pt_yield->Sumw2 ();

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    inTree->GetEntry (iEvent);

    h_g_pt_yield->Fill (photon_pt);
  } // end loop over iEvent

  h_g_pt_yield->Scale (1./((double)nEvents), "width");
  return h_g_pt_yield;
}




int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: generate_photon_weights INFILENAME1 INFILENAME2 OUTFILENAME" << std::endl;
    return 0;
  }

  const string inFileName1 = string (argv[1]);
  const string inFileName2 = string (argv[2]);
  const string outFileName = string (argv[3]);

  TFile* inFile1 = nullptr;
  TFile* inFile2 = nullptr;
  TFile* outFile = nullptr;
  TTree* inTree = nullptr;

  inFile1 = new TFile (inFileName1.c_str (), "read");
  inFile2 = new TFile (inFileName2.c_str (), "read");
  outFile = new TFile (outFileName.c_str (), "recreate"); 

  inTree = (TTree*) inFile1->Get ("tree");
  TH1D* h_g_pt_numerator = GetPhotonSpectrum (inTree, "num");
  inFile1->Close ();
  SaferDelete (&inFile1);

  inTree = (TTree*) inFile2->Get ("tree");
  TH1D* h_g_pt_denominator = GetPhotonSpectrum (inTree, "den");
  inFile2->Close ();
  SaferDelete (&inFile2);

  TH1D* h_g_pt_wgts = (TH1D*) h_g_pt_numerator->Clone ("h_g_pt_wgts");
  h_g_pt_wgts->Divide (h_g_pt_denominator);

  // now save histograms to a rootfile
  h_g_pt_wgts->Write ();
  outFile->Close ();
  SaferDelete (&outFile);

  return 0;
}

#endif
