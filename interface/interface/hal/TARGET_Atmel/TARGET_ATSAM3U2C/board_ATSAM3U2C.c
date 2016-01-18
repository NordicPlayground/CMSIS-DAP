/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "board.h"
#include "read_uid.h"
#include "sam3u2c.h"

#define BOARD_ID_NRF51_DK "1100"
#define BOARD_ID_NRF51_Dongle "1120"
#define BOARD_ID_NRF52_DK "1101"

void set_correct_board_id(void) {
  // Work around for setting the correct board id based on GPIOs.
  uint8_t bit1;
  uint8_t bit2;
  //uint8_t bit3;
  uint8_t *ptr; // Trick to avoid const of board.id
  
  PIOB->PIO_PER = (1 << 1); // Enable PIO pin PB1
  PIOB->PIO_PER = (1 << 2); // Enable PIO pin PB2
  PIOB->PIO_PER = (1 << 3); // Enable PIO pin PB2
  PIOB->PIO_ODR = (1 << 1); // Disabe output
  PIOB->PIO_ODR = (1 << 2); // Disabe output
  PIOB->PIO_ODR = (1 << 3); // Disabe output
  PIOB->PIO_PUER = (1 << 1); // Enable pull-up 
  PIOB->PIO_PUER = (1 << 2); // Enable pull-up
  PIOB->PIO_PUER = (1 << 3); // Enable pull-up
  
  bit1 = (PIOB->PIO_PDSR >> 1) & 1; // Read PB1
  bit2 = (PIOB->PIO_PDSR >> 2) & 1; // Read PB2
  
  if (!bit2 && bit1) { //NRF51_DK
    ptr = (uint8_t*)(&(board.id)); // Trick to avoid const of board.id
    *(ptr++) = BOARD_ID_NRF51_DK[0];
    *(ptr++) = BOARD_ID_NRF51_DK[1];
    *(ptr++) = BOARD_ID_NRF51_DK[2];
    *(ptr++) = BOARD_ID_NRF51_DK[3];
  }
  else if (!bit2 && !bit1) { //NRF51_Dongle
    ptr = (uint8_t*)(&(board.id)); // Trick to avoid const of board.id
    *(ptr++) = BOARD_ID_NRF51_Dongle[0];
    *(ptr++) = BOARD_ID_NRF51_Dongle[1];
    *(ptr++) = BOARD_ID_NRF51_Dongle[2];
    *(ptr++) = BOARD_ID_NRF51_Dongle[3];
  }
  else if (bit2 && !bit1) {
    ptr = (uint8_t*)(&(board.id)); // Trick to avoid const of board.id
    *(ptr++) = BOARD_ID_NRF52_DK[0];
    *(ptr++) = BOARD_ID_NRF52_DK[1];
    *(ptr++) = BOARD_ID_NRF52_DK[2];
    *(ptr++) = BOARD_ID_NRF52_DK[3];  
  }
  else { //mkit
    // board.id already set
  }
  
  PIOB->PIO_PUDR = (1 << 1); // Disable pull-up 
  PIOB->PIO_PUDR = (1 << 2); // Disable pull-up
  PIOB->PIO_PUDR = (1 << 3); // Disable pull-up
}

void board_init(void)
{
    set_correct_board_id();
  create_unique_id();
}

void HardFault_Handler(void)
{
    NVIC_SystemReset();
}
