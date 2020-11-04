#! /bin/zsh

bin/z_gen 1  5020 2.5 10000 rootFiles/z_gen_sqrts5020GeV_job1.root > outputs/z_gen_sqrts5020GeV_job1.out 2> errors/z_gen_sqrts5020GeV_job1.err &
bin/z_gen 2  5020 2.5 10000 rootFiles/z_gen_sqrts5020GeV_job2.root > outputs/z_gen_sqrts5020GeV_job2.out 2> errors/z_gen_sqrts5020GeV_job2.err &
bin/z_gen 3  5020 2.5 10000 rootFiles/z_gen_sqrts5020GeV_job3.root > outputs/z_gen_sqrts5020GeV_job3.out 2> errors/z_gen_sqrts5020GeV_job3.err &
bin/z_gen 4  5020 2.5 10000 rootFiles/z_gen_sqrts5020GeV_job4.root > outputs/z_gen_sqrts5020GeV_job4.out 2> errors/z_gen_sqrts5020GeV_job4.err &
bin/z_gen 5  5020 2.5 10000 rootFiles/z_gen_sqrts5020GeV_job5.root > outputs/z_gen_sqrts5020GeV_job5.out 2> errors/z_gen_sqrts5020GeV_job5.err &
bin/z_gen 6  8160 2.5 10000 rootFiles/z_gen_sqrts8160GeV_job1.root > outputs/z_gen_sqrts8160GeV_job1.out 2> errors/z_gen_sqrts8160GeV_job1.err &
bin/z_gen 7  8160 2.5 10000 rootFiles/z_gen_sqrts8160GeV_job2.root > outputs/z_gen_sqrts8160GeV_job2.out 2> errors/z_gen_sqrts8160GeV_job2.err &
bin/z_gen 8  8160 2.5 10000 rootFiles/z_gen_sqrts8160GeV_job3.root > outputs/z_gen_sqrts8160GeV_job3.out 2> errors/z_gen_sqrts8160GeV_job3.err &
bin/z_gen 9  8160 2.5 10000 rootFiles/z_gen_sqrts8160GeV_job4.root > outputs/z_gen_sqrts8160GeV_job4.out 2> errors/z_gen_sqrts8160GeV_job4.err &
bin/z_gen 10 8160 2.5 10000 rootFiles/z_gen_sqrts8160GeV_job5.root > outputs/z_gen_sqrts8160GeV_job5.out 2> errors/z_gen_sqrts8160GeV_job5.err &

wait

hadd -f rootFiles/z_gen_sqrts5020GeV.root rootFiles/z_gen_sqrts5020GeV_job*.root
hadd -f rootFiles/z_gen_sqrts8160GeV.root rootFiles/z_gen_sqrts8160GeV_job*.root
rm rootFiles/z_gen_sqrts5020GeV_job*.root
rm rootFiles/z_gen_sqrts8160GeV_job*.root


#bin/photon_gen 3  5020 50 10000 rootFiles/photon_gen_sqrts5020GeV_job1.root on on > outputs/photon_gen_sqrts5020GeV_job1.out 2> errors/photon_gen_sqrts5020GeV_job1.err &
#bin/photon_gen 4  5020 50 10000 rootFiles/photon_gen_sqrts5020GeV_job2.root on on > outputs/photon_gen_sqrts5020GeV_job2.out 2> errors/photon_gen_sqrts5020GeV_job2.err &
#bin/photon_gen 5  5020 50 10000 rootFiles/photon_gen_sqrts5020GeV_job3.root on on > outputs/photon_gen_sqrts5020GeV_job3.out 2> errors/photon_gen_sqrts5020GeV_job3.err &
#bin/photon_gen 6  5020 50 10000 rootFiles/photon_gen_sqrts5020GeV_job4.root on on > outputs/photon_gen_sqrts5020GeV_job4.out 2> errors/photon_gen_sqrts5020GeV_job4.err &
#bin/photon_gen 7  5020 50 10000 rootFiles/photon_gen_sqrts5020GeV_job5.root on on > outputs/photon_gen_sqrts5020GeV_job5.out 2> errors/photon_gen_sqrts5020GeV_job5.err &
#bin/photon_gen 8  8160 50 10000 rootFiles/photon_gen_sqrts8160GeV_job1.root on on > outputs/photon_gen_sqrts8160GeV_job1.out 2> errors/photon_gen_sqrts8160GeV_job1.err &
#bin/photon_gen 9  8160 50 10000 rootFiles/photon_gen_sqrts8160GeV_job2.root on on > outputs/photon_gen_sqrts8160GeV_job2.out 2> errors/photon_gen_sqrts8160GeV_job2.err &
#bin/photon_gen 10 8160 50 10000 rootFiles/photon_gen_sqrts8160GeV_job3.root on on > outputs/photon_gen_sqrts8160GeV_job3.out 2> errors/photon_gen_sqrts8160GeV_job3.err &
#bin/photon_gen 11 8160 50 10000 rootFiles/photon_gen_sqrts8160GeV_job4.root on on > outputs/photon_gen_sqrts8160GeV_job4.out 2> errors/photon_gen_sqrts8160GeV_job4.err &
#bin/photon_gen 12 8160 50 10000 rootFiles/photon_gen_sqrts8160GeV_job5.root on on > outputs/photon_gen_sqrts8160GeV_job5.out 2> errors/photon_gen_sqrts8160GeV_job5.err &
#
#wait
#
#hadd -f rootFiles/photon_gen_sqrts5020GeV.root rootFiles/photon_gen_sqrts5020GeV_job*.root
#hadd -f rootFiles/photon_gen_sqrts8160GeV.root rootFiles/photon_gen_sqrts8160GeV_job*.root
rm rootFiles/photon_gen_sqrts5020GeV_job*.root
rm rootFiles/photon_gen_sqrts8160GeV_job*.root

#bin/photon_gen 3 5020 50 10000 rootFiles/photon_gen_noIFSR_sqrts5020GeV_job1.root on on > outputs/photon_gen_noIFSR_sqrts5020GeV_job1.out 2> errors/photon_gen_noIFSR_sqrts5020GeV_job1.err &
#bin/photon_gen 4 5020 50 10000 rootFiles/photon_gen_noIFSR_sqrts5020GeV_job2.root on on > outputs/photon_gen_noIFSR_sqrts5020GeV_job2.out 2> errors/photon_gen_noIFSR_sqrts5020GeV_job2.err &
#bin/photon_gen 5 5020 50 10000 rootFiles/photon_gen_noIFSR_sqrts5020GeV_job3.root on on > outputs/photon_gen_noIFSR_sqrts5020GeV_job3.out 2> errors/photon_gen_noIFSR_sqrts5020GeV_job3.err &
#bin/photon_gen 6 5020 50 10000 rootFiles/photon_gen_noIFSR_sqrts5020GeV_job4.root on on > outputs/photon_gen_noIFSR_sqrts5020GeV_job4.out 2> errors/photon_gen_noIFSR_sqrts5020GeV_job4.err &
#bin/photon_gen 7 5020 50 10000 rootFiles/photon_gen_noIFSR_sqrts5020GeV_job5.root on on > outputs/photon_gen_noIFSR_sqrts5020GeV_job5.out 2> errors/photon_gen_noIFSR_sqrts5020GeV_job5.err &
#bin/photon_gen 8 8160 50 10000 rootFiles/photon_gen_noIFSR_sqrts8160GeV_job1.root on on > outputs/photon_gen_noIFSR_sqrts8160GeV_job1.out 2> errors/photon_gen_noIFSR_sqrts8160GeV_job1.err &
#bin/photon_gen 9 8160 50 10000 rootFiles/photon_gen_noIFSR_sqrts8160GeV_job2.root on on > outputs/photon_gen_noIFSR_sqrts8160GeV_job2.out 2> errors/photon_gen_noIFSR_sqrts8160GeV_job2.err &
#bin/photon_gen 10 8160 50 10000 rootFiles/photon_gen_noIFSR_sqrts8160GeV_job3.root on on > outputs/photon_gen_noIFSR_sqrts8160GeV_job3.out 2> errors/photon_gen_noIFSR_sqrts8160GeV_job3.err &
#bin/photon_gen 11 8160 50 10000 rootFiles/photon_gen_noIFSR_sqrts8160GeV_job4.root on on > outputs/photon_gen_noIFSR_sqrts8160GeV_job4.out 2> errors/photon_gen_noIFSR_sqrts8160GeV_job4.err &
#bin/photon_gen 12 8160 50 10000 rootFiles/photon_gen_noIFSR_sqrts8160GeV_job5.root on on > outputs/photon_gen_noIFSR_sqrts8160GeV_job5.out 2> errors/photon_gen_noIFSR_sqrts8160GeV_job5.err &
#
#hadd -f rootFiles/photon_gen_noIFSR_sqrts5020GeV.root rootFiles/photon_gen_noIFSR_sqrts5020GeV_job*.root
#hadd -f rootFiles/photon_gen_noIFSR_sqrts8160GeV.root rootFiles/photon_gen_noIFSR_sqrts8160GeV_job*.root
#
#wait

