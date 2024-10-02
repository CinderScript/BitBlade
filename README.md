# BitBlade


## Overview

Bit Blade is an MCU-based console game engine designed to efficiently manage graphical elements by offloading graphics processing to dedicated hardware. It employs a modular architecture with distinct components responsible for game logic, graphics rendering, and communication between devices. The engine is compatible with both Windows and targeted microcontrollers, enabling developers to implement and test programs in a Windows environment before deploying them to microcontroller hardware.

The Game Engine utilizes a traditional object-oriented component design and incorporates object pooling techniques similar to Entity Component Systems (ECS) to enhance cache performance and address heap segmentation.

- **BladeConsole**: Static library the user links against. The resulting executable runs on one of the targeted microcontrollers (e.g., Pi Pico) and handles communication with BladeGraphics as well as loading and running developed games. It sends Gfx command codes via spi to BladeGraphics for loading image data and the manipulation of sprites.

- **BladeGameEngine**: Static library for game logic and building graphics instructions to be handled by BladeConsole. Includes all objects needed for 2D game design.

- **BladeGraphics**: Executable. Operates on an STM32H7 microcontroller connected to the main display. It processes graphics commands received from BladeGameEngine to update and render sprites. Additionally, BladeGraphics has a Windows version that facilitates game development by allowing developers to create and test games on a Windows environment before deploying them to the MCU.



## Getting Started

*Instructions for building and running the project to be added here.*


## Project Files

* [BladeCommon/](BladeCommon)
  * [include/](BladeCommon/include)
    * [BladeConfig.h](BladeCommon/include/BladeConfig.h)
    * [Event.h](BladeCommon/include/Event.h)
    * [GfxLinkCommon.h](BladeCommon/include/GfxLinkCommon.h)
    * [IGfxMessagePacker.h](BladeCommon/include/IGfxMessagePacker.h)
    * [IGfxMessageProducer.h](BladeCommon/include/IGfxMessageProducer.h)
    * [Logging.h](BladeCommon/include/Logging.h)
  * [CMakeLists.txt](BladeCommon/CMakeLists.txt)
* [BladeConsole/](BladeConsole)
  * [include/](BladeConsole/include)
    * [windows/](BladeConsole/include/windows)
      * [ConsoleLink.h](BladeConsole/include/windows/ConsoleLink.h)
    * [BladeConsole.h](BladeConsole/include/BladeConsole.h)
  * [src/](BladeConsole/src)
    * [windows/](BladeConsole/src/windows)
      * [ConsoleLink.cpp](BladeConsole/src/windows/ConsoleLink.cpp)
    * [BladeConsole.cpp](BladeConsole/src/BladeConsole.cpp)
  * [CMakeLists.txt](BladeConsole/CMakeLists.txt)
* [BladeGameEngine/](BladeGameEngine)
  * [components/](BladeGameEngine/components)
    * [include/](BladeGameEngine/components/include)
      * [Sprite.h](BladeGameEngine/components/include/Sprite.h)
      * [Transform.h](BladeGameEngine/components/include/Transform.h)
    * [src/](BladeGameEngine/components/src)
      * [Sprite.cpp](BladeGameEngine/components/src/Sprite.cpp)
      * [Transform.cpp](BladeGameEngine/components/src/Transform.cpp)
  * [include/](BladeGameEngine/include)
    * [BitBladeGame.h](BladeGameEngine/include/BitBladeGame.h)
    * [BladeStart.h](BladeGameEngine/include/BladeStart.h)
    * [Component.h](BladeGameEngine/include/Component.h)
    * [DataCluster.h](BladeGameEngine/include/DataCluster.h)
    * [DataPool.h](BladeGameEngine/include/DataPool.h)
    * [DataPoolMember.h](BladeGameEngine/include/DataPoolMember.h)
    * [DataTypeID.h](BladeGameEngine/include/DataTypeID.h)
    * [GameObject.h](BladeGameEngine/include/GameObject.h)
    * [ImageSource.h](BladeGameEngine/include/ImageSource.h)
    * [Vector2.h](BladeGameEngine/include/Vector2.h)
  * [src/](BladeGameEngine/src)
    * [BitBladeGame.cpp](BladeGameEngine/src/BitBladeGame.cpp)
    * [Component.cpp](BladeGameEngine/src/Component.cpp)
    * [GameObject.cpp](BladeGameEngine/src/GameObject.cpp)
    * [ImageSource.cpp](BladeGameEngine/src/ImageSource.cpp)
  * [CMakeLists.txt](BladeGameEngine/CMakeLists.txt)
* [BladeGraphics/](BladeGraphics)
  * [include/](BladeGraphics/include)
    * [stm32h7/](BladeGraphics/include/stm32h7)
    * [windows/](BladeGraphics/include/windows)
      * [GraphicsLink.h](BladeGraphics/include/windows/GraphicsLink.h)
    * [BladeGraphics.h](BladeGraphics/include/BladeGraphics.h)
    * [BladeGraphicsProcessor.h](BladeGraphics/include/BladeGraphicsProcessor.h)
    * [DisplayDriver.h](BladeGraphics/include/DisplayDriver.h)
    * [ImageData.h](BladeGraphics/include/ImageData.h)
    * [ImageLoader.h](BladeGraphics/include/ImageLoader.h)
    * [MasterSpriteData.h](BladeGraphics/include/MasterSpriteData.h)
    * [PixelOperations.h](BladeGraphics/include/PixelOperations.h)
    * [SpriteInstance.h](BladeGraphics/include/SpriteInstance.h)
  * [src/](BladeGraphics/src)
    * [stm32h7/](BladeGraphics/src/stm32h7)
    * [windows/](BladeGraphics/src/windows)
      * [DisplayDriver.cpp](BladeGraphics/src/windows/DisplayDriver.cpp)
      * [GraphicsLink.cpp](BladeGraphics/src/windows/GraphicsLink.cpp)
      * [ImageLoader.cpp](BladeGraphics/src/windows/ImageLoader.cpp)
      * [PixelOperations.cpp](BladeGraphics/src/windows/PixelOperations.cpp)
    * [BladeGraphics.cpp](BladeGraphics/src/BladeGraphics.cpp)
    * [BladeGraphicsProcessor.cpp](BladeGraphics/src/BladeGraphicsProcessor.cpp)
    * [ImageData.cpp](BladeGraphics/src/ImageData.cpp)
    * [MasterSpriteData.cpp](BladeGraphics/src/MasterSpriteData.cpp)
    * [SpriteInstance.cpp](BladeGraphics/src/SpriteInstance.cpp)
  * [CMakeLists.txt](BladeGraphics/CMakeLists.txt)
  * [main.cpp](BladeGraphics/main.cpp)
* [BladeTestGame/](BladeTestGame)
  * [components/](BladeTestGame/components)
    * [include/](BladeTestGame/components/include)
      * [Hero.h](BladeTestGame/components/include/Hero.h)
      * [ObjectUpdatePrinter.h](BladeTestGame/components/include/ObjectUpdatePrinter.h)
      * [QuitTimer.h](BladeTestGame/components/include/QuitTimer.h)
    * [src/](BladeTestGame/components/src)
      * [Hero.cpp](BladeTestGame/components/src/Hero.cpp)
      * [ObjectUpdatePrinter.cpp](BladeTestGame/components/src/ObjectUpdatePrinter.cpp)
    * [CMakeLists.txt](BladeTestGame/components/CMakeLists.txt)
  * [include/](BladeTestGame/include)
    * [TestGame.h](BladeTestGame/include/TestGame.h)
  * [src/](BladeTestGame/src)
    * [TestGame.cpp](BladeTestGame/src/TestGame.cpp)
  * [CMakeLists.txt](BladeTestGame/CMakeLists.txt)
  * [main.cpp](BladeTestGame/main.cpp)
* [Tests/](Tests)
  * [BladeGameEngine/](Tests/BladeGameEngine)
    * [include/](Tests/BladeGameEngine/include)
      * [DataPoolTypes_test.h](Tests/BladeGameEngine/include/DataPoolTypes_test.h)
      * [DebugComponents.h](Tests/BladeGameEngine/include/DebugComponents.h)
      * [GameTestRunner.h](Tests/BladeGameEngine/include/GameTestRunner.h)
      * [GfxTestPacker_test.h](Tests/BladeGameEngine/include/GfxTestPacker_test.h)
    * [src/](Tests/BladeGameEngine/src)
      * [BitBladeGame_testDriver.cpp](Tests/BladeGameEngine/src/BitBladeGame_testDriver.cpp)
      * [DataCluster_testDriver.cpp](Tests/BladeGameEngine/src/DataCluster_testDriver.cpp)
      * [DataPool_testDriver.cpp](Tests/BladeGameEngine/src/DataPool_testDriver.cpp)
      * [DebugComponents.cpp](Tests/BladeGameEngine/src/DebugComponents.cpp)
      * [Event_testDriver.cpp](Tests/BladeGameEngine/src/Event_testDriver.cpp)
      * [GfxPacking_testDriver.cpp](Tests/BladeGameEngine/src/GfxPacking_testDriver.cpp)
      * [TransformComponent_testDriver.cpp](Tests/BladeGameEngine/src/TransformComponent_testDriver.cpp)
    * [CMakeLists.txt](Tests/BladeGameEngine/CMakeLists.txt)
  * [CMakeLists.txt](Tests/CMakeLists.txt)
* [CMakeLists.txt](CMakeLists.txt)


## License

No license at this time.

---


Copyright 2024 - Gregory Maynard