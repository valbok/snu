/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#ifndef SNU_CONNECTOMEPARSER
#define SNU_CONNECTOMEPARSER

#include <tuple>
#include <string>
#include <vector>

namespace NSnu
{

/**
 * Parses connectome from file.
 */
class ConnectomeParser
{
public:

    /**
     * Connection between neurones.
     * Neuron 1, Neuron 2, number of synapses between them.
     */
    typedef std::tuple<std::string, std::string, int> TConnection;

    /**
     * Array of connections.
     */
    typedef std::vector<TConnection> TConnections;

    /**
     * Default constructor.
     */
    ConnectomeParser(const char* filename) throw();

    /**
     * Parses connections.
     *
     * @param Result container.
     * @return True if at least one connection found.
     */
    bool parse(TConnections& result);

private:

    /**
     * Filename with connectome.
     */
    std::string mFilename;
};

} // namespace NSnu

#endif // SNU_NEURON