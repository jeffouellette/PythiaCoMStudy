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
  int part_n = 0;
  float part_pt[10000];
  float part_eta[10000];
  float part_phi[10000];

  inFile = new TFile (inFileName.c_str (), "read");
  inTree = (TTree*) inFile->Get ("tree");

  const int nEvents = inTree->GetEntries ();

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
  inTree->SetBranchAddress ("part_n",     &part_n);
  inTree->SetBranchAddress ("part_pt",    &part_pt);
  inTree->SetBranchAddress ("part_eta",   &part_eta);
  inTree->SetBranchAddress ("part_phi",   &part_phi);


  TH1D* h_trk_z_pth_yield;
  TH1D* h_trk_z_xhz_yield;
  TH2D* h2_trk_z_pth_cov;
  TH2D* h2_trk_z_xhz_cov;
  TH1D* h_trk_z_pth_bkg_yield;
  TH1D* h_trk_z_xhz_bkg_yield;
  TH2D* h2_trk_z_pth_bkg_cov;
  TH2D* h2_trk_z_xhz_bkg_cov;

  TH1D* h_z_pt_yield;

  TH1D* h_z_pids;

  const float pthBins[10] = {0.5, 1, 2, 4, 8, 15, 30, 60, 120, 240};
  const short nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
  const float xhzBins[9] = {1./120., 1./60., 1./30., 1./15., 1./8., 1./4., 1./2., 1., 2.};
  const short nXhZBins = sizeof (xhzBins) / sizeof (xhzBins[0]) - 1;
  const int nPtZBins = 30;
  const double* pTZBins = logspace (5, 300, nPtZBins);

  float trk_counts[2][10] = {{}, {}};
  float trk_counts_bkg[2][10] = {{}, {}};

  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_trk_z_pth_yield = new TH1D (Form ("h_trk_z_pth_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_z_pth_yield->Sumw2 ();
  h_trk_z_xhz_yield = new TH1D (Form ("h_trk_z_xhz_yield_%s", name.c_str ()), ";#it{x}_{hZ}", nXhZBins, xhzBins);
  h_trk_z_xhz_yield->Sumw2 ();
  h2_trk_z_pth_cov = new TH2D (Form ("h2_trk_z_pth_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_z_pth_cov->Sumw2 ();
  h2_trk_z_xhz_cov = new TH2D (Form ("h2_trk_z_xhz_cov_%s", name.c_str ()), ";#it{x}_{hZ};#it{x}_{hZ}", nXhZBins, xhzBins, nXhZBins, xhzBins);
  h2_trk_z_xhz_cov->Sumw2 ();

  h_trk_z_pth_bkg_yield = new TH1D (Form ("h_trk_z_pth_bkg_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_z_pth_bkg_yield->Sumw2 ();
  h_trk_z_xhz_bkg_yield = new TH1D (Form ("h_trk_z_xhz_bkg_yield_%s", name.c_str ()), ";#it{x}_{hZ}", nXhZBins, xhzBins);
  h_trk_z_xhz_bkg_yield->Sumw2 ();
  h2_trk_z_pth_bkg_cov = new TH2D (Form ("h2_trk_z_pth_bkg_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_z_pth_bkg_cov->Sumw2 ();
  h2_trk_z_xhz_bkg_cov = new TH2D (Form ("h2_trk_z_xhz_bkg_cov_%s", name.c_str ()), ";#it{x}_{hZ};#it{x}_{hZ}", nXhZBins, xhzBins, nXhZBins, xhzBins);
  h2_trk_z_xhz_bkg_cov->Sumw2 ();

  h_z_pt_yield = new TH1D (Form ("h_z_pt_yield_%s", name.c_str ()), ";#it{p}_{T}^{Z} [GeV]", nPtZBins, pTZBins);
  h_z_pt_yield->Sumw2 ();

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

      const double trkpt = part_pt[iPart];
      const double xhz = trkpt / z_pt;

      if (DeltaPhi (part_phi[iPart], z_phi) >= 3*pi/4) {
        for (short i = 0; i < nPthBins-1; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1])
            trk_counts[0][i] += 1.;
        }
        
        for (short i = 0; i < nXhZBins-1; i++) {
          if (xhzBins[i] <= xhz && xhz < xhzBins[i+1])
            trk_counts[1][i] += 1.;
        }
      }
      else if (DeltaPhi (part_phi[iPart], z_phi_transmin) < pi/8.) {
        for (short i = 0; i < nPthBins-1; i++) {
          if (pthBins[i] <= trkpt && trkpt < pthBins[i+1])
            trk_counts_bkg[0][i] += 1.;
        }
        
        for (short i = 0; i < nXhZBins-1; i++) {
          if (xhzBins[i] <= xhz && xhz < xhzBins[i+1])
            trk_counts_bkg[1][i] += 1.;
        }
      }

    } // end loop over iPart

    h_z_pt_yield->Fill (z_pt);

    for (short iX = 0; iX < nPthBins; iX++) {
      h_trk_z_pth_yield->SetBinContent (iX+1, h_trk_z_pth_yield->GetBinContent (iX+1) + trk_counts[0][iX]);
      for (short iY = 0; iY < nPthBins; iY++) {
        h2_trk_z_pth_cov->SetBinContent (iX+1, iY+1, h2_trk_z_pth_cov->GetBinContent (iX+1, iY+1) + (trk_counts[0][iX])*(trk_counts[0][iY]));
      }
    }
    for (short iX = 0; iX < nXhZBins; iX++) {
      h_trk_z_xhz_yield->SetBinContent (iX+1, h_trk_z_xhz_yield->GetBinContent (iX+1) + trk_counts[1][iX]);
      for (short iY = 0; iY < nXhZBins; iY++) {
        h2_trk_z_xhz_cov->SetBinContent (iX+1, iY+1, h2_trk_z_xhz_cov->GetBinContent (iX+1, iY+1) + (trk_counts[1][iX])*(trk_counts[1][iY]));
      }
    }
    for (short iX = 0; iX < nPthBins; iX++) {
      h_trk_z_pth_bkg_yield->SetBinContent (iX+1, h_trk_z_pth_bkg_yield->GetBinContent (iX+1) + trk_counts_bkg[0][iX]);
      for (short iY = 0; iY < nPthBins; iY++) {
        h2_trk_z_pth_bkg_cov->SetBinContent (iX+1, iY+1, h2_trk_z_pth_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_counts_bkg[0][iX])*(trk_counts_bkg[0][iY]));
      }
    }
    for (short iX = 0; iX < nXhZBins; iX++) {
      h_trk_z_xhz_bkg_yield->SetBinContent (iX+1, h_trk_z_xhz_bkg_yield->GetBinContent (iX+1) + trk_counts_bkg[1][iX]);
      for (short iY = 0; iY < nXhZBins; iY++) {
        h2_trk_z_xhz_bkg_cov->SetBinContent (iX+1, iY+1, h2_trk_z_xhz_bkg_cov->GetBinContent (iX+1, iY+1) + (trk_counts_bkg[1][iX])*(trk_counts_bkg[1][iY]));
      }
    }

    for (int i = 0; i < 10; i++) {
      trk_counts[0][i] = 0;
      trk_counts[1][i] = 0;
      trk_counts_bkg[0][i] = 0;
      trk_counts_bkg[1][i] = 0;
    }
  } // end loop over iEvent

  inFile->Close ();
  SaferDelete (&inFile);

  {
    h_trk_z_pth_yield->Scale (1./(nEvents * pi/4.), "width");
    h_trk_z_xhz_yield->Scale (1./(nEvents * pi/4.), "width");

    h2_trk_z_pth_cov->Scale (1./(pow (pi/4., 2)), "width");
    h2_trk_z_xhz_cov->Scale (1./(pow (pi/4., 2)), "width");

    TH2D* h2 = h2_trk_z_pth_cov;
    TH1D* h = h_trk_z_pth_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));
    h2 = h2_trk_z_xhz_cov;
    h = h_trk_z_xhz_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));

    h2_trk_z_pth_cov->Scale (1./(nEvents * (nEvents - 1)));
    h2_trk_z_xhz_cov->Scale (1./(nEvents * (nEvents - 1)));


    h_trk_z_pth_bkg_yield->Scale (1./(nEvents * pi/8.), "width");
    h_trk_z_xhz_bkg_yield->Scale (1./(nEvents * pi/8.), "width");

    h2_trk_z_pth_bkg_cov->Scale (1./(pow (pi/8., 2)), "width");
    h2_trk_z_xhz_bkg_cov->Scale (1./(pow (pi/8., 2)), "width");

    h2 = h2_trk_z_pth_bkg_cov;
    h = h_trk_z_pth_bkg_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));
    h2 = h2_trk_z_xhz_bkg_cov;
    h = h_trk_z_xhz_bkg_yield;
    for (short iX = 0; iX < h2->GetNbinsX (); iX++)
      for (short iY = 0; iY < h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) - nEvents * (h->GetBinContent (iX+1))*(h->GetBinContent (iY+1)));

    h2_trk_z_pth_bkg_cov->Scale (1./(nEvents * (nEvents - 1)));
    h2_trk_z_xhz_bkg_cov->Scale (1./(nEvents * (nEvents - 1)));

    h_z_pt_yield->Scale (1./nEvents, "width");
  }


  // now save histograms to a rootfile
  h_trk_z_pth_yield->Write ();
  h_trk_z_xhz_yield->Write ();
  h2_trk_z_pth_cov->Write ();
  h2_trk_z_xhz_cov->Write ();
  h_trk_z_pth_bkg_yield->Write ();
  h_trk_z_xhz_bkg_yield->Write ();
  h2_trk_z_pth_bkg_cov->Write ();
  h2_trk_z_xhz_bkg_cov->Write ();
  h_z_pt_yield->Write ();
  h_z_pids->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
