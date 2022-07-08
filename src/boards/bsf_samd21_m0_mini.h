/*******************************************************************************
 * 
 *  File:         bsf_samd21_m0_mini.h
 * 
 *  Function:     Board Support File for SAMD21 M0-Mini
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
 *                On some boards a user LED is present while on
 *                others it PA17 / 13 which is shared which is also used for SPI SCK.
 *                Therefore onboard LED is not supported by LMIC-node.
 * 
 *                The SPI pins are located on the top side of the PCB as part of 
 *                the ICSP connector. It is strange that this board has an ICSP 
 *                connector because the board has an ARM MCU (not AVR)
 *                So the SPI pins are not breadboard friendly.
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
 *                LED                 -          (LED_BUILTIN, PIN_LED, PIN_LED_13)
 *                LED2  <――――――――――>  PB03 / 25  (RX, PIN_LED2, PIN_LED_RXL)
 *                LED3  <――――――――――>  PA27 / 26  (TX, PIN_LED3, PIN_LED_TXL)
 * 
 *                I2C [display]       GPIO  
 *                ---                 ----
 *                SDA   <――――――――――>  PA22 / 20  (SDA)
 *                SCL   <――――――――――>  PA23 / 21  (SCL)
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  PB10 / 23  (MOSI) ICSP connector pin 4
 *                MISO  <――――――――――>  PA12 / 22  (MISO) ICSP connector pin 1
 *                SCK   <――――――――――>  PB11 / 24  (SCK)  ICSP connector pin 3
 *                NSS   <――――――――――>  PA20 /  6
 *                RST   <――――――――――>  PA21 /  7
 *                DIO0  <――――――――――>  PA06 /  8
 *                DIO1  <――――――――――>  PA07 /  9
 *                DIO2                -          Not needed for LoRa.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/atmelsam/zeroUSB.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         samd21_m0_mini
 *                PlatformIO
 *                    board:         zeroUSB
 *                    platform:      atmelsam
 *                Arduino
 *                    board:         ARDUINO_SAMD_ZERO
 *                    architecture:  ARDUINO_ARCH_SAMD
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_SAMD21_M0_MINI_
#define BSF_SAMD21_M0_MINI_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "samd21-m0-mini"  // Default deviceid value

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
    .nss = 6,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 7,
    .dio = { /*dio0*/ 8, /*dio1*/ 9, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    Serial_& serial = SerialUSB;
#endif    

#ifdef USE_LED
    #error Invalid option: USE_LED. Onboard LED is not supported.
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


#endif  // BSF_SAMD21_M0_MINI_
