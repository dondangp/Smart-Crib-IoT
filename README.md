# Smart Crib IoT Abstract

The Smart Crib project integrates hardware and software components to provide users with a seamless and efficient smart home management solution. The system features smart lights, a smart door lock, and a smart fan, all of which can be controlled remotely through a user-friendly mobile application. The application serves as the central interface, allowing users to adjust light brightness, lock and unlock doors via a keypad or app controls, and set fan speeds. Communication between the app and devices is facilitated through an Azure Virtual Machine (VM) server, which processes HTTP control commands and maintains device statuses in real time.

The hardware layer includes an ESP32 microcontroller for the smart door lock & light and a Raspberry Pi Pico for the smart fan, enabling autonomous operations and network communication. The smart door lock features a keypad for local passcode input, providing an additional layer of security. The smart fan allows speed adjustments for customized comfort, while the smart lights support dynamic brightness levels for personalized lighting. By leveraging cloud-based infrastructure and efficient task management on microcontrollers, the Smart Crib project ensures reliable, responsive, and secure smart home control, blending convenience with cutting-edge IoT technology.

# Contributors
-  **Don Dang (Project Lead)**: Created the Smart Home React Native App and Final Poster. Scheduled meetings and assigned tasks for each individual and monitored who completed tasks and attended meetings.
-  **Luis Del Rio**: Created Azure server, Smart Light and Smart Doorlock devices. 
-  **Zait Martinez**: Created the hardware, software development, and Printed Circuit Board (PCB) for Smart Fan

*As of December 03, 2024 - This project will be wrapping up to a finish for Demo Day with no more commits to be made.*

## Features

- **Smart Lights**: Set the brightness of connected smart lights (0-100). Motion detection when someone walks nearby it will turn on the lights.
- **Smart Door Lock**: Lock or unlock the smart door with a simple toggle (Lock/Unlock). Keypad integration to unlock/lock the door lock.
- **Smart Fan**: Adjust the fan speed (Off, Cool, or Heat) to suit your comfort. Has a thermostat attached to it to indicate the degree in fahrenheit and also red and blue LED which will turn on to detect cool or heat mode of the fan.
- **Settings**: Settings to change the Smart Door Lock password.

## Built With

- **React Native**: A popular framework for building mobile applications using React.
- **Axios**: For making HTTP requests to the server.
- **@react-navigation**: For implementing tab-based navigation.
- **@expo/vector-icons**: For using Ionicons in the application.
- **ESP-32**: Smart light & lock hardware/software development
- **Raspberry Pi Pico**: Smart Fan & hardware/software development

## Prerequisites

- **Node.js**: Make sure you have Node.js installed. [Download Node.js](https://nodejs.org/)
- **XCode**: Make sure you have XCode installed to use React Native. [Download XCode](https://developer.apple.com/xcode/)
- **npm or yarn**: Installed along with Node.js to manage packages.
- **Expo CLI**: To run the application on a mobile device or emulator.
- ```npm install```
- ``` npm install axios```
- ```npx expo install expo@latest```
- ```npx expo install --fix```
- Note: UTA WiFi Blocks React Native Expo *use alternative WiFi instead*
- **Python 3.12.0+**

## App Demos
[Smart Light & Lock Demo](https://www.youtube.com/watch?v=xLqYLixEVfE)

[Smart Fan Demo](https://www.youtube.com/shorts/cb8d_wNGdIk)

[Final Demo](https://youtu.be/l44uO0imUA0?si=sPAH7cayEWo_ZzFX)



## Demo Images
### Smart Fan Printed Circuit Board
![Smart Fan Printed Circuit Board](https://github.com/user-attachments/assets/38e58b25-7ef9-4a2c-a602-1b57cadaa950)
### Smart Fan
![smartfan](https://github.com/user-attachments/assets/228eeb5b-4c4a-4481-beed-48c52abebc90)
### Smart Door Lock Front
![doorlock_front](https://github.com/user-attachments/assets/7e719720-0bbe-4ef5-94f9-4bee3648675a)
### Smart Door Lock Keypad
![doorlock_keypad](https://github.com/user-attachments/assets/baf48498-03ff-4a5e-af4b-974112406d6c)
### Smart Light (Off)
![smart_light_off](https://github.com/user-attachments/assets/37215d81-a919-42f2-9cb9-87bf2047a19b)
### Smart Light (On)
![smart_light_on](https://github.com/user-attachments/assets/d279b928-8816-485b-9071-0d43e1bdcaa5)


