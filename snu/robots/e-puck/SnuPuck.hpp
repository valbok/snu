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
#include <snu/worm/CElegans.hpp>

namespace NSnu
{

/**
 * Robot with worm's brain.
 * Can drive forward before it finds an obstacle.
 * In this case will change direction and before finds another obstacle.
 */
class SnuPuck : public Enki::EPuck
{
public:

    /**
     * Default constructor.
     */
    SnuPuck() throw();

    /**
     * Processes the robot behaviour within time slice.
     *
     * @param Time step.
     * @param Number of iterations per period.
     */
    void step(float h, unsigned period);

    /**
     * @copydoc CElegans::setNosePeriodFrequency()
     */
    void setNosePeriodFrequency(float value);

    /**
     * @copydoc CElegans::setFoodPeriodFrequency()
     */
    void setFoodPeriodFrequency(float value);

    /**
     * Stops motors.
     */
    void stop();

    /**
     * Makes one move.
     */
    void move();

    /**
     * Checks if front or back sensors touched.
     */
    bool frontOrBackTouched() const;

private:

    /**
     * Handles the movment.
     */
    void drive();

    /**
     * Local current temporary left speed.
     * @see leftSpeed
     */
    float mLeftSpeed;

    /**
     * Local current temporary right speed.
     * @see rightSpeed
     */
    float mRightSpeed;

    /**
     * Defines current direction.
     * Should be kept before it finds obstacle.
     */
    int mDirection;

    /**
     * Defines of direction recently has been changed.
     * If so, no need to change it again.
     */
    bool mDirectionChanged;

    /**
     * Worm's neuronet.
     */
    CElegans mBrain;
};

} // namespace NSnu

#endif // SNU_SNUPUCK