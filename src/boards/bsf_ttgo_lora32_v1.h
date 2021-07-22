/*******************************************************************************
 * 
 *  File:         bsf_ttgo_lora32_v1.h
 * 
 *  Function:     Board Support File for TTGO LoRa32 (aka T3) v1.3.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by onboard USB to serial).
 *                It supports automatic firmware upload and serial over USB. 
 *                Has onboard display.
 * 
 *                The standard I2C pins defined in the BSP do not match the 
 *                GPIO pins that the display is connected to. Therefore the
 *                the I2C Wire object is explicitly initialized with the
 *                correct pins (see boardInit() below).
 * 
 *                Schematic diagram and and pinout diagram show no onboard 
 *                user programmable LED while LED_BUILTIN is defined in BSP.
 *                Definition in BSP is incorrect.
 * 
 *                OLED_RST and LORA_RST are defined in BSP but neither is connected to GPIO.
 *                Definitions in BSP are incorrect.               
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 *                
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----
 *                LED                 -            Incorrectly defined in BSP as LED_BUILTIN (2).
 * 
 *                I2C/Display         GPIO
 *                ---                 ---- 
 *                SDA   <――――――――――>  4   Not SDA! (OLED_SDA)
 *                SCL   <――――――――――>  15  Not SCL! (OLED_SCL)
 *                RST                     OLED_RST is defined in BSP but not connected to GPIO.
 *
 *                SPI/LoRa            GPIO
 *                ---                 ---- 
 *                MOSI  <――――――――――>  27  (MOSI) (LORA_MOSI)
 *                MISO  <――――――――――>  19  (MISO) (LORA_MISO)
 *                SCK   <――――――――――>   5  (SCK)  (LORA_SCK)
 *                NSS   <――――――――――>  18  (SS)   (LORA_CS)
 *                RST   <――――――――――>  14         (LORA_RST)
 *                DIO0  <――――――――――>  26         (LORA_IRQ)
 *                DIO1  <――――――――――>  33
 *                DIO2  <――――――――――>  32
 * 
 *                Button switches     GPIO
 *                ------              ---- 
 *                Button <―――――――――>  36  (V_SP) Active-low
 * 
 *                Battery measure     GPIO
 *                -------             ---- 
 *                VBAT  <――――――――――>  35  Battery voltage via 50% voltage divider
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/ttgo-lora32-v1.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         ttgo_lora32_v1
 *                PlatformIO
 *                    board:         ttgo-lora32-v1
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_TTGO_LoRa32_V1
 *                    architecture:  ARDUINO_ARCH_ESP32
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_TTGO_LORA32_V1_H_
#define BSF_TTGO_LORA32_V1_H_

#include "LMIC-node.h"

#define DEVICEID_DEFAULT "ttgo-lora32-v1"  // Default deviceid value

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
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,                      // See remark about LORA_RST above.
    .dio = { /*dio0*/ 26, /*dio1*/ 33, /*dio2*/ 32 }
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
    #error Invalid option: USE_LED. This board has no onboard user LED.
    // EasyLed led(<external LED GPIO>, EasyLed::ActiveLevel::Low);
#endif

#ifdef USE_DISPLAY
    // Create U8x8 instance for SSD1306 OLED display (no reset) using hardware I2C.
    U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ U8X8_PIN_NONE, /*scl*/ 15, /*sda*/ 4);
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

            #ifdef USE_DISPLAY
                // Initialize I2C Wire object with GPIO pins the display is connected to.
                // These pins will be remembered and will not change if any library
                // later calls Wire.begin() without parameters.
                Wire.begin(4, 15);
            #endif
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;
    }
    return success;
}


#endif  // BSF_TTGO_LORA32_V1_H_