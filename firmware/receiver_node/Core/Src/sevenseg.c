/**
 * @file 	sevenseg.c
 * @ingroup Receiver_Node
 * @brief 	7-segment display driver
 *
 * This module implements the control of a multiplexed 7-segment display
 * using GPIO pins on STM32. It provides a function to update the segments
 * according to a numeric value (0-9).
 */
#include "sevenseg.h"
#include "main.h"

/** Segment encodings for digits 0-9 (GFEDCBA) */
uint8_t segmentNumber[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,
    0x6d, 0x7d, 0x07, 0x7f, 0x67
};

/**
 * @brief Update the 7-segment display segments for a given number.
 * @param number The number to display encoded as bits for segments A-G.
 *               Each bit corresponds to a segment (bit 0 = A, bit 6 = G).
 * @retval None
 */
void SevenSegment_Update(uint8_t number)
{
    HAL_GPIO_WritePin(GPIOB, A_Pin, (GPIO_PinState)((number >> 0) & 0x01));
    HAL_GPIO_WritePin(GPIOB, B_Pin, (GPIO_PinState)((number >> 1) & 0x01));
    HAL_GPIO_WritePin(GPIOB, C_Pin, (GPIO_PinState)((number >> 2) & 0x01));
    HAL_GPIO_WritePin(GPIOB, D_Pin, (GPIO_PinState)((number >> 3) & 0x01));
    HAL_GPIO_WritePin(GPIOB, E_Pin, (GPIO_PinState)((number >> 4) & 0x01));
    HAL_GPIO_WritePin(GPIOB, F_Pin, (GPIO_PinState)((number >> 5) & 0x01));
    HAL_GPIO_WritePin(GPIOB, G_Pin, (GPIO_PinState)((number >> 6) & 0x01));
}
