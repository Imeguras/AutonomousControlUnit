# ACUity Shard


## Introduction

ACUity Shard is the firmware "excerpt" used in the acuity board, check its [repository, here](https://github.com/Imeguras/ACUity-Board).

The aim of this "sub-project" is to provide an easy to operate interface between a ROS2 environment and the low level T24-e CAN bus. The firmware is designed to be modular and easy to extend, with the ability to add new features and functionalities with ease. There are however some extra functionalities that worryingly keep growing but thats not to be discussed here.

The board is supposed to be robust, which also applies to its features, unless its cost of implementation and violation of the aforementioned aspects becomes aparent. In which case "lgtmm"(lets get the monolith merged)

## Migrations / Warranty

This project is not **uniform** nor shall you expect any support for a given board/microcontroller as the meme goes:

THE FIRMWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE FIRMWARE.

For more info heres all the microcontroller that some version of this firmware has ran on:

RA6M3
RA6M5
RA8M1
RA8T1(to be tested)

This also includes any firmware features/environments...

## Features

General features:

Runs on Azure ThreadX RTOS. As of right now it uses callbacks and interrupts as its designed to be reactive to outward events.

| Feature                                        | Implementation | Notes                                                                                 |
| ---------------------------------------------- | -------------- | ------------------------------------------------------------------------------------- |
| CAN-BUS 2.0 Driver for the RA Family           | ≈             | Lacks more testing                                                                    |
| CAN FD Driver for the RA Family                | ✔️           | Backwards compatible with CAN 2.0                                                     |
| CAN FD Lite Driver for the RA Family           | x              | The code is nearly identical to FD driver, might<br />get implemented in the future   |
| CAN XL Driver for the RA Family                | x              | Wont-Fix?                                                                             |
| FlexRay Driver                                 | x              | Wont-Fix?                                                                             |
| EtherCat Slave/Master Driver                  | x              | Might get Implemented                                                                 |
| Preprocessed Auto-Omit Can Libraries           | ✔️           | Works for Renesas RA                                                                  |
| Static Ip Ethernet Interface for the RA Family | ≈             | Lacks ability to write/read, its meant to be<br /> used as an abstract/testing class |
| DHCP Ethernet Interface                        | x              | Most likely will not be implemented                                                  |
| ROS 2 Interface (UDP)                          | ✔️           |                                                                                       |
| ROS 2 Interface (TCP)                          | x              | Might get Implemented                                                                 |
| ROS 2 Interface (SCI)                          | x              | Most likely will not be implemented                                                  |
| ROS 2 Interface (USB FS/HS)                    | x              | Most likely will not be implemented                                                  |
| ROS 2 Interface (Can)                          | x              | Most likely will not be implemented                                                  |
| ROS 2 Domain Auto-Discovery                    | x              | Wont-Fix (depends on eProsima                                                         |
| SD card Logging                                | x              | To Be Implemented                                                                     |
| FAT32 FS for SD                                | x              | To Be Implemented                                                                     |
| exFat FS for SD                                | x              | Might get Implemented                                                                |
| ext4 FS for SD                                 | x              | Might get Implemented                                                                 |
| UART debugging mode                            | x              | Most likely will not be implemented                                                  |
| MCUBoot/ Over the Air flashes(Via TCP/IP)      | x              | Might get Implemented                                                                 |
| Zephyr Port                                    | x              | Most likely will not be implemented                                                  |

## This repo might be pulled!

Right now the code has no inherent license issues as of my knowledge, ie its proprietary of the owner, but due to the owner being somewhat lazy it might depend on some payed, proprietary source code, which means it might force the code to become closed once again.
