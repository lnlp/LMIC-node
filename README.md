This repository contains LMIC-node, an example LoRaWAN application for a node for The Things Network.  
Get your node quickly up and running with LMIC-node.

# LMIC-node
[![GitHub release](https://img.shields.io/github/release/lnlp/LMIC-node.svg)](https://github.com/lnlp/LMIC-node/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/lnlp/LMIC-node/v1.0.0.svg)](https://github.com/lnlp/LMIC-node/compare/v1.0.0...main)  
One example to rule them all

## Contents

- [1 Introduction](#1-introduction)
  - [1.1 What does it do?](#11-what-does-it-do)
  - [1.2 Implemented features](#12-implemented-features)
  - [1.3 Requirements](#13-requirements)
- [2 Supported Boards](#2-supported-boards)
  - [2.1 LoRa development boards](#21-lora-development-boards)
  - [2.2 Development boards with external SPI LoRa module](#22-development-boards-with-external-spi-lora-module)
- [3 Details](#3-details)
  - [3.1 Uplink messages](#31-uplink-messages)
  - [3.2 Downlink messages](#32-downlink-messages)
    - [3.2.1  Reset-counter downlink command](#321--reset-counter-downlink-command)
  - [3.3 Status information](#33-status-information)
    - [3.3.1 Serial port and display](#331-serial-port-and-display)
    - [3.3.2 LED](#332-led)
  - [3.4 User modifiable code](#34-user-modifiable-code)
  - [3.5 Board-id](#35-board-id)
  - [3.6 Device-id](#36-device-id)
  - [3.7 platformio.ini](#37-platformioini)
  - [3.8 lorawan-keys.h](#38-lorawan-keysh)
  - [3.9 Board Support Files (BSF)](#39-board-support-files-bsf)
  - [3.10 Payload formatters](#310-payload-formatters)
    - [3.10.1 Uplink decoder](#3101-uplink-decoder)
- [4 Settings](#4-settings)
  - [4.1 Board selection](#41-board-selection)
  - [4.2 Common settings](#42-common-settings)
  - [4.3 LoRaWAN library settings](#43-lorawan-library-settings)
    - [4.3.1 MCCI LoRaWAN LMIC library settings](#431-mcci-lorawan-lmic-library-settings)
    - [4.3.2 IBM LMIC framework settings](#432-ibm-lmic-framework-settings)
  - [4.4 Board specific settings](#44-board-specific-settings)
  - [4.5 Board Support Files](#45-board-support-files)
- [5 Instructions](#5-instructions)
  - [5.1 Select your board](#51-select-your-board)
  - [5.2 Select your LoRaWAN region](#52-select-your-lorawan-region)
  - [5.3 Provide the LoRaWAN keys for your node](#53-provide-the-lorawan-keys-for-your-node)
  - [5.4 Compile and upload](#54-compile-and-upload)
  - [5.5 Add uplink decoder function in TTN Console](#55-add-uplink-decoder-function-in-ttn-console)
- [6 Some tips](#6-some-tips)
  - [6.1 Serial Monitor](#61-serial-monitor)
  - [6.2 Antenna](#62-antenna)
  - [6.3 Distance to gateway](#63-distance-to-gateway)
- [7 Additional information](#7-additional-information)
  - [7.1 External libraries](#71-external-libraries)
  - [7.2 Not yet tested](#72-not-yet-tested)
- [8 Example Output](#8-example-output)
  - [8.1 Serial Monitor](#81-serial-monitor)
  - [8.2 Display](#82-display)

## 1 Introduction

LMIC-node is an example LoRaWAN application for a node that can be used with [The Things Network](https://www.thethingsnetwork.org/). It demonstrates how to send uplink messages,  how to receive downlink messages, how to implement a downlink command and it provides useful status information. With LMIC-node it is easy to get a working node quickly up and running. LMIC-node supports many popular (LoRa) development boards out of the box. It uses the Arduino framework, the LMIC LoRaWAN library and PlatformIO.

Basic steps to get a node up and running with LMIC-node:

- Select a supported board in `platformio.ini`.
- Select your LoRaWAN region in `platformio.ini`.
- Provide the LoRaWAN keys for your node (end device) in `lorawan-keys.h`.
- Compile and upload the firmware and you're ready to go!

### 1.1 What does it do?

- LMIC-node uses a counter to simulate a real sensor. The counter gets automatically incremented each time its value is read. The counter value is periodically (`DO_WORK_INTERVAL_SECONDS`) read and then transmitted via an uplink message to The Things Network (TTN).
- In addition LMIC-node also implements a downlink command to reset the counter. The command is sent to the node via a downlink message. When the 'reset counter' command is received by the node it will reset the counter value.
- While all this is happening LMIC-node outputs status information to the serial port for viewing on the serial monitor and also outputs information to the display (if present). The status information will show time, events (e.g. EV_JOINED, EV_TXCOMPLETE), uplink and downlink framecounters, RSSI and SNR of received downlink messages and when a downlink contains data the data will be displayed as bytes.
- The uplink messages can be viewed in the TTN Console. It is also possible to send the 'reset counter' downlink message from the console to the node. The effect of resetting the counter can be watched on the console as arriving uplink messages will show the new counter value.  

Once the node is up and running you can start to explore and customize the source code to your own needs, e.g. add support for sensors. It is always easier to start with something that already works and then continue from there. Have fun!

### 1.2 Implemented features

- Send uplink messages.
- Receive downlink messages.
- Implements a downlink command.
- Provide detailed status feedback via serial port, LED and OLED display.  
*Each of these output channels can be separately enabled/disabled (e.g. to save memory or power).*
- Supports both OTAA and ABP activation. Simply switch via configuration option.
- LoRaWAN keys are placed in a separate file: `lorawan-keys.h`.
  - Protection against accidentally uploading LoRaWAN keys to public Git(Hub) repositories.
  - Easy to swap LoRaWAN keys for testing or for use with multiple devices.
- Requires only some configuration, but no programming or modification of source code to get it up and running.
- Configuration is done in a single project configuration file: `platformio.ini`.
- User modifiable code is clearly marked in the source code.
- Supports two different LMIC libraries: MCCI LoRaWAN LMIC library and IBM LMIC framework.
- Support for many popular (LoRa) development boards.
- Cross-platform, tested on STM32, SAMD21, ESP32, ESP8266, ATmega32u4 and ATmega328 boards.
- Hardware dependencies are handled in separate Board Support Files (BSF).
- Built-in 'wait for serial port ready' with adjustable timeout and countdown visible on display.  
*Useful when using the serial monitor with boards with MCU with integrated USB support.*
- Abstraction of the serial port so code can print to `serial` without needing to know if it must print to `Serial` or `SerialUSB`.
- Use correct GPIO pins for onboard LED, display, LoRa hardware, I2C and SPI ports even if these are incorrectly defined in the BSP.
- Avoid hardware conflicts i.e. GPIO's shared between onboard LED, I2C, SPI and/or Vext.
- Explicitly initialize I2C and SPI interfaces with correct pins if default pins are incorrectly defined in the BSP.
- When using LMIC debugging, output is automatically routed to the correct serial port.
*No need to set the `LMIC_PRINTF_TO` parameter to `Serial` or `SerialUSB` manually.*


### 1.3 Requirements

The following is required to use LMIC-node:

- **A supported** LoRa **development board** or development board with external SX127x or RFM9x SPI LoRa module.
- **A computer with [PlatformIO](https://platformio.org/) installed**. PlatformIO is used instead of Arduino IDE because more flexible, more powerful and it better supports cross-platform development. For installation see [PlatformIO installation instructions](https://docs.platformio.org/en/latest/integration/ide/vscode.html).
- **Internet connection**. PlatformIO will automatically download and install packages (Arduino cores, toolchains and libraries) as needed.  
If PlatformIO is freshly installed the downloading may take some time. Once installed it will be possible to work offline.
- **USB cable**. For boards without onboard USB also a **USB to serial adapter** is needed (for STM32 optionally a STLink programmer).
- **Wiring**: Some LoRa development boards require manual wiring of LoRa DIO1 (see table below). When using a development board with external LoRa module then everything must be manually wired. For wiring details see the board's Board Support File (BSF).
- **Node registration**: A node (end device) must be created/registered in The Things Network (TTN) Console before it can be used. The LoRaWAN keys for the device must be copied from the TTN Console to file `lorawan-keys.h`. Registration is not further described here. See the [TTN documentation](https://www.thethingsnetwork.org/docs/index.html) and visit the [TTN Forum](https://www.thethingsnetwork.org/forum/) for more information.
- **Skills**: You should already be familiar with compiling and uploading basic Arduino sketches to your board and how to use a serial monitor.

**Display**:  
LMIC-node supports the following display type: SSD1306 128x64 I2C OLED. These are the displays used on Heltec Wifi LoRa 32 and TTGO LoRa32 boards. When connecting an external display use this type. Use of other I2C displays is possible but requires modification of LMIC-node which is not further described here.

Not yet a requirement but this document assumes that you will be using The Things Network V3 as The Things Network V2 will cease operation by the end of 2021 and should not be used for new development.

## 2 Supported Boards

The following tables list the boards currently supported by LMIC-node.

Explanation of columns: MCU: microcontroller. Wiring required: yes means manual wiring of DIO1 is required. USB: has onboard USB. LED: yes: has onboard LED *and* is usable. Display: yes means has onboard display. Board-id: board identifier as used by LMIC-node.
### 2.1 LoRa development boards

The following LoRa development boards have onboard LoRa support. Most have onboard USB that supports automatic firmware upload and serial over USB for serial monitoring. Some boards require manual wiring of the LoRa DIO1 port. For boards without onboard display an external display can be optionally connected. For details and wiring instructions see the board's BSP.

| Board name                        | MCU            | Wiring required | USB    | LED     | Display | Board-id |
| ----------                        | ---            | ---             | ---    | ---     | ---     | --- |  
| Adafruit Feather M0 RFMx LoRa     | SAMD21         | yes _\*1_       | yes    | yes     | no      | adafruit_feather_m0_lora |
| ST B-L072Z-LRWAN1 Discovery kit   | STM32L072CZ    | no              | yes    | yes     | no      | discovery_l072z_lrwan1 |
| Heltec WiFi LoRa 32 V2            | ESP32          | no              | yes    | yes     | yes     | heltec_wifilora32_v2 |
| Heltec WiFi LoRa 32 (1.x)         | ESP32          | no              | yes    | yes     | yes     | heltec_wifilora32 |
| Heltec Wireless Stick             | ESP32          | no              | yes    | yes     | yes _\*5_ | heltec_wireless_stick |
| Heltec Wireless Stick Lite        | ESP32          | no              | yes    | yes     | no      | heltec_wireless_stick_lite |
| Pycom LoPy4                       | ESP32          | no              | no _\*4_ | no      | no      | lopy4 |
| BSFrance LoRa32u4 II              | ATmega32u4     | yes _\*2_       | yes    | yes     | no      | lora32u4II |
| TTGO LoRa32 V1.3                  | ESP32          | no              | yes    | unknown | yes     | ttgo_lora32_v1 |
| TTGO LoRa32 V2.0                  | ESP32          | yes _\*3_       | yes    | no      | yes     | ttgo_lora32_v2 |
| TTGO LoRa32 V2.1.6                | ESP32          | no              | yes    | no      | yes     | ttgo_lora32_v21 |
| TTGO T-Beam V0.7                  | ESP32          | no              | yes    | yes     | no      | ttgo_tbeam |
| TTGO T-Beam V1.0                  | ESP32          | no              | yes    | yes     | no      | ttgo_tbeam_v1 |

_\*1_: DIO1 must be manually wired to GPIO5.  
_\*2_: For versions 1.0, 1.1 and 1.2 DIO1 must be manually wired to GPIO5 (version 1.3 is already wired on the PCB).  
_\*3_: DIO1 must be manually wired to GPIO33.  
_\*4_: Requires USB to Serial adapter or Pycom Expansion Board which is explained further below.  
_\*5_: Display (64x32) not supported by LMIC-node because resolution is too small.

### 2.2 Development boards with external SPI LoRa module

The following development boards require an external SX127x or RFM9x SPI LoRa module.
Most boards have onboard USB that supports automatic firmware upload and serial over USB for serial monitoring.
An external display can be optionally connected. For details and wiring instructions see the board's BSP.

| Board name                        | MCU            | Wiring required | USB     | LED | Display | Board-id |
| ----------                        | ---            | ---             | ---     | --- | ---     | -------- |
| Black Pill STM32F103C8 128k       | STM32F103C8T6  | yes             | yes _\*6_ | yes | no      | blackpill_f103c8_128k |
| Black Pill STM32F103C8 64k        | STM32F103C8T6  | yes             | yes _\*6_ | yes | no      | blackill_f103c8 |
| Blue Pill STM32F103C8 128k        | STM32F103C8T6  | yes             | yes _\*6_ | yes | no      | bluepill_f103c8_128k |
| Blue Pill STM32F103C8 64k         | STM32F103C8T6  | yes             | yes _\*6_ | yes | no      | bluepill_f103c8 |
| Lolin D32 Pro                     | ESP32          | yes             | yes     | yes | no      | lolin_d32_pro |
| Lolin D32                         | ESP32          | yes             | yes     | yes | no      | lolin_d32 |
| Lolin32                           | ESP32          | yes             | yes     | yes | no      | lolin32 |
| NodeMCU-32S                       | ESP32          | yes             | yes     | yes | no      | nodemcu_32 |
| NodeMCU V2 (aka v1.0)             | ESP8266        | yes             | yes     | yes | no      | nodemcuv2 |
| Arduino Pro Mini (ATmega328 8mHz) | ATmega328      | yes             | yes     | no  | no      | pro8mhzatmega328 |
| Arduino Zero (USB)                | SAMD21         | yes             | yes     | yes | no      | zerousb |

_\*6_: These boards have onboard USB but by default do not support firmware upload over USB or serial over USB. For upload use a STLink programmer or USB to serial adapter.


## 3 Details


### 3.1 Uplink messages

The counter is implemented as an unsigned 16 bit integer.
The uplink payload consists of the counter value, 2 bytes in msb format (most significant byte first).
The frame port number used for uplink messages is 10.
Why 10? Because it shows that other port numbers than the default number 1 can be used.


### 3.2 Downlink messages

There are two types of downlink messages. Downlink messages containing user data and downlink messages containing MAC commands. MAC commands are sent by the network server to set or query network related settings.

When a downlink message is received its RSSI and SNR values will be displayed as well as the port number. If the port number is 0 and no data is shown then a MAC command was received.

If the port number is greater than 0 and user data was received the data will be displayed as a sequence of byte values. Contents of downlink data will only be output to the serial port and not to the display because the display is too small to fit all information on a single screen.

#### 3.2.1  Reset-counter downlink command

The reset-counter downlink uses 100 as frame port number.
The reset command is represented by a single byte with hex value 0xC0 (for Counter 0).
When a downlink message is received on port 100, the length of the data is 1 byte and the value is 0xC0 then the `resetCounter()` will be called and the counter will be reset to 0. If the received payload data is longer than a single byte, the reset-counter command will not be performed.
### 3.3 Status information

The following status information is shown:

#### 3.3.1 Serial port and display

At the start:

- Timeout countdown value while waiting for serial port to become ready is shown on the display.  
*This value is only shown for boards where `WAITFOR_SERIAL_SECONDS_DEFAULT` is defined in the BSF with a value unequal to 0.*

In the header:

- Program title "LMIC-node".
- Device-id.
- DoWork job time interval (DO_WORK_INTERVAL_SECONDS).
- If OTAA or ABP activation is used *(serial port only)*.

Continuously:

- During transmission of uplink and downlink messages a transmit symbol will be shown in the top-right of the display.
- A notification when the doWork job is started.
- A notification when the counter is read, with its value.
- A notification "packet queued" when an uplink message is scheduled.
- A notification if a packet cannot be scheduled because transmission or reception of another message is still pending.
- An error message if scheduling of an uplink message failed.
- Events generated by the LMIC library (e.g. EV_JOINED, EV_TXCOMPLETE).
- When event EV_TXCOMPLETE is received the uplink and downlink framecounters are printed.
- For each downlink message the following will be shown:
  - Message that downlink was received
  - RSSI and SNR values
  - Frame port number
  - If data was received the length is shown
  - If data was received the data is shown as a sequence of byte vales *(serial port only)*
  - Message if reset-counter command was received
- Notification when counter is reset.

For events and notification a timestamp (`ostime`) will be shown.

#### 3.3.2 LED

During transmission of uplink and downlink messages the LED will be on and on other moments it will be off (similar to the transmit symbol on the display).

### 3.4 User modifiable code

LMIC-node will work out of the box without having to do any programming or modifying of source code. However, LMIC-node will only do a few tricks: Send counter value via uplink messages, handle reset-counter downlink command and show detailed status information.

To make LMIC-node do other, more useful things e.g. reading values from a temperature and humidity sensor or from a water-level sensor and sending these via uplink messages, requires modifying and extending the source code.

Most of the source code is boiler plate code that does not need to be changed.
Code for things like adding support for sensors or implement other downlink commands is called *user code*. Two sections in the source code are marked as *user code*. Try to put your user code in these sections (this will prevent accidentally messing things up).

If you are aware of what you are doing you are of course free to change every single line of code to your needs, but if this is new to you it might be safer to restrict modifications to the user code area's.

Reading sensors (etc), preparing uplink payload and scheduling an uplink message for transmission can be done in function `processWork()`. Handling of downlink messages and adding your own downlink commands can be done in function `processDownlink()`.

The user code sections are marked as follows:

```cpp
//  █ █ █▀▀ █▀▀ █▀▄   █▀▀ █▀█ █▀▄ █▀▀   █▀▄ █▀▀ █▀▀ ▀█▀ █▀█
//  █ █ ▀▀█ █▀▀ █▀▄   █   █ █ █ █ █▀▀   █▀▄ █▀▀ █ █  █  █ █
//  ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀   ▀▀▀ ▀▀▀ ▀▀  ▀▀▀   ▀▀  ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀


const uint16_t payloadBufferLength = 4;    // Adjust to fit max payload length


//  █ █ █▀▀ █▀▀ █▀▄   █▀▀ █▀█ █▀▄ █▀▀   █▀▀ █▀█ █▀▄
//  █ █ ▀▀█ █▀▀ █▀▄   █   █ █ █ █ █▀▀   █▀▀ █ █ █ █
//  ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀   ▀▀▀ ▀▀▀ ▀▀  ▀▀▀   ▀▀▀ ▀ ▀ ▀▀    
```

### 3.5 Board-id

LMIC-node uses a _board-id_ to identify a specific type of board. _board-id_ is similar to PlatformIO's _board_ but latter is limited to BSP's defined in Arduino cores. Unfortunately there does not exist a dedicated BSP for each board supported by LMIC-node. Therefore LMIC-node defines its own board identifiers.

A Board Support Package (BSP) provides support for a specific board and provides standard pin definitions for a board. BSP's are part of an Arduino core, which contains a BSP for each supported board.

_board-id_ is kept identical or as similar as possible to PlatformIO's _board_. For simplicity and consistency _board-id_ only uses underscores and lowercase characters (e.g. heltec_wifi_lora_32_v2 and ttgo_lora32_v2) while PlatformIO's _board_ uses both hyphens and underscores and mixed case characters (e.g. heltec_wifi_lora_32_V2 and ttgo-lora32-v2). Where needed _board-id_ adds a version suffix (if a matching BSP does not yet exist) e.g. ttgo_tbeam_v1 (v1 stands for version 1.0), because there is only one single T-Beam BSP (and one PlatformIO board) for the T-Beam while there are important hardware differences between versions 0.x and 1.0 of the T-Beam board.
### 3.6 Device-id

LMIC-node uses a device-id to identify a device. The device-id is used for display purposes in status information that is output to the serial port and display. Device-id's allow different devices to be easily recognized when they have different device-id's, because their device-id is shown on the display (if present) and/or serial monitor (serial port). The length of a device-id should be limited to max 16 characters long, otherwise it will not fit on a display.

When creating a device in the TTN console one must specify a unique device identifier for the device. The device-id used in LMIC-node is only used for display purposes (and is not the same as the device identifier in the TTN Console) but it is useful to use the same device-id for LMIC-node as the device identifier in the TTN console (or at least make it similar). That will make it easier to recognize traffic in the TTN console because the device identifier displayed on the console and the device-id displayed on the node's display (or serial monitor) will match.

In the BSF a default device-id (DEVICEID_DEFAULT) is defined per board type. The default device-id can be overriden by defining DEVICEID in lorawan-keys.h, or defining ABP_DEVICEID in lorawan-keys.h. Latter will be used only if DEVICEID is not defined and ABP activation is used.

lorawan-keys.h can contain both the keys used OTAA activation as well as the keys for ABP activation. These keys are different and have different names. Only the keys for the used actvation type (OTAA or ABP) need to be specified.

Tip: For testing purposes it is possible to create two different devices in the TTN console for the same hardware device, one for OTAA activation and the other for ABP activation. Both sets of keys can be added to lorawan-keys.h and for both a different device-id can be added to lorawan-keys.h. This way a single hardware device can be used for both OTAA and ABP (only one at a time). All that to needs to be done to switch the device from OTAA to ABP is to enable the `-D ABP_ACTIVATION` setting in the `[common]` section in platformio.ini (and vice versa) and then recompile and upload the firmware.


### 3.7 platformio.ini

`platformio.ini` is the project configuration file. This file contains all configuration settings like program options, names of libraries and build options. It contains below sections:

- **[platformio]**  
  Contains a list of board-id's ('environment definitions').  
  This is used to select a board (by uncommenting the line with the board-id to be selected).

- **[common]**  
  Contains settings used for all boards.

- **[mcci_lmic]**  
  Contains MCCI LoRaWAN LMIC library specific settings.  
  MCCI LoRaWAN LMIC library is used for most boards except for boards with 8-bit AVR MCU.

- **[classic_lmic]**  
  Contains IBM LMIC framework library specific settings.  
  This is only used for boards with 8-bit AVR MCU.

- **[env:_\<board-id\>_]**  
  Board specific sections (called _Environments_ in PlatformIO terminology). E.g. [env:lolin32].  
  A board specific section contains settings that are specific for one type of board.
  It contain settings like which LMIC library to use and program options like USE_SERIAL, USE_LED and USE_DISPLAY.  

Comments in platformio.ini start with a semicolon ( ; ) character. To uncomment a line  remove the semicolon prefix. To comment a line add a semicolon prefix.

### 3.8 lorawan-keys.h

File `lorawan-keys.h` contains the LoRaWAN keys for a node. The keys are placed in a separate file for:

- Protection against accidentally uploading LoRaWAN keys to public Git(Hub) repositories.
- Prevent that source code needs to be modified for changing keys.
- Make it easy to swap LoRaWAN keys for testing purposes or for use with multiple devices.

The keys use three different formats (lsb, msb and uint32_t). lsb: least significant byte, msb: most significant byte. The TTN console provides options to copy keys in different formats. Use the correct formats as shown in below extract from `lorawan-keys_example.h`:

```cpp
// Optional: If DEVICEID is defined it will be used instead of the default defined in the BSF.
// #define DEVICEID "<deviceid>"

// Keys required for OTAA activation:

// End-device Identifier (u1_t[8]) in lsb format
#define OTAA_DEVEUI 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

// Application Identifier (u1_t[8]) in lsb format
#define OTAA_APPEUI 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

// Application Key (u1_t[16]) in msb format
#define OTAA_APPKEY 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00


// -----------------------------------------------------------------------------

// Optional: If DEVICEID_ABP is defined it will be used for ABP instead of the default defined in the BSF.
// #define ABP_DEVICEID "<abp-deviceid>"

// Keys required for ABP activation:

// End-device Address (u4_t) in uint32_t format. 
// Note: The value must start with 0x (current version of TTN Console does not provide this).
#define ABP_DEVADDR 0x00000000

// Network Session Key (u1_t[16]) in msb format
#define ABP_NWKSKEY 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

// Application Session K (u1_t[16]) in msb format
#define ABP_APPSKEY 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

```

The `lorawan-keys.h` file is located in folder `keyfiles`.

All files with name pattern `*lorawan-keys.h` and all files in folder `keyfiles` (except for file `lorawan-keys_example.h`) are excluded from the Git(Hub) repository (defined in file `.gitignore`) to prevent that LoRaWAN keys will be accidentally committed to a public repository.

`loarawan-keys_example.h` is included as example for `lorawan-keys.h`.

### 3.9 Board Support Files (BSF)

A Board Support File (BSF) isolates hardware dependencies for a board into a single file: the BSF. There is a separate BSF per board type.

A BSF acts like a mini Board Support Package (BSP). A BSF adds functionality on top of the BSP and can also contain corrections if pins are incorrectly defined in the BSP.

A BSF has the same name as the board-id and is suffixed with `.h` extension. BSF's are located in the `src/boards` folder.

A BSF contains important information about a board (including connection/wiring details) and provides the following functionality:

- Defines the default device-id.
- Defines 'wait for serial port' timeout default value (if needed).
- Defines if LMIC timing should be relaxed (if needed).
- Defines a reference named `serial` that points to the correct serial port (Serial or SerialUSB).
- Defines and initializes the `led` object.
- Defines the `display` object.
- Defines the `lmic_pins` pin mapping structure used by the LMIC library.
- Contains the `boardInit()` function.
- Uses correct GPIO pins for onboard LED, display, LoRa hardware, I2C and SPI ports even if these are incorrectly defined in the BSP in the Arduino core.
- Avoids hardware conflicts i.e. GPIO's shared between onboard LED, I2C, SPI and/or Vext.

The `boardInit(initType)` function provides a generic mechanism for performing custom hardware initialization. It is called twice during setup(). The `initType` parameter indicates which initialization code must be executed.

- `InitType::Hardware`:
  - Explicitly initializes I2C and SPI interfaces with correct pins if default pins are incorrectly defined in the BSP (if needed).
  - Special hardware initialization (if needed, e.g. initializing T-Beam v1.x power management chip).

- `InitType::PostInitSerial`:
  - Inserts a delay after serial port initialization to prevent losing the first output printed to the serial port (if needed).

Example of the `boardInit()` function for a board that does not require custom hardware initialization:
```cpp
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
```

### 3.10 Payload formatters

Payload formatter functions are located in the `payload-formatters` folder.
#### 3.10.1 Uplink decoder

LMIC-node comes with a JavaScript payload formatter function for decoding the uplink messages so the counter value gets displayed in 'Live data' on the TTN Console. The `decodeUplink()` function can be found in file `lmic-node-uplink-formatters.js` located in the `payload-formatters` folder and is shown below:

```js
function decodeUplink(input) {
    var data = {};
    var warnings = [];

    if (input.fPort == 10) {
        data.counter = (input.bytes[0] << 8) + input.bytes[1];
    }
    else {
        warnings.push("Unsupported fPort");
    }
    return {
        data: data,
        warnings: warnings
    };
}
```

In the TTN Console this function should be added to the device (or application) as uplink payload formatter function.
When this function is installed, the counter value will become visible in uplink messages in 'Live data' on the TTN Console.

## 4 Settings

### 4.1 Board selection

In platformio.ini the board must be selected. Select only a single board by uncommenting the line with its board-id. Comment the line starting with "\<platformio.ini board selector guard\>". The guard meant to cause a meaningful error message if no board is selected.

Warning: If no board is selected (and the guard is disabled) PlatformIO will compile ALL listed boards.

```ini
[platformio]
default_envs = 
    <platformio.ini board selector guard> Comment this line and uncomment one board-id below:

    ; LoRa development boards with integrated LoRa support:

    ; Board-id                            Board name
    ;---------                            ----------
    ; adafruit_feather_m0_lora          ; Adafruit Feather M0 LoRa
    ; disco_l072cz_lrwan1               ; Discovery B-L072Z-LRWAN1
    ; heltec_wifi_lora_32_v2            ; Heltec Wifi LoRa 32 V2
    ; heltec_wifi_lora_32               ; Heltec Wifi LoRa 32
    ; heltec_wireless_stick_lite        ; Heltec Wireless Stick Lite
    ; heltec_wireless_stick             ; Heltec Wireless Stick
    ; lopy4                             ; Pycom Lopy4
    ; lora32u4II                        ; BSFrance LoRa32u4 II
    ; ttgo_lora32_v1                    ; TTGO LoRa32 V1.3
    ; ttgo_lora32_v2                    ; TTGO LoRa32 V2.0
    ; ttgo_lora32_v21                   ; TTGO LoRa32 V2.1.6
    ; ttgo_tbeam                        ; TTGO T-Beam V0.7
    ; ttgo_tbeam_v1                     ; TTGO T-Beam V1.0

    ; Development boards that require an external SPI LoRa module:

    ; Board-id                            Board name
    ;---------                            ----------    
    ; blackpill_f103c8_128k             ; Black Pill 128k
    ; blackpill_f103c8                  ; Black Pill  64k
    ; bluepill_f103c8_128k              ; Blue Pill 128k
    ; bluepill_f103c8                   ; Blue Pill  64k
    ; lolin_d32_pro                     ; Lolin D32 Pro
    ; lolin_d32                         ; Lolin D32
    ; lolin32                           ; Lolin32
    ; nodemcu_32s                       ; NodeMCU-32S
    ; nodemcuv2                         ; NodeMCU V2
    ; pro8mhzatmega328                  ; Arduino Pro Mini 3.3V 8Mhz
    ; zerousb                           ; Arduino Zero (USB)
```

### 4.2 Common settings

```ini
[common]
monitor_speed = 115200

build_flags =
    -D DO_WORK_INTERVAL_SECONDS=60

    ; -D ABP_ACTIVATION             ; Use ABP instead of OTAA activation
    ;
    ; -D WAITFOR_SERIAL_SECONDS=10  ; Can be used to override the default value (10)
                                    ; Only used for boards with default set to != 0 in BSF
lib_deps =
    olikraus/U8g2                   ; OLED display library
    lnlp/EasyLed                    ; LED library
```

**monitor_speed**  
Sets the monitor speed for the serial port. 115200 bps should be fine for most purposes and does not need to be changed.

**DO_WORK_INTERVAL_SECONDS**  
Defines the interval for when the doWork job runs where the actual work is done.
Be aware that this is also the interval that uplink messages will be sent. The interval should not exceed TTN's fair use policy and regulatory constraints.

**ABP_ACTIVATION**  
If enabled will use ABP activation instead of OTAA activation (default).

**WAITFOR_SERIAL_SECONDS**  
Can be used to overrule the default value defined in BSP for testing purposes but normally not needed to change this. This setting only has effect for boards where a default value (>0) is already defined and will not have effect for other boards.
'Wait for serial' only is useful when USB functionality is provided by the MCU.

A 'wait for serial' delay of 10 seconds is configured by default in boards where USB support is integrated into the MCU (instead of using onboard USB to serial).
Waiting until the serial (over USB) port is actually ready (via 'wait for serial') prevents the first output printed to the serial port getting lost.

A positive value will wait with a countdown delay (visible on display) for the number of seconds specified. A value of 0 will not wait. A value of -1 will wait indefinitely.

Be aware that the serial port must not only be ready but also a serial monitor needs to be connected. The countdown gives some time to start the serial monitor if not already started. A timeout value prevents that the node waits indefinitely if not connected to a computer. It will continue when the countdown ends.

### 4.3 LoRaWAN library settings

#### 4.3.1 MCCI LoRaWAN LMIC library settings

```ini
[mcci_lmic]
; LMIC-node was tested with MCCI LoRaWAN LMIC library v3.3.0.
; Some changes have been announced for future versions of the MCCI library
; which may be incompatible with LMIC-node. In case of problems just
; use mcci-catena/MCCI LoRaWAN LMIC library@3.3.0 below which will
; explicitly use v3.3.0 of the library.
; Perform PlatformIO: Clean after changing library version and
; in case of issues remove the old version from .pio/libdeps/*.

; If LMIC_DEBUG_LEVEL is set to value > 0 then LMIC_PRINTF_TO will 
; be automatically set to serial (do not set it explicitly).  

lib_deps =
    ; Only ONE of below LMIC libraries should be enabled.
    mcci-catena/MCCI LoRaWAN LMIC library           ; MCCI LMIC library (latest release)
    ; mcci-catena/MCCI LoRaWAN LMIC library@3.3.0   ; MCCI LMIC library v3.3.0

build_flags =
    ; Use platformio.ini for settings instead lmic_project_config.h.
    -D ARDUINO_LMIC_PROJECT_CONFIG_H_SUPPRESS

    ; Ping and beacons not supported for class A, disable to save memory.
    -D DISABLE_PING
    -D DISABLE_BEACONS

    ; If LMIC_DEBUG_LEVEL is set to value > 0 then LMIC_PRINTF_TO will 
    ; be automatically set to serial (do not set it explicitly).
    ; -D LMIC_DEBUG_LEVEL=1            ; 0, 1 or 2
    
    ; -D CFG_sx1272_radio=1            ; Use for SX1272 radio
    -D CFG_sx1276_radio=1              ; Use for SX1276 radio
    -D USE_ORIGINAL_AES                ; Faster but larger, see docs
    ; -D LMIC_USE_INTERRUPTS           ; Not tested or supported on many platforms
    ; -D LMIC_ENABLE_DeviceTimeReq=1   ; Network time support

    ; --- Regional settings -----
    ; Enable only one of the following regions:    
    ; -D CFG_as923=1
    ; -D CFG_as923jp=1   
    ; -D CFG_au915=1
    ; -D CFG_cn490=1                   ; Not yet supported
    ; -D CFG_cn783=1                   ; Not yet supported
    ; -D CFG_eu433=1                   ; Not yet supported
    -D CFG_eu868=1
    ; -D CFG_in866=1
    ; -D CFG_kr920=1
    ; -D CFG_us915=1
```

**Regional setting**
--- Regional settings --- is where the regional setting needs to be selected.
Uncomment the line with the appropriate region setting (and comment the other region settings). Only one region may be selected. By default CFG_eu868 (Europe) is selected.

**LMIC_DEBUG_LEVEL**  
This can be uncommented to allow debug information from the LMIC library to be printed.
Possible values are 0, 1, 2 and 3 where 0 provides no debugging information and 3 provides the most information.  
Be aware that enabling debug will increase memory requirements.

Other settings are listed for information but are not further explained here.
For more information see the MCCI LoRaWAN LMIC library documentation.


#### 4.3.2 IBM LMIC framework settings

```ini
; IMPORTANT:
; This library was recently deprecated and is no longer maintained.
; It is not fully LoRaWAN compliant (e.g. in handling of MAC commands)
; and is therefore less suitable for use with The Things Network V3.
;
; Region, radio and debug settings CANNOT be changed in platformio.ini.
; They must be configured in file: config.h in the following location:
; .pio/libdeps/<board-id>/IBM LMIC framework/src/lmic
;
; When making changes to config.h: 
; CONFIG.H MUST BE CHANGED FOR EACH BOARD SEPARATELY!
; (By default libraries are installed per project per build config/board.)

; If LMIC_DEBUG_LEVEL is set to value > 0 then LMIC_PRINTF_TO will 
; be automatically set to serial (do not set it explicitly).  

lib_deps =
    matthijskooijman/IBM LMIC framework   ; [Deprecated] Classic LMIC library

build_flags =
    ; DEFAULT VALUES defined in config.h:
    ; CFG_sx1276_radio 1
    ; CFG_eu868 1
    ; LMIC_DEBUG_LEVEL 0 

    ; Ping and beacons not supported for class A, disable to save memory.
    -D DISABLE_PING
    -D DISABLE_BEACONS
```

**Region, radio and debug settings CANNOT be changed in platformio.ini.**  
**When making changes to `config.h` these must be changed separately per board!**
File `config.h` is located in these folders: `.pio/libdeps/<board-id>/IBM LMIC framework/src/lmic`

These settings cannot be changed in platformio.ini because they are 'hardcoded' in config.h which is located in a library source code location.
These settings must be changed separately per board because the way PlatformIO handles libraries (a copy of each library is added for each board).

### 4.4 Board specific settings

All options listed below are specified for each board separately. This is done for flexibility and to provide the optimum configuration for each board out of the box.

Example board section for BSFrance LoRa32u4 II board:

```ini
[env:lora32u4II]
; BSFrance LoRa32u4 II (ATmega32u4)
; Depending on the version this board may require additional wiring for DIO1.
; lmic-node will not work with MCCI LMIC for this board because not enough memory.
; Therefore Classic LMIC is hardcoded here (independent of setting in [common]).
; No onboard display.
; This is an 8-bit MCU with limited memory, therefore IBM LMIC framework library is used.
platform = atmelavr
board = lora32u4II
framework = arduino
monitor_speed = ${common.monitor_speed}  
lib_deps =
    ${common.lib_deps}  
    ${classic_lmic.lib_deps}
    ; ${mcci_lmic.lib_deps}  ; MCCI LMIC better LoRaWAN compliance but uses more memory
build_flags =
    ${common.build_flags}
    ${classic_lmic.build_flags}
    ; ${mcci_lmic.build_flags}  ; MCCI LMIC better LoRaWAN compliance but uses more memory 
    -D BSFILE=\"boards/lora32u4II.h\"
    -D MONITOR_SPEED=${common.monitor_speed}
    -D USE_SERIAL
    -D USE_LED
    ; -D USE_DISPLAY         ; Requires external I2C OLED display
```

Example board section for Blue Pill board with external LoRa module:

```ini
[env:bluepill_f103c8]
; Bluepill F103C8 (64k) (STMF103C8T6)
; Select the upload protocol to use below.
platform = ststm32
board = bluepill_f103c8
framework = arduino
; upload_protocol = serial
upload_protocol = stlink
; upload_protocol = jlink
monitor_speed = ${common.monitor_speed}    
lib_deps =
    ${common.lib_deps}    
    ${mcci_lmic.lib_deps}
build_flags =
    ${common.build_flags}
    ${mcci_lmic.build_flags} 
    -D BSFILE=\"boards/bluepill_f103c8.h\"
    -D MONITOR_SPEED=${common.monitor_speed}
    -D USE_SERIAL
    -D USE_LED
    ; -D USE_DISPLAY         ; Requires external I2C OLED display
```

By default all USE\_ options for a board are enabled. If the option is not enabled by default then it is either not supported (e.g. due to hardware conflict) or in case of USE_DISPLAY requires an external display to be connected. The USE_DISPLAY option can then be enabled after the display is connected.

Each of these options uses memory. In case of memory constrainted 8-bit boards disabling some of these option may free some memory for other use (e.g. use MCCI LIMIC library or enable LMIC debugging).

**USE_SERIAL**
If enabled status output will be send to the serial port to be viewed on serial monitor.

**USE_LED**
If enabled it will use the onboard LED. The board will be lit during Tx/Rx transmission and is off otherwise. For some boards the onboard LED cannot be used because of a hardware conflict, because the LED type is not supported (e.g. WS2812 RGB LED) or because there is no onboard user LED. If the onboard LED cannot be used then this option is disabled by default and the line will be commented to make this clear.

**USE_DISPLAY**
If enabled status output will be send to the display.

**upload_protocol**
For some boards it may be necessary to change the upload protocol, e.g. for bluepill change it from `stlink` to `serial` if firmware is uploaded via USB to serial adapter instead of using a STLink programmer.

**LMIC library**  
By default all boards with 32-bit MCU are configured to use the MCCI LoRaWAN LMIC library (MCCI LMIC) because this is the library to use. It is the most LoRaWAN compliant LMIC library for Arduino and it is actively maintained.  

 By default all boards with 8-bit MCU are configured to use the IBM LMIC framework library (Classic LMIC). These boards have limited memory capacity and Classic LMIC uses less memory than MCCI LMIC. From LoRaWAN compliance and improved functionality perspective MCCI LMIC is preferred but with MCCI LMIC it is not possible to use all LMIC-node features on these 8-bit boards.

 Two use an LMIC library two entries are required, one for `lib_deps` and one for `build_flags`. For the 8-bit boards these are included for both the Classic LMIC and MCCI LMIC libraries but the ones for MCCI LMIC are disabled (commented).
 To use the MCCI LMIC library instead of the Classic LMIC library, the entries for Classic LMIC must be disabled (commented) and the entries for MCCI LMIC must be enabled.

```ini
lib_deps =
    ${classic_lmic.lib_deps}
    ; ${mcci_lmic.lib_deps}  ; MCCI LMIC better LoRaWAN compliance but uses more memory
build_flags =
    ${classic_lmic.build_flags}
    ; ${mcci_lmic.build_flags}  ; MCCI LMIC better LoRaWAN compliance but uses more memory
```

The other entries in the board sections should stay unchanged.

See PlatformIO documentation for other possible platformio.ini options.

### 4.5 Board Support Files

Each BSF contains the following settings. Values shown here are examples and need not to be defined for each board.

```cpp
#define DEVICEID_DEFAULT "pro-mini"  // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
#define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// Value 30000 was determined empirically.
#define LMIC_CLOCK_ERROR_PPM 30000
```

**DEVICEID_DEFAULT**  
If no `DEVICEID` is defined and no `ABP_DEVICEID` is defined when ABP activation is used then `DEVICEID_DEFAULT` is used.  
There is no need to change this value in the BSP. `DEVICEID` and `ABP_DEVICEID` can be defined in file `lorawan-keys.h` where they are kept nicely together the LoRaWAN keys of the same device.

**WAITFOR_SERIAL_SECONDS_DEFAULT**
Defines the default value used for the 'wait for serial' timeout. This value is only defined in the BSP for boards where USB support is integrated into the MCU. It has no use for other boards.  
If there is need to change this value then don't change the default value in the BSP and change `WAITFOR_SERIAL_SECONDS` in the `[common]` section in `platformio.ini` instead.

**LMIC_CLOCK_ERROR_PPM**
Setting this value will cause LMIC timing to be less strict. This is only needed for some boards (usually 8-bit AVR based boards). This value should normally not be changed especially for 32-bit MCU boards.  
If there are any issues with timing then increasing this value could possibly help. All boards supported by LMIC-node have already been tested and for boards that need it LMIC_CLOCK_ERROR_PPM is already enabled with a value that has proven to be sufficient.

For more information about Clock Error consult the MCCI LoRaWAN LMIC library documentation.
## 5 Instructions

For prerequisites see [1.3 Requirements](#13-requirements)

### 5.1 Select your board

In `platformio.ini`:

**Step 1a**: Select exactly one supported board by uncommenting the line with its board-id and name.

**Step 1b**: Comment the line starting with `<platformio.ini board selector guard>` to disable the guard.


### 5.2 Select your LoRaWAN region

When using **MCCI LMIC** (set as default for 32-bit boards):  
**Step 2:** In platformio.ini select you LoRaWAN region.

When using **Classic LMIC** (set as default for 8-bit boards):  
**Step 2**: If your region is not CFG_eu868, select your region in config.h. See [IBM LMIC framework settings](#432-ibm-lmic-framework).

### 5.3 Provide the LoRaWAN keys for your node

**Step 3a**: If this has not been done before: In the `keyfiles` folder copy file `lorawan-keys_example.h` to `lorawan-keys.h`.

**Step 3b**: In file `lorawan-keys.h` add the LoRaWAN keys for the device that were copied from the TTN Console.

**Step 3c**: Optional: if preferred, set values for `DEVICEID` and `ABP_DEVICEID` in `lorawan-keys.h` (otherwise default value will be used).

### 5.4 Compile and upload

**Step 4a**: Compile the code.

**Step 4b**: Start the serial monitor (if USE_SERIAL is enabled).

**Step 4c**: Upload the code (and manually reset the device if needed).

**Your node should now be up and running.**

The node will be running but to be able to see the counter value in uplink messages displayed on the console, an uplink decoder function needs to be installed (see next step).

### 5.5 Add uplink decoder function in TTN Console

**Step 5**: In the TTN Console, add the `decodeUplink()` JavaScript function as uplink payload formatter to the device. As formatter type select `JavaScript` and the function must be pasted in the field `Formatter parameter`.

The `decodeUplink()` JavaScript function can be found in file `lmic-node-uplink-formatters.js` which is located in folder `payload-formatters`.

When this function is installed, the counter value will be shown in uplink messages in 'Live data' on the TTN Console.

## 6 Some tips

### 6.1 Serial Monitor

If a development board has a MCU with built-in USB support (e.g. ATmega32u4 and SAMD21) and the serial monitor is connected, resetting the board (e.g. with onboard reset button) will temporarily disconnect the serial port and crash the serial monitor. The serial monitor will then have to be manually restarted and the output will be lost. For boards that use a USB to serial adapter (either onboard or external) this problem does not exist.

To prevent issues with the serial monitor it is best to start the serial monitor _before_ uploading a new sketch. After uploading, PlatformIO will automatically switch back from the upload window to the serial monitor and the serial monitor will contine without crashing. If you connect the serial monitor after uploading you will most probably lose the first output, in which case you may be tempted to press the reset button to restart the output, but in case of MCU with built-in USB support this will only crash the serial monitor.

For boards with MCU with built-in USB support LMIC-node by default waits with a 10 seconds timeout for the serial port to become ready so the first output does not get lost. If not yet connected the serial monitor needs to be connected (or restarted) within these 10 seconds. The countdown stops when the serial port becomes ready or when the countdown ends (whichever is first). The value of the countdown period can be changed as needed by enabling and changing the value of (`WAITFOR_SERIAL_SECONDS` in `platformio.ini`). A value 0 will disable the 'wait for serial' countdown, a value of -1 will wait indefinitely and not contiunue until the serial port is ready. The serial port is detected as ready only when a serial monitor is connected.

### 6.2 Antenna

- **The antenna should always be connected when the device is powered!**
Powering the board while the antenna is disconnected may damage the LoRa radio.  

- For best results the antenna should be used in upright (vertical) position.

### 6.3 Distance to gateway

- The distance between the node and a gateway should be 3 meters at minimum. If less than 3 meters this may cause communication issues.

## 7 Additional information

### 7.1 External libraries

The following external libraries are used:

| Library name | Category | Repository URL |
| --- | --- | --- |
| MCCI LoRaWAN LMIC library | LoRaWAN | https://github.com/mcci-catena/arduino-lmic |
| IBM LMIC framework | LoRaWAN | https://github.com/matthijskooijman/arduino-lmic |
| U8g2 | Display | https://github.com/olikraus/u8g2 |
| EasyLed | LED | https://github.com/lnlp/EasyLed |

### 7.2 Not yet tested

All supported boards have been tested except for below boards for which hardware was not available.
These boards should work but some may need some extra attention (see _remarks_ below).  

- Adafruit Feather M0 RFMx LoRa
- Heltec Wireless Stick and Wireless Stick Lite
_It is assumed that the onboard LED is active-high (like the WiFi LoRa 32 boards).
If incorrect then in the BSF file in the led constructor activeLevel should be changed to EasyLed::ActiveLevel::Low)._  
- BSFrance LoRa32u4 II PCB versions 1.1 and 1.3
_PCB versions 1.0 and 1.2 were tested._  
- TTGO LoRa32 V1.3
_It is unknown if this board has an onboard LED and if it is usable. USE_LED is therefore disabled._
- TTGO LoRa32 V2.1.6
Unknown if LED_BUILTIN is active-low or active-high. Assuming active-low just like v2. 
- Lolin D32
_Lolin D32 Pro was tested which is very similar._
- TTGO T-Beam V1.1
_T-Beam V1.1 may work by using board ID ttgo_tbeam_v1_0 but this has not been tested.
It is currently unknown what the differences between V1.1 and V1.0 are._

## 8 Example Output

Below are some examples of status output from serial monitor and display.
### 8.1 Serial Monitor

To be added.
### 8.2 Display

To be added.
<br>

---

Copyright (c) 2021 Leonel Lopes Parente
