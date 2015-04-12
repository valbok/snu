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
#include <snu/model/Neuron.hpp>
#include <snu/connectome/ConnectomeParser.hpp>

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
     *
     * @paramp Connectome of the worm.
     */
    CElegans(const ConnectomeParser::TConnectome& connectome) throw();

    /**
     * Returns count of submitted neurons.
     */
    unsigned getNeuronsCount() const;
    
private:

    /**
     * Connects neurons.
     *
     * @param Name of source neuron.
     * @param Name of target neuron.
     * @param Count of synapses.
     */
    void connect(std::string from, std::string to, int count);

    /**
     * Maps names to neurons.
     */
    std::map<std::string, Neuron> mNeurons;

};

} // namespace NSnu

#endif // SNU_CELEGANS