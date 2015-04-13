/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#ifndef SNU_SNUPUCK
#define SNU_SNUPUCK

#include <enki/robots/e-puck/EPuck.h>
#include <snu/brain/CElegans.hpp>

namespace NSnu
{

/**
 * 
 */
class SnuPuck : public Enki::EPuck
{
public:

    /**
     * Processes the robot behaviour within time slice.
     *
     * @param Time step.
     */
    void tick(float h);

protected:
    CElegans mBrain;
};

} // namespace NSnu

#endif // SNU_SNUPUCK