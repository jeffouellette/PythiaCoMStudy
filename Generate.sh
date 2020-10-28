#! /bin/zsh

#bin/z_gen 1 5020 2.5 20000 rootFiles/z_gen_sqrts5020GeV.root > outputs/z_gen_sqrts5020GeV.out 2> errors/z_gen_sqrts5020GeV.err &
#bin/z_gen 2 8160 2.5 20000 rootFiles/z_gen_sqrts8160GeV.root > outputs/z_gen_sqrts8160GeV.out 2> errors/z_gen_sqrts8160GeV.err &

bin/photon_gen 3 5020 50 50000 rootFiles/photon_gen_sqrts5020GeV.root on on > outputs/photon_gen_sqrts5020GeV.out 2> errors/photon_gen_sqrts5020GeV.err &
bin/photon_gen 4 8160 50 50000 rootFiles/photon_gen_sqrts8160GeV.root on on > outputs/photon_gen_sqrts8160GeV.out 2> errors/photon_gen_sqrts8160GeV.err &

bin/photon_gen 3 5020 50 50000 rootFiles/photon_gen_noIFSR_sqrts5020GeV.root off off > outputs/photon_gen_noIFSR_sqrts5020GeV.out 2> errors/photon_gen_noIFSR_sqrts5020GeV.err &
bin/photon_gen 4 8160 50 50000 rootFiles/photon_gen_noIFSR_sqrts8160GeV.root off off > outputs/photon_gen_noIFSR_sqrts8160GeV.out 2> errors/photon_gen_noIFSR_sqrts8160GeV.err &

wait

