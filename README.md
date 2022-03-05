# Door-Locking-System
Door locking system with two microcontrollers of ATmega16 version.

First Microcontroller: HMI ECU that is responsible for interacting with LCD, keypad, displaying the status of door and alerts and sending keypad pressed keys to Control ECU
using UART.

Second Microcontroller: Control ECU that is responsible for password comparison received from HMI ECU using UART, saving passwords in an external EEPROM using I2C and
 activating Buzzer and motor to open and close door.

![Door Lcoking System](https://user-images.githubusercontent.com/63435727/156897193-874ec3ef-24d8-4824-a8a0-7d7b71318727.png)

