#!/usr/bin/env python
import os
import numpy as np

NSLAB = 7
NCHIP = 16
NSCA = 15
NCHAN = 64

BCID_VALEVT = 1245

## global storages
chan_map = {}
ped_map = {}
mip_map = {}
mask_map = {}

pos_z = []
pos_xzero = []

slab_map = {
    0: '_dif_1_1_1',
    1: '_dif_1_1_2',
    2: '_dif_1_1_3',
    3: '_dif_1_1_4',
    4: '_dif_1_1_5',
    5: '_dif_1_2_1',
    6: '_dif_1_2_2'
}

class EcalHit:
    def __init__(self,slab,chip,chan,sca,hg,lg,isHit):
        self.slab = slab
        self.chip = chip
        self.chan = chan
        self.sca = sca
        self.hg = hg
        self.lg = lg
        self.isHit = isHit

        ## check channel is masked
        self.isMasked = int(mask_map[self.slab][self.chip][self.chan])

        ## get x-y coordinates
        self.x0 = pos_xzero[slab]
        self.z = pos_z[slab]
        (self.x,self.y) = chan_map[(chip,chan)]

        # do pedestal subtraction
        self.hg -= ped_map[self.slab][self.chip][self.chan][self.sca]
        # MIP calibration
        if mip_map[self.slab][self.chip][self.chan] > 10:
            self.energy = self.hg / mip_map[self.slab][self.chip][self.chan]
        else:
            self.energy = 0

def build_w_config(config = 1):

    global pos_z, pos_xzero
    # SLAB positions
    pos_z = [0,1,2,3,4,5,9] * 15#mm gap

    ## Tungsten / W configuration
    if config == 1:
        # Config 1
        abs_thick = [2,2,2,2,4,4,6]
    elif config == 2:
        # Config 2
        abs_thick = [4,2,2,4,4,6,6]
    elif config == 3:
        # Config 3
        abs_thick = [6,2,4,4,6,6,6]

    ## sum up thickness
    w_xzero = 0.56#Xo per mm of W
    pos_xzero = [sum(abs_thick[:i+1])*w_xzero for i in range(len(abs_thick))]
    ## Print
    print("W config %i used:" %config )
    print(abs_thick, pos_xzero)

def read_mapping(fname = "fev10_chip_channel_x_y_mapping.txt"):

    global chan_map# = {}

    with open(fname) as fmap:
        for i,line in enumerate(fmap.readlines()):
            if i == 0: continue

            # items: chip x0 y0 channel x y
            items = line.split()

            chip = int(items[0]); chan = int(items[3])
            x = float(items[4]); y = float(items[5])

            chan_map[(chip,chan)] = (x,y)

    return chan_map

def read_pedestals(indir_prefix = "./pedestals/"):

    global slab_map
    global ped_map

    ## pedestal map (n-dim numpy array)
    pedestal_map = np.zeros((NSLAB,NCHIP,NCHAN,NSCA))

    for slab in slab_map:
        fname = indir_prefix + "Pedestal" + slab_map[slab] + ".txt"
        print("Reading pedestals for %s from %s" %(slab,fname))
        if not os.path.exists(fname):
            print fname, " does not exist"
            continue

        with open(fname) as fmap:
            for i,line in enumerate(fmap.readlines()):
                if '#' in line: continue

                items = [float(item) for item in line.split()]

                chip,chan = int(items[0]),int(items[1])
                peds = items[2]
                peds_err = items[3]
                peds_width = items[4]
                pedestal_map[slab][chip][chan] = peds
    ped_map = pedestal_map
    return pedestal_map

def read_mip_values(indir_prefix = "./mip_calib/"):

    global slab_map
    global mip_map

    ## mip MPV map (n-dim numpy array)
    mip_map = np.zeros((NSLAB,NCHIP,NCHAN))

    for slab in slab_map:
        fname = indir_prefix + "MIP%s.txt" % slab_map[slab]
        print("Reading MIP values for %s from %s" %(slab,fname))
        if not os.path.exists(fname):
            print fname, " does not exist"
            continue

        with open(fname) as fmap:
            for i,line in enumerate(fmap.readlines()):
                if '#' in line: continue

                items = [float(item) for item in line.split()]

                chip,chan = int(items[0]),int(items[1])
                mpv = items[2]
                mpv_err = items[3]
                mip_map[slab][chip][chan] = mpv

    #mip_map = mpv_map
    return mip_map

def read_masked(indir_prefix = "./masked/"):

    global slab_map
    global mask_map

    ## masked channels map (n-dim numpy array)
    mask_map = np.zeros((NSLAB,NCHIP,NCHAN))

    for slab in slab_map:
        fname = indir_prefix + "masked%s.txt" % slab_map[slab]
        print("Reading masked channels for %s from %s" %(slab,fname))
        if not os.path.exists(fname):
            print fname, " does not exist"
            continue

        with open(fname) as fmap:
            for i,line in enumerate(fmap.readlines()):
                if '#' in line: continue

                items = [int(item) for item in line.split()]

                chip,chan = int(items[0]),int(items[1])
                masked = int(items[2])
                mask_map[slab][chip][chan] = masked

    return mask_map

if __name__ == "__main__":

    #print read_pedestals()
    #print read_mip_values()
    print read_masked()
