/** @file
 *  @brief Main file.
 *  @description Just contains some example code. Adapt it in the way you like.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "motor_torque.h"
#include "throttle_pedal.h"
#include "drivers/adc_driver/adc_driver.h"
#include "drivers/error_led/error_led.h"

int main(int argc, char *argv[])
{
  error_led_init();

  CThrottlePedal pedal;
  while (1) {
    bool ok = pedal.ReadThrottleSensors();
    error_led_set(!ok);

    float car_speed = 50.0;  //TODO: The current car speed in km/h.

    float torque = CMotorTorque::Torque(pedal.ThrottlePosition(), car_speed);
    (void)torque;  //TODO

    usleep(10*1000);
  }
}
