#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as pl
import csv

def read(fn):
    f = open(fn, "r")
    rdr = csv.reader(f, delimiter=";")

    t = []
    V_mean = []
    for l in rdr:
        t.append(np.float64(l[0]))
        V_mean.append(l[1])

    return (t, V_mean)

fn1 = sys.argv[1]
(t1, V_mean1) = read(fn1)
title = fn1
fn2 = None
t2 = None
V_mean2 = None
if len(sys.argv) > 2:
    fn2 = sys.argv[2]
    (t2, V_mean2) = read(fn2)

fig = pl.figure()
a = pl.plot()
pl.plot(t1, V_mean1, 'r')
if t2 != None: 
    pl.plot(t2, V_mean2, 'b')
    pl.legend([fn1, fn2])
    title = title + " " + fn2;

fig.canvas.set_window_title(title)
pl.title(u"Membrane potentials")
pl.ylabel(u'Membrane potentials')
pl.grid()
pl.xlabel(u"Time")

pl.show()
