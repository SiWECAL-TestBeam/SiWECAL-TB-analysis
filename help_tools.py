#!/usr/bin/env python
import os
import numpy as np

NSLAB = 7
NCHIP = 16
NSCA = 15
NCHAN = 64

def read_pedestals():
    slab_map = {
        0: '_dif_1_1_1.log',
        1: '_dif_1_1_2.log',
        2: '_dif_1_1_3.log',
        3: '_dif_1_1_4.log',
        4: '_dif_1_1_5.log',
        5: '_dif_1_2_1.log',
        6: '_dif_1_2_2.log'
    }

    ## pedestal map (n-dim numpy array)
    pedestal_map = np.zeros((NSLAB,NCHIP,NCHAN,NSCA))

    indir_prefix = "/Users/artur/cernbox/CALICE/TB2017/data/Jun_2017_TB/BT2017/pedestals/Pedestal"

    for slab in slab_map:
        fname = indir_prefix + slab_map[slab]
        print("Reading pedestals for %s from %s" %(slab,fname))
        if not os.path.exists(fname):
            print fname, " does not exist"
            continue

        with open(fname) as fmap:
            for i,line in enumerate(fmap.readlines()):
                if '#' in line: continue

                items = [float(item) for item in line.split()]

                chip,chan = int(items[0]),int(items[1])
                peds = items[2::2]
                peds_err = items[3::2]
                pedestal_map[slab][chip][chan] = peds
                '''
                print chip,chan
                print peds
                print peds_err

                #break
                '''
    return pedestal_map

if __name__ == "__main__":

    print read_pedestals()
