/*******************************************************************************
 * 
 *  File:         bsf_unphone9.h
 * 
 *  Function:     Board Support File for unPhone9 RFM95W LoRa.
 * 
 *  Copyright:    Copyright (c) 2022 Hamish Cunningham
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Hamish Cunningham
 * 
 *  Description:  This is an ESP32S3 board with onboard USB (provided by the MCU).
 *                It supports automatic firmware upload and serial over USB.
 *                Onboard display via HX8357
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----        
 *                LED   <――――――――――>  13  (LED_BUILTIN) active-low
 * 
 *                I2C [display]       GPIO
 *                ---                 ----
 *                SDA   <――――――――――>   3  (SDA)
 *                SCL   <――――――――――>   4  (SCL)
 *
 *                SPI/LoRa            GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  40  (MOSI)
 *                MISO  <――――――――――>  41  (MISO)
 *                SCK   <――――――――――>  39  (SCK)
 *                NSS   <――――――――――>  44  (unPhone::LORA_CS)
 *                RST   <――――――――――>  42  (unPhone::LORA_RESET)
 *                DIO0  <――――――――――>  10  (unPhone::LMIC_DIO0)
 *                DIO1  <――――――――――>  11  (unPhone::LMIC_DIO1)
 *
 *  Docs:         https://unphone.net
 *                https://iot.unphone.net
 *                https://gitlab.com/hamishcunningham/unphone
 *                https://gitlab.com/hamishcunningham/unPhoneLibrary
 *
 *  Identifiers:  LMIC-node
 *                    board-id:      unphone9_lora
 *                PlatformIO
 *                    board:         unphone9
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_UNPHONE9
 *                    architecture:  ARDUINO_ARCH_ESP32
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_UNPHONE9_LORA_H_
#define BSF_UNPHONE9_LORA_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "unphone9"  // Default deviceid value

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
// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 44,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 42,
    .dio = { /*dio0*/ 10, /*dio1*/ 11, /*dio2*/ LMIC_UNUSED_PIN },
#ifdef MCCI_LMIC
    .rxtx_rx_active = 0,
    .rssi_cal = 8, // TODO in unphone library code is 0; 8 works here though...
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
#  define serial Serial
// Serial_& serial = Serial;  (creates compile errors)
#endif

#ifdef USE_LED
    EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::Low);
#endif

#ifdef USE_DISPLAY
    // TODO HX8357

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


#endif  // BSF_UNPHONE9_LORA_H_
