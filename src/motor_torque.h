/** @file
 *  @brief Interface of the wheel-torque calculation.
 */

#ifndef MOTOR_TORQUE_H_
#define MOTOR_TORQUE_H_

/** @brief Calculate the amount of torque that the motors must produce. */
class CMotorTorque {
private:
  CMotorTorque();

public:
  /** @brief Calculate the desired motor torque from throttle-pedal and speed.
   *  @param [in] aThrottle = The position of the throttle pedal as 0.0 .. 1.0.
   *  @param [in] aSpeed    = The current car speed in km/h, negative when reversing.
   *  @returns The desired motor torque in Nm.
   */
  static float Torque(float aThrottle, float aSpeed);
};

#endif  // MOTOR_TORQUE_H_

