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


// Let's define an enumerable `RewardFunctionType`to
// constrain the possible values the reward
// function setting can have. In case you want to
// add RewardFunctionTypes, please add it to this list.
enum class RewardFunctionType {
    Linear,
    Trigonometric,
    TwoPiecewise,
};

class RewardFunction
{
public:
    float getReward(float angle);
    float normalizeAngle(float angle);
    float r2d(float rads);
private:
    virtual float calcReward(float angle) = 0;
};

class LinearAngularReward: public RewardFunction
{
private:
    float calcReward(float angle);
};

class TrigonometricReward: public RewardFunction
{
private:
    float calcReward(float angle);
};

class TwoPiecewiseReward : public RewardFunction
{
public:
    TwoPiecewiseReward(RewardFunction *first, RewardFunction *second): firstSection(first), secondSection(second) {};
private:
    float calcReward(float angle);
    RewardFunction *firstSection;
    RewardFunction *secondSection;
};

#endif /* RewardFunction_h */
