/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Mai 2018
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "Column.h"
#include "Neuron.h"

Column::Column(size_t x, size_t y, size_t z)
{
    m_grid.resize(z);
    for (size_t k = 0; k < z; ++k) {
        auto& layer = m_grid[k];
        layer.resize(y);
        for (size_t j = 0; j < y; ++j) {
            auto& line = layer[j];
            line.resize(x);
            for (size_t i = 0; i < x; ++i)
                line[i] = new Neuron;
        }
    }
}

Column::~Column()
{
    for (auto& layer : m_grid) {
        for (auto& line : layer) {
            for (auto& n : line) {
                delete n;
            }
        }
    }
}


Neuron& Column::neuron(size_t x, size_t y, size_t z)
{
    return *m_grid[z][y][x];
}