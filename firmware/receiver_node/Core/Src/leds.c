/**
 * @file 	leds.c
 * @ingroup Receiver_Node
 * @brief 	LED control implementation for distance indication.
 *
 * Each function corresponds to a specific LED pattern,
 * representing different distance thresholds from the CAN receiver.
 */
#include "leds.h"
#include "main.h"

/**
 * @brief Turn on all LEDs (pattern 7) – closest distance.
 */
void leds_7() {
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin|Blue2_Pin|Red1_Pin|Red2_Pin, GPIO_PIN_SET);
}

/**
 * @brief Turn on LEDs pattern 6.
 */
void leds_6() {
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin|Blue2_Pin|Red1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Red2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn on LEDs pattern 5.
 */
void leds_5() {
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin|Blue2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Red1_Pin|Red2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn on LEDs pattern 4.
 */
void leds_4() {
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Blue2_Pin|Red1_Pin|Red2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn on LEDs pattern 3.
 */
void leds_3() {
    HAL_GPIO_WritePin(GPIOB, Green3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, Blue1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, Blue2_Pin|Red1_Pin|Red2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn on LEDs pattern 2.
 */
void leds_2() {
    HAL_GPIO_WritePin(GPIOA, Green1_Pin|Green2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, Blue2_Pin|Red1_Pin|Red2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn on LEDs pattern 1 – only Green1 for farthest distance.
 */
void leds_1() {
    HAL_GPIO_WritePin(GPIOA, Green1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, Green3_Pin|Blue1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, Green2_Pin|Blue2_Pin|Red1_Pin|Red2_Pin, GPIO_PIN_RESET);
}
