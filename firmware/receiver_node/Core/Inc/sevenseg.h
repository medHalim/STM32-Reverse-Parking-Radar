/**
 * @file    sevenseg.h
 * @brief 	Update the 7-segment display with a digit.
 * @ingroup Receiver_Node
 *
 * Sets the GPIO outputs corresponding to the given digit value.
 * The function does not handle digit multiplexing; it only updates
 * the segment lines (A–G, DP).
 *
 * @param number Digit to display (0–9 or predefined segment pattern).
 */
#ifndef __SEVENSEG_H
#define __SEVENSEG_H

#include "main.h"

void SevenSegment_Update(uint8_t number);

#endif /* __SEVENSEG_H */
