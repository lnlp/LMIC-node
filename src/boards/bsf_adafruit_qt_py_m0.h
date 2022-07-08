/*******************************************************************************
 * 
 *  File:         bsf_adafruit_qt_py_m0.h
 * 
 *  Function:     Board Support File for Adafruit QT Py (SAMD21)
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
 *                No onboard display. Optionally an external display can be connected.
 *                Has onboard LED but this is a Neopixel RGB LED which requires
 *                a separate library and is currently not supported by LMIC-node.
 * 
 *                Connect the LoRa module and optional display
 *                according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                 GPIO 
 *                ----                 ----        
 *                                     13  (LED_BUILTIN) defined but there is no LED
 *                NEOPIX <――――――――――>  11  (PIN_NEOPIXEL) WS2812B Neopixel LED
 *                                     12  Neopixel enable. Default enabled = HIGH.
 * 
 *                I2C [display]        GPIO
 *                ---                  ----
 *                SDA    <――――――――――>  20  (SDA)
 *                SCL    <――――――――――>  21  (SCL)
 *
 *                SPI/LoRa             GPIO
 *                ---                  ----
 *                MOSI   <――――――――――>  10  (MOSI)
 *                MISO   <――――――――――>   9  (MISO)
 *                SCK    <――――――――――>   8  (SCK)
 *                NSS    <――――――――――>   1  NOT SS, SS = 0
 *                RST    <――――――――――>   -  not connected
 *                DIO0   <――――――――――>   2
 *                DIO1   <---------->   3
 *                DIO2                  -  Not needed for LoRa
 *
 *  Docs:         https://docs.platformio.org/en/latest/boards/atmelsam/adafruit_qt_py_m0.html 
 *
 *  Identifiers:  LMIC-node
 *                    board-id:      adafruit_qt_py_m0
 *                PlatformIO
 *                    board:         adafruit_qt_py_m0
 *                    platform:      atmelsam
 *                Arduino
 *                    board:         ADAFRUIT_QTPY_M0
 *                    architecture:  ARDUINO_ARCH_SAMD
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_ADAFRUIT_QT_PY_M0_H_
#define BSF_ADAFRUIT_QT_PY_M0_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "qt-py"  // Default deviceid value

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
    .nss = 1,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = { /*dio0*/ 2, /*dio1*/ 3, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 8,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    Serial_& serial = Serial;
#endif    

#ifdef USE_LED
    #error "Invalid option: USE_LED. Onboard Neopixel RGB LED is currently not supported."
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


#endif  // BSF_ADAFRUIT_QT_PY_M0_H_