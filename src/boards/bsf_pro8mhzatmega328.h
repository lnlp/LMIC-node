/*******************************************************************************
 * 
 *  File:          bsf_pro8mhzatmega328.h
 * 
 *  Description:   Board Support File for Arduino Pro Mini ATmega328 3.3V 8MHz 
 *                 with external SPI LoRa module.
 * 
 *  Copyright:     Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:       MIT License. See included LICENSE file.
 * 
 *  Description:  This Board has no onboard USB and no onboard display.
 *                Optionally an external display can be connected.
 *                Onboard LED cannot be used due to hardware conflict.
 * 
 *                For firmware upload and serial monitor use a USB to serial 
 *                adapter that supports DTR (for automatic firmware upload).
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
 *                LED   <――――――――――>  13  (LED_BUILTIN) (SCK) Active-high, 
 *                                        Useless, shared with SCK.
 * 
 *                I2C [display]       GPIO  
 *                ---                 ---- 
 *                SDA   <――――――――――>   2  (SDA)  This is labeled A4 on the PCB.
 *                SCL   <――――――――――>   3  (SCL)  This is labeled A5 on the PCB.
 *
 *                SPI/LoRa module     GPIO
 *                ----                ----
 *                MOSI  <――――――――――>  11  (MOSI)
 *                MISO  <――――――――――>  12  (MISO)
 *                SCK   <――――――――――>  13  (SCK)
 *                NSS   <――――――――――>  10  (SS)
 *                RST   <――――――――――>   7
 *                DIO0  <――――――――――>   8
 *                DIO1  <――――――――――>   9
 *                DIO2                 -  Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/atmelavr/pro8MHzatmega328.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         pro8mhzatmega328
 *                PlatformIO
 *                    board:         pro8MHzatmega328
 *                    platform:      atmelavr
 *                Arduino
 *                    board:         ARDUINO_AVR_PRO
 *                    architecture:  ARDUINO_ARCH_AVR
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_PRO8MHZATMEGA328_H_
#define BSF_PRO8MHZATMEGA328_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "pro-mini"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
// #define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// Value 30000 was determined empirically.
#ifndef LMIC_CLOCK_ERROR_PPM
    #define LMIC_CLOCK_ERROR_PPM 30000
#endif   

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 7,
    .dio = { /*dio0*/ 8, /*dio1*/ 9, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 1000000     /* 1 MHz */
#endif    
};

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif  

#ifdef USE_LED
    #error Invalid option: USE_LED. Onboard LED cannot be used due to hardware conflict.
    // EasyLed led(<external LED GPIO>, EasyLed::ActiveLevel::Low);
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


#endif  // BSF_PRO8MHZATMEGA328_H_