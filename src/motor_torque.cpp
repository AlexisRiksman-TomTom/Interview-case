/** @file
 *  @brief Implementation of the wheel-torque calculation.
 */

#include "motor_torque.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

namespace {
  const float kMaxSpeedForward = 250.0;  //<! The maximum allowed speed.
  const float kMaxSpeedReverse = -25.0;  //!< The maximum reverse speed.

  //! A reference point for torque calculation.
  struct TTorqueSpec {
    float mThrottle;  //!< Position of the throttle-pedal, 0 .. 100 percent.
    float mTorque0;   //!< Desired motor torque at 0 km/h.
    float mTorque50;  //!< Desired motor torque at 50 km/h.
  };

  //! The list of reference points for torque calculaton.
  const TTorqueSpec kTorqueSpec[]
  = {
      { 0.00,   0, -30 },
      { 0.10,  18, -10 },
      { 0.20,  35,  10 },
      { 0.30,  50,  30 },
      { 0.40,  62,  45 },
      { 0.60,  82,  72 },
      { 0.80, 103,  95 },
      { 1.00, 120, 120 }
    };
}

float CMotorTorque::Torque(float aThrottle, float aSpeed)
{
  if (aThrottle < 0.0 || aThrottle > 1.0) {
    printf("[TORQUE] Invalid throttle %.1f\n", aThrottle);
    assert(false);
    return 0.0;
  }
  if (aSpeed < kMaxSpeedReverse || aSpeed > kMaxSpeedForward) {
    printf("[TORQUE] Invalid speed %.1f\n", aSpeed);
    return 0.0;
  }

  // Find the next higher reference point for this throttle position.
  const TTorqueSpec* specNext = kTorqueSpec;
  while (aThrottle > specNext->mThrottle) {
    ++specNext;
  }

  // Interpolate for throttle position.
  TTorqueSpec spec = kTorqueSpec[0];
  if (specNext != kTorqueSpec) {
    const TTorqueSpec* specPrev = specNext - 1;
    float fraction =   (specNext->mThrottle - specPrev->mThrottle)
                     / (aThrottle - specPrev->mThrottle);
    spec.mThrottle = aThrottle;
    spec.mTorque0  =   specPrev->mTorque0
                     + (fraction * (specNext->mTorque0 - specPrev->mTorque0));
    spec.mTorque50 =   specPrev->mTorque50
                     + (fraction * (specNext->mTorque50 - specPrev->mTorque50));
  }

  // Interpolate/extrapolate for speed.
  float fraction = aSpeed / 50.0;  // Note: "magic" value must match TTorqueSpec fields.
  return spec.mTorque0 + (fraction * (spec.mTorque50 - spec.mTorque0));
}

