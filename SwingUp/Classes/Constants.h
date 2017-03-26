//
//  ConstantsAndSettings.h
//  SwingUp
//
//  Created by Deniz Saner on 3/26/17.
//
//

#ifndef ConstantsAndSettings_h
#define ConstantsAndSettings_h

// Globals definition
// ==================
// PTM_RATIO: Pixel to meter ratio
// As we're initiliazing our display 1136px wide, a PTM ratio
// of 1136/3 sets our screen width to equal 3 meters.

// VELOCITY
// Horizontal Velocity of the agent
const static float PTM_RATIO = 1136.0f / 3.0f;
const static float VELOCTY = 1.7f;
const static int VELOCITY_ITERATIONS =10;
const static int POSITION_ITERATIONS = 10;

// Agent attribtues
// ==================
// AGENT_MASS: Mass of the agent [kg]
// AGENT_FRICTION: Friction of the agent [-]
// AGENT_RESTITUTION: Agent restitution [-]
// AGENT_WIDTH: Agent width [m]
// AGENT_HEIGHT: Agent height [m]
const static float AGENT_MASS = 2.8f;
const static float AGENT_FRICTION = 0.001f;
const static float AGENT_RESTITUTION = 0.0f;
const static float AGENT_WIDTH = 0.1f;
const static float AGENT_HEIGHT = 0.05f;

// Pole attribtues
// ==================
// POLE_MASS: Mass of the agent [kg]
// POLE_FRICTION: Friction of the agent [-]
// POLE_RESTITUTION: Agent restitution [-]
// POLE_WIDTH: Agent width [m]
// POLE_HEIGHT: Agent height [m]
const static float POLE_MASS = 1.0f;
const static float POLE_FRICTION = 0.0f;
const static float POLE_RESTITUTION = 0.0f;
const static float POLE_WIDTH = 0.01f;
const static float POLE_HEIGHT = 0.35f;
const static float POLE_ANGULAR_DAMPING = 0.2f;


// Let's define an enumerable `RewardFunctionType`to
// constrain the possible values the reward
// function setting can have. In case you want to
// add RewardFunctionTypes, please add it to this list.
enum class RewardFunctionType {
    Linear,
    Trigonometric,
    TwoPiecewise,
};

#endif /* ConstantsAndSettings_h */
