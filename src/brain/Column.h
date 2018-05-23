/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Mai 2018
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package snu
 */

#ifndef COLUMN
#define COLUMN

#include <vector>

using std::size_t;

class Neuron;

/**
 * 
 */
class Column
{
public:
    Column(size_t x, size_t y, size_t z);
    ~Column();

    Neuron& neuron(size_t x, size_t y, size_t z);

private:
    typedef std::vector<Neuron*> Line;
    typedef std::vector<Line> Layer;
    std::vector<Layer> m_grid;
};

#endif // COLUMN
