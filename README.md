# Smart Home IoT Abstract

The Smart Home project integrates hardware and software components to provide users with a seamless and efficient smart home management solution. The system features smart lights, a smart door lock, and a smart fan, all of which can be controlled remotely through a user-friendly mobile application. The application serves as the central interface, allowing users to adjust light brightness, lock and unlock doors via a keypad or app controls, and set fan speeds. Communication between the app and devices is facilitated through an Azure Virtual Machine (VM) server, which processes HTTP control commands and maintains device statuses in real time.

The hardware layer includes an ESP32 microcontroller for each device, enabling autonomous operations and network communication. The smart door lock features a keypad for local passcode input, providing an additional layer of security. The smart fan allows speed adjustments for customized comfort, while the smart lights support dynamic brightness levels for personalized lighting. By leveraging cloud-based infrastructure and efficient task management on microcontrollers, the Smart Crib project ensures reliable, responsive, and secure smart home control, blending convenience with cutting-edge IoT technology.

## Features

- **Smart Lights**: Set the brightness of connected smart lights (0-100).
- **Smart Door Lock**: Lock or unlock the smart door with a simple toggle (Lock/Unlock). Keypad integration to unlock/lock the door lock.
- **Smart Fan**: Adjust the fan speed (Off, Low, Medium, or High) to suit your comfort.
- **Settings**: Settings to change the Smart Door Lock password.

## Built With

- **React Native**: A popular framework for building mobile applications using React.
- **Axios**: For making HTTP requests to the server.
- **@react-navigation**: For implementing tab-based navigation.
- **@expo/vector-icons**: For using Ionicons in the application.

## Prerequisites

- **Node.js**: Make sure you have Node.js installed. [Download Node.js](https://nodejs.org/)
- **npm or yarn**: Installed along with Node.js to manage packages.
- **Expo CLI**: To run the application on a mobile device or emulator. You can install Expo CLI globally using:
- **Python 3.12.0+**

## App Demos
[Smart Light & Lock Demo](https://www.youtube.com/watch?v=xLqYLixEVfE)

[Smart Fan Demo](https://www.youtube.com/shorts/cb8d_wNGdIk)

## Contributors
-  **Don Dang (Project Lead)**: Created the Smart Home React Native App and Final Poster. Scheduled meetings and assigned tasks for each individual and monitored completed tasks and attendance.
-  **Luis Del Rio**: Created Azure server, Smart Light and Smart Doorlock devices. 
-  **Zait Martinez**: Created the hardware and software development for Smart Fan.
