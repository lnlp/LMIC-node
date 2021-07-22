/*******************************************************************************
 * 
 *  File:         bsf_lora32u4II.h
 * 
 *  Function:     Board Support File for BSFrance LoRa32u4 II versions 1.0 to 1.3.
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
 *                ██ DIO1 MUST BE MANUALLY WIRED TO GPIO5 for versions < 1.3 (see below) ██
 *                   For version 1.3 (see label on PCB) DIO1 is already wired on PCB.
 * 
 *                On versions 1.0 and 1.1 DIO1 is available as a separate pin. On
 *                version 1.2 DIO1 is available on a solder pad (from a solder jumper) 
 *                on the bottom side of the PCB. On version 1.3 DIO is already wired on the PCB.
 * 
 *                Connect DIO1 and optional display according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----        
 *                LED   <――――――――――>  13  (LED_BUILTIN)   Active-high
 * 
 *                I2C [display]       GPIO  
 *                SDA   <――――――――――>   2  (SDA)
 *                SCL   <――――――――――>   3  (SCL)
 *
 *                SPI/LoRa module     GPIO
 *                MOSI  <――――――――――>  16  (MOSI)
 *                MISO  <――――――――――>  14  (MISO)
 *                SCK   <――――――――――>  15  (SCK)
 *                NSS   <――――――――――>   8
 *                RST   <――――――――――>   4
 *                DIO0  <――――――――――>   7
 *                DIO1  <---------->   5  ██ NOT WIRED on PCB for versions < 1.3 ██
 *                DIO2                 -  Not needed for LoRa
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/atmelavr/lora32u4II.html
 *
 *  Identifiers:   LMIC-node
 *                     board-id:      lora32u4II
 *                 PlatformIO
 *                     board:         lora32u4II
 *                     platform:      atmelavr
 *                 Arduino
 *                     board:         ARDUINO_AVR_FEATHER32U4
 *                     architecture:  ARDUINO_ARCH_AVR
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_LORA32U4II_H_
#define BSF_LORA32U4II_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "lora32u4II"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
#define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// Value (30000) was determined empirically.
#ifndef LMIC_CLOCK_ERROR_PPM
    #define LMIC_CLOCK_ERROR_PPM 30000
#endif    

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = { /*dio0*/ 7, /*dio1*/ 5, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 8,
    .spi_freq = 1000000     /* 1 MHz */
#endif    
};

#ifdef USE_SERIAL
    Serial_& serial = Serial;
    // HardwareSerial& serial = Serial;
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


#endif  // BSF_LORA32U4II_H_