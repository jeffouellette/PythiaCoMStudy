#! /bin/bash

bin/z_analyze sqrts5020GeV rootFiles/z_gen_sqrts5020GeV.root rootFiles/z_out_sqrts5020GeV.root &
bin/z_analyze sqrts8160GeV rootFiles/z_gen_sqrts8160GeV.root rootFiles/z_out_sqrts8160GeV.root &

bin/photon_analyze sqrts5020GeV rootFiles/photon_gen_sqrts5020GeV.root rootFiles/photon_out_sqrts5020GeV.root &
bin/photon_analyze sqrts8160GeV rootFiles/photon_gen_sqrts8160GeV.root rootFiles/photon_out_sqrts8160GeV.root &
wait
