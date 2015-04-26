# Snu

Spiking NeUrons or Snu is a simulation project to model the C. elegans nematode based on a [Simple Model of Spiking Neurons](http://www.izhikevich.org/publications/spikes.htm) of [Eugene M. Izhikevich](http://www.izhikevich.org/).

This project inspired by [OpenWorm](http://openworm.org) - open source project dedicated to creating a virtual C. elegans nematode in a computer.

# Intro

The main goal is to implement nematode's brain and install it to a robot's body. This robot's body should be ONLY controlled by an artificial neural network. 

Regarding to real behavior of C. elegans there are few actions that we expect from the robot:

1. Moving forward.
2. In case of an obstacle move backward.
3. Turn left/right.

# How

Firstly simple spiking model of a neuron has been implemented. Also added Differential Hebbian Learning which allows to correlate weights of synapses to strengthen connections.

After that the [connectome of C. elegans](http://www.wormatlas.org/neuronalwiring.html) nematode used to build neural network based on our spiking neurons.

Now it is possible to stimulate some sensors and look at neurons activities. 

Since purposes of every neuron are quite known, implemented possibility to activate or stimulate few sensors groups:

1. Food sensors: ADFL, ADFR, ASGL, ASGR, ASIL, ASIR, ASJL, ASJR, AWCL, AWCR, AWAL and AWAR.
2. Nose touch sensors: ASHL, ASHR, FLPL, FLPR, OLQDL, OLQDR, OLQVL, OLQVR, IL1VL, IL1VR, IL1L, IL1R, IL1DL, IL1DR.
3. Left touch sensors: PLML, PVDL, PDEL, PVM and LUAL.
4. Right touch sensors: PLMR, PVDR, PDER, PVM and LUAR.


