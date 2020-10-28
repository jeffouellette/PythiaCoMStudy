#! /bin/bash

#bin/z_analyze sqrts5020GeV rootFiles/z_gen_sqrts5020GeV.root rootFiles/z_out_sqrts5020GeV.root &
#bin/z_analyze sqrts8160GeV rootFiles/z_gen_sqrts8160GeV.root rootFiles/z_out_sqrts8160GeV.root &

bin/photon_analyze sqrts5020GeV rootFiles/photon_gen_sqrts5020GeV.root rootFiles/photon_out_sqrts5020GeV.root &
bin/photon_analyze sqrts8160GeV rootFiles/photon_gen_sqrts8160GeV.root rootFiles/photon_out_sqrts8160GeV.root &

#bin/photon_analyze noISR_sqrts5020GeV rootFiles/photon_gen_noISR_sqrts5020GeV.root rootFiles/photon_out_noISR_sqrts5020GeV.root &
#bin/photon_analyze noISR_sqrts8160GeV rootFiles/photon_gen_noISR_sqrts8160GeV.root rootFiles/photon_out_noISR_sqrts8160GeV.root &

#bin/photon_analyze noFSR_sqrts5020GeV rootFiles/photon_gen_noFSR_sqrts5020GeV.root rootFiles/photon_out_noFSR_sqrts5020GeV.root &
#bin/photon_analyze noFSR_sqrts8160GeV rootFiles/photon_gen_noFSR_sqrts8160GeV.root rootFiles/photon_out_noFSR_sqrts8160GeV.root &

bin/photon_analyze noIFSR_sqrts5020GeV rootFiles/photon_gen_noIFSR_sqrts5020GeV.root rootFiles/photon_out_noIFSR_sqrts5020GeV.root &
bin/photon_analyze noIFSR_sqrts8160GeV rootFiles/photon_gen_noIFSR_sqrts8160GeV.root rootFiles/photon_out_noIFSR_sqrts8160GeV.root &

wait
