#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TF1.h>
#include <TBox.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <math.h>

#include <AtlasStyle.h>
#include <AtlasUtils.h>

#include <GlobalParams.h>
#include <Utilities.h>

#include "header.h"

using namespace PythiaCoMStudy;

typedef TGraphAsymmErrors TGAE;

TH1D* h_trk_z_pth_yield[2];
TH1D* h_trk_z_pth_bkg_yield[2];
TH1D* h_trk_z_pth_yield_ratio;
TH2D* h2_trk_z_pth_cov[2];
TH2D* h2_trk_z_pth_bkg_cov[2];

TH1D* h_trk_z_dphi_pth_gt4_yield[2];
TH1D* h_trk_z_dphi_pth_gt4_yield_diff;
TH2D* h2_trk_z_dphi_pth_gt4_cov[2];
TH1D* h_trk_z_dphi_pth_lt4_yield[2];
TH1D* h_trk_z_dphi_pth_lt4_yield_diff;
TH2D* h2_trk_z_dphi_pth_lt4_cov[2];

TH1D* h_trk_g_pth_yield[2];
TH1D* h_trk_g_pth_bkg_yield[2];
TH1D* h_trk_g_pth_yield_ratio;
TH2D* h2_trk_g_pth_cov[2];
TH2D* h2_trk_g_pth_bkg_cov[2];

TH1D* h_trk_g_dphi_pth_gt4_yield[2];
TH1D* h_trk_g_dphi_pth_gt4_yield_diff;
TH2D* h2_trk_g_dphi_pth_gt4_cov[2];
TH1D* h_trk_g_dphi_pth_lt4_yield[2];
TH1D* h_trk_g_dphi_pth_lt4_yield_diff;
TH2D* h2_trk_g_dphi_pth_lt4_cov[2];

TH1D* h_trk_g_dphi_pth_gt4_noIFSR_yield[2];
TH1D* h_trk_g_dphi_pth_gt4_noIFSR_yield_diff;
TH2D* h2_trk_g_dphi_pth_gt4_noIFSR_cov[2];
TH1D* h_trk_g_dphi_pth_lt4_noIFSR_yield[2];
TH1D* h_trk_g_dphi_pth_lt4_noIFSR_yield_diff;
TH2D* h2_trk_g_dphi_pth_lt4_noIFSR_cov[2];

TH1D* h_z_pt_yield[2];
TH1D* h_z_pt_yield_ratio;
TH1D* h_g_pt_yield[2];
TH1D* h_g_pt_yield_ratio;

TH1D* h_z_pids[2];
TH1D* h_g_pids[2];

TH1D* h_z_jet_pt_yield[2];
TH2D* h2_z_jet_pt_cov[2];
TH1D* h_z_jet_pt_yield_ratio;
TH1D* h_z_jet_pt_dphi_gt7p8_yield[2];
TH2D* h2_z_jet_pt_dphi_gt7p8_cov[2];
TH1D* h_z_jet_pt_dphi_gt7p8_yield_ratio;
TH1D* h_z_jet_dphi_yield[2];
TH2D* h2_z_jet_dphi_cov[2];
TH1D* h_z_jet_dphi_yield_diff;

TH1D* h_g_jet_pt_yield[2];
TH2D* h2_g_jet_pt_cov[2];
TH1D* h_g_jet_pt_yield_ratio;
TH1D* h_g_jet_pt_dphi_gt7p8_yield[2];
TH2D* h2_g_jet_pt_dphi_gt7p8_cov[2];
TH1D* h_g_jet_pt_dphi_gt7p8_yield_ratio;
TH1D* h_g_jet_dphi_yield[2];
TH2D* h2_g_jet_dphi_cov[2];
TH1D* h_g_jet_dphi_yield_diff;
TH1D* h_g_jet_dphi_noIFSR_yield[2];
TH2D* h2_g_jet_dphi_noIFSR_cov[2];
TH1D* h_g_jet_dphi_noIFSR_yield_diff;

TH1D* h_g_chjet_dphi_yield[2];
TH2D* h2_g_chjet_dphi_cov[2];
TH1D* h_g_chjet_dphi_yield_diff;
TH1D* h_g_chjet_dphi_noIFSR_yield[2];
TH2D* h2_g_chjet_dphi_noIFSR_cov[2];
TH1D* h_g_chjet_dphi_noIFSR_yield_diff;

TH1D* h_g_jet_yield[2];
TH1D* h_g_jet_yield_ratio;
TH1D* h_g_jet_noIFSR_yield[2];
TH1D* h_g_jet_noIFSR_yield_ratio;


void SetVariances (TH1D* h, TH2D* h2) {
  const int nb = h->GetNbinsX ();
  assert (nb == h2->GetNbinsX () && nb == h2->GetNbinsY ());

  for (int iX = 1; iX <= nb; iX++) {
    assert (h2->GetBinContent (iX, iX) >= 0);
    h->SetBinError (iX, sqrt (h2->GetBinContent (iX, iX)));
  }
}


Color_t colors[2] = {kAzure-2, kRed+1};
Color_t bgColors[2] = {kRed-9, kRed-3};


int main () {

  SetAtlasStyle(); 

  TFile* inFile = nullptr;



  inFile = new TFile ("rootFiles/z_out_sqrts5020GeV.root", "read");

  h_trk_z_pth_yield[0] = (TH1D*) inFile->Get ("h_trk_z_pth_yield_sqrts5020GeV");
  h_trk_z_pth_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_z_pth_bkg_yield_sqrts5020GeV");
  h2_trk_z_pth_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_pth_cov_sqrts5020GeV");
  h2_trk_z_pth_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_pth_bkg_cov_sqrts5020GeV");
  SetVariances (h_trk_z_pth_yield[0], h2_trk_z_pth_cov[0]);
  SetVariances (h_trk_z_pth_bkg_yield[0], h2_trk_z_pth_bkg_cov[0]);
  h_trk_z_pth_yield[0]->Add (h_trk_z_pth_bkg_yield[0], -1);

  h_trk_z_dphi_pth_gt4_yield[0] = (TH1D*) inFile->Get ("h_trk_z_dphi_pth_gt4_yield_sqrts5020GeV");
  h_trk_z_dphi_pth_lt4_yield[0] = (TH1D*) inFile->Get ("h_trk_z_dphi_pth_lt4_yield_sqrts5020GeV");
  h2_trk_z_dphi_pth_gt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_dphi_pth_gt4_cov_sqrts5020GeV");
  h2_trk_z_dphi_pth_lt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_dphi_pth_lt4_cov_sqrts5020GeV");
  SetVariances (h_trk_z_dphi_pth_gt4_yield[0], h2_trk_z_dphi_pth_gt4_cov[0]);
  SetVariances (h_trk_z_dphi_pth_lt4_yield[0], h2_trk_z_dphi_pth_lt4_cov[0]);

  h_z_pt_yield[0] = (TH1D*) inFile->Get ("h_z_pt_yield_sqrts5020GeV");

  h_z_pids[0] = (TH1D*) inFile->Get ("h_z_pids_sqrts5020GeV");

  h_z_jet_pt_yield[0] = (TH1D*) inFile->Get ("h_z_jet_pt_yield_sqrts5020GeV");
  h2_z_jet_pt_cov[0] = (TH2D*) inFile->Get ("h2_z_jet_pt_cov_sqrts5020GeV");
  SetVariances (h_z_jet_pt_yield[0], h2_z_jet_pt_cov[0]);
  h_z_jet_pt_dphi_gt7p8_yield[0] = (TH1D*) inFile->Get ("h_z_jet_pt_dphi_gt7p8_yield_sqrts5020GeV");
  h2_z_jet_pt_dphi_gt7p8_cov[0] = (TH2D*) inFile->Get ("h2_z_jet_pt_dphi_gt7p8_cov_sqrts5020GeV");
  SetVariances (h_z_jet_pt_dphi_gt7p8_yield[0], h2_z_jet_pt_dphi_gt7p8_cov[0]);

  h_z_jet_dphi_yield[0] = (TH1D*) inFile->Get ("h_z_jet_dphi_yield_sqrts5020GeV");
  h2_z_jet_dphi_cov[0] = (TH2D*) inFile->Get ("h2_z_jet_dphi_cov_sqrts5020GeV");
  SetVariances (h_z_jet_dphi_yield[0], h2_z_jet_dphi_cov[0]);



  inFile = new TFile ("rootFiles/z_out_sqrts8160GeV.root", "read");

  h_trk_z_pth_yield[1] = (TH1D*) inFile->Get ("h_trk_z_pth_yield_sqrts8160GeV");
  h_trk_z_pth_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_z_pth_bkg_yield_sqrts8160GeV");
  h2_trk_z_pth_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_pth_cov_sqrts8160GeV");
  h2_trk_z_pth_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_pth_bkg_cov_sqrts8160GeV");
  SetVariances (h_trk_z_pth_yield[1], h2_trk_z_pth_cov[1]);
  SetVariances (h_trk_z_pth_bkg_yield[1], h2_trk_z_pth_bkg_cov[1]);
  h_trk_z_pth_yield[1]->Add (h_trk_z_pth_bkg_yield[1], -1);
  h_z_pt_yield[1] = (TH1D*) inFile->Get ("h_z_pt_yield_sqrts8160GeV");

  h_z_pids[1] = (TH1D*) inFile->Get ("h_z_pids_sqrts8160GeV");

  h_trk_z_dphi_pth_gt4_yield[1] = (TH1D*) inFile->Get ("h_trk_z_dphi_pth_gt4_yield_sqrts8160GeV");
  h_trk_z_dphi_pth_lt4_yield[1] = (TH1D*) inFile->Get ("h_trk_z_dphi_pth_lt4_yield_sqrts8160GeV");
  h2_trk_z_dphi_pth_gt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_dphi_pth_gt4_cov_sqrts8160GeV");
  h2_trk_z_dphi_pth_lt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_dphi_pth_lt4_cov_sqrts8160GeV");
  SetVariances (h_trk_z_dphi_pth_gt4_yield[1], h2_trk_z_dphi_pth_gt4_cov[1]);
  SetVariances (h_trk_z_dphi_pth_lt4_yield[1], h2_trk_z_dphi_pth_lt4_cov[1]);

  h_z_jet_pt_yield[1] = (TH1D*) inFile->Get ("h_z_jet_pt_yield_sqrts8160GeV");
  h2_z_jet_pt_cov[1] = (TH2D*) inFile->Get ("h2_z_jet_pt_cov_sqrts8160GeV");
  SetVariances (h_z_jet_pt_yield[1], h2_z_jet_pt_cov[1]);
  h_z_jet_pt_dphi_gt7p8_yield[1] = (TH1D*) inFile->Get ("h_z_jet_pt_dphi_gt7p8_yield_sqrts8160GeV");
  h2_z_jet_pt_dphi_gt7p8_cov[1] = (TH2D*) inFile->Get ("h2_z_jet_pt_dphi_gt7p8_cov_sqrts8160GeV");
  SetVariances (h_z_jet_pt_dphi_gt7p8_yield[1], h2_z_jet_pt_dphi_gt7p8_cov[1]);

  h_z_jet_dphi_yield[1] = (TH1D*) inFile->Get ("h_z_jet_dphi_yield_sqrts8160GeV");
  h2_z_jet_dphi_cov[1] = (TH2D*) inFile->Get ("h2_z_jet_dphi_cov_sqrts8160GeV");
  SetVariances (h_z_jet_dphi_yield[1], h2_z_jet_dphi_cov[1]);


  h_trk_z_pth_yield_ratio = (TH1D*) h_trk_z_pth_yield[1]->Clone ("h_trk_z_pth_yield_ratio");
  h_trk_z_pth_yield_ratio->Divide (h_trk_z_pth_yield[0]);
  h_trk_z_dphi_pth_gt4_yield_diff = (TH1D*) h_trk_z_dphi_pth_gt4_yield[1]->Clone ("h_trk_z_dphi_pth_gt4_yield_diff");
  h_trk_z_dphi_pth_gt4_yield_diff->Add (h_trk_z_dphi_pth_gt4_yield[0], -1);
  h_trk_z_dphi_pth_lt4_yield_diff = (TH1D*) h_trk_z_dphi_pth_lt4_yield[1]->Clone ("h_trk_z_dphi_pth_lt4_yield_diff");
  h_trk_z_dphi_pth_lt4_yield_diff->Add (h_trk_z_dphi_pth_lt4_yield[0], -1);
  h_z_pt_yield_ratio = (TH1D*) h_z_pt_yield[1]->Clone ("h_z_pt_yield_ratio");
  h_z_pt_yield_ratio->Divide (h_z_pt_yield[0]);
  h_z_jet_pt_yield_ratio = (TH1D*) h_z_jet_pt_yield[1]->Clone ("h_z_jet_pt_yield_ratio");
  h_z_jet_pt_yield_ratio->Divide (h_z_jet_pt_yield[0]);
  h_z_jet_pt_dphi_gt7p8_yield_ratio = (TH1D*) h_z_jet_pt_dphi_gt7p8_yield[1]->Clone ("h_z_jet_pt_dphi_gt7p8_yield_ratio");
  h_z_jet_pt_dphi_gt7p8_yield_ratio->Divide (h_z_jet_pt_dphi_gt7p8_yield[0]);
  h_z_jet_dphi_yield_diff = (TH1D*) h_z_jet_dphi_yield[1]->Clone ("h_z_jet_dphi_yield_diff");
  h_z_jet_dphi_yield_diff->Add (h_z_jet_dphi_yield[0], -1);



  inFile = new TFile ("rootFiles/photon_out_sqrts5020GeV.root", "read");

  h_trk_g_pth_yield[0] = (TH1D*) inFile->Get ("h_trk_g_pth_yield_sqrts5020GeV");
  h_trk_g_pth_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_g_pth_bkg_yield_sqrts5020GeV");
  h2_trk_g_pth_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_pth_cov_sqrts5020GeV");
  h2_trk_g_pth_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_pth_bkg_cov_sqrts5020GeV");
  SetVariances (h_trk_g_pth_yield[0], h2_trk_g_pth_cov[0]);
  SetVariances (h_trk_g_pth_bkg_yield[0], h2_trk_g_pth_bkg_cov[0]);
  h_trk_g_pth_yield[0]->Add (h_trk_g_pth_bkg_yield[0], -1);

  h_trk_g_dphi_pth_gt4_yield[0] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_gt4_yield_sqrts5020GeV");
  h_trk_g_dphi_pth_lt4_yield[0] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_lt4_yield_sqrts5020GeV");
  h2_trk_g_dphi_pth_gt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_gt4_cov_sqrts5020GeV");
  h2_trk_g_dphi_pth_lt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_lt4_cov_sqrts5020GeV");
  SetVariances (h_trk_g_dphi_pth_gt4_yield[0], h2_trk_g_dphi_pth_gt4_cov[0]);
  SetVariances (h_trk_g_dphi_pth_lt4_yield[0], h2_trk_g_dphi_pth_lt4_cov[0]);

  h_g_pt_yield[0] = (TH1D*) inFile->Get ("h_g_pt_yield_sqrts5020GeV");

  h_g_pids[0] = (TH1D*) inFile->Get ("h_g_pids_sqrts5020GeV");

  h_g_jet_pt_yield[0] = (TH1D*) inFile->Get ("h_g_jet_pt_yield_sqrts5020GeV");
  h2_g_jet_pt_cov[0] = (TH2D*) inFile->Get ("h2_g_jet_pt_cov_sqrts5020GeV");
  SetVariances (h_g_jet_pt_yield[0], h2_g_jet_pt_cov[0]);
  h_g_jet_pt_dphi_gt7p8_yield[0] = (TH1D*) inFile->Get ("h_g_jet_pt_dphi_gt7p8_yield_sqrts5020GeV");
  h2_g_jet_pt_dphi_gt7p8_cov[0] = (TH2D*) inFile->Get ("h2_g_jet_pt_dphi_gt7p8_cov_sqrts5020GeV");
  SetVariances (h_g_jet_pt_dphi_gt7p8_yield[0], h2_g_jet_pt_dphi_gt7p8_cov[0]);

  h_g_jet_dphi_yield[0] = (TH1D*) inFile->Get ("h_g_jet_dphi_yield_sqrts5020GeV");
  h2_g_jet_dphi_cov[0] = (TH2D*) inFile->Get ("h2_g_jet_dphi_cov_sqrts5020GeV");
  SetVariances (h_g_jet_dphi_yield[0], h2_g_jet_dphi_cov[0]);
  h_g_chjet_dphi_yield[0] = (TH1D*) inFile->Get ("h_g_chjet_dphi_yield_sqrts5020GeV");
  h2_g_chjet_dphi_cov[0] = (TH2D*) inFile->Get ("h2_g_chjet_dphi_cov_sqrts5020GeV");
  SetVariances (h_g_chjet_dphi_yield[0], h2_g_chjet_dphi_cov[0]);

  h_g_jet_yield[0] = (TH1D*) inFile->Get ("h_g_jet_yield_sqrts5020GeV");



  inFile = new TFile ("rootFiles/photon_out_sqrts8160GeV.root", "read");

  h_trk_g_pth_yield[1] = (TH1D*) inFile->Get ("h_trk_g_pth_yield_sqrts8160GeV");
  h_trk_g_pth_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_g_pth_bkg_yield_sqrts8160GeV");
  h2_trk_g_pth_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_pth_cov_sqrts8160GeV");
  h2_trk_g_pth_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_pth_bkg_cov_sqrts8160GeV");
  SetVariances (h_trk_g_pth_yield[1], h2_trk_g_pth_cov[1]);
  SetVariances (h_trk_g_pth_bkg_yield[1], h2_trk_g_pth_bkg_cov[1]);
  h_trk_g_pth_yield[1]->Add (h_trk_g_pth_bkg_yield[1], -1);

  h_trk_g_dphi_pth_gt4_yield[1] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_gt4_yield_sqrts8160GeV");
  h_trk_g_dphi_pth_lt4_yield[1] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_lt4_yield_sqrts8160GeV");
  h2_trk_g_dphi_pth_gt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_gt4_cov_sqrts8160GeV");
  h2_trk_g_dphi_pth_lt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_lt4_cov_sqrts8160GeV");
  SetVariances (h_trk_g_dphi_pth_gt4_yield[1], h2_trk_g_dphi_pth_gt4_cov[1]);
  SetVariances (h_trk_g_dphi_pth_lt4_yield[1], h2_trk_g_dphi_pth_lt4_cov[1]);

  h_g_pt_yield[1] = (TH1D*) inFile->Get ("h_g_pt_yield_sqrts8160GeV");

  h_g_pids[1] = (TH1D*) inFile->Get ("h_g_pids_sqrts8160GeV");

  h_g_jet_pt_yield[1] = (TH1D*) inFile->Get ("h_g_jet_pt_yield_sqrts8160GeV");
  h2_g_jet_pt_cov[1] = (TH2D*) inFile->Get ("h2_g_jet_pt_cov_sqrts8160GeV");
  SetVariances (h_g_jet_pt_yield[1], h2_g_jet_pt_cov[1]);
  h_g_jet_pt_dphi_gt7p8_yield[1] = (TH1D*) inFile->Get ("h_g_jet_pt_dphi_gt7p8_yield_sqrts8160GeV");
  h2_g_jet_pt_dphi_gt7p8_cov[1] = (TH2D*) inFile->Get ("h2_g_jet_pt_dphi_gt7p8_cov_sqrts8160GeV");
  SetVariances (h_g_jet_pt_dphi_gt7p8_yield[1], h2_g_jet_pt_dphi_gt7p8_cov[1]);

  h_g_jet_dphi_yield[1] = (TH1D*) inFile->Get ("h_g_jet_dphi_yield_sqrts8160GeV");
  h2_g_jet_dphi_cov[1] = (TH2D*) inFile->Get ("h2_g_jet_dphi_cov_sqrts8160GeV");
  SetVariances (h_g_jet_dphi_yield[1], h2_g_jet_dphi_cov[1]);
  h_g_chjet_dphi_yield[1] = (TH1D*) inFile->Get ("h_g_chjet_dphi_yield_sqrts8160GeV");
  h2_g_chjet_dphi_cov[1] = (TH2D*) inFile->Get ("h2_g_chjet_dphi_cov_sqrts8160GeV");
  SetVariances (h_g_chjet_dphi_yield[1], h2_g_chjet_dphi_cov[1]);

  h_g_jet_yield[1] = (TH1D*) inFile->Get ("h_g_jet_yield_sqrts8160GeV");


  h_trk_g_pth_yield_ratio = (TH1D*) h_trk_g_pth_yield[1]->Clone ("h_trk_g_pth_yield_ratio");
  h_trk_g_pth_yield_ratio->Divide (h_trk_g_pth_yield[0]);
  h_trk_g_dphi_pth_gt4_yield_diff = (TH1D*) h_trk_g_dphi_pth_gt4_yield[1]->Clone ("h_trk_g_dphi_pth_gt4_yield_diff");
  h_trk_g_dphi_pth_gt4_yield_diff->Add (h_trk_g_dphi_pth_gt4_yield[0], -1);
  h_trk_g_dphi_pth_lt4_yield_diff = (TH1D*) h_trk_g_dphi_pth_lt4_yield[1]->Clone ("h_trk_g_dphi_pth_lt4_yield_diff");
  h_trk_g_dphi_pth_lt4_yield_diff->Add (h_trk_g_dphi_pth_lt4_yield[0], -1);
  h_g_pt_yield_ratio = (TH1D*) h_g_pt_yield[1]->Clone ("h_g_pt_yield_ratio");
  h_g_pt_yield_ratio->Divide (h_g_pt_yield[0]);
  h_g_jet_pt_yield_ratio = (TH1D*) h_g_jet_pt_yield[1]->Clone ("h_g_jet_pt_yield_ratio");
  h_g_jet_pt_yield_ratio->Divide (h_g_jet_pt_yield[0]);
  h_g_jet_pt_dphi_gt7p8_yield_ratio = (TH1D*) h_g_jet_pt_dphi_gt7p8_yield[1]->Clone ("h_g_jet_pt_dphi_gt7p8_yield_ratio");
  h_g_jet_pt_dphi_gt7p8_yield_ratio->Divide (h_g_jet_pt_dphi_gt7p8_yield[0]);
  h_g_jet_dphi_yield_diff = (TH1D*) h_g_jet_dphi_yield[1]->Clone ("h_g_jet_dphi_yield_diff");
  h_g_jet_dphi_yield_diff->Add (h_g_jet_dphi_yield[0], -1);
  h_g_chjet_dphi_yield_diff = (TH1D*) h_g_chjet_dphi_yield[1]->Clone ("h_g_chjet_dphi_yield_diff");
  h_g_chjet_dphi_yield_diff->Add (h_g_chjet_dphi_yield[0], -1);
  h_g_jet_yield_ratio = (TH1D*) h_g_jet_yield[1]->Clone ("h_g_jet_yield_ratio");
  h_g_jet_yield_ratio->Divide (h_g_jet_yield[0]);




  inFile = new TFile ("rootFiles/photon_out_noIFSR_sqrts5020GeV.root", "read");

  h_trk_g_dphi_pth_gt4_noIFSR_yield[0] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_gt4_yield_noIFSR_sqrts5020GeV");
  h_trk_g_dphi_pth_lt4_noIFSR_yield[0] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_lt4_yield_noIFSR_sqrts5020GeV");
  h2_trk_g_dphi_pth_gt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_gt4_cov_noIFSR_sqrts5020GeV");
  h2_trk_g_dphi_pth_lt4_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_lt4_cov_noIFSR_sqrts5020GeV");
  SetVariances (h_trk_g_dphi_pth_gt4_noIFSR_yield[0], h2_trk_g_dphi_pth_gt4_cov[0]);
  SetVariances (h_trk_g_dphi_pth_lt4_noIFSR_yield[0], h2_trk_g_dphi_pth_lt4_cov[0]);

  h_g_jet_dphi_noIFSR_yield[0] = (TH1D*) inFile->Get ("h_g_jet_dphi_yield_noIFSR_sqrts5020GeV");
  h2_g_jet_dphi_cov[0] = (TH2D*) inFile->Get ("h2_g_jet_dphi_cov_noIFSR_sqrts5020GeV");
  SetVariances (h_g_jet_dphi_noIFSR_yield[0], h2_g_jet_dphi_cov[0]);
  h_g_chjet_dphi_noIFSR_yield[0] = (TH1D*) inFile->Get ("h_g_chjet_dphi_yield_noIFSR_sqrts5020GeV");
  h2_g_chjet_dphi_cov[0] = (TH2D*) inFile->Get ("h2_g_chjet_dphi_cov_noIFSR_sqrts5020GeV");
  SetVariances (h_g_chjet_dphi_noIFSR_yield[0], h2_g_chjet_dphi_cov[0]);

  h_g_jet_noIFSR_yield[0] = (TH1D*) inFile->Get ("h_g_jet_yield_noIFSR_sqrts5020GeV");



  inFile = new TFile ("rootFiles/photon_out_noIFSR_sqrts8160GeV.root", "read");

  h_trk_g_dphi_pth_gt4_noIFSR_yield[1] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_gt4_yield_noIFSR_sqrts8160GeV");
  h_trk_g_dphi_pth_lt4_noIFSR_yield[1] = (TH1D*) inFile->Get ("h_trk_g_dphi_pth_lt4_yield_noIFSR_sqrts8160GeV");
  h2_trk_g_dphi_pth_gt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_gt4_cov_noIFSR_sqrts8160GeV");
  h2_trk_g_dphi_pth_lt4_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_dphi_pth_lt4_cov_noIFSR_sqrts8160GeV");
  SetVariances (h_trk_g_dphi_pth_gt4_noIFSR_yield[1], h2_trk_g_dphi_pth_gt4_cov[1]);
  SetVariances (h_trk_g_dphi_pth_lt4_noIFSR_yield[1], h2_trk_g_dphi_pth_lt4_cov[1]);

  h_g_jet_dphi_noIFSR_yield[1] = (TH1D*) inFile->Get ("h_g_jet_dphi_yield_noIFSR_sqrts8160GeV");
  h2_g_jet_dphi_cov[1] = (TH2D*) inFile->Get ("h2_g_jet_dphi_cov_noIFSR_sqrts8160GeV");
  SetVariances (h_g_jet_dphi_noIFSR_yield[1], h2_g_jet_dphi_cov[1]);
  h_g_chjet_dphi_noIFSR_yield[1] = (TH1D*) inFile->Get ("h_g_chjet_dphi_yield_noIFSR_sqrts8160GeV");
  h2_g_chjet_dphi_cov[1] = (TH2D*) inFile->Get ("h2_g_chjet_dphi_cov_noIFSR_sqrts8160GeV");
  SetVariances (h_g_chjet_dphi_noIFSR_yield[1], h2_g_chjet_dphi_cov[1]);

  h_g_jet_noIFSR_yield[1] = (TH1D*) inFile->Get ("h_g_jet_yield_noIFSR_sqrts8160GeV");


  h_trk_g_dphi_pth_gt4_noIFSR_yield_diff = (TH1D*) h_trk_g_dphi_pth_gt4_noIFSR_yield[1]->Clone ("h_trk_g_dphi_pth_gt4_yield_diff");
  h_trk_g_dphi_pth_gt4_noIFSR_yield_diff->Add (h_trk_g_dphi_pth_gt4_noIFSR_yield[0], -1);
  h_trk_g_dphi_pth_lt4_noIFSR_yield_diff = (TH1D*) h_trk_g_dphi_pth_lt4_noIFSR_yield[1]->Clone ("h_trk_g_dphi_pth_lt4_yield_diff");
  h_trk_g_dphi_pth_lt4_noIFSR_yield_diff->Add (h_trk_g_dphi_pth_lt4_noIFSR_yield[0], -1);
  h_g_jet_dphi_noIFSR_yield_diff = (TH1D*) h_g_jet_dphi_noIFSR_yield[1]->Clone ("h_g_jet_dphi_yield_diff");
  h_g_jet_dphi_noIFSR_yield_diff->Add (h_g_jet_dphi_noIFSR_yield[0], -1);
  h_g_chjet_dphi_noIFSR_yield_diff = (TH1D*) h_g_chjet_dphi_noIFSR_yield[1]->Clone ("h_g_chjet_dphi_yield_diff");
  h_g_chjet_dphi_noIFSR_yield_diff->Add (h_g_chjet_dphi_noIFSR_yield[0], -1);
  h_g_jet_noIFSR_yield_ratio = (TH1D*) h_g_jet_noIFSR_yield[1]->Clone ("h_g_jet_noIFSR_yield_ratio");
  h_g_jet_noIFSR_yield_ratio->Divide (h_g_jet_noIFSR_yield[0]);



  {
    const char* canvasName = "c_trk_z_pth_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 2e-6;
    float ymax = 10;

    h = (TH1D*) h_trk_z_pth_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("d^{2}N_{ch} / d#it{p}_{T} #Delta#phi [GeV^{-1}]");
    h->GetYaxis ()->SetTitle ("(1/N_{Z}) (d^{2}N_{ch} / d#it{p}_{T}^{ch} d#Delta#phi_{ch,Z}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_trk_z_pth_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_z_pth_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.36, kBlack, "#bf{Pythia 8.303} #it{Z}-h correlations", 0.032/0.6);
    myText (0.23, 0.29, kBlack, "Inclusive #it{Z} production, #it{p}_{T}^{#it{Z}} > 5 GeV, #Delta#phi > 3#pi/4", 0.028/0.6);
    myText (0.23, 0.22, kBlack, "Transmin UE subtraction", 0.028/0.6);
    myLineText (0.29, 0.16, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.09, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.74;
    ymax = 1.26;

    h = (TH1D*) h_trk_z_pth_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.04*0.4);
    h->GetYaxis ()->SetTitle ("8.16 / 5.02");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_trk_z_pth_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Ztagged_ptch_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_trk_g_pth_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 5e-7;
    float ymax = 15;

    h = (TH1D*) h_trk_g_pth_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (d^{2}N_{ch} / d#it{p}_{T}^{ch} d#Delta#phi_{ch,#gamma}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_trk_g_pth_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_pth_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.36, kBlack, "#bf{Pythia 8.303} #gamma-h correlations", 0.032/0.6);
    myText (0.23, 0.29, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} > 50 GeV, #Delta#phi > 3#pi/4", 0.028/0.6);
    myText (0.23, 0.22, kBlack, "Transmin UE subtraction", 0.028/0.6);
    myLineText (0.29, 0.16, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.09, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.74;
    ymax = 1.26;

    h = (TH1D*) h_trk_g_pth_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.04*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_trk_g_pth_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Photontagged_ptch_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_z_pt_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 1.1e-6;
    float ymax = 2e-1;

    h = (TH1D*) h_z_pt_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{Z} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{Z}) (dN_{Z} / d#it{p}_{T}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);
    
    h = h_z_pt_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_pt_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.29, kBlack, "#bf{Pythia 8.303}", 0.032/0.6);
    myText (0.23, 0.22, kBlack, "Inclusive #it{Z} production, #it{p}_{T}^{#it{Z}} > 5 GeV", 0.028/0.6);
    myLineText (0.29, 0.16, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.09, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.74;
    ymax = 1.26;

    h = (TH1D*) h_z_pt_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{Z} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_z_pt_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Z_pt_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_g_pt_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 1.1e-6;
    float ymax = 2e-1;

    h = (TH1D*) h_g_pt_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{#gamma} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{#gamma} / d#it{p}_{T}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_g_pt_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_pt_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.29, kBlack, "#bf{Pythia 8.303}", 0.032/0.6);
    myText (0.23, 0.22, kBlack, "Isolated photons", 0.028/0.6);
    myLineText (0.29, 0.16, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.09, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.74;
    ymax = 1.26;

    h = (TH1D*) h_g_pt_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{#gamma} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_g_pt_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Photon_pt_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_g_pids";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);

    //gStyle->SetHistMinimumZero ();

    TH1D* h = nullptr; 

    h = h_g_pids[0];
    h->SetFillColor (bgColors[1]);
    h->SetMinimum (0);
    h->GetXaxis ()->SetTitle ("Pythia Process");
    h->GetXaxis ()->SetTitleSize (0.04);
    //h->GetXaxis ()->SetLabelSize (0.04);
    h->GetXaxis ()->SetTitleOffset (1.5);
    h->GetYaxis ()->SetTitle ("Counts");
    h->GetYaxis ()->SetTitleSize (0.04);
    h->GetYaxis ()->SetLabelSize (0.03);
    h->GetYaxis ()->SetTitleOffset (1.8);
    h->SetBarWidth (0.2);
    h->SetBarOffset (0.3);

    h->DrawCopy ("b");

    h = h_g_pids[1]; 
    h->SetFillColor (bgColors[0]);
    h->SetBarWidth (0.2);
    h->SetBarOffset (0.5);
    h->DrawCopy ("b same");

    myText (0.50, 0.80, kBlack, "#bf{Pythia 8.303}", 0.032);
    myText (0.50, 0.75, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} > 50 GeV", 0.032);
    myBoxTextNoLine (0.56, 0.70, 0.05, bgColors[1], "#sqrt{s} = #bf{5.02 TeV}", 0.032);
    myBoxTextNoLine (0.56, 0.65, 0.05, bgColors[0], "#sqrt{s} = #bf{8.16 TeV}", 0.032);

    c->SaveAs ("Plots/Photon_pids.pdf");
  }




  {
    const char* canvasName = "c_z_pids";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);

    //gStyle->SetHistMinimumZero ();

    TH1D* h = nullptr; 

    h = h_z_pids[0];
    h->SetFillColor (bgColors[1]);
    h->SetMinimum (0);
    h->SetMaximum (15000);
    h->GetXaxis ()->SetTitle ("Pythia Process");
    h->GetXaxis ()->SetTitleSize (0.04);
    //h->GetXaxis ()->SetLabelSize (0.04);
    h->GetXaxis ()->SetTitleOffset (1.5);
    h->GetYaxis ()->SetTitle ("Counts");
    h->GetYaxis ()->SetTitleSize (0.04);
    h->GetYaxis ()->SetLabelSize (0.03);
    h->GetYaxis ()->SetTitleOffset (1.8);
    h->SetBarWidth (0.3);
    h->SetBarOffset (0.2);

    h->DrawCopy ("b");

    h = h_z_pids[1]; 
    h->SetFillColor (bgColors[0]);
    h->SetBarWidth (0.3);
    h->SetBarOffset (0.5);
    h->DrawCopy ("b same");

    myText (0.43, 0.80, kBlack, "#bf{Pythia 8.303}", 0.032);
    myText (0.43, 0.75, kBlack, "Inclusive #it{Z} production, #it{p}_{T}^{#it{Z}} > 5 GeV", 0.032);
    myBoxTextNoLine (0.49, 0.70, 0.05, bgColors[1], "#sqrt{s} = #bf{5.02 TeV}", 0.032);
    myBoxTextNoLine (0.49, 0.65, 0.05, bgColors[0], "#sqrt{s} = #bf{8.16 TeV}", 0.032);

    c->SaveAs ("Plots/Z_pids.pdf");
  }




  {
    const char* canvasName = "c_g_jet_pt_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 1.1e-4;
    float ymax = 1;

    h = (TH1D*) h_g_jet_pt_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{jet} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{jet} / d#it{p}_{T}^{jet}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->Draw ("hist ][");

    h = h_g_jet_pt_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_pt_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_pt_dphi_gt7p8_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_pt_dphi_gt7p8_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.42, kBlack, "#bf{Pythia 8.303} #gamma-jet correlations", 0.032/0.6);
    myText (0.23, 0.35, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} = 60-90 GeV", 0.028/0.6);
    myText (0.23, 0.28, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4", 0.028/0.6);
    myText (0.23, 0.21, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.16, colors[0], 1, "All #Delta#phi", 1.2, 0.028/0.6);
    myLineText (0.32, 0.09, colors[0], 2, "#Delta#phi > 7#pi/8", 1.2, 0.028/0.6);
    myText (0.48, 0.21, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.57, 0.16, colors[1], 1, "All #Delta#phi", 1.2, 0.028/0.6);
    myLineText (0.57, 0.09, colors[1], 2, "#Delta#phi > 7#pi/8", 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.1;
    ymax = 1.9;

    h = (TH1D*) h_g_jet_pt_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{jet} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->Draw ("hist ][");

    h = h_g_jet_pt_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_pt_dphi_gt7p8_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "All #Delta#phi", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "#Delta#phi > 7#pi/8", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Photontagged_ptj_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_z_jet_pt_dphi_gt7p8_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 1.1e-4;
    float ymax = 1;

    h = (TH1D*) h_z_jet_pt_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{jet} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#it{Z}}) (dN_{jet} / d#it{p}_{T}^{jet}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_z_jet_pt_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_jet_pt_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_jet_pt_dphi_gt7p8_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_jet_pt_dphi_gt7p8_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.42, kBlack, "#bf{Pythia 8.303} #it{Z}-jet correlations", 0.032/0.6);
    myText (0.23, 0.35, kBlack, "Inclusive #it{Z} production, #it{p}_{T}^{#it{Z}} = 30-50 GeV", 0.028/0.6);
    myText (0.23, 0.28, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4", 0.028/0.6);
    myText (0.23, 0.21, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.16, colors[0], 1, "All #Delta#phi", 1.2, 0.028/0.6);
    myLineText (0.32, 0.09, colors[0], 2, "#Delta#phi > 7#pi/8", 1.2, 0.028/0.6);
    myText (0.48, 0.21, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.57, 0.16, colors[1], 1, "All #Delta#phi", 1.2, 0.028/0.6);
    myLineText (0.57, 0.09, colors[1], 2, "#Delta#phi > 7#pi/8", 1.2, 0.028/0.6);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.1;
    ymax = 1.9;

    h = (TH1D*) h_z_jet_pt_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{jet} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->Draw ("hist ][");

    h = h_z_jet_pt_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_jet_pt_dphi_gt7p8_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "All #Delta#phi", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "#Delta#phi > 7#pi/8", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Ztagged_ptj_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_trk_g_dphi_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -0.25;
    float ymax = 8.5;

    h = (TH1D*) h_trk_g_dphi_pth_gt4_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch,#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{ch} / d#Delta#phi_{ch,#gamma})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (3.*pi/4., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (3.*pi/4., ymin, 3.*pi/4., ymax);

    h = h_trk_g_dphi_pth_gt4_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_gt4_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_lt4_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_lt4_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #gamma-h correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} > 50 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.61, colors[0], 1, "#it{p}_{T}^{ch} > 4 GeV", 1.2, 0.028/0.6);
    myLineText (0.32, 0.54, colors[0], 2, "#it{p}_{T}^{ch} = 2-4 GeV", 1.2, 0.028/0.6);
    myText (0.23, 0.45, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.32, 0.40, colors[1], 1, "#it{p}_{T}^{ch} > 4 GeV", 1.2, 0.028/0.6);
    myLineText (0.32, 0.33, colors[1], 2, "#it{p}_{T}^{ch} = 2-4 GeV", 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -0.45;
    ymax = 0.45;

    h = (TH1D*) h_trk_g_dphi_pth_gt4_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch,#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (3.*pi/4., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (3.*pi/4., ymin, 3.*pi/4., ymax);

    h = h_trk_g_dphi_pth_gt4_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_lt4_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "#it{p}_{T}^{ch} > 4 GeV", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "#it{p}_{T}^{ch} = 2-4 GeV", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Photontagged_trk_dphi_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_g_jet_dphi_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -0.15;
    float ymax = 4.5;

    h = (TH1D*) h_g_jet_dphi_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{jet} / d#Delta#phi_{j#gamma})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_jet_dphi_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_dphi_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #gamma-jet correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} = 60-90 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4, #it{p}_{T}^{jet} > 8 GeV", 0.028/0.6);
    myLineText (0.29, 0.60, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.53, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -0.55;
    ymax = 0.55;

    h = (TH1D*) h_g_jet_dphi_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_jet_dphi_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Photontagged_Jet_dphi_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_z_jet_dphi_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    float ymin = -0.15;
    float ymax = 4.5;

    uPad->cd (); 

    h = (TH1D*) h_z_jet_dphi_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#it{Z}}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#it{Z}}) (dN_{jet} / d#Delta#phi_{j#it{Z}})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_z_jet_dphi_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_z_jet_dphi_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #it{Z}-jet correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Inclusive #it{Z} production, #it{p}_{T}^{#it{Z}} = 30-50 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4, #it{p}_{T}^{jet} > 8 GeV", 0.028/0.6);
    myLineText (0.29, 0.60, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.29, 0.53, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -0.55;
    ymax = 0.55;

    h = (TH1D*) h_z_jet_dphi_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#it{Z}}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);
  
    h = h_z_jet_dphi_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Ztagged_Jet_dphi_yields_comparison.pdf"); 
  }




  {
    const char* canvasName = "c_g_jet_dphi_IFSR_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -0.25;
    float ymax = 8.5;

    h = (TH1D*) h_g_jet_dphi_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{jet} / d#Delta#phi_{j#gamma})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_jet_dphi_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_dphi_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_dphi_noIFSR_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_dphi_noIFSR_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #gamma-jet correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} = 60-90 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4, #it{p}_{T}^{jet} > 8 GeV", 0.028/0.6);
    myText (0.23, 0.59, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.54, colors[0], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.47, colors[0], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);
    myText (0.23, 0.38, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.32, 0.33, colors[1], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.26, colors[1], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -0.55;
    ymax = 0.55;

    h = (TH1D*) h_g_jet_dphi_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_jet_dphi_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_jet_dphi_noIFSR_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "ISR/FSR #bf{on}", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "ISR/FSR #bf{off}", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Photontagged_Jet_dphi_yields_comparison_IFSR.pdf"); 
  }




  {
    const char* canvasName = "c_g_chjet_dphi_IFSR_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -0.25;
    float ymax = 8.5;

    h = (TH1D*) h_g_chjet_dphi_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{jet} / d#Delta#phi_{j#gamma})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_chjet_dphi_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_chjet_dphi_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_chjet_dphi_noIFSR_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_chjet_dphi_noIFSR_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #gamma-jet correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} = 60-90 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, "Anti-#it{k}_{T} #bf{charged} jets, R=0.4, #it{p}_{T}^{jet} > 8 GeV", 0.028/0.6);
    myText (0.23, 0.59, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.54, colors[0], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.47, colors[0], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);
    myText (0.23, 0.38, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.32, 0.33, colors[1], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.26, colors[1], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -0.55;
    ymax = 0.55;

    h = (TH1D*) h_g_chjet_dphi_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{j#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (7.*pi/8., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (7.*pi/8., ymin, 7.*pi/8., ymax);

    h = h_g_chjet_dphi_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_g_chjet_dphi_noIFSR_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "ISR/FSR #bf{on}", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "ISR/FSR #bf{off}", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Photontagged_ChargedJet_dphi_yields_comparison_IFSR.pdf"); 
  }



  {
    const char* canvasName = "c_trk_g_dphi_pth_gt4_IFSR_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -0.6;
    float ymax = 20;

    h = (TH1D*) h_trk_g_dphi_pth_gt4_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch,#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{ch} / d#Delta#phi_{ch,#gamma})");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    TLine l;
    l.SetLineStyle (7);
    l.SetLineWidth (1);
    l.SetLineColor (kBlack);

    TBox* shadedBox = new TBox (3.*pi/4., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (3.*pi/4., ymin, 3.*pi/4., ymax);

    h = h_trk_g_dphi_pth_gt4_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_gt4_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_gt4_noIFSR_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_gt4_noIFSR_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->SetLineStyle (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    g->SetLineStyle (1);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myText (0.23, 0.80, kBlack, "#bf{Pythia 8.303} #gamma-h correlations", 0.032/0.6);
    myText (0.23, 0.73, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} > 50 GeV", 0.028/0.6);
    myText (0.23, 0.66, kBlack, "Charged hadrons, #it{p}_{T}^{ch} > 4 GeV", 0.028/0.6);
    myText (0.23, 0.59, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 0.028/0.6);
    myLineText (0.32, 0.54, colors[0], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.47, colors[0], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);
    myText (0.23, 0.38, kBlack, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 0.028/0.6);
    myLineText (0.32, 0.33, colors[1], 1, "ISR/FSR #bf{on}", 1.2, 0.028/0.6);
    myLineText (0.32, 0.26, colors[1], 2, "ISR/FSR #bf{off}", 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = -1.95;
    ymax = 1.95;

    h = (TH1D*) h_trk_g_dphi_pth_gt4_yield_diff->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch,#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} #minus #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    shadedBox = new TBox (3.*pi/4., ymin, pi, ymax);
    shadedBox->SetFillColorAlpha (kGray, 0.3);
    shadedBox->Draw ();
    l.DrawLine (3.*pi/4., ymin, 3.*pi/4., ymax);

    h = h_trk_g_dphi_pth_gt4_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    h = h_trk_g_dphi_pth_gt4_noIFSR_yield_diff;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    myMarkerText (0.32, 0.47, kBlack, kFullCircle, "ISR/FSR #bf{on}", 0.8, 0.028/0.4, false);
    myMarkerText (0.32, 0.37, kBlack, kOpenCircle, "ISR/FSR #bf{off}", 0.8, 0.028/0.4, false);

    c->SaveAs ("Plots/Photontagged_trk_dphi_yields_comparison_IFSR.pdf"); 
  }




  {
    const char* canvasName = "c_g_jet_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;
    //TF1* f = nullptr;

    uPad->cd (); 

    float ymin = -0.01 * 1.5e4;
    float ymax = 0.24 * 1.5e4;

    h = (TH1D*) h_g_jet_yield[0]->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("N_{jet} / N_{#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("Events");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_g_jet_yield[0];
    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[0]);
    g->SetLineColor (colors[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);
    //f = new TF1 ("f", "[0]^x * TMath::Exp(-[0]) / TMath::Gamma(x+1)", 0, 14);
    //h->Fit (f, "RLN0Q");
    //cout << f->GetParameter (0) << endl;
    //f->SetLineColor (colors[0]);
    //((TGAE*) f->Clone ())->Draw ("same");
    //SaferDelete (&f);

    h = h_g_jet_yield[1];
    h->SetLineColor (colors[1]);
    h->SetLineWidth (2);
    h->Draw ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (colors[1]);
    g->SetLineColor (colors[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);
    //f = new TF1 ("f", "[0]^x * TMath::Exp(-[0]) / TMath::Gamma(x+1)", 0, 14);
    //h->Fit (f, "RLN0Q");
    //cout << f->GetParameter (0) << endl;
    //f->SetLineColor (colors[1]);
    //((TGAE*) f->Clone ())->Draw ("same");
    //SaferDelete (&f);

    myText (0.49, 0.82, kBlack, "#bf{Pythia 8.303} #gamma-jet correlations", 0.032/0.6);
    myText (0.49, 0.75, kBlack, "Isolated photons, #it{E}_{T}^{#gamma} = 60-90 GeV", 0.028/0.6);
    myText (0.49, 0.68, kBlack, "Anti-#it{k}_{T} truth jets, R=0.4, #it{p}_{T}^{jet} > 8 GeV", 0.028/0.6);
    myLineText (0.55, 0.61, colors[0], 1, Form ("#sqrt{s} = #color[%i]{#bf{5.02 TeV}}", (int)(colors[0])), 1.2, 0.028/0.6);
    myLineText (0.55, 0.54, colors[1], 1, Form ("#sqrt{s} = #color[%i]{#bf{8.16 TeV}}", (int)(colors[1])), 1.2, 0.028/0.6);


    dPad->cd (); 

    ymin = 0.00;
    ymax = 2.90;

    h = (TH1D*) h_g_jet_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("N_{jet} / N_{#gamma}");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    //h->GetXaxis ()->SetLabelOffset (-0.05*0.4);
    h->GetYaxis ()->SetTitle (Form ("#color[%i]{Red} / #color[%i]{Blue}", (int)(colors[1]), (int)(colors[0])));
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = h_g_jet_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (kBlack);
    g->SetLineWidth (2);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Photontagged_Jet_yields_comparison.pdf"); 
  }

  return 0;
}


void plotter () {
  main ();
}
