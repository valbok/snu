/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2018
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package snu
 */

#ifndef NEURON
#define NEURON

#include <vector>
#include <cstdio>

class Neuron;

/**
 * Internal structure to store info about target neurons.
 */
class Connection
{
public:

    /**
     * @param target neuron.
     * @param initial weight.
     * @param max weight.
     * @param expire speed of fading weight to max.
     */
    Connection(Neuron* target, float weight, float max = 100, float expire = 100);

    /**
     * Produces spike on current synapse.
     */
    void spike();

    /**
     * Teaches the connection.
     */
    void teach(float value);

    /**
     * Returns calculated weight of current axon.
     * Prevents increasing weight too fast.
     */
    float weight() const;

private:

    /**
     * Target neuron.
     */
    Neuron* m_target;

    /**
     * Teached weight of the connection.
     */
    float m_weight;

    /**
     * Max weight.
     */
    float m_max;

    /**
     * Expire speed of weight.
     */
    float m_expire;
};

/**
 * Array of connections from current neuron.
 */
typedef std::vector<Connection> Connections;

/**
 * Izhikevich neuron model.
 */
class Neuron
{
public:

    /**
     * @param External current applied to the neuron. To activate neuron easily.
     *        Neuron can be activated without stimulation.
     */
    Neuron(float extI = 10, float expireSpeedFired = 1, float expireSpeedNotFired = 50);

    /**
     * Connects to another neuron with connection weight.
     *
     * @param Target neuron.
     * @param Weight of the connection. Can be negative.
     */
    void connectTo(Neuron* target, float weight);

    /**
     * Applies a spike to the neuron.
     */
    void spike();

    /**
     * Processes the neuron within time slice.
     *
     * @param Time step.
     * @return True if spike occured.
     */
    bool tick(float h);

    /**
     * Returns membrane potential value of current neuron.
     */
    float mem() const;

    /**
     * Returns synaptic current. Sum from all dendrite neurons.
     */
    float syn() const;

    /**
     * Checks if the neuron fired and handled axons.
     */
    bool fired() const;

    /**
     * Returns synapses of current neuron.
     */
    const Connections& connections() const;

    /**
     * Adjustes current of post synaptic membrane.
     * Accumulates the current applied from dendrites.
     *
     * @param Submitted value.
     */
    bool apply(float value, Connection* connection = nullptr);

    /**
     * Returns synapses that produced spike to this neuron.
     */
    const std::vector<Connection*>& incomingSpikes() const;

private:

    /**
     * Returns membrane value of previous call.
     * to calculate membrane potentials.
     */
    float prevMem() const;

    /**
     * Returns some additional value of previous call.
     * to calculate membrane potentials.
     */
    float prevAdd() const;

    /**
     * External current.
     */
    float m_extI;

    /**
     * Speed of fading of the current when fired occured.
     */
    float m_expireSpeedFired;

    /**
     * Speed of fading of the current when no fired occured.
     */
    float m_expireSpeedNotFired;

    /**
     * Membrane potential value of previous call.
     */
    float m_prevMem;

    /**
     * Membrane potential value of last/recent call.
     */
    float m_curMem;

    /**
     * Defines if the neuron fired in current step.
     */
    bool m_fired = false;

    /**
     * Synaptic current.
     */
    float m_synI = 0;

    /**
     * Additional value of previous call.
     */
    float m_prevAdd = 0;

    /**
     * Additional value of last/recent call.
     */
    float m_curAdd = 0;

    /**
     * A vector with info about target neurons.
     */
    Connections m_connections;

    /**
     * Accomulates the synapses that applied spike to current neuron.
     * These synapses should be teached.
     */
    std::vector<Connection*> m_incomingSpikes;

    /**
     * Defines if refracotry period is on.
     */
    bool m_refractoryPeriod = false;

    /**
     * Defines if the neuron has been fired
     * and membrane potential is fading down.
     */
    bool m_firedPeriod = false;

};

#endif // NEURON