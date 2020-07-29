/** @file
 *  @brief Implementation of the throttle-pedal handling.
 */

#include "throttle_pedal.h"
#include "drivers/adc_driver/adc_driver.h"

#include <assert.h>
#include <stdio.h>

namespace {
  //! The maximum angle to which the throttle-pedal can be pushed.
  const float kMaxPedalAngle = 30.0;

  /** The number of sensor samples that must be averaged out, thus the length
   *  of the low-pass filter on each sensor.
   */
  const int kNumFilterSamples = 5;

  /** Details of a throttle-pedal sensor.
   *  adc_value = adc_offset + (adc_factor * angle)
   */
  struct TSensorDetails {
    adc_channel_id_t mAdcChannel;  //!< The ADC channel to read sensor data.
    float            mAdcOffset;   //!< The offset to calculate the pedal angle.
    float            mAdcFactor;   //!< The scale factor to calculate pedal angle.
  };
  
  const TSensorDetails kSensorDetails[kNumThrottleSensors]
  = {
      { ADC_CHANNEL0, 0.5, 0.10 },  //!< Throttle sensor 1.
      { ADC_CHANNEL1, 1.0, 0.08 }   //!< Throttle sensor 2.
    };
}

CThrottlePedal::CThrottlePedal()
  : mSensorPosition()
{
  // Initialize the ADC channel for each sensor.
  for (auto& sensor : kSensorDetails) {
    if (adc_init(sensor.mAdcChannel) == ADC_RET_OK) {
      printf("[PEDAL] ADC channel %i initialized ok\n", sensor.mAdcChannel);
    } else {
      printf("[PEDAL] Faled to initialize ADC channel %i\n", sensor.mAdcChannel);
      assert(false);
    }
  }
}

CThrottlePedal::~CThrottlePedal()
{
}

bool CThrottlePedal::ReadThrottleSensors()
{
  assert(   (sizeof(kSensorDetails)  / sizeof(kSensorDetails[0]))
         == (sizeof(mSensorPosition) / sizeof(mSensorPosition[0])));

  bool ok = true;  // no problems yet

  for (int sensor = 0; sensor < kNumThrottleSensors; ++sensor) {
    auto& details = kSensorDetails[sensor];
    auto& values  = mSensorPosition[sensor];

    // Discard the oldest sensor value.
    values.erase(values.begin());

    // Read the ADC channel for this sensor and convert this to pedal position:
    //   angle = (adc_value - adc_offset) / adc_factor
    //   position = angle / max-angle
    float position = 0.0;
    adc_value_t value;
    if (adc_read(details.mAdcChannel, &value) == ADC_RET_OK) {
      float angle = (static_cast<float>(value) - details.mAdcOffset) / details.mAdcFactor;
      if (angle >= 0.0 && angle <= kMaxPedalAngle) {
        position = angle / kMaxPedalAngle;
      } else {
        printf("[PEDAL] Invalid ADC angle %.1f for ADC channel %i value %u\n",
               angle, details.mAdcChannel, value);
        ok = false;
      }
    } else {
      printf("[PEDAL] Failed to read ADC channel %i\n", details.mAdcChannel);
      ok = false;
    }

    // Store the new pedal position.
    values.push_back(position);
  }

  return ok;
}

float CThrottlePedal::ThrottlePosition() const
{
  float total = 0.0;
  int   numSensors = 0;

  // Calculate the average value per sensor and add this to the total.
  for (auto& values : mSensorPosition) {
    float sum = 0.0;
    for (auto& val : values) {
      sum += val;
    }
    float average = sum / values.size();

    total += average;
    ++numSensors;
  }

  // Return the average of all sensors.
  return total / numSensors;
}

