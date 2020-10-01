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
//TH1D* h_trk_z_xhz_yield[2];
//TH1D* h_trk_z_xhz_bkg_yield[2];
//TH1D* h_trk_z_xhz_yield_ratio;
TH2D* h2_trk_z_pth_cov[2];
TH2D* h2_trk_z_pth_bkg_cov[2];
//TH2D* h2_trk_z_xhz_cov[2];
//TH2D* h2_trk_z_xhz_bkg_cov[2];
TH1D* h_trk_g_pth_yield[2];
TH1D* h_trk_g_pth_bkg_yield[2];
TH1D* h_trk_g_pth_yield_ratio;
//TH1D* h_trk_g_xhg_yield[2];
//TH1D* h_trk_g_xhg_bkg_yield[2];
//TH1D* h_trk_g_xhg_yield_ratio;
TH2D* h2_trk_g_pth_cov[2];
TH2D* h2_trk_g_pth_bkg_cov[2];
//TH2D* h2_trk_g_xhg_cov[2];
//TH2D* h2_trk_g_xhg_bkg_cov[2];

TH1D* h_z_pt_yield[2];
TH1D* h_z_pt_yield_ratio;
TH1D* h_g_pt_yield[2];
TH1D* h_g_pt_yield_ratio;


void SetVariances (TH1D* h, TH2D* h2) {
  const int nb = h->GetNbinsX ();
  assert (nb == h2->GetNbinsX () && nb == h2->GetNbinsY ());

  for (int iX = 1; iX <= nb; iX++) {
    assert (h2->GetBinContent (iX, iX) >= 0);
    h->SetBinError (iX, sqrt (h2->GetBinContent (iX, iX)));
  }
}


int main () {

  SetAtlasStyle(); 

  TFile* inFile = nullptr;
  inFile = new TFile ("rootFiles/z_out_sqrts5020GeV.root", "read");

  h_trk_z_pth_yield[0] = (TH1D*) inFile->Get ("h_trk_z_pth_yield_sqrts5020GeV");
  h_trk_z_pth_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_z_pth_bkg_yield_sqrts5020GeV");
  //h_trk_z_xhz_yield[0] = (TH1D*) inFile->Get ("h_trk_z_xhz_yield_sqrts5020GeV");
  //h_trk_z_xhz_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_z_xhz_bkg_yield_sqrts5020GeV");
  h2_trk_z_pth_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_pth_cov_sqrts5020GeV");
  h2_trk_z_pth_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_pth_bkg_cov_sqrts5020GeV");
  //h2_trk_z_xhz_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_xhz_cov_sqrts5020GeV");
  //h2_trk_z_xhz_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_z_xhz_bkg_cov_sqrts5020GeV");
  SetVariances (h_trk_z_pth_yield[0], h2_trk_z_pth_cov[0]);
  SetVariances (h_trk_z_pth_bkg_yield[0], h2_trk_z_pth_bkg_cov[0]);
  //SetVariances (h_trk_z_xhz_yield[0], h2_trk_z_xhz_cov[0]);
  //SetVariances (h_trk_z_xhz_bkg_yield[0], h2_trk_z_xhz_bkg_cov[0]);
  h_trk_z_pth_yield[0]->Add (h_trk_z_pth_bkg_yield[0], -1);
  //h_trk_z_xhz_yield[0]->Add (h_trk_z_xhz_bkg_yield[0], -1);
  h_z_pt_yield[0] = (TH1D*) inFile->Get ("h_z_pt_yield_sqrts5020GeV");

  inFile = new TFile ("rootFiles/z_out_sqrts8160GeV.root", "read");

  h_trk_z_pth_yield[1] = (TH1D*) inFile->Get ("h_trk_z_pth_yield_sqrts8160GeV");
  h_trk_z_pth_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_z_pth_bkg_yield_sqrts8160GeV");
  //h_trk_z_xhz_yield[1] = (TH1D*) inFile->Get ("h_trk_z_xhz_yield_sqrts8160GeV");
  //h_trk_z_xhz_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_z_xhz_bkg_yield_sqrts8160GeV");
  h2_trk_z_pth_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_pth_cov_sqrts8160GeV");
  h2_trk_z_pth_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_pth_bkg_cov_sqrts8160GeV");
  //h2_trk_z_xhz_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_xhz_cov_sqrts8160GeV");
  //h2_trk_z_xhz_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_z_xhz_bkg_cov_sqrts8160GeV");
  SetVariances (h_trk_z_pth_yield[1], h2_trk_z_pth_cov[1]);
  SetVariances (h_trk_z_pth_bkg_yield[1], h2_trk_z_pth_bkg_cov[1]);
  //SetVariances (h_trk_z_xhz_yield[1], h2_trk_z_xhz_cov[1]);
  //SetVariances (h_trk_z_xhz_bkg_yield[1], h2_trk_z_xhz_bkg_cov[1]);
  h_trk_z_pth_yield[1]->Add (h_trk_z_pth_bkg_yield[1], -1);
  //h_trk_z_xhz_yield[1]->Add (h_trk_z_xhz_bkg_yield[1], -1);
  h_z_pt_yield[1] = (TH1D*) inFile->Get ("h_z_pt_yield_sqrts8160GeV");

  h_trk_z_pth_yield_ratio = (TH1D*) h_trk_z_pth_yield[1]->Clone ("h_trk_z_pth_yield_ratio");
  h_trk_z_pth_yield_ratio->Divide (h_trk_z_pth_yield[0]);
  h_z_pt_yield_ratio = (TH1D*) h_z_pt_yield[1]->Clone ("h_z_pt_yield_ratio");
  h_z_pt_yield_ratio->Divide (h_z_pt_yield[0]);


  inFile = new TFile ("rootFiles/photon_out_sqrts5020GeV.root", "read");

  h_trk_g_pth_yield[0] = (TH1D*) inFile->Get ("h_trk_g_pth_yield_sqrts5020GeV");
  h_trk_g_pth_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_g_pth_bkg_yield_sqrts5020GeV");
  //h_trk_g_xhg_yield[0] = (TH1D*) inFile->Get ("h_trk_g_xhg_yield_sqrts5020GeV");
  //h_trk_g_xhg_bkg_yield[0] = (TH1D*) inFile->Get ("h_trk_g_xhg_bkg_yield_sqrts5020GeV");
  h2_trk_g_pth_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_pth_cov_sqrts5020GeV");
  h2_trk_g_pth_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_pth_bkg_cov_sqrts5020GeV");
  //h2_trk_g_xhg_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_xhg_cov_sqrts5020GeV");
  //h2_trk_g_xhg_bkg_cov[0] = (TH2D*) inFile->Get ("h2_trk_g_xhg_bkg_cov_sqrts5020GeV");
  SetVariances (h_trk_g_pth_yield[0], h2_trk_g_pth_cov[0]);
  SetVariances (h_trk_g_pth_bkg_yield[0], h2_trk_g_pth_bkg_cov[0]);
  //SetVariances (h_trk_g_xhg_yield[0], h2_trk_g_xhg_cov[0]);
  //SetVariances (h_trk_g_xhg_bkg_yield[0], h2_trk_g_xhg_bkg_cov[0]);
  h_trk_g_pth_yield[0]->Add (h_trk_g_pth_bkg_yield[0], -1);
  //h_trk_g_xhg_yield[0]->Add (h_trk_g_xhg_bkg_yield[0], -1);
  h_g_pt_yield[0] = (TH1D*) inFile->Get ("h_g_pt_yield_sqrts5020GeV");

  inFile = new TFile ("rootFiles/photon_out_sqrts8160GeV.root", "read");

  h_trk_g_pth_yield[1] = (TH1D*) inFile->Get ("h_trk_g_pth_yield_sqrts8160GeV");
  h_trk_g_pth_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_g_pth_bkg_yield_sqrts8160GeV");
  //h_trk_g_xhg_yield[1] = (TH1D*) inFile->Get ("h_trk_g_xhg_yield_sqrts8160GeV");
  //h_trk_g_xhg_bkg_yield[1] = (TH1D*) inFile->Get ("h_trk_g_xhg_bkg_yield_sqrts8160GeV");
  h2_trk_g_pth_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_pth_cov_sqrts8160GeV");
  h2_trk_g_pth_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_pth_bkg_cov_sqrts8160GeV");
  //h2_trk_g_xhg_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_xhg_cov_sqrts8160GeV");
  //h2_trk_g_xhg_bkg_cov[1] = (TH2D*) inFile->Get ("h2_trk_g_xhg_bkg_cov_sqrts8160GeV");
  SetVariances (h_trk_g_pth_yield[1], h2_trk_g_pth_cov[1]);
  SetVariances (h_trk_g_pth_bkg_yield[1], h2_trk_g_pth_bkg_cov[1]);
  //SetVariances (h_trk_g_xhg_yield[1], h2_trk_g_xhg_cov[1]);
  //SetVariances (h_trk_g_xhg_bkg_yield[1], h2_trk_g_xhg_bkg_cov[1]);
  h_trk_g_pth_yield[1]->Add (h_trk_g_pth_bkg_yield[1], -1);
  //h_trk_g_xhg_yield[1]->Add (h_trk_g_xhg_bkg_yield[1], -1);
  h_g_pt_yield[1] = (TH1D*) inFile->Get ("h_g_pt_yield_sqrts8160GeV");

  h_trk_g_pth_yield_ratio = (TH1D*) h_trk_g_pth_yield[1]->Clone ("h_trk_g_pth_yield_ratio");
  h_trk_g_pth_yield_ratio->Divide (h_trk_g_pth_yield[0]);
  h_g_pt_yield_ratio = (TH1D*) h_g_pt_yield[1]->Clone ("h_g_pt_yield_ratio");
  h_g_pt_yield_ratio->Divide (h_g_pt_yield[0]);



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

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();
    h = h_trk_z_pth_yield[0];
    h->SetLineColor (kAzure-2);
    h->SetLineWidth (2);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("d^{2}N_{ch} / d#it{p}_{T} d#Delta#phi [GeV^{-1}]");
    h->GetYaxis ()->SetTitle ("(1/N_{Z}) (d^{2}N_{ch} / d#it{p}_{T}^{ch} d#Delta#phi_{ch,Z}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->Draw ("hist");

    h = h_trk_z_pth_yield[1];
    h->SetLineColor (kRed+1);
    h->SetLineWidth (2);

    h->Draw ("hist same");

    myText (0.65, 0.80, kBlack, "Pythia 8 #it{pp}", 0.04/0.6);
    myText (0.65, 0.71, kBlack, "#it{p}_{T}^{Z} > 30 GeV", 0.04/0.6);
    myText (0.65, 0.62, kAzure-2, "#sqrt{s} = 5.02 TeV", 0.04/0.6);
    myText (0.65, 0.53, kRed+1, "#sqrt{s} = 8.16 TeV", 0.04/0.6);

    dPad->cd (); 
    dPad->SetLogx ();
    h = (TH1D*) h_trk_z_pth_yield_ratio->Clone ("h");
    TGAE* g = make_graph (h);
    h->Reset ();
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetXaxis ()->SetRangeUser (h->GetBinLowEdge (1), h->GetBinLowEdge (h->GetNbinsX ()) + h->GetBinWidth (h->GetNbinsX ()));
    h->GetYaxis ()->SetRangeUser (0.74, 1.26);
    //h->GetYaxis ()->SetRangeUser (0.0, 2.0);

    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (3.0*0.4);
    h->GetYaxis ()->SetTitle ("8.16 / 5.02");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (0);
    h->Draw ("hist");

    g->SetLineColor (kBlack);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    ((TGAE*) g->Clone ())->Draw ("P");

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

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();
    h = h_trk_g_pth_yield[0];
    h->SetLineColor (kAzure-2);
    h->SetLineWidth (2);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (d^{2}N_{ch} / d#it{p}_{T}^{ch} d#Delta#phi_{ch,#gamma}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->Draw ("hist");

    h = h_trk_g_pth_yield[1];
    h->SetLineColor (kRed+1);
    h->SetLineWidth (2);

    h->Draw ("hist same");

    myText (0.65, 0.80, kBlack, "Pythia 8 #it{pp}", 0.04/0.6);
    myText (0.65, 0.71, kBlack, "#it{p}_{T}^{#gamma} > 50 GeV", 0.04/0.6);
    myText (0.65, 0.62, kAzure-2, "#sqrt{s} = 5.02 TeV", 0.04/0.6);
    myText (0.65, 0.53, kRed+1, "#sqrt{s} = 8.16 TeV", 0.04/0.6);

    dPad->cd (); 
    dPad->SetLogx ();
    h = (TH1D*) h_trk_g_pth_yield_ratio->Clone ("h");
    TGAE* g = make_graph (h);
    h->Reset ();
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetXaxis ()->SetRangeUser (h->GetBinLowEdge (1), h->GetBinLowEdge (h->GetNbinsX ()) + h->GetBinWidth (h->GetNbinsX ()));
    h->GetYaxis ()->SetRangeUser (0.74, 1.26);
    //h->GetYaxis ()->SetRangeUser (0.0, 2.0);

    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (3.0*0.4);
    h->GetYaxis ()->SetTitle ("8.16 / 5.02");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (0);
    h->Draw ("hist");

    g->SetLineColor (kBlack);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    ((TGAE*) g->Clone ())->Draw ("P");

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

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();
    h = h_z_pt_yield[0];
    h->SetLineColor (kAzure-2);
    h->SetLineWidth (2);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{Z} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{Z}) (dN_{Z} / d#it{p}_{T}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->Draw ("hist");

    h = h_z_pt_yield[1];
    h->SetLineColor (kRed+1);
    h->SetLineWidth (2);

    h->Draw ("hist same");

    myText (0.65, 0.80, kBlack, "Pythia 8 #it{pp}", 0.04/0.6);
    myText (0.65, 0.71, kAzure-2, "#sqrt{s} = 5.02 TeV", 0.04/0.6);
    myText (0.65, 0.62, kRed+1, "#sqrt{s} = 8.16 TeV", 0.04/0.6);

    dPad->cd (); 
    dPad->SetLogx ();
    h = (TH1D*) h_z_pt_yield_ratio->Clone ("h");
    TGAE* g = make_graph (h);
    h->Reset ();
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetXaxis ()->SetRangeUser (h->GetBinLowEdge (1), h->GetBinLowEdge (h->GetNbinsX ()) + h->GetBinWidth (h->GetNbinsX ()));
    h->GetYaxis ()->SetRangeUser (0.74, 1.26);

    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{Z} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (3.0*0.4);
    h->GetYaxis ()->SetTitle ("8.16 / 5.02");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (0);
    h->Draw ("hist");

    g->SetLineColor (kBlack);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    ((TGAE*) g->Clone ())->Draw ("P");

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

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();
    h = h_g_pt_yield[0];
    h->SetLineColor (kAzure-2);
    h->SetLineWidth (2);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{#gamma} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("(1/N_{#gamma}) (dN_{#gamma} / d#it{p}_{T}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->Draw ("hist");

    h = h_g_pt_yield[1];
    h->SetLineColor (kRed+1);
    h->SetLineWidth (2);

    h->Draw ("hist same");

    myText (0.65, 0.80, kBlack, "Pythia 8 #it{pp}", 0.04/0.6);
    myText (0.65, 0.71, kAzure-2, "#sqrt{s} = 5.02 TeV", 0.04/0.6);
    myText (0.65, 0.62, kRed+1, "#sqrt{s} = 8.16 TeV", 0.04/0.6);

    dPad->cd (); 
    dPad->SetLogx ();
    h = (TH1D*) h_g_pt_yield_ratio->Clone ("h");
    TGAE* g = make_graph (h);
    h->Reset ();
    h->GetXaxis ()->SetMoreLogLabels ();
    h->GetXaxis ()->SetRangeUser (h->GetBinLowEdge (1), h->GetBinLowEdge (h->GetNbinsX ()) + h->GetBinWidth (h->GetNbinsX ()));
    h->GetYaxis ()->SetRangeUser (0.74, 1.26);

    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{#gamma} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (3.0*0.4);
    h->GetYaxis ()->SetTitle ("8.16 / 5.02");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (0);
    h->Draw ("hist");

    g->SetLineColor (kBlack);
    g->SetMarkerColor (kBlack);
    g->SetMarkerStyle (kFullCircle);
    ((TGAE*) g->Clone ())->Draw ("P");

    c->SaveAs ("Plots/Photon_pt_yields_comparison.pdf"); 
  }

  

  return 0;
}


void plotter () {
  main ();
}
