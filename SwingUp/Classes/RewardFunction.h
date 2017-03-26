//
//  RewardFunction.hpp
//  SwingUp
//
//  Created by Deniz Saner on 3/25/17.
//
//

#ifndef RewardFunction_h
#define RewardFunction_h
#include <iostream>
#include <map>
#include <cstdio>
#include "Constants.h"


// Reward settings
// ==================
// Choose which reward function to use:
// + Linear - either starting from 0 or -1, see LINEAR_STARTING_FROM_ZERO
// + Trigonometric
// + TwoPiecewise (One of the above reward functions between 0-90° and one
//   of the above reward functions between 0-180°)
// In order to use TwoPiecewise (meaning two functions for 0-90° and 90-180°)
// you'll also have to define the PIECEWISE_FIRST and PIECEWISE_SECOND parameter.
const static RewardFunctionType REWARD_FUNCTION = RewardFunctionType::TwoPiecewise;

// Linear settings
// ================
const static bool LINEAR_REWARD_STARTING_FROM_ZERO = false;

// TwoPiecewiseReward settings
// ===========================
const static RewardFunctionType PIECEWISE_FIRST_SECTION = RewardFunctionType::Linear;
const static RewardFunctionType PIECEWISE_SECOND_SECTION = RewardFunctionType::Trigonometric;

/**
 * Here, we define the base class of our Reward function.
 * We will define two virtual functions: 
 * `clone` will help as creating a pointer to
 * the desired child classes of RewardFunction 
 * so that we can create a mapping between
 * RewardFunctionTypes and RewardFunctions.
 * `calcReward` implements the actual calculation
 * of our reward. It is overidden by every child class.
 * Also, we declare a private helper function
 * called `normalizeAngle` which helps us to normalize
 * the pole's deflection angle between -180° and 180° degrees respectively.
 */
class RewardFunction
{
public:
    virtual RewardFunction* clone() const = 0;
    virtual float calcReward(float angle) = 0;
    float getReward(float angle);

private:
    float normalizeAngle(float angle);
};


/**
 * Our first RewardFunction is the `LinearReward`.
 * The `calcReward` function takes a normalized angle
 * of type flaot as a single argument and returns a
 * reward between -1 and +1, while -1 corresponds to
 * `fabs(angle)=0.0f` and 1 to `fabs(angle)=180.0f`.
 */
class LinearReward: public RewardFunction
{
public:
    virtual LinearReward* clone() const { return new LinearReward(); }
    LinearReward(): startFromZero(LINEAR_REWARD_STARTING_FROM_ZERO) {};
    float calcReward(float angle);
private:
    bool startFromZero;
};


/**
 * The `TrigonometricReward` class is our second
 * reward function: Instead of interpolating linearly,
 * we utilize the trigonometric consinus function to
 * calculate our current reward. For more details, 
 * please read the function description of
 * `TrigonometricReward::calcReward(float angle)`.
 */
class TrigonometricReward: public RewardFunction
{
public:
    virtual TrigonometricReward* clone() const { return new TrigonometricReward(); }
    float calcReward(float angle);
};


/**
 * The `TwoPiecewiseReward` represents a hybrid
 * approach to calculate rewards. It allows us
 * to define two reward functions which will be
 * called dependent on the input angle. The sections
 * are defined between 0° and 90°, and 90° and 180°.
 *
 * NOTE: If `RewardFunctionTypes::TwoPieceWise` is selected
 * as the desired reward function, `TWOPIECEWISE_FIRST_SECTION`
 * and `TWOPIECEWISE_SECOND_SECTION` have to be defined.
 *
 * ENHANCEMENT: In the future, we might introduce 
 * additional constructor arguments, which would allow
 * us to customize the currently static angular sections.
 */
class TwoPiecewiseReward : public RewardFunction
{
public:
    TwoPiecewiseReward(){};
    TwoPiecewiseReward(RewardFunction* first, RewardFunction* second): firstSection(first), secondSection(second) {};
    
    virtual TwoPiecewiseReward* clone() const { return new TwoPiecewiseReward(); }
    void setSections(RewardFunction* first, RewardFunction* second);
    float calcReward(float angle);
    RewardFunction *firstSection;
    RewardFunction *secondSection;
};


#endif /* RewardFunction_h */
