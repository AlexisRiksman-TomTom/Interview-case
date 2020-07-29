/** @file
 *  @brief Interface of the throttle-pedal handling.
 */

#ifndef THROTTLE_PEDAL_H_
#define THROTTLE_PEDAL_H_

#include <vector>

const int kNumThrottleSensors = 2;  //!< The number of sensors on the throttle pedal.

/** @brief Sample the throttle sensors and report the throttle-pedal status.
 *  @brief Apply validity checking and low-pass filtering on the sensor values.
 */
class CThrottlePedal {
public:
  CThrottlePedal();
  ~CThrottlePedal();

  /** @brief Update the throttle-pedal status using the current sensor values.
   *  @biref Assumption: This function is called at 10 millisecond interval.
   *  @returns true  = sensor values are read properly,
   *  @returns false = sensor values cannot be read or are invalid.
   */
  bool ReadThrottleSensors();

  /** @brief Report the current position of the throttle pedal.
   *  @returns The current position of the throttle pedal:
   *  @returns   0.0 = pedal in idle position
   *  @returns   1.0 = pedal pressed to its maximum
   */
  float ThrottlePosition() const;

private:
  // Note: Is it safe to use std::vector in automotive applications?
  //!< Samples from the ADC1 pedal sensors.
  std::vector<float> mSensorPosition[kNumThrottleSensors];
};

#endif  // THROTTLE_PEDAL_H_
