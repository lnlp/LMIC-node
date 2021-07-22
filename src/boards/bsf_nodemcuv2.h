/*******************************************************************************
 * 
 *  File:         bsf_nodemcuv2.h
 * 
 *  Function:     Board Support File for NodeMCU V2 (aka NodeMCU 1.0) with
 *                external SPI LoRa module.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by onboard USB to serial).
 *                It supports automatic firmware upload and serial over USB. 
 *                No onboard display.
 * 
 *                Due to the limited amount of available GPIO pins on the ESP8266
 *                and limitations for some of those pins it is not possible
 *                to use I2C and an external display.
 * 
 *                Connect the LoRa module according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 *                
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----
 *                On NodeMCU <―――――>  D0 / 16  (LED_BUILTIN)
 *                On ESP12E  <―――――>  D4 /  2  On ESP12E module, Active-low.
 *                                              
 *                I2C                 GPIO     ██ I2C cannot be used ██
 *                ---                 ---- 
 *                SCL   <――――――――――>  D1 /  5  (SCL) is used for DIO0
 *                SDA   <――――――――――>  D2 /  4  (SDA) is used for DIO1
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                NSS   <――――――――――>  D8 / 15  (SS)
 *                MOSI  <――――――――――>  D7 / 13  (MOSI)
 *                MISO  <――――――――――>  D6 / 12  (MISO)
 *                SCK   <――――――――――>  D5 / 14  (SCK)
 *                RST   <――――――――――>   -       Not connected (or connect to NodeMCU RST)
 *                DIO0  <――――――――――>  D1 /  5  (SCL)
 *                DIO1  <――――――――――>  D2 /  6  (SDA)
 *                DIO2                 -       Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif8266/nodemcuv2.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         nodemcuv2
 *                PlatformIO
 *                    board:         nodemcuv2
 *                    platform:      espressif8266
 *                Arduino
 *                    board:         ARDUINO_ESP8266_NODEMCU
 *                    architecture:  ARDUINO_ARCH_ESP8266
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_NODEMCU_V2_H_
#define BSF_NODEMCU_V2_H_

#include <ESP8266WiFi.h>
#include "LMIC-node.h"

#define DEVICEID_DEFAULT "nodemcuv2"  // Default deviceid value

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
    .nss = 15,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = { /*dio0*/ 5, /*dio1*/ 4, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 1000000     /* 1 MHz */
#endif    
};

#define LORA_NSS      15  //D8
#define LORA_RST      LMIC_UNUSED_PIN
#define LORA_DIO0      5  //D1
#define LORA_DIO1      4  //D2
#define LORA_DIO2     LMIC_UNUSED_PIN
#define LORA_RXTX     LMIC_UNUSED_PIN

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif   

#ifdef USE_LED
    EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::Low);
#endif

#ifdef USE_DISPLAY
    #error "Invalid option: USE_DISPLAY. I2C and display are not supported due to shortage of available GPIO pins."
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


#endif  // BSF_NODEMCU_V2_H_