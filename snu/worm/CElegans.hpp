/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#ifndef SNU_CELEGANS
#define SNU_CELEGANS

#include <snu/brain/CElegansBrain.hpp>

namespace NSnu
{

/**
 * Count of magnitudes of ventral or dorsal muscles.
 * @see CElegans::getDorsalMagnitudes(), CElegans::getVentralMagnitudes()
 */
static const unsigned MUSCLE_MAGNITUDES = 12;

/**
 * CElegans behavior model.
 * Includes connectome and basic behavior like moving forward befor it finds an obstacle
 * and move backward.
 * This behavior is controlled by connectome and neuron model.
 */
class CElegans
{
public:

    /**
     * Default constructor.
     */
    CElegans() throw();

    /**
     * Processes the time step for all neurons.
     *
     * @param Time step.
     */
    void step(float h, unsigned period);

    /**
     * Teaches food direction factors.
     *
     * @param Time step
     * @param Number of iterations per period.
     */
    void teachFoodDirection(float h, unsigned period);

    /**
     * Teaches nose touching factors.
     *
     * @param Time step
     * @param Number of iterations per period.
     */
    void teachNoseDirection(float h, unsigned period);

    /**
     * Activates sensors related to food.
     */
    void findFood();

    /**
     * Activates sensors related to touching nose.
     */
    void touchNose();

    /**
     * Activates sensors related to touching left body.
     */
    void touchLeft();

    /**
     * Activates sensors related to touching right body.
     */
    void touchRight();

    /**
     * Checks if the nose is touched.
     */
    bool noseTouched() const;

    /**
     * Returns assigned average frequency per some period if food neurons activated.
     * @see teachFoodDirection()
     */
    float getFoodPeriodFrequency() const;

    /**
     * Sets average frequency per some period if food neurons activated.
     *
     * @param New value
     */
    void setFoodPeriodFrequency(float value);

    /**
     * Returns assigned frequency per some period if nose neurons activated.
     * @see teachNoseDirection()
     */
    float getNosePeriodFrequency() const;

    /**
     * Sets average frequency per some period if nose neurons activated.
     *
     * @param New value
     */
    void setNosePeriodFrequency(float value);

    /**
     * Returns muscle ventral neurons activities.
     *
     * @param[out] Result vector.
     */
    void getVentralMagnitudes(float result[MUSCLE_MAGNITUDES]) const;

    /**
     * Returns muscle dorsal neurons activities.
     *
     * @param[out] Result vector.
     */
    void getDorsalMagnitudes(float result[MUSCLE_MAGNITUDES]) const;

    /**
     * Returns spikes of muscule neurones.
     *
     * @param[out] Left dorsal and ventral muscle neurons.
     * @param[out] Right dorsal and ventral muscle neurons.
     */
    void getMusclesActivites(
        unsigned left[SIDE_MUSCLE_NEURONS][MUSCLE_SIDES],
        unsigned right[SIDE_MUSCLE_NEURONS][MUSCLE_SIDES]
        ) const;

protected:

    /**
     * Provides average frequency per period.
     *
     * @param Number of iterations per period.
     * @param[out] Result value.
     * @return true if succeed.
     */
    bool getPeriodFrequency(unsigned period, float& value);

    /**
     * Calculates average frecuency of neurons activities.
     *
     * @param Number of iterations per period.
     */
    void calculateDirection(unsigned period);

    /**
     * Returns average spikes value for VA neurons.
     */
    float getAverageVA() const;

    /**
     * Index of iteration per one period.
     */
    unsigned mPeriodIndex;

    /**
     * Count of spikes per period to calculate frequency.
     */
    unsigned mPeriodSpikesCount;

    /**
     * Is the nose touched?
     */
    bool mNoseTouched;

    /**
     * Average value of spikes per period if nose touched.
     */
    float mAverageNosePeriodFrequency;

    /**
     * Average value of spikes per period if food neurons activated
     */
    float mAverageFoodPeriodFrequency;

    /**
     * Sum of frequencies to calculate average value if nose touched.
     * @see mAverageNosePeriodFrequency
     */
    float mSumNoseFrequencies;

    /**
     * Sum of frequencies to calculate average value if food activated.
     * @see mAverageFoodPeriodFrequency
     */
    float mSumFoodFrequencies;

    /**
     * Index of iteration to calculate average frequency value if nose touched.
     * @see mAverageNosePeriodFrequency
     */
    unsigned mTeachNoseIndex;

    /**
     * Index of iteration to calculate average frequency value if food activated.
     * @see mAverageFoodPeriodFrequency
     */
    unsigned mTeachFoodIndex;

    /**
     * CElegans connectome brain.
     */
    CElegansBrain mBrain;
};

} // namespace NSnu

#endif // SNU_CELEGANS