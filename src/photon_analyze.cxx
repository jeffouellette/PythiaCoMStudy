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


int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: photon_analyze NAME INFILENAME OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = string (argv[1]);
  const string inFileName = string (argv[2]);
  const string outFileName = string (argv[3]);

  TFile* inFile;
  TTree* inTree;

  //int code = 0;
  //int id1 = 0;
  //int id2 = 0;
  //float x1pdf = 0;
  //float x2pdf = 0;
  //float Q = 0;
  //bool isValence1 = false;
  //bool isValence2 = false;

  float photon_pt = 0;
  float photon_eta = 0;
  float photon_phi = 0;
  int part_n = 0;
  float part_pt[10000];
  float part_eta[10000];
  float part_phi[10000];

  inFile = new TFile (inFileName.c_str (), "read");
  inTree = (TTree*) inFile->Get ("tree");

  const int nEvents = inTree->GetEntries ();

  //inTree->SetBranchAddress ("code",       &code);
  //inTree->SetBranchAddress ("id1",        &id1);
  //inTree->SetBranchAddress ("id2",        &id2);
  //inTree->SetBranchAddress ("x1pdf",      &x1pdf);
  //inTree->SetBranchAddress ("x2pdf",      &x2pdf);
  //inTree->SetBranchAddress ("Q",          &Q);
  //inTree->SetBranchAddress ("isValence1", &isValence1);
  //inTree->SetBranchAddress ("isValence2", &isValence2);
  inTree->SetBranchAddress ("photon_pt",       &photon_pt);
  inTree->SetBranchAddress ("photon_eta",      &photon_eta);
  inTree->SetBranchAddress ("photon_phi",      &photon_phi);
  inTree->SetBranchAddress ("part_n",     &part_n);
  inTree->SetBranchAddress ("part_pt",    &part_pt);
  inTree->SetBranchAddress ("part_eta",   &part_eta);
  inTree->SetBranchAddress ("part_phi",   &part_phi);


  TH1D* h_trk_g_pth_yield;
  TH1D* h_trk_g_xhg_yield;
  TH2D* h2_trk_g_pth_cov;
  TH2D* h2_trk_g_xhg_cov;
  TH1D* h_trk_g_pth_bkg_yield;
  TH1D* h_trk_g_xhg_bkg_yield;
  TH2D* h2_trk_g_pth_bkg_cov;
  TH2D* h2_trk_g_xhg_bkg_cov;

  TH1D* h_g_pt_yield;

  const float pthBins[11] = {1, 1.5, 2, 3, 4, 6, 8, 10, 15, 30, 60};
  const short nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
  const float xhgBins[12] = {1./60., 1./30., 1./15., 1./10., 1./8., 1./6., 1./4., 1./3., 1./2., 1./1.5, 1., 2.};
  const short nXhGBins = sizeof (xhgBins) / sizeof (xhgBins[0]) - 1;
  const int nPtGBins = 60;
  const double* pTGBins = logspace (5, 300, nPtGBins);

  float trk_counts[2][11] = {{}, {}};
  float trk_counts_bkg[2][11] = {{}, {}};

  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_trk_g_pth_yield = new TH1D (Form ("h_trk_g_pth_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_g_pth_yield->Sumw2 ();
  h_trk_g_xhg_yield = new TH1D (Form ("h_trk_g_xhg_yield_%s", name.c_str ()), ";#it{x}_{h#gamma}", nXhGBins, xhgBins);
  h_trk_g_xhg_yield->Sumw2 ();
  h2_trk_g_pth_cov = new TH2D (Form ("h2_trk_g_pth_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_g_pth_cov->Sumw2 ();
  h2_trk_g_xhg_cov = new TH2D (Form ("h2_trk_g_xhg_cov_%s", name.c_str ()), ";#it{x}_{h#gamma};#it{x}_{h#gamma}", nXhGBins, xhgBins, nXhGBins, xhgBins);
  h2_trk_g_xhg_cov->Sumw2 ();

  h_trk_g_pth_bkg_yield = new TH1D (Form ("h_trk_g_pth_bkg_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_g_pth_bkg_yield->Sumw2 ();
  h_trk_g_xhg_bkg_yield = new TH1D (Form ("h_trk_g_xhg_bkg_yield_%s", name.c_str ()), ";#it{x}_{h#gamma}", nXhGBins, xhgBins);
  h_trk_g_xhg_bkg_yield->Sumw2 ();
  h2_trk_g_pth_bkg_cov = new TH2D (Form ("h2_trk_g_pth_bkg_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_g_pth_bkg_cov->Sumw2 ();
  h2_trk_g_xhg_bkg_cov = new TH2D (Form ("h2_trk_g_xhg_bkg_cov_%s", name.c_str ()), ";#it{x}_{h#gamma};#it{x}_{h#gamma}", nXhGBins, xhgBins, nXhGBins, xhgBins);
  h2_trk_g_xhg_bkg_cov->Sumw2 ();

  h_g_pt_yield = new TH1D (Form ("h_g_pt_yield_%s", name.c_str ()), ";#it{p}_{T}^{#gamma} [GeV]", nPtGBins, pTGBins);
  h_g_pt_yield->Sumw2 ();

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    inTree->GetEntry (iEvent);

    float sumptcw = 0;
    float sumptccw = 0;

    //float photon_phi_transmax = 0;
    float photon_phi_transmin = 0;

    for (int iPart = 0; iPart < part_n; iPart++) {
       float dphi = DeltaPhi (part_phi[iPart], photon_phi, true);
       if (pi/3. < fabs (dphi) && fabs (dphi) < 2.*pi/3.) {
         if (dphi < 0) // then trackphi is ccw of zphi
           sumptccw += part_pt[iPart];
         else
           sumptcw += part_pt[iPart];
       }
    }

    if (sumptccw > sumptcw) {
      //photon_phi_transmax = photon_phi + pi/2.;
      photon_phi_transmin = photon_phi - pi/2.;
    }
    else {
      //photon_phi_transmax = photon_phi - pi/2.;
      photon_phi_transmin = photon_phi + pi/2.;
    }

    // first loop over the particles in the recorded event
    for (int iPart = 0; iPart < part_n; iPart++) {
      if (fabs (part_eta[iPart]) > 2.5)
        continue;

      const double trkpt = part_pt[iPart];
      const double xhg = trkpt / photon_pt;

      if (DeltaPhi (part_phi[iPart], photon_phi) >= 3*pi/4) {
        for (short i = 0; i < nPthBins-1; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1])
            trk_counts_bkg[0][i] += 1.;
        }
        
        for (short i = 0; i < nXhGBins-1; i++) {
          if (xhgBins[i] <= xhg && xhg < xhgBins[i+1])
            trk_counts_bkg[1][i] += 1.;
        }
      }
      else if (DeltaPhi (part_phi[iPart], photon_phi_transmin) < pi/8.) {
        for (short i = 0; i < nPthBins-1; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1])
            trk_counts_bkg[0][i] += 1.;
        }
        
        for (short i = 0; i < nXhGBins-1; i++) {
          if (xhgBins[i] <= xhg && xhg < xhgBins[i+1])
            trk_counts_bkg[1][i] += 1.;
        }
      }

    } // end loop over iPart

    h_g_pt_yield->Fill (photon_pt);

    for (short iX = 0; iX < nPthBins; iX++) {
      h_trk_g_pth_yield->SetBinContent (iX+1, h_trk_g_pth_yield->GetBinContent (iX+1) + trk_counts[0][iX]);
      for (short iY = 0; iY < nPthBins; iY++) {
        h2_trk_g_pth_cov->SetBinContent (iX+1, iY+1, h2_trk_g_pth_cov->GetBinContent (iX+1, iY+1) + (trk_counts[0][iX])*(trk_counts[0][iY]));
      }
    }
    for (short iX = 0; iX < nXhGBins; iX++) {
      h_trk_g_xhg_yield->SetBinContent (iX+1, h_trk_g_xhg_yield->GetBinContent (iX+1) + trk_counts[1][iX]);
      for (short iY = 0; iY < nXhGBins; iY++) {
        h2_trk_g_xhg_cov->SetBinContent (iX+1, iY+1, h2_trk_g_xhg_cov->GetBinContent (iX+1, iY+1) + (trk_counts[1][iX])*(trk_counts[1][iY]));
      }
    }
    for (short iX = 0; iX < nPthBins; iX++) {
      h_trk_g_pth_bkg_yield->SetBinContent (iX+1, h_trk_g_pth_bkg_yield->GetBinContent (iX+1) + trk_counts_bkg[0][iX]);
      for (short iY = 0; iY < nPthBins; iY++) {
        h2_trk_g_pth_bkg_cov->SetBinContent (iX+1, iY+1, h2_trk_g_pth_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_counts_bkg[0][iX])*(trk_counts_bkg[0][iY]));
      }
    }
    for (short iX = 0; iX < nXhGBins; iX++) {
      h_trk_g_xhg_bkg_yield->SetBinContent (iX+1, h_trk_g_xhg_bkg_yield->GetBinContent (iX+1) + trk_counts_bkg[1][iX]);
      for (short iY = 0; iY < nXhGBins; iY++) {
        h2_trk_g_xhg_bkg_cov->SetBinContent (iX+1, iY+1, h2_trk_g_xhg_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_counts_bkg[1][iX])*(trk_counts_bkg[1][iY]));
      }
    }

    for (int i = 0; i < 11; i++) {
      trk_counts[0][i] = 0;
      trk_counts[1][i] = 0;
      trk_counts_bkg[0][i] = 0;
      trk_counts_bkg[1][i] = 0;
    }
  } // end loop over iEvent

  inFile->Close ();
  SaferDelete (&inFile);

  {
    h_trk_g_pth_yield->Scale (1./(nEvents * pi/4.), "width");
    h_trk_g_xhg_yield->Scale (1./(nEvents * pi/4.), "width");

    h2_trk_g_pth_cov->Scale (1./(pow (pi/4., 2)), "width");
    h2_trk_g_xhg_cov->Scale (1./(pow (pi/4., 2)), "width");

    TH2D* h2 = h2_trk_g_pth_cov;
    TH1D* h = h_trk_g_pth_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));
    h2 = h2_trk_g_xhg_cov;
    h = h_trk_g_xhg_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));

    h2_trk_g_pth_cov->Scale (1./(nEvents * (nEvents - 1)));
    h2_trk_g_xhg_cov->Scale (1./(nEvents * (nEvents - 1)));


    h_trk_g_pth_bkg_yield->Scale (1./(nEvents * pi/8.), "width");
    h_trk_g_xhg_bkg_yield->Scale (1./(nEvents * pi/8.), "width");

    h2_trk_g_pth_bkg_cov->Scale (1./(pow (pi/8., 2)), "width");
    h2_trk_g_xhg_bkg_cov->Scale (1./(pow (pi/8., 2)), "width");

    h2 = h2_trk_g_pth_bkg_cov;
    h = h_trk_g_pth_bkg_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));
    h2 = h2_trk_g_xhg_bkg_cov;
    h = h_trk_g_xhg_bkg_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));

    h2_trk_g_pth_bkg_cov->Scale (1./(nEvents * (nEvents - 1)));
    h2_trk_g_xhg_bkg_cov->Scale (1./(nEvents * (nEvents - 1)));
  }


  // now save histograms to a rootfile
  h_trk_g_pth_yield->Write ();
  h_trk_g_xhg_yield->Write ();
  h2_trk_g_pth_cov->Write ();
  h2_trk_g_xhg_cov->Write ();
  h_trk_g_pth_bkg_yield->Write ();
  h_trk_g_xhg_bkg_yield->Write ();
  h2_trk_g_pth_bkg_cov->Write ();
  h2_trk_g_xhg_bkg_cov->Write ();
  h_g_pt_yield->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
