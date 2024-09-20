# BitBlade


## Overview

Bit Blade is an MCU-based console game engine designed to efficiently manage graphical elements by offloading graphics processing to dedicated hardware. It employs a modular architecture with distinct components responsible for game logic, graphics rendering, and communication between devices. The engine is compatible with both Windows and targeted microcontrollers, enabling developers to implement and test programs in a Windows environment before deploying them to microcontroller hardware.

The Game Engine utilizes a traditional object-oriented component design and incorporates object pooling techniques similar to Entity Component Systems (ECS) to enhance cache performance and address heap segmentation.


- **BladeGameEngine**: Runs on client devices such as microcontrollers (e.g., Pi Pico). It handles game logic and communicates with BladeGraphics to offload image storage and manipulation tasks.

- **BladeGraphics**: Operates on an STM32H7 microcontroller connected to the main display. It processes graphics commands received from BladeGameEngine to update and render sprites. Additionally, BladeGraphics has a Windows version that facilitates game development by allowing developers to create and test games on a Windows environment before deploying them to the MCU.

## Components

- **BladeConsole**: A static library the user links against in order to develop their game. The resulting executable runs on one of the targeted microcontrollers (e.g., Pi Pico) and handles communication with BladeGraphics as well as loading and running developed games. It sends Gfx command codes via spi to BladeGraphics for loading image data and the manipulation of sprites.

- **BladeGameEngine**: A module of BladeConsole that serves as the core of the game engine, defining all elements the user needs to implement their own game.

- **BladeGraphics**: Operates on an STM32H7 microcontroller connected to the main display. BladeGraphics processes commands received from BladeConsole to load image data and render sprites.



## Communication Protocol

Bit Blade utilizes a communication protocol between BladeConsole and BladeGraphics to manage game objects and sprite instances effectively.

### Current Approach

1. **SpritePrefab Creation**:
   - **BladeConsole** sends a command to **BladeGraphics** to create a `MasterSpriteData` entry.
   - **BladeGraphics** responds with an index indicating the position of the newly created `MasterSpriteData`.

2. **GameObject Instantiation**:
   - **BladeConsole** creates a `GameObject` with an unresolved ID and sends a command to **BladeGraphics** to create a `SpriteInstance`.
   - **BladeGraphics** returns the address of the `SpriteInstance` in its deque, which **BladeConsole** stores for future reference.

3. **Property Updates**:
   - **BladeConsole** sends commands like `SetPosition`, `SetLayer`, `SetVelocity`, `SetImage`, and `SetGravity` to **BladeGraphics**, referencing the specific `SpriteInstance` by its address.



## Getting Started

*Instructions for building and running the project to be added here.*

## License

No license at this time.

---


Copyright [2024] [Gregory Maynard]