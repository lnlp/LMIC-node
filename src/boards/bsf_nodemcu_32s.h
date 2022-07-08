/*******************************************************************************
 * 
 *  File:         bsf_nodemcu_32s.h
 * 
 *  Function:     Board Support File for NodeMCU-32S with external SPI LoRa module.
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
 *                LED   <――――――――――>   2  (LED_BUILTIN)  Active-high
 *  
 *                I2C [display]       GPIO
 *                ---                 ---- 
 *                SDA   <――――――――――>  21  (SDA)
 *                SCL   <――――――――――>  22  (SCL)
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  23  (MOSI)
 *                MISO  <――――――――――>  19  (MISO)
 *                SCK   <――――――――――>  18  (SCK)
 *                NSS   <――――――――――>   5  (SS)
 *                RST   <――――――――――>  27
 *                DIO0  <――――――――――>  34
 *                DIO1  <――――――――――>  35
 *                DIO2                 -  Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/nodemcu-32s.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         nodemcu_32s
 *                PlatformIO
 *                    board:         nodemcu-32s
 *                    platform:      espressif32
 *                rduino
 *                    board:         ARDUINO_NodeMCU_32S
 *                    architecture:  ARDUINO_ARCH_ESP32
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_NODEMCU_32S_H_
#define BSF_NODEMCU_32S_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "nodemcu-32s"  // Default deviceid value

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
    .nss = 5,
    .rxtx = LMIC_UNUSED_PIN,
    .rst =27,
    .dio = { /*dio0*/ 34, /*dio1*/ 35, /*dio2*/ LMIC_UNUSED_PIN }
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
            // No actions required for this board.
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;           
    }
    return success;
}


#endif  // BSF_NODEMCU_32S_H_