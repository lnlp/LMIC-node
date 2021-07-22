/*******************************************************************************
 * 
 *  File:         bsf_disco_l072cz_lrwan1.h
 * 
 *  Function:     Board Support File for ST B-L072Z-LRWAN1 Discovery kit.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by onboard STLink programmer).
 *                It supports automatic upload (via stlink) and serial over USB.
 *                No onboard display. Optionally an external display can be connected.
 * 
 *                The standard SPI pins defined in the BSP do not match the 
 *                GPIO pins that the SX1276 LoRa chip is connected to.
 *                LMIC uses the default SPI parameters for initializing the SPI bus
 *                which will not work here. Therefore the SPI object is explicitly
 *                initialized with the correct pins (see boardInit() below).
 * 
 *                This board requires a workaround to prevent losing the first
 *                output printed to the serial port (see boardInit() below).
 * 
 *                Connect an optional display according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 *                
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----
 *                LED   <――――――――――>  PA5  / 32  (LED_BUILTIN) Useless because PA5 is used for DIO4
 *                RED   <――――――――――>  PB7  /  5  (LED_RED, LED_LD4)
 *                GREEN <――――――――――>  PB5  /  4  (LED_GREEN, LED_LD1) Also used for SS1
 *                BLUE  <――――――――――>  PB6  / 10  (LED_BLUE, LED3)     Also used for SS
 * 
 *                I2C [display]       GPIO
 *                ---                 ----
 *                SDA   <――――――――――>  PB9  / 14  (SDA)
 *                SCL   <――――――――――>  PB8  / 15  (SCL)
 * 
 *                SPI                 GPIO
 *                ---                 ---- 
 *                MOSI  <――――――――――>  PB15 / 11  (MOSI)
 *                MISO  <――――――――――>  PB14 / 12  (MISO)
 *                SCK   <――――――――――>  PB13 / 13  (SCK)
 *                SS    <――――――――――>  PB6  / 10  (SS)
 *                SS1   <――――――――――>  PB5  /  4  (SS1)
 *                SS2   <――――――――――>  PA8  /  7  (SS2)
 *                SS3   <――――――――――>  PA9  /  8  (SS3)
 * 
 *                LoRa                GPIO
 *                ----                ----
 *                MOSI  <――――――――――>  PA7  / 34  (RADIO_MOSI_PORT)
 *                MISO  <――――――――――>  PA6  / 35  (RADIO_MISO_PORT)
 *                SCK   <――――――――――>  PB3  / 36  (RADIO_SCLK_PORT)
 *                NSS   <――――――――――>  PA15 / 37  (RADIO_NSS_PORT)
 *                RST   <――――――――――>  PC0  / 33  (RADIO_RESET_PORT)
 *                RXTX  <――――――――――>  PA1  / 21
 *                DIO0  <――――――――――>  PB4  / 38  (RADIO_DIO_0_PORT)
 *                DIO1  <――――――――――>  PB1  / 39  (RADIO_DIO_1_PORT)
 *                DIO2  <――――――――――>  PB0  / 40  (RADIO_DIO_2_PORT)
 *                DIO3  <――――――――――>  PC13 / 41  (RADIO_DIO_3_PORT)
 *                DIO4  <――――――――――>  PA5  / 32  (RADIO_DIO_4_PORT)
 *                DIO5  <――――――――――>  PA4  / 28  (RADIO_DIO_5_PORT)
 * 
 *                Serial              GPIO
 *                ------              ----
 *                RX    <――――――――――>  PA3  /  0  (PIN_SERIAL_RX)  ST-Link RX
 *                TX    <――――――――――>  PA2  /  1  (PIN_SERIAL_TX)  ST-Link TX
 * 
 *                Button switch       GPIO
 *                ------              ----
 *                USER  <――――――――――>  PB2  /  6  (USER_BTN)
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/ststm32/disco_l072cz_lrwan1.html
 *
 *  Definitions:  LMIC-node
 *                    board-id:      disco_l072cz_lrwan1
 *                PlatformIO
 *                    board:         disco_l072cz_lrwan1
 *                    platform:      ststm32
 *                Arduino
 *                    board:         ARDUINO_DISCO_L072CZ_LRWAN1
 *                    architecture:  ARDUINO_ARCH_STM32
 *
 ******************************************************************************/

#pragma once

#ifndef BSF_DISCO_L072CZ_LRWAN1_H_
#define BSF_DISCO_L072CZ_LRWAN1_H_

#include <SPI.h>
#include "LMIC-node.h"

#define DEVICEID_DEFAULT "l072cz-lrwan1"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
// #define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// Board appears to work better with clock error enabled (value 4000 empirically determined).
#ifndef LMIC_CLOCK_ERROR_PPM
    #define LMIC_CLOCK_ERROR_PPM 4000
#endif   

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = PA15,
    .rxtx = PA1,
    .rst = PC0,
    .dio = { /*dio0*/ PB4, /*dio1*/ PB1, /*dio2*/ PB0 }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 1,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif    

#ifdef USE_LED
    EasyLed led(LED_BLUE, EasyLed::ActiveLevel::High);
#endif

#ifdef USE_DISPLAY
    // Create U8x8 instance for SSD1306 OLED display (no reset) using hardware I2C.
    U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ U8X8_PIN_NONE, /*scl*/ PB8, /*sda*/ PB9);
#endif

#ifndef STM32_POST_INITSERIAL_DELAY_MS
    #define STM32_POST_INITSERIAL_DELAY_MS 1500
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

            // Redefine GPIO pins of default SPI object.
            // These pins will be remembered and will not change if any library
            // later calls SPI.begin() without parameters.
            SPI.setMOSI(PA7);
            SPI.setMISO(PA6);
            SPI.setSCLK(PB3);
            SPI.setSSEL(PA15);
            SPI.begin();      
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.

            // Required workaround:
            // Data printed to the serial port during the first second gets lost.
            // Inserting a sufficient delay will usually fix this.
#ifdef USE_SERIAL
            delay(STM32_POST_INITSERIAL_DELAY_MS);
#endif            
            break;      
    }
    return success;
}


#endif  // BSF_DISCO_L072CZ_LRWAN1_H_