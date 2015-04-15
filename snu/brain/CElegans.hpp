/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#ifndef SNU_CELEGANS
#define SNU_CELEGANS

#include <map>
#include <string>
#include <snu/brain/Neuron.hpp>

namespace NSnu
{

/**
 * CElegans model
 */
class CElegans
{
public:

    /**
     * Default constructor.
     */
    CElegans() throw();

    /**
     * Returns count of submitted neurons.
     */
    unsigned getNeuronsCount() const;

    /**
     * Connects neurons.
     *
     * @param Name of source neuron.
     * @param Name of target neuron.
     * @param Count of synapses.
     */
    inline void connect(const std::string& from, const std::string& to, int count);

    /**
     * Processes the time step for all neurons.
     *
     * @param Time step.
     */
    void tick(float h);

    /**
     * Creates a spike on requested neuron.
     *
     * @param Name of a neuron.
     * @return true If success.
     */
    bool spike(const std::string& name);

    /**
     * Checks if neuron just spiked.
     */
    bool fired(const std::string& name) const;

    /**
     * Checks if requested neuron exists.
     */
    bool hasNeuron(const std::string& name) const;

    /**
     * Returns neuron by name
     *
     * @param Name of neuron
     * @exception std::out_of_range in case if the name has not been found.
     */
    const Neuron& getNeuron(const std::string& name) const;

private:

    /**
     * Maps names to neurons.
     */
    std::map<std::string, Neuron> mNeurons;
};

} // namespace NSnu

#endif // SNU_CELEGANS