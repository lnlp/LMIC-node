/*******************************************************************************
 * 
 *  File:         bsf_teensylc.h
 * 
 *  Function:     Board Support File for Teensy LC with external SPI LoRa module.
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
 *                Onboard LED and SPI SCK use the same GPIO (13) which causes a hardware conflict.
 *                Fortunately SCK can be remapped to a different GPIO (14).
 * 
 *                Connect the LoRa module and optional display
 *                according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----      
 *                LED   <――――――――――>  13  (LED_BUILTIN) (SCK) Hardware conflict. 
 *                                        Cannot use LED_BUILTIN and SCK together.
 * 
 *                I2C [display]       GPIO  
 *                ---                 ----
 *                SDA   <――――――――――>  18  (SDA)
 *                SCL   <――――――――――>  19  (SCL)
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  11  (MOSI)
 *                MISO  <――――――――――>  12  (MISO)
 *                SCK   <――――――――――>  14  NOT SCK! 
 *                                        SPI SCK is remapped to GPIO14 because
 *                                        SCK definition conflicts with onboard LED.
 *                NSS   <――――――――――>  10
 *                RST   <――――――――――>  9
 *                DIO0  <――――――――――>  8
 *                DIO1  <――――――――――>  7
 *                DIO2                -   Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/teensy/teensylc.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         teensylc
 *                PlatformIO
 *                    board:         teensylc
 *                    platform:      teensy
 *                Arduino
 *                    board:         ARDUINO_TEENSYLC
 *                    architecture:  <not defined>
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_TEENSYLC_H_
#define BSF_TEENSYLC_H_

#include "LMIC-node.h"
#include <SPI.h>

#define DEVICEID_DEFAULT "teensylc"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
#define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// #ifndef LMIC_CLOCK_ERROR_PPM
//     #define LMIC_CLOCK_ERROR_PPM 0
// #endif   

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = { /*dio0*/ 8, /*dio1*/ 7, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    usb_serial_class& serial = Serial;
#endif    

#ifdef USE_LED
    EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::High);
#endif

#ifdef USE_DISPLAY
    // Create U8x8 instance for SSD1306 OLED display (no reset) using hardware I2C.
    U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ U8X8_PIN_NONE, /*scl*/ SCL, /*sda*/ SDA);
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
            // SPI SCK needs to remapped to GPIO14 because it conflicts with LED_BUILTIN.
            SPI.setSCK(14);
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;           
    }
    return success;
}


#endif  // BSF_TEENSYLC_H_