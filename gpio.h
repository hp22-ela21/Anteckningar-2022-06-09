#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include <stdio.h> /* Innehåller printf, FILE* med mera. */
#include <stdlib.h> /* Innehåller malloc, atoi med mera. */
#include <stdbool.h> /* Innehåller bool. */
#include <stdint.h> /* Innehåller uint8_t med mera. */
#include "unistd.h" /* Innehåller funktioner sleep samt usleep. */

/* Enumerationer: */
enum gpio_direction { GPIO_DIRECTION_IN, GPIO_DIRECTION_OUT };
enum gpio_event { GPIO_EVENT_RISING_EDGE, GPIO_EVENT_FALLING_EDGE, GPIO_EVENT_BOTH_EDGES };

/* Externa funktioner: */
void gpio_new(const uint8_t pin, const enum gpio_direction direction);
void gpio_delete(const uint8_t pin);
void gpio_write(const uint8_t pin, const uint8_t value);
uint8_t gpio_read(const uint8_t pin);
void gpio_delay(const size_t delay_time);
void gpio_blink(const uint8_t pin, const size_t delay_time);
bool gpio_event_detected(const uint8_t pin, const enum gpio_event event, uint8_t* last_value);

#endif /* GPIO_H_ */




