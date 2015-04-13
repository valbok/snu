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

/**
 * Izhikevich neuron model.
 */
class Neuron
{
public:

    /**
     * Callback type.
     */
    typedef void (*TCallback)(const Neuron*);

    /**
     * Default constructor.
     *
     * @param Callback function when spike fired.
     */
    Neuron(TCallback callback = 0) throw();

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
    void connectTo(Neuron* target, bool isPositive);

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
    float getMembraneValue() const;

    /**
     * Checks if the neuron is active now. Means fired.
     */
    bool spiked() const;

private:

    /**
     * Teaches post synaptic membrane.
     *
     * @param Submitted value.
     */
    void teach(float value);

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
     * Internal structure to store info about target neurons.
     */
    struct STarget
    {
        /**
         * Target neuron.
         */
        Neuron* mTarget;

        /**
         * Weight of the connection. Can be negative.
         */
        float mWeight;

        /**
         * Synaptic current of previous call.
         */
        float mPrevSynI;

        /**
         * Synaptic current of last call.
         */
        float mCurSynI;
    };

    /**
     * A vector with info about target neurons.
     */
    std::vector<STarget> mTargets;

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
     * Force defines if there is a spike on the neuron.
     * @see spike()
     */
    bool mForceSpike;

    /**
     * Defines if the neuron fired in current step.
     */
    bool mSpiked;

    /**
     * Callback function will be called if skipe occured.
     */
    TCallback mCallback;
};

} // namespace NSnu

#endif // SNU_NEURON