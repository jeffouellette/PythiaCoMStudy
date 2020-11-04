#ifndef __z_analyze_cxx__
#define __z_analyze_cxx__

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TLine.h>
#include <TProfile.h>
#include <TLorentzVector.h>

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



float GetZPIDBin (int code) {
  switch (code) {
  case 221: return 0.5;
  case 231: return 1.5;
  case 232: return 2.5;
  case 241: return 3.5;
  case 242: return 4.5;
  case 243: return 5.5;
  case 244: return 6.5;
  default: return -0.5;
  }
}



void SetVariances (TH1D* h, TH2D* h2) {
  const int nb = h->GetNbinsX ();
  assert (nb == h2->GetNbinsX () && nb == h2->GetNbinsY ());

  for (int iX = 1; iX <= nb; iX++) {
    if (h2->GetBinContent (iX, iX) < 0) {
      cout << "Assertion failed for " << h->GetName () << endl;
      cout << "  --> at iX =       " << iX << endl;
      cout << "  --> bin content = " << h->GetBinContent (iX) << endl;
      cout << "  --> variance =    " << h2->GetBinContent (iX, iX) << endl;
      assert (h2->GetBinContent (iX, iX) >= 0);
    }
    h->SetBinError (iX, sqrt (h2->GetBinContent (iX, iX)));
  }
}



void ScaleHist (TH1D* h, const double sf, const bool doWidth) {
  const int nb = h->GetNbinsX ();
  for (int iX = 1; iX <= nb; iX++) {
    h->SetBinContent (iX, h->GetBinContent (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
    h->SetBinError (iX, h->GetBinError (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
  }
  return;
}



void ScaleHist (TH2D* h, const double sf, const bool doWidth) {
  const int nbx = h->GetNbinsX ();
  const int nby = h->GetNbinsY ();
  for (int iX = 1; iX <= nbx; iX++) {
    for (int iY = 1; iY <= nby; iY++) {
      h->SetBinContent (iX, iY, h->GetBinContent (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
      h->SetBinError (iX, iY, h->GetBinError (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
    }
  }
  return;
}




int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: z_analyze NAME INFILENAME OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = string (argv[1]);
  const string inFileName = string (argv[2]);
  const string outFileName = string (argv[3]);

  TFile* inFile;
  TTree* inTree;

  int code = 0;
  //int id1 = 0;
  //int id2 = 0;
  //float x1pdf = 0;
  //float x2pdf = 0;
  //float Q = 0;
  //bool isValence1 = false;
  //bool isValence2 = false;

  float z_pt = 0;
  float z_eta = 0;
  float z_phi = 0;
  float z_m = 0;

  int akt4_jet_n = 0;
  float akt4_jet_pt[1000];
  float akt4_jet_eta[1000];
  float akt4_jet_phi[1000];
  float akt4_jet_e[1000];

  int part_n = 0;
  float part_pt[10000];
  float part_eta[10000];
  float part_phi[10000];
  float part_e[10000];

  inFile = new TFile (inFileName.c_str (), "read");
  inTree = (TTree*) inFile->Get ("tree");

  const int nEvents = inTree->GetEntries ();
  int nJetEvents = 0;

  inTree->SetBranchAddress ("code",       &code);
  //inTree->SetBranchAddress ("id1",        &id1);
  //inTree->SetBranchAddress ("id2",        &id2);
  //inTree->SetBranchAddress ("x1pdf",      &x1pdf);
  //inTree->SetBranchAddress ("x2pdf",      &x2pdf);
  //inTree->SetBranchAddress ("Q",          &Q);
  //inTree->SetBranchAddress ("isValence1", &isValence1);
  //inTree->SetBranchAddress ("isValence2", &isValence2);
  inTree->SetBranchAddress ("z_pt",       &z_pt);
  inTree->SetBranchAddress ("z_eta",      &z_eta);
  inTree->SetBranchAddress ("z_phi",      &z_phi);
  inTree->SetBranchAddress ("z_m",        &z_m);

  inTree->SetBranchAddress ("akt4_jet_n",   &akt4_jet_n);
  inTree->SetBranchAddress ("akt4_jet_pt",  &akt4_jet_pt);
  inTree->SetBranchAddress ("akt4_jet_eta", &akt4_jet_eta);
  inTree->SetBranchAddress ("akt4_jet_phi", &akt4_jet_phi);
  inTree->SetBranchAddress ("akt4_jet_e",   &akt4_jet_e);

  inTree->SetBranchAddress ("part_n",     &part_n);
  inTree->SetBranchAddress ("part_pt",    &part_pt);
  inTree->SetBranchAddress ("part_eta",   &part_eta);
  inTree->SetBranchAddress ("part_phi",   &part_phi);
  inTree->SetBranchAddress ("part_e",     &part_e);


  TH1D* h_z_trk_pth_yield;
  TH1D* h_z_trk_xhz_yield;
  TH2D* h2_z_trk_pth_cov;
  TH2D* h2_z_trk_xhz_cov;
  TH1D* h_z_trk_pth_bkg_yield;
  TH1D* h_z_trk_xhz_bkg_yield;
  TH2D* h2_z_trk_pth_bkg_cov;
  TH2D* h2_z_trk_xhz_bkg_cov;

  TH1D* h_z_trk_dphi_pth_gt4_yield;
  TH2D* h2_z_trk_dphi_pth_gt4_cov;
  TH1D* h_z_trk_dphi_pth_lt4_yield;
  TH2D* h2_z_trk_dphi_pth_lt4_cov;

  TH1D* h_z_pt_yield;

  TH1D* h_z_pids;

  TH1D* h_z_jet_pt_yield;
  TH2D* h2_z_jet_pt_cov;
  TH1D* h_z_jet_pt_dphi_gt7p8_yield;
  TH2D* h2_z_jet_pt_dphi_gt7p8_cov;
  TH1D* h_z_jet_dphi_yield;
  TH2D* h2_z_jet_dphi_cov;

  const float pthBins[8] = {0.5, 1, 2, 4, 8, 15, 30, 60};
  const short nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
  const float xhzBins[8] = {1./60., 1./30., 1./15., 1./8., 1./4., 1./2., 1., 2.};
  const short nXhZBins = sizeof (xhzBins) / sizeof (xhzBins[0]) - 1;
  const short nDPhiBins = 16;
  const double* dPhiBins = linspace (0, pi, nDPhiBins);
  const int nPtZBins = 30;
  const double* pTZBins = logspace (5, 300, nPtZBins);
  const int nPtJBins = 25;
  const double* pTJBins = logspace (8, 100, nPtJBins);


  float trk_pth_counts[nPthBins] = {};
  float trk_pth_counts_bkg[nPthBins] = {};
  float trk_xhz_counts[nXhZBins] = {};
  float trk_xhz_counts_bkg[nXhZBins] = {};
  float trk_dphi_pth_gt4_counts[nDPhiBins] = {};
  float trk_dphi_pth_lt4_counts[nDPhiBins] = {};
  float jet_ptj_counts[nPtJBins] = {};
  float jet_ptj_dphi_gt7p8_counts[nPtJBins] = {};
  float jet_dphi_counts[nDPhiBins] = {};

  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_z_trk_pth_yield = new TH1D (Form ("h_z_trk_pth_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_z_trk_xhz_yield = new TH1D (Form ("h_z_trk_xhz_yield_%s", name.c_str ()), ";#it{x}_{hZ}", nXhZBins, xhzBins);
  h2_z_trk_pth_cov = new TH2D (Form ("h2_z_trk_pth_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_z_trk_xhz_cov = new TH2D (Form ("h2_z_trk_xhz_cov_%s", name.c_str ()), ";#it{x}_{hZ};#it{x}_{hZ}", nXhZBins, xhzBins, nXhZBins, xhzBins);

  h_z_trk_pth_bkg_yield = new TH1D (Form ("h_z_trk_pth_bkg_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_z_trk_xhz_bkg_yield = new TH1D (Form ("h_z_trk_xhz_bkg_yield_%s", name.c_str ()), ";#it{x}_{hZ}", nXhZBins, xhzBins);
  h2_z_trk_pth_bkg_cov = new TH2D (Form ("h2_z_trk_pth_bkg_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_z_trk_xhz_bkg_cov = new TH2D (Form ("h2_z_trk_xhz_bkg_cov_%s", name.c_str ()), ";#it{x}_{hZ};#it{x}_{hZ}", nXhZBins, xhzBins, nXhZBins, xhzBins);

  h_z_trk_dphi_pth_gt4_yield = new TH1D (Form ("h_z_trk_dphi_pth_gt4_yield_%s", name.c_str ()), ";#Delta#phi_{ch,#it{Z}}", nDPhiBins, dPhiBins);
  h2_z_trk_dphi_pth_gt4_cov = new TH2D (Form ("h2_z_trk_dphi_pth_gt4_cov_%s", name.c_str ()), ";#Delta#phi_{ch,#it{Z}}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
  h_z_trk_dphi_pth_lt4_yield = new TH1D (Form ("h_z_trk_dphi_pth_lt4_yield_%s", name.c_str ()), ";#Delta#phi_{ch,#it{Z}}", nDPhiBins, dPhiBins);
  h2_z_trk_dphi_pth_lt4_cov = new TH2D (Form ("h2_z_trk_dphi_pth_lt4_cov_%s", name.c_str ()), ";#Delta#phi_{ch,#it{Z}}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);

  h_z_pt_yield = new TH1D (Form ("h_z_pt_yield_%s", name.c_str ()), ";#it{p}_{T}^{Z} [GeV]", nPtZBins, pTZBins);
  h_z_pt_yield->Sumw2 ();

  h_z_jet_pt_yield = new TH1D (Form ("h_z_jet_pt_yield_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins);
  h2_z_jet_pt_cov = new TH2D (Form ("h2_z_jet_pt_cov_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV];#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins, nPtJBins, pTJBins);
  h_z_jet_pt_dphi_gt7p8_yield = new TH1D (Form ("h_z_jet_pt_dphi_gt7p8_yield_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins);
  h2_z_jet_pt_dphi_gt7p8_cov = new TH2D (Form ("h2_z_jet_pt_dphi_gt7p8_cov_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV];#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins, nPtJBins, pTJBins);

  h_z_jet_dphi_yield = new TH1D (Form ("h_z_jet_dphi_yield_%s", name.c_str ()), ";#Delta#phi_{j#it{Z}}", nDPhiBins, dPhiBins);
  h2_z_jet_dphi_cov = new TH2D (Form ("h2_z_jet_dphi_cov_%s", name.c_str ()), ";#Delta#phi_{j#it{Z}}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);

  h_z_pids = new TH1D (Form ("h_z_pids_%s", name.c_str ()), "", 7, 0, 7);
  h_z_pids->GetXaxis ()->SetBinLabel (1, "f#bar{f}#rightarrowZ");       // code 221
  h_z_pids->GetXaxis ()->SetBinLabel (2, "f#bar{f}#rightarrowZZ");      // code 231
  h_z_pids->GetXaxis ()->SetBinLabel (3, "f#bar{f}#rightarrowZW");      // code 232
  h_z_pids->GetXaxis ()->SetBinLabel (4, "q#bar{q}#rightarrowZg");      // code 241
  h_z_pids->GetXaxis ()->SetBinLabel (5, "qg#rightarrowZq");            // code 242
  h_z_pids->GetXaxis ()->SetBinLabel (6, "f#bar{f}#rightarrowZ#gamma"); // code 243
  h_z_pids->GetXaxis ()->SetBinLabel (7, "f#gamma#rightarrowZf");       // code 244

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    inTree->GetEntry (iEvent);

    h_z_pids->Fill (GetZPIDBin (code));

    float sumptcw = 0;
    float sumptccw = 0;

    //float z_phi_transmax = 0;
    float z_phi_transmin = 0;

    for (int iPart = 0; iPart < part_n; iPart++) {
       float dphi = DeltaPhi (part_phi[iPart], z_phi, true);
       if (pi/3. < fabs (dphi) && fabs (dphi) < 2.*pi/3.) {
         if (dphi < 0) // then trackphi is ccw of zphi
           sumptccw += part_pt[iPart];
         else
           sumptcw += part_pt[iPart];
       }
    }

    if (sumptccw > sumptcw) {
      //z_phi_transmax = z_phi + pi/2.;
      z_phi_transmin = z_phi - pi/2.;
    }
    else {
      //z_phi_transmax = z_phi - pi/2.;
      z_phi_transmin = z_phi + pi/2.;
    }

    // first loop over the particles in the recorded event
    for (int iPart = 0; iPart < part_n; iPart++) {
      if (fabs (part_eta[iPart]) > 2.5)
        continue;

      const float trkpt = part_pt[iPart];
      const float xhz = trkpt / z_pt;
      const float dphi = DeltaPhi (part_phi[iPart], z_phi);

      for (short i = 0; i < nDPhiBins; i++) {
        if (dPhiBins[i] <= dphi && dphi < dPhiBins[i+1]) {
          if (trkpt > 4)
            trk_dphi_pth_gt4_counts[i]++;
          else if (trkpt > 2)
            trk_dphi_pth_lt4_counts[i]++;
          break;
        }
      }

      if (dphi >= 3*pi/4) {
        for (short i = 0; i < nPthBins; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1]) {
            trk_pth_counts[i]++;
            break;
          }
        }
        
        for (short i = 0; i < nXhZBins; i++) {
          if (xhzBins[i] <= xhz && xhz < xhzBins[i+1]) {
            trk_xhz_counts[i]++;
            break;
          }
        }
      }
      else if (DeltaPhi (part_phi[iPart], z_phi_transmin) < pi/8.) {
        for (short i = 0; i < nPthBins; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1]) {
            trk_pth_counts_bkg[i]++;
            break;
          }
        }
        
        for (short i = 0; i < nXhZBins; i++) {
          if (xhzBins[i] <= xhz && xhz < xhzBins[i+1]) {
            trk_xhz_counts_bkg[i]++;
            break;
          }
        }
      }

    } // end loop over iPart

    // now loop over jets
    if (z_pt >= 30 && z_pt < 50) {
      for (int iJ = 0; iJ < akt4_jet_n; iJ++) {
        const float jpt = akt4_jet_pt[iJ];
        const float jeta = akt4_jet_eta[iJ];
        const float dphi = DeltaPhi (akt4_jet_phi[iJ], z_phi);

        if (jpt < 8)
          continue;
        if (fabs (jeta) > 2.5)
          continue;

        for (short i = 0; i < nDPhiBins; i++) {
          if (dPhiBins[i] <= dphi && dphi < dPhiBins[i+1]) {
            jet_dphi_counts[i]++;
            break;
          }
        }

        for (short i = 0; i < nPtJBins; i++) {
          if (pTJBins[i] <= jpt && jpt < pTJBins[i+1]) {
            if (dphi > 7.*pi/8.)
              jet_ptj_dphi_gt7p8_counts[i]++;
            jet_ptj_counts[i]++;
            break;
          }
        }
      }
      nJetEvents++;
    }

    h_z_pt_yield->Fill (z_pt);

    for (short iX = 0; iX < nPthBins; iX++) {
      h_z_trk_pth_yield->SetBinContent (iX+1, h_z_trk_pth_yield->GetBinContent (iX+1) + trk_pth_counts[iX]);
      for (short iY = 0; iY < nPthBins; iY++)
        h2_z_trk_pth_cov->SetBinContent (iX+1, iY+1, h2_z_trk_pth_cov->GetBinContent (iX+1, iY+1) + (trk_pth_counts[iX])*(trk_pth_counts[iY]));
    }
    for (short iX = 0; iX < nXhZBins; iX++) {
      h_z_trk_xhz_yield->SetBinContent (iX+1, h_z_trk_xhz_yield->GetBinContent (iX+1) + trk_xhz_counts[iX]);
      for (short iY = 0; iY < nXhZBins; iY++)
        h2_z_trk_xhz_cov->SetBinContent (iX+1, iY+1, h2_z_trk_xhz_cov->GetBinContent (iX+1, iY+1) + (trk_xhz_counts[iX])*(trk_xhz_counts[iY]));
    }
    for (short iX = 0; iX < nPthBins; iX++) {
      h_z_trk_pth_bkg_yield->SetBinContent (iX+1, h_z_trk_pth_bkg_yield->GetBinContent (iX+1) + trk_pth_counts_bkg[iX]);
      for (short iY = 0; iY < nPthBins; iY++)
        h2_z_trk_pth_bkg_cov->SetBinContent (iX+1, iY+1, h2_z_trk_pth_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_pth_counts_bkg[iX])*(trk_pth_counts_bkg[iY]));
    }
    for (short iX = 0; iX < nXhZBins; iX++) {
      h_z_trk_xhz_bkg_yield->SetBinContent (iX+1, h_z_trk_xhz_bkg_yield->GetBinContent (iX+1) + trk_xhz_counts_bkg[iX]);
      for (short iY = 0; iY < nXhZBins; iY++)
        h2_z_trk_xhz_bkg_cov->SetBinContent (iX+1, iY+1, h2_z_trk_xhz_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_xhz_counts_bkg[iX])*(trk_xhz_counts_bkg[iY]));
    }
    for (short iX = 0; iX < nDPhiBins; iX++) {
      h_z_trk_dphi_pth_gt4_yield->SetBinContent (iX+1, h_z_trk_dphi_pth_gt4_yield->GetBinContent (iX+1) + trk_dphi_pth_gt4_counts[iX]);
      for (short iY = 0; iY < nDPhiBins; iY++)
        h2_z_trk_dphi_pth_gt4_cov->SetBinContent (iX+1, iY+1, h2_z_trk_dphi_pth_gt4_cov->GetBinContent (iX+1, iY+1) + (trk_dphi_pth_gt4_counts[iX])*(trk_dphi_pth_gt4_counts[iY]));
    }
    for (short iX = 0; iX < nDPhiBins; iX++) {
      h_z_trk_dphi_pth_lt4_yield->SetBinContent (iX+1, h_z_trk_dphi_pth_lt4_yield->GetBinContent (iX+1) + trk_dphi_pth_lt4_counts[iX]);
      for (short iY = 0; iY < nDPhiBins; iY++)
        h2_z_trk_dphi_pth_lt4_cov->SetBinContent (iX+1, iY+1, h2_z_trk_dphi_pth_lt4_cov->GetBinContent (iX+1, iY+1) + (trk_dphi_pth_lt4_counts[iX])*(trk_dphi_pth_lt4_counts[iY]));
    }

    for (short iX = 0; iX < nPtJBins; iX++) {
      h_z_jet_pt_yield->SetBinContent (iX+1, h_z_jet_pt_yield->GetBinContent (iX+1) + jet_ptj_counts[iX]);
      for (short iY = 0; iY < nPtJBins; iY++)
        h2_z_jet_pt_cov->SetBinContent (iX+1, iY+1, h2_z_jet_pt_cov->GetBinContent (iX+1, iY+1) + (jet_ptj_counts[iX])*(jet_ptj_counts[iY]));
    }
    for (short iX = 0; iX < nPtJBins; iX++) {
      h_z_jet_pt_dphi_gt7p8_yield->SetBinContent (iX+1, h_z_jet_pt_dphi_gt7p8_yield->GetBinContent (iX+1) + jet_ptj_dphi_gt7p8_counts[iX]);
      for (short iY = 0; iY < nPtJBins; iY++)
        h2_z_jet_pt_dphi_gt7p8_cov->SetBinContent (iX+1, iY+1, h2_z_jet_pt_dphi_gt7p8_cov->GetBinContent (iX+1, iY+1) + (jet_ptj_dphi_gt7p8_counts[iX])*(jet_ptj_dphi_gt7p8_counts[iY]));
    }

    for (short iX = 0; iX < nDPhiBins; iX++) {
      h_z_jet_dphi_yield->SetBinContent (iX+1, h_z_jet_dphi_yield->GetBinContent (iX+1) + jet_dphi_counts[iX]);
      for (short iY = 0; iY < nDPhiBins; iY++)
        h2_z_jet_dphi_cov->SetBinContent (iX+1, iY+1, h2_z_jet_dphi_cov->GetBinContent (iX+1, iY+1) + (jet_dphi_counts[iX])*(jet_dphi_counts[iY]));
    }


    for (int i = 0; i < nPthBins; i++) {
      trk_pth_counts[i] = 0;
      trk_pth_counts_bkg[i] = 0;
    }
    for (int i = 0; i < nXhZBins; i++) {
      trk_xhz_counts[i] = 0;
      trk_xhz_counts_bkg[i] = 0;
    }
    for (int i = 0; i < nDPhiBins; i++) {
      trk_dphi_pth_gt4_counts[i] = 0;
      trk_dphi_pth_lt4_counts[i] = 0;
      jet_dphi_counts[i] = 0;
    }
    for (int i = 0; i < nPtJBins; i++) {
      jet_ptj_counts[i] = 0;
      jet_ptj_dphi_gt7p8_counts[i] = 0;
    }
  } // end loop over iEvent

  inFile->Close ();
  SaferDelete (&inFile);


  const double fnEvents = (double) nEvents;
  const double fnJetEvents = (double) nJetEvents;

  {
    TH2D* h2 = nullptr;
    TH1D* h = nullptr;

    h2 = h2_z_trk_pth_cov;
    h = h_z_trk_pth_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents * pi/4., -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1) * pow(pi/4., 2), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_trk_xhz_cov;
    h = h_z_trk_xhz_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents * pi/4., -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1) * pow(pi/4., 2), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_trk_pth_bkg_cov;
    h = h_z_trk_pth_bkg_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents * pi/8., -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1) * pow(pi/8., 2), -1), true);
    SetVariances (h, h2);

    h2 = h2_z_trk_xhz_bkg_cov;
    h = h_z_trk_xhz_bkg_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents * pi/8., -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1) * pow(pi/8., 2), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_trk_dphi_pth_gt4_cov;
    h = h_z_trk_dphi_pth_gt4_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents, -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_trk_dphi_pth_lt4_cov;
    h = h_z_trk_dphi_pth_lt4_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnEvents));
    ScaleHist (h, pow (fnEvents, -1), true);
    ScaleHist (h2, pow (fnEvents * (fnEvents-1), -1), true);
    SetVariances (h, h2);


    h_z_pt_yield->Scale (1./fnEvents, "width");


    h2 = h2_z_jet_pt_cov;
    h = h_z_jet_pt_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnJetEvents));
    ScaleHist (h, pow (fnJetEvents, -1), true);
    ScaleHist (h2, pow (fnJetEvents * (fnJetEvents-1), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_jet_pt_dphi_gt7p8_cov;
    h = h_z_jet_pt_dphi_gt7p8_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnJetEvents));
    ScaleHist (h, pow (fnJetEvents, -1), true);
    ScaleHist (h2, pow (fnJetEvents * (fnJetEvents-1), -1), true);
    SetVariances (h, h2);


    h2 = h2_z_jet_dphi_cov;
    h = h_z_jet_dphi_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, ((double)(h2->GetBinContent (iX, iY))) - ((double)(h->GetBinContent (iX)))*((double)(h->GetBinContent (iY)))/((double)fnJetEvents));
    ScaleHist (h, pow (fnJetEvents, -1), true);
    ScaleHist (h2, pow (fnJetEvents * (fnJetEvents-1), -1), true);
    SetVariances (h, h2);
  }


  // now save histograms to a rootfile
  h_z_trk_pth_yield->Write ();
  h_z_trk_xhz_yield->Write ();
  h2_z_trk_pth_cov->Write ();
  h2_z_trk_xhz_cov->Write ();
  h_z_trk_pth_bkg_yield->Write ();
  h_z_trk_xhz_bkg_yield->Write ();
  h2_z_trk_pth_bkg_cov->Write ();
  h2_z_trk_xhz_bkg_cov->Write ();
  h_z_trk_dphi_pth_gt4_yield->Write ();
  h2_z_trk_dphi_pth_gt4_cov->Write ();
  h_z_trk_dphi_pth_lt4_yield->Write ();
  h2_z_trk_dphi_pth_lt4_cov->Write ();
  h_z_pt_yield->Write ();
  h_z_pids->Write ();
  h_z_jet_pt_yield->Write ();
  h2_z_jet_pt_cov->Write ();
  h_z_jet_pt_dphi_gt7p8_yield->Write ();
  h2_z_jet_pt_dphi_gt7p8_cov->Write ();
  h_z_jet_dphi_yield->Write ();
  h2_z_jet_dphi_cov->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
