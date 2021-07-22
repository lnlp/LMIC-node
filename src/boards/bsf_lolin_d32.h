/*******************************************************************************
 * 
 *  File:         bsf_lolin_d32.h
 * 
 *  Function:     Board Support File for Lolin D32 with external SPI LoRa module.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by onboard USB to serial).
 *                It supports automatic firmware upload and serial over USB. 
 *                No onboard display. Optionally an external display can be connected.
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
 *                LED   <――――――――――>   5  (LED_BUILTIN) (SS) Active-low
 *                                        Conflicts with SS.
 *  
 *                I2C [display]      GPIO
 *                ---                 ---- 
 *                SDA   <――――――――――>  21  (SDA)
 *                SCL   <――――――――――>  22  (SCL)
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  23  (MOSI)
 *                MISO  <――――――――――>  19  (MISO)
 *                SCK   <――――――――――>  18  (SCK)
 *                NSS   <――――――――――>  27  Do not use SS because conflicts with LED_BUILTIN.
 *                RST   <――――――――――>  32
 *                DIO0  <――――――――――>  33
 *                DIO1  <――――――――――>  34
 *                DIO2                 -  Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/lolin_d32.html
 *
 *  Identifiers:  LMIC-node
 *                    board-id:      lolin_d32
 *                PlatformIO
 *                    board:         lolin_d32
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_LOLIN_D32
 *                    architecture:  ARDUINO_ARCH_ESP32
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_LOLIN_D32_H_
#define BSF_LOLIN_D32_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "lolin-d32"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
// #define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// #ifndef LMIC_CLOCK_ERROR_PPM
//     #define LMIC_CLOCK_ERROR_PPM 0
// #endif   

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 27,
    .rxtx = LMIC_UNUSED_PIN,
    .rst =32,
    .dio = { /*dio0*/ 33, /*dio1*/ 34, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif    

#ifdef USE_LED
    EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::Low);
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
            // No actions required for this board.
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;           
    }
    return success;
}


#endif  // BSF_LOLIN_D32_H_