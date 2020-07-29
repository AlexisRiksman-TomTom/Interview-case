/** @file
 *  @brief Source file for a (dummy) ADC driver.
 */

#include "adc_driver.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// Check whether an ADC channel-id is valid.
static bool adc_channel_is_valid(adc_channel_id_t id) {
  return (ADC_CHANNEL0 <= id && id < ADC_NUM_CHANNELS);
}

// Some utilities that might help you with testing.
static adc_value_t adc_read_values[ADC_NUM_CHANNELS] = {0};
static adc_return_t adc_read_return_value[ADC_NUM_CHANNELS] = {ADC_RET_OK};

void adc_read_set_output(adc_channel_id_t id, adc_value_t value,
                    adc_return_t return_value) {
  if (!adc_channel_is_valid(id)) {
    printf("[ADC] adc_read_set_output: Invalid channel %i\n", id);
    assert(false);
    return;
  }

  if (id < ADC_NUM_CHANNELS) {
    adc_read_values[id] = value;
    adc_read_return_value[id] = return_value;
  }
}

adc_return_t adc_init(adc_channel_id_t id) {
  if (!adc_channel_is_valid(id)) {
    printf("[ADC] adc_init: Invalid channel %i\n", id);
    assert(false);
    return ADC_RET_NOK;
  }

  printf("[ADC] Initializing channel %i\n", id);
  return ADC_RET_OK;
}

adc_return_t adc_read(adc_channel_id_t id, adc_value_t* value) {
  if (!adc_channel_is_valid(id)) {
    printf("[ADC] adc_read: Invalid channel %i\n", id);
    assert(false);
    return ADC_RET_NOK;
  }

  *value = adc_read_values[id];
  adc_return_t return_value = adc_read_return_value[id];

  printf("[ADC] Reading channel %i = %u. Ret = %i\n", id, *value, return_value);

  return return_value;
}
