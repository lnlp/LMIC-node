/*******************************************************************************
 * 
 *  File:         rpi_pico.h
 * 
 *  Function:     Board Support File for Raspberry PI Pico (USB Native port)
 *                with external SPI LoRa module.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by the MCU).
 *                It supports automatic firmware upload and serial over USB.
 *                No onboard display. Optionally an external display con be connected.
 * 
 *                Connect an optional display according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----      
 *                LED   <――――――――――>  25		Internal
 * 
 *                I2C [display]       GPIO  
 *                ---                 ----
 *                SDA   <――――――――――>  6                 pin 9
 *                SCL   <――――――――――>  7                 pin 10 
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  3          (MOSI) pin 5
 *                MISO  <――――――――――>  4          (MISO) pin 6
 *                SCK   <――――――――――>  2          (SCK)  pin 4
 *                NSS   <――――――――――>  5                 pin 7
 *                RST   <――――――――――>  10                pin 14
 *                DIO0  <――――――――――>  11                pin 15
 *                DIO1  <――――――――――>  12                pin 16
 *                DIO2                -          Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/
 *
 *  Identifiers:  LMIC-node
 *                    board:         rpi_pico
 *                PlatformIO
 *                    board:         pico
 *                    platform:      arduino
 *                Arduino
 *                    board:         
 *                    architecture:  
 * 
 ******************************************************************************/

#pragma once

#ifndef RPI_PICO_USB_H_
#define RPI_PICO_USB_H_

#include "LMIC-node.h"
#include "Serial.h"

#define DEVICEID_DEFAULT "rpi-pico"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
#define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// #define LMIC_CLOCK_ERROR_PPM 0

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 5,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 10,
    .dio = { /*dio0*/ 11, /*dio1*/ 12, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    UART& serial = Serial;
#endif    

#ifdef USE_LED
    EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::High);
#endif

#ifdef USE_DISPLAY
    // Create U8x8 instance for SSD1306 OLED display (no reset) using hardware I2C.
    U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ U8X8_PIN_NONE, /*scl*/ 7, /*sda*/ 6);
#endif


bool boardInit(InitType initType)
{
    // This function is used to perform board specific initializations.
    // Required as part of standard template.

    // InitType::Hardware        Must be called at start of setup() before anything else.
    // InitType::PostInitSerial  Must be called after initSerial() before other initializations.    

    bool success = true;
    switch (initType)
    {
        case InitType::Hardware:
            // Note: Serial port and display are not yet initialized and cannot be used use here.
            // No actions required for this board.
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;           
    }
    return success;
}


#endif  // RPI_PICO_USB_H_
