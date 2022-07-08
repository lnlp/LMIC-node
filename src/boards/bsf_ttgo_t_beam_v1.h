/*******************************************************************************
 * 
 *  File:         bsf_ttgo_t_beam_v1.h
 * 
 *  Description:  Board Support File for TTGO T-Beam (aka T22) V1.0 and V1.1.
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
 *                No onboard user programmable LED. Onboard GPS not used by LMIC-node.
 * 
 *                This board uses an AXP192 power management chip to power
 *                onboard components and the +3.3V output pin.
 *                The AXP192 must be correctly configured for things to work
 *                (see boardInit() below).
 * 
 *                Connect an optional display according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 *                
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                  GPIO 
 *                ----                  ----
 *                LED                   -          No onboard user LED
 * 
 *                I2C [display]         GPIO
 *                ----                  ----
 *                SDA     <――――――――――>  21  (SDA)
 *                SCL     <――――――――――>  22  (SCL)
 *                RST                    -
 *
 *                SPI/LoRa              GPIO
 *                ---                   ----
 *                MOSI    <――――――――――>  27  (MOSI) (LORA_MOSI)
 *                MISO    <――――――――――>  19  (MISO) (LORA_MISO)
 *                SCK     <――――――――――>   5  (SCK)  (LORA_SCK)
 *                NSS     <――――――――――>  18  (SS)   (LORA_CS)
 *                RST     <――――――――――>  23         (LORA_RST)
 *                DIO0    <――――――――――>  26         (LORA_IO0)
 *                DIO1    <――――――――――>  33         (LORA_IO1)
 *                DIO2    <――――――――――>  32         (LORA_IO2)
 * 
 *                GPS                   GPIO
 *                ---                   ----
 *                RX      <――――――――――>  34  (or 12?) 
 *                TX      <――――――――――>  12  (or 34?)
 *                PPS     <――――――――――>  37
 * 
 *                Power Management      GPIO
 *                -----                 ----
 *                PMU     <――――――――――>  35
 *  
 *                Button switches       GPIO 
 *                ------                ----
 *                USR_SW  <――――――――――>  39  (KEY_BUILTIN)
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/ttgo-t-beam.html
 *
 *  Identifiers:  LMIC-node
 *                    board:         ttgo_tbeam_v1
 *                PlatformIO
 *                    board:         ttgo-t-beam
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_T_Beam
 *                    architecture:  ARDUINO_ARCH_ESP32
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_TTGO_T_BEAM_V1_H_
#define BSF_TTGO_T_BEAM_V1_H_

#include "axp20x.h"
#include "LMIC-node.h"

#define DEVICEID_DEFAULT "ttgo-tbeam-v1"  // Default deviceid value 

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
    .rst = 23,
    .dio = { /*dio0*/ 26, /*dio1*/ 33, /*dio2*/ 32 }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

AXP20X_Class axp;

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif                                        

#ifdef USE_LED
    #error Invalid option: USE_LED. This board has no onboard user LED.
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

            // This board contains an AXP192 power management chip.
            // Power output must be enabled for several components before they can be initialized.
            Wire.begin(SDA, SCL);
            if (axp.begin(Wire, AXP192_SLAVE_ADDRESS) != 0) 
            {
                success = false;
            } 
            else 
            {
                axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
                axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
                axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
                axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
                axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON); 

                // Explicitly set voltages because AXP192 power-on values may be lower.
                axp.setDCDC1Voltage(3300);  // 3.3V pin
                axp.setLDO2Voltage (3300);  // LoRa
                axp.setLDO3Voltage (3300);  // GPS                
            }
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;      
    }
    return success;
}


#endif  // BSF_TTGO_T_BEAM_V1_H_
