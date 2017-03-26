//
//  RewardFunction.cpp
//  SwingUp
//
//  Created by Deniz Saner on 3/25/17.
//
//

#include "RewardFunction.h"

/**
 Calls the `calcReward` which is implemented by all
 children of the RewardFunction class.
 */
float RewardFunction::getReward(float angle)
{
    float normalizedAngle = this->normalizeAngle(angle);
    return calcReward(normalizedAngle);
}

/**
 This function normalizes any given angle to a value between
 -180° and 180°.
 @param <float> The unnormalized angle [°]
 @return <float> The normalized angle [°]
 */
float RewardFunction::normalizeAngle(float angle)
{    
    angle = fmod(angle + M_PI, M_PI*2.0f);
    if (angle < 0)
        angle += 2.0f * M_PI;
    return angle - M_PI;
}

/**
 Calculates a reward based on the float angle input of the pole.
 The input angles from |0°| to |180°| are interpolated linearly
 to values between -1 and 1 or 0 and 1 depending on the 
 `LINEAR_REWARD_STARTING_FROM_ZERO` setting, which can be found
 in the RewardFunction header file. The differentiation of
 starting points originates from two physical units being 
 leveraged to calculate the current reward:
 
 If we consider the deflection angle as our reference value,
 our reward function is r(phi) = -cos(phi) with values of r:[-1, 1].
 On the other hand, if we leverage the pole's tip's y-coordinate,
 the reward is calculated via 1/ pi* arccos(-y/l). If we transform
 the y-coordinate to y(phi) = -l*cos(phi), our reward finally can
 be written as R(phi) = 1/ phi with values of r: [0, 1].
 
 @param <float> The angle of the pole from the resting position
 @return <float> The reward corresponding to the current angle.
 */
float LinearReward::calcReward(float angle)
{
    float absAngle = fabsf(angle);
    float sfmo = !LINEAR_REWARD_STARTING_FROM_ZERO;
    float ret = -1.0f * sfmo + (1.0f + sfmo) / M_PI  * absAngle;
    return ret;
}


/**
 Calculates a reward based on the float angle input of the pole.
 The input angles from |0°| to |180°| are mapped to values
 between -1 and 1 utilizing a cosine function.
 @param <float> The angle of the pole from the resting position
 @return <float> The reward corresponding to the current angle.
 */
float TrigonometricReward::calcReward(float angle)
{
    float absAngle = fabsf(angle);
    float ret = -cos(absAngle);
    return ret;
}


/**
 Calculates a reward based on the float angle input of the pole.
 The input angles from |0°| to |180°| are mapped to values
 between -1 and 1 utilizing a cosine function. Depending on the
 internal assignment of firstSection and secondSection, different
 reward functions will be used. 
 @param <float> The angle of the pole from the resting position
 @return <float> The reward corresponding to the current angle.
 */
float TwoPiecewiseReward::calcReward(float angle)
{
    if (angle < 90.0f)
    {
        return this->firstSection->calcReward(angle);
    } else
    {
        return this->secondSection->calcReward(angle);
    }
}
