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
        t.append(l[0])
        V_mean.append(l[1])

    return (t, V_mean)

def draw(t, V_mean, title):
    fig = pl.figure()
    fig.canvas.set_window_title(title)
    a = pl.plot()
    pl.plot(t, V_mean, 'r')
    pl.title(u"Membrane potentials")
    pl.ylabel(u'Membrane potentials')
    pl.grid()
    pl.xlabel(u"Time")

for i in range(1, len(sys.argv)):
    (t, V_mean) = read(sys.argv[i])
    draw(t, V_mean, sys.argv[i])

pl.show()
