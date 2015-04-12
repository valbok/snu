#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as pl
import csv

fn = sys.argv[1]
f = open(fn, "r")
rdr = csv.reader(f,delimiter=";")
t = []
V_mean = []
for l in rdr:
    t.append(l[0])
    V_mean.append(l[1])

pl.figure()
pl.plot(t, V_mean, 'r')
pl.title(u"Membrane potentials")
pl.ylabel(u'Membrane potentials')
pl.grid()
pl.xlabel(u"Time")
pl.show()
