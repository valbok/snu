# Snu

Spiking NeUrons or Snu is a simulation project to model the C. elegans nematode based on a [Simple Model of Spiking Neurons](http://www.izhikevich.org/publications/spikes.htm) of [Eugene M. Izhikevich](http://www.izhikevich.org/).

This project inspired by [OpenWorm](http://openworm.org) - open source project dedicated to creating a virtual C. elegans nematode in a computer.

# Intro

The main goal is to implement nematode's brain and install it to a robot's body. This robot's body should be ONLY controlled by an artificial neural network. 

Regarding to real behavior of C. elegans there are few actions that we expect from the robot:

1. Moving forward.
2. Move backward in case of an obstacle.
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

Stimulation the food sensors leads to the nematode move forward or backward.
Stimulation the nose sensors leads [initiating backward locomotion](http://www.wormbook.org/chapters/www_behavior/behavior.html).

The main problem is to understand when and how the nematode changes its direction.
After googling found [Motoneurons Dedicated to Either Forward or Backward Locomotion in the Nematode Caenorhabditis elegans](http://www.jneurosci.org/content/30/33/11151.full) which describes the logic of changing the locomotion.

Some testing and investigating our spiking connectome model found that food stimulation and nose touching produces different activities on VA and VB neurons.
Nose stimulation leads higher frequency of average activities.

Based on this idea implemented a logic of changing the locomotion: If frequency of VA activities is higher than normal then nose has been touched and need to change direction.

It allowed to implement robot's brain.

To simulate, visualize and test robots used [Enki - The fast 2D robot simulator](http://home.gna.org/enki/).

[E-puck - education robot](http://www.e-puck.org/) was used to inject worm's brain to.

E-puck contains a list of IR sensors which is used to control touching.

# Result

As a real nematode our robot named SnuPuck (extended version of E-puck) can move by stimulation food sensors.
Can change his locomotion direction suddenly. And initiate backward locomotion in case of nose touching.

The main point that robot's behavior is controlled by C. elegans brain implemented on spiking neruons model.

# Build

    $ cmake .
    $ make

Run neuron tests:

    $ ./snu/brain/tests/NeuronTest
    
Run Enki playground simulation:

    $ ./viewer/playground
