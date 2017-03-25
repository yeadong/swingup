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
    float degrees = this->r2d(angle);
    float normalizedAngle = this->normalizeAngle(degrees);
    return calcReward(normalizedAngle);
}

/**
 This function transforms radiants to degrees.
 Following physical principle is leveraged for the calculation:
 1rad = pi/180° -> phi[°] = radAngle*180°/pi
 @param <float> The pole angle in radiants.
 @return <float> The pole angle in degrees.
 */
float RewardFunction::r2d(float r)
{
    return r * 180.0f / M_PI;
}


/**
 This function normalizes any given angle to a value between
 -180° and 180°.
 @param <float> The unnormalized angle [°]
 @return <float> The normalized angle [°]
 */
float RewardFunction::normalizeAngle(float angle)
{
    angle = fmod(angle + 180,360);
    if (angle < 0)
        angle += 360;
    return angle - 180.0f;
}

/**
 Calculates a reward based on the float angle input of the pole.
 The input angles from |0°| to |180°| are interpolated linearly
 to values between -1 and 1.
 @param <float> The angle of the pole from the resting position
 @return <float> The reward corresponding to the current angle.
 */
float LinearAngularReward::calcReward(float angle)
{
    float absAngle = fabsf(angle);
    float ret = -1.0f + 1.0f / 90.0f * absAngle;
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
    float ret = -cos(absAngle * M_PI / 180.0f);
    return ret;
}

float TwoPiecewiseReward::calcReward(float angle)
{
 if (angle < 90.0f)
 {
     return this->firstSection->getReward(angle);
 } else
 {
     return this->secondSection->getReward(angle);
 }
    
}
