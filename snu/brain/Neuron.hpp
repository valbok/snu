/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#ifndef SNU_NEURON
#define SNU_NEURON

#include <vector>

namespace NSnu
{

class Neuron;

/**
 * Internal structure to store info about target neurons.
 */
struct SAxon
{
    /**
     * Target neuron.
     */
    Neuron* target;

    /**
     * Const weight of the connection. Can be negative.
     */
    const float baseWeight;

    /**
     * Teached weight of the connection. Always positive.
     */
    float teachedWeight;

    /**
     * Synaptic current of previous call.
     */
    float prevSynI;

    /**
     * Synaptic current of last call.
     */
    float curSynI;

    /**
     * Returns calculated weight of current axon.
     */
    float getWeight() const;
};

/**
 * Array of connections from current neuron.
 */
typedef std::vector<SAxon> TAxones;

/**
 * Connection to current neuron.
 */
struct SDendrite
{
    /**
     * Source neuron.
     */
    Neuron* source;

    /**
     * Id of a connection where axon can be found in source neuron.
     */
    unsigned axonId;
};

/**
 * Array of connections to current neuron.
 */
typedef std::vector<SDendrite> TDendrites;

/**
 * Izhikevich neuron model.
 */
class Neuron
{
public:

    /**
     * Default constructor.
     *
     * @param External current applied to the neuron. To activate neuron easily.
     *        Neuron can be activated without stimulation.
     */
    Neuron(float extI = 0) throw();

    /**
     * Connects to another neuron with connection weight.
     *
     * @param Target neuron.
     * @param Weight of the connection. Can be negative.
     */
    void connectTo(Neuron* target, float weight);

    /**
     * Connects to another neuron with connection weight.
     *
     * @param Target neuron.
     * @param True if connection is positive and weight will be random.
     */
    void connectTo(Neuron* target, bool isPositive = true);

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
    bool step(float h);

    /**
     * Returns membrane potential value of current neuron.
     */
    float getMembraneValue() const;

    /**
     * Checks if the neuron fired and handled axons.
     */
    bool fired() const;

    /**
     * Checks if the neuron fired in previous step and handled axons.
     */
    bool prevFired() const;

    /**
     * Returns axons of current neuron.
     */
    const TAxones& getAxons() const;

    /**
     * Returns dendrites of current neuron.
     */
    const TDendrites& getDendrites() const;

    /**
     * Returns synaptic current. Sum from all dendrite neurons.
     */
    float getSynI() const;

private:

    /**
     * Teaches post synaptic membrane.
     *
     * @param Submitted value.
     */
    void teachSynI(float value);

    /**
     * Teaches synapse weights using differencial learning of Hebb.
     *
     * @param Id of a connection.
     * @param Current value of target neuron.
     * @param Previous value of target neuron.
     */
    bool teachSynWeight(unsigned axonId, unsigned curTarget, unsigned prevTarget);

    /**
     * Returns membrane value of previous call.
     * to calculate membrane potentials.
     */
    float getPrevMembraneValue() const;

    /**
     * Returns some additional value of previous call.
     * to calculate membrane potentials.
     */
    float getPrevAdditionalValue() const;

    /**
     * Creates dendrite connection from source neuron to current.
     *
     * @param Source neuron.
     * @param Id of axon where info is located.
     */
    void connectFrom(Neuron* source, unsigned axonId);

    /**
     * A vector with info about target neurons.
     */
    TAxones mAxones;

    /**
     * A vector of dendrites. Neurons that connected to current one.
     */
    TDendrites mDendrites;

    /**
     * External current.
     */
    float mExtI;

    /**
     * Synaptic current.
     * @see teach()
     */
    float mSynI;

    /**
     * Membrane potential value of previous call.
     */
    float mPrevMembrane;

    /**
     * Membrane potential value of last/recent call.
     */
    float mCurMembrane;

    /**
     * Additional value of previous call.
     */
    float mPrevAdditional;

    /**
     * Additional value of last/recent call.
     */
    float mCurAdditional;

    /**
     * Defines if the neuron fired in current step.
     */
    bool mFired;

    /**
     * Defines if the neuron fired in current step.
     */
    bool mPrevFired;
};

} // namespace NSnu

#endif // SNU_NEURON