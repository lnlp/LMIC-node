/*******************************************************************************
 * 
 *  File:         bsf_lopy4.h
 * 
 *  Description:  Board Support File for Pycom LoPy4.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This Board has no onboard USB and no onboard display.
 *                Optionally an external display can be connected.
 *                Has onboard LED but this is a special WS2812 RGB LED which requires
 *                a separate library and is currently not supported by LMIC-node.
 *
 *                The standard SPI pins defined in the BSP do not match the 
 *                GPIO pins that the SX1276 LoRa chip is connected to.
 *                LMIC uses the default SPI parameters for initializing the SPI bus
 *                which will not work here. Therefore the SPI object is explicitly
 *                initialized with the correct pins (see boardInit() below).
 * 
 *                For firmware upload and serial monitor use a USB to serial adapter.
 *                Lopy4 is made for use with MicroPython and does not provide
 *                automatic upload for C/C++ firmware (and no GPIO0 button).
 *                To put an ESP32 in firmware upload mode: First press the GPIO0 
 *                button, then while keeping it pressed press the reset button, 
 *                then release the reset button and then release the GPIO0 button.
 *                Because the Lopy4 does not have a GPIO0 button, instead of pressing
 *                the GPIO0 button connect a wire from GPIO0 (labeled '2' on the PCB) 
 *                to GND and instead of releasing the GPIO0 button remove the wire again.
 *                Pycom also sells an Expansion board with onboard USB that
 *                supports firmware upload and serial over USB. The upload is not
 *                automatic however and the ESP32 must still be put in upload mode
 *                manually which still requires a wire because it also has no GPIO0 button.
 *                After firmware upload the Lopy4 must be manually reset (with button).
 * 
 *                LoRa DIO0, DIO1 and DIO2 are all wired (via diodes) to the same single GPIO port.
 * 
 *                WARNING: The 3.3V pin is OUTPUT ONLY don't use it to power the board!!
 *                         The board must be powered on pin Vin with +3.5V to +5V.
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
 *                WS2812 <--------->   0  (LED_BUILTIN) WS2812 RGB LED, not regular LED!
 *                                        pin labeled '2' on the PCB.
 * 
 *                I2C [display]       GPIO  
 *                ---                 ---- 
 *                SDA   <――――――――――>  12  (SDA)
 *                SCL   <――――――――――>  13  (SCL)
 * 
 *                SPI                 GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  22  (MOSI)
 *                MISO  <――――――――――>  37  (MISO)
 *                SCK   <――――――――――>  13  (SCK)
 *                SS    <――――――――――>  18  (SS)
 * 
 *                LoRa                GPIO
 *                ----                ----
 *                MOSI  <――――――――――>  27  (LORA_MOSI)
 *                MISO  <――――――――――>  19  (LORA_MISO)
 *                SCK   <――――――――――>   5  (LORA_SCK)
 *                NSS   <――――――――――>  18  (LORA_CS)
 *                RST                  -
 *                RXTX                 -
 *                DIO0  <――――――――――>  23  (LORA_IRQ, LORA_IO0)
 *                DIO1  <――――――――――>  23  (LORA_IRQ, LORA_IO1)
 *                DIO2  <――――――――――>  23  (LORA_IRQ, LORA_IO2)
 * 
 *                Serial              GPIO
 *                ------              ----
 *                RX    <――――――――――>   3  (RX) pin labeled '0' on the PCB.
 *                TX    <――――――――――>   1  (TX) pin labeled '1' on the PCB.
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/lopy4.html
 *
 *  Identifiers:  LMIC-node
 *                    board-id:      lopy4
 *                PlatformIO
 *                    board:         lopy4
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_LoPy4
 *                    architecture:  ARDUINO_ARCH_ESP32
 *
 ******************************************************************************/

#pragma once

#ifndef BSF_LOPY4_H_
#define BSF_LOPY4_H_

#include <SPI.h>
#include "LMIC-node.h"

#define DEVICEID_DEFAULT "lopy4"  // Default deviceid value

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
    .rst =LMIC_UNUSED_PIN,
    .dio = { /*dio0*/ 23, /*dio1*/ 23, /*dio2*/ 23 }
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
    #error "Invalid option: USE_LED. Onboard WS2812 RGB LED is currently not supported."
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

            // Initialize standard SPI object with non-standard SPI pins for LoRa module.
            // These pins will be remembered and will not change if any library
            // later calls SPI.begin() without parameters.
            SPI.begin(5, 19, 27, 18);     
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;  
    }
    return success;
}


#endif  // BSF_LOPY4_H_