/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "SnuPuck.hpp"

namespace NSnu
{

void SnuPuck::tick(float h)
{
    mBrain.tick(h);
}

} // namespace NSnu
