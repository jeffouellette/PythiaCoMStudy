#ifndef __photongen_cxx__
#define __photongen_cxx__

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
    std::cout << " usage: photon_gen SEED SQRTS MINPTHAT NEVT FILENAMEOUT" << std::endl;
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
  pythia.readString("PromptPhoton:all  = on");

  pythia.readString (Form ("PhaseSpace:pTHatMin = %g", ptHatMin));

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

  float b_photon_pt = 0;
  float b_photon_eta = 0;
  float b_photon_phi = 0;

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

  outTree->Branch ("photon_pt",  &b_photon_pt,  "photon_pt/F");
  outTree->Branch ("photon_eta", &b_photon_eta, "photon_eta/F");
  outTree->Branch ("photon_phi", &b_photon_phi, "photon_phi/F");

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

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    if (!pythia.next ())
      continue;


    b_photon_pt = 0;
    for (int i = 0; i < pythia.event.size (); i++) {

      if (!pythia.event[i].isFinal()) continue; // check if in final state

      if (pythia.event[i].id () != 22) continue; // check if photon

      if (pythia.event[i].pT () < b_photon_pt) continue; // we want the highest pT photon

      b_photon_pt = pythia.event[i].pT ();
      b_photon_eta = pythia.event[i].eta ();
      b_photon_phi = pythia.event[i].phi ();
    }

    if (b_photon_pt < 50) {
      iEvent--;
      continue;
    }


    float b_photon_etcone40 = 0;
    for (int i = 0; i < pythia.event.size (); i++) {
      
      if (!(pythia.event[i].isFinal () && pythia.event[i].isHadron ()))
        continue;

      if (DeltaR (b_photon_eta, pythia.event[i].eta (), b_photon_phi, pythia.event[i].phi ()) < 0.4)
        b_photon_etcone40 += pythia.event[i].eT ();
    }

    if (b_photon_etcone40 >= 4.8 + 4.2e-3 * b_photon_pt) {
      iEvent--;
      continue;
    }


    vector <fastjet::PseudoJet> particles;
    b_part_n = 0;
    for (int i = 0; i < pythia.event.size (); i++) {
      if (pythia.event[i].isFinal () && pythia.event[i].isHadron ()) {
        particles.push_back (fastjet::PseudoJet (pythia.event[i].px (), pythia.event[i].py (), pythia.event[i].py (), pythia.event[i].e ()));
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
