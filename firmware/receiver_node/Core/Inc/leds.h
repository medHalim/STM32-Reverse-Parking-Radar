/**
 * @file 	leds.h
 * @ingroup Receiver_Node
 * @brief 	LED control functions for the distance indicator.
 *
 * This module provides functions to turn on a specific number of LEDs
 * according to the measured distance from the CAN receiver node.
 */
#ifndef __LEDS_H
#define __LEDS_H

#include "main.h"

/**
 * @brief Turn on LED pattern 1 (closest distance indication).
 */
void leds_1(void);

/**
 * @brief Turn on LED pattern 2.
 */
void leds_2(void);

/**
 * @brief Turn on LED pattern 3.
 */
void leds_3(void);

/**
 * @brief Turn on LED pattern 4.
 */
void leds_4(void);

/**
 * @brief Turn on LED pattern 5.
 */
void leds_5(void);

/**
 * @brief Turn on LED pattern 6.
 */
void leds_6(void);

/**
 * @brief Turn on LED pattern 7 (all LEDs on for very close distance).
 */
void leds_7(void);

#endif /* __LEDS_H */
