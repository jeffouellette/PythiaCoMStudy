#ifndef __zgen_cxx__
#define __zgen_cxx__

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <sstream>

#include "fastjet/ClusterSequence.hh"

#include "Pythia8/Pythia.h"

#include <Utilities.h>

#include "header.h"

using namespace Pythia8;
using namespace PythiaCoMStudy;


int main (int argc, char** argv) {

  if (argc < 6) {
    std::cout << " usage: z_gen SEED SQRTS MINPTHAT NEVT FILENAMEOUT" << std::endl;
    return 0;
  }

  // get arguments
  const int seed = atoi (argv[1]);
  const float sqrts = atof (argv[2]);
  const float ptHatMin = atof (argv[3]);
  const int nEvents = atoi (argv[4]);
  const string outFileName = string (argv[5]);

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;

  pythia.readString ("Random:setSeed = on");
  pythia.readString (Form ("Random:seed = %i", seed));

  pythia.readString (Form ("Beams:eCM = %g", sqrts));

  pythia.readString ("23:onMode = off");
  pythia.readString ("23:onIfAny = 11 13");

  pythia.readString ("WeakZ0:gmZmode = 2"); // set to Z's
  pythia.readString ("WeakSingleBoson:ffbar2gmZ = on");       // code 221
  pythia.readString ("WeakDoubleBoson:ffbar2gmZgmZ = on");    // code 231
  pythia.readString ("WeakDoubleBoson:ffbar2ZW = on");        // code 232
  pythia.readString ("WeakBosonAndParton:qqbar2gmZg = on");   // code 241
  pythia.readString ("WeakBosonAndParton:qg2gmZq = on");      // code 242
  pythia.readString ("WeakBosonAndParton:ffbar2gmZgm = on");  // code 243
  pythia.readString ("WeakBosonAndParton:fgm2gmZf = on");     // code 244

  pythia.readString (Form ("PhaseSpace:pTHatMin = %g", ptHatMin));
  pythia.readString ("PhaseSpace:mHatMin = 60");

  pythia.init ();


  fastjet::JetDefinition antiKt4 (fastjet::antikt_algorithm, 0.4);


  TFile* outFile = new TFile (outFileName.c_str (), "RECREATE");

  int b_code = 0;
  int b_id1 = 0;
  int b_id2 = 0;
  float b_x1pdf = 0;
  float b_x2pdf = 0;
  float b_Q = 0;
  bool b_isValence1 = false;
  bool b_isValence2 = false;

  float b_z_pt = 0;
  float b_z_eta = 0;
  float b_z_phi = 0;
  float b_z_m = 0;

  int b_akt4_jet_n = 0;
  float b_akt4_jet_pt[1000];
  float b_akt4_jet_eta[1000];
  float b_akt4_jet_phi[1000];
  float b_akt4_jet_e[1000];

  int b_part_n = 0;
  float b_part_pt[10000];
  float b_part_eta[10000];
  float b_part_phi[10000];
  float b_part_e[10000];

  TTree* outTree = new TTree ("tree", "tree");

  outTree->Branch ("code",        &b_code,        "code/I");
  outTree->Branch ("id1",         &b_id1,         "id1/I");
  outTree->Branch ("id2",         &b_id2,         "id2/I");
  outTree->Branch ("x1pdf",       &b_x1pdf,       "x1pdf/F");
  outTree->Branch ("x2pdf",       &b_x2pdf,       "x2pdf/F");
  outTree->Branch ("Q",           &b_Q,           "Q/F");
  outTree->Branch ("isValence1",  &b_isValence1,  "isValence1/O");
  outTree->Branch ("isValence2",  &b_isValence2,  "isValence2/O");

  outTree->Branch ("z_pt",  &b_z_pt,  "z_pt/F");
  outTree->Branch ("z_eta", &b_z_eta, "z_eta/F");
  outTree->Branch ("z_phi", &b_z_phi, "z_phi/F");
  outTree->Branch ("z_m",   &b_z_m,   "z_m/F");

  outTree->Branch ("akt4_jet_n",    &b_akt4_jet_n,    "akt4_jet_n/I");
  outTree->Branch ("akt4_jet_pt",   &b_akt4_jet_pt,   "akt4_jet_pt[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_eta",  &b_akt4_jet_eta,  "akt4_jet_eta[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_phi",  &b_akt4_jet_phi,  "akt4_jet_phi[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_e",    &b_akt4_jet_e,    "akt4_jet_e[akt4_jet_n]/F");

  outTree->Branch ("part_n",    &b_part_n,    "part_n/I");
  outTree->Branch ("part_pt",   &b_part_pt,   "part_pt[part_n]/F");
  outTree->Branch ("part_eta",  &b_part_eta,  "part_eta[part_n]/F");
  outTree->Branch ("part_phi",  &b_part_phi,  "part_phi[part_n]/F");
  outTree->Branch ("part_e",    &b_part_e,    "part_e[part_n]/F");

  TLorentzVector l1, l2, z;
  
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    if (!pythia.next ())
      continue;

    bool foundZ = false;
    b_z_pt = 0;

    for (int i = 0; i < pythia.event.size (); i++) {

      if (!pythia.event[i].isFinal()) continue; // check if in final state

      if (abs (pythia.event[i].id ()) != 11 && abs (pythia.event[i].id ()) != 13) continue; // check if electron or muon, resp.

      l1.SetPtEtaPhiM (pythia.event[i].pT (), pythia.event[i].eta (), pythia.event[i].phi (), pythia.event[i].m ());

      for (int j = 0; j < i; j++) {

        if (!pythia.event[j].isFinal()) continue; // check if in final state

        if (pythia.event[i].id () != -pythia.event[j].id ()) continue; // check if anti-particle of first particle

        l2.SetPtEtaPhiM (pythia.event[j].pT (), pythia.event[j].eta (), pythia.event[j].phi (), pythia.event[j].m ());

        // reconstruct Z
        z = l1+l2;

        b_z_m   = z.M ();
        if (b_z_m < 66 || 111 < b_z_m) continue; // loose invariant mass cut to make sure these are from Z decays

        b_z_pt  = z.Pt ();
        b_z_eta = z.Eta ();
        b_z_phi = z.Phi ();
        foundZ = true;
        break;
      }
    }

    if (!foundZ || b_z_pt < 5) {
      iEvent--;
      continue;
    }


    vector <fastjet::PseudoJet> particles;
    b_part_n = 0;
    for (int i = 0; i < pythia.event.size (); i++) {
      if (pythia.event[i].isFinal () && pythia.event[i].isHadron ()) {
        particles.push_back (fastjet::PseudoJet (pythia.event[i].pT (), pythia.event[i].eta (), pythia.event[i].phi (), pythia.event[i].e ()));
        if (pythia.event[i].isCharged ()) {
          b_part_pt[b_part_n]   = pythia.event[i].pT ();
          b_part_eta[b_part_n]  = pythia.event[i].eta ();
          b_part_phi[b_part_n]  = pythia.event[i].phi ();
          b_part_e[b_part_n]    = pythia.event[i].e ();
          b_part_n++;
        }
      }
    }


    // now run jet clustering
    fastjet::ClusterSequence clusterSeqAkt4 (particles, antiKt4);
    vector<fastjet::PseudoJet> sortedAkt4Jets = fastjet::sorted_by_pt (clusterSeqAkt4.inclusive_jets ());

    b_akt4_jet_n = 0;
    for (fastjet::PseudoJet jet : sortedAkt4Jets) {
      b_akt4_jet_pt[b_akt4_jet_n] = jet.perp ();
      b_akt4_jet_eta[b_akt4_jet_n] = jet.pseudorapidity ();
      b_akt4_jet_phi[b_akt4_jet_n] = jet.phi ();
      b_akt4_jet_e[b_akt4_jet_n] = jet.e ();
      b_akt4_jet_n++;
    }


    b_code = pythia.info.code ();
    b_id1 = pythia.info.id1pdf ();
    b_id2 = pythia.info.id2pdf ();
    b_x1pdf = pythia.info.x1pdf ();
    b_x2pdf = pythia.info.x2pdf ();
    b_Q =  pythia.info.QFac ();
    
    b_isValence1 = pythia.info.isValence1 ();
    b_isValence2 = pythia.info.isValence2 ();

    outTree->Fill();

    if (iEvent % (nEvents/100) == 0)
      std::cout << iEvent / (nEvents/100) << "\% done...\r" << std::flush;
  }

  pythia.stat();
  
  outFile->Write();
  outFile->Close();

  return 0;
}

#endif
