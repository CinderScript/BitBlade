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

* [.VSCodeCounter/](.\BitBlade\.VSCodeCounter)
  * [2024-09-28_23-04-18/](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18)
    * [details.md](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\details.md)
    * [diff-details.md](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\diff-details.md)
    * [diff.csv](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\diff.csv)
    * [diff.md](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\diff.md)
    * [diff.txt](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\diff.txt)
    * [results.csv](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\results.csv)
    * [results.json](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\results.json)
    * [results.md](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\results.md)
    * [results.txt](.\BitBlade\.VSCodeCounter\2024-09-28_23-04-18\results.txt)
* [BladeCommon/](.\BitBlade\BladeCommon)
  * [include/](.\BitBlade\BladeCommon\include)
    * [BladeConfig.h](.\BitBlade\BladeCommon\include\BladeConfig.h)
    * [Event.h](.\BitBlade\BladeCommon\include\Event.h)
    * [GfxLinkCommon.h](.\BitBlade\BladeCommon\include\GfxLinkCommon.h)
    * [IGfxMessagePacker.h](.\BitBlade\BladeCommon\include\IGfxMessagePacker.h)
    * [IGfxMessageProducer.h](.\BitBlade\BladeCommon\include\IGfxMessageProducer.h)
    * [Logging.h](.\BitBlade\BladeCommon\include\Logging.h)
  * [CMakeLists.txt](.\BitBlade\BladeCommon\CMakeLists.txt)
* [BladeConsole/](.\BitBlade\BladeConsole)
  * [include/](.\BitBlade\BladeConsole\include)
    * [windows/](.\BitBlade\BladeConsole\include\windows)
      * [ConsoleLink.h](.\BitBlade\BladeConsole\include\windows\ConsoleLink.h)
    * [BladeConsole.h](.\BitBlade\BladeConsole\include\BladeConsole.h)
  * [src/](.\BitBlade\BladeConsole\src)
    * [windows/](.\BitBlade\BladeConsole\src\windows)
      * [ConsoleLink.cpp](.\BitBlade\BladeConsole\src\windows\ConsoleLink.cpp)
    * [BladeConsole.cpp](.\BitBlade\BladeConsole\src\BladeConsole.cpp)
  * [CMakeLists.txt](.\BitBlade\BladeConsole\CMakeLists.txt)
* [BladeGameEngine/](.\BitBlade\BladeGameEngine)
  * [components/](.\BitBlade\BladeGameEngine\components)
    * [include/](.\BitBlade\BladeGameEngine\components\include)
      * [Sprite.h](.\BitBlade\BladeGameEngine\components\include\Sprite.h)
      * [Transform.h](.\BitBlade\BladeGameEngine\components\include\Transform.h)
    * [src/](.\BitBlade\BladeGameEngine\components\src)
      * [Sprite.cpp](.\BitBlade\BladeGameEngine\components\src\Sprite.cpp)
      * [Transform.cpp](.\BitBlade\BladeGameEngine\components\src\Transform.cpp)
  * [include/](.\BitBlade\BladeGameEngine\include)
    * [BitBladeGame.h](.\BitBlade\BladeGameEngine\include\BitBladeGame.h)
    * [BladeStart.h](.\BitBlade\BladeGameEngine\include\BladeStart.h)
    * [Component.h](.\BitBlade\BladeGameEngine\include\Component.h)
    * [DataCluster.h](.\BitBlade\BladeGameEngine\include\DataCluster.h)
    * [DataPool.h](.\BitBlade\BladeGameEngine\include\DataPool.h)
    * [DataPoolMember.h](.\BitBlade\BladeGameEngine\include\DataPoolMember.h)
    * [DataTypeID.h](.\BitBlade\BladeGameEngine\include\DataTypeID.h)
    * [GameObject.h](.\BitBlade\BladeGameEngine\include\GameObject.h)
    * [ImageSource.h](.\BitBlade\BladeGameEngine\include\ImageSource.h)
    * [Vector2.h](.\BitBlade\BladeGameEngine\include\Vector2.h)
  * [src/](.\BitBlade\BladeGameEngine\src)
    * [BitBladeGame.cpp](.\BitBlade\BladeGameEngine\src\BitBladeGame.cpp)
    * [Component.cpp](.\BitBlade\BladeGameEngine\src\Component.cpp)
    * [GameObject.cpp](.\BitBlade\BladeGameEngine\src\GameObject.cpp)
    * [ImageSource.cpp](.\BitBlade\BladeGameEngine\src\ImageSource.cpp)
  * [CMakeLists.txt](.\BitBlade\BladeGameEngine\CMakeLists.txt)
* [BladeGraphics/](.\BitBlade\BladeGraphics)
  * [include/](.\BitBlade\BladeGraphics\include)
    * [stm32h7/](.\BitBlade\BladeGraphics\include\stm32h7)
    * [windows/](.\BitBlade\BladeGraphics\include\windows)
      * [GraphicsLink.h](.\BitBlade\BladeGraphics\include\windows\GraphicsLink.h)
    * [BladeGraphics.h](.\BitBlade\BladeGraphics\include\BladeGraphics.h)
    * [BladeGraphicsProcessor.h](.\BitBlade\BladeGraphics\include\BladeGraphicsProcessor.h)
    * [DisplayDriver.h](.\BitBlade\BladeGraphics\include\DisplayDriver.h)
    * [ImageData.h](.\BitBlade\BladeGraphics\include\ImageData.h)
    * [ImageLoader.h](.\BitBlade\BladeGraphics\include\ImageLoader.h)
    * [MasterSpriteData.h](.\BitBlade\BladeGraphics\include\MasterSpriteData.h)
    * [PixelOperations.h](.\BitBlade\BladeGraphics\include\PixelOperations.h)
    * [SpriteInstance.h](.\BitBlade\BladeGraphics\include\SpriteInstance.h)
  * [src/](.\BitBlade\BladeGraphics\src)
    * [stm32h7/](.\BitBlade\BladeGraphics\src\stm32h7)
    * [windows/](.\BitBlade\BladeGraphics\src\windows)
      * [DisplayDriver.cpp](.\BitBlade\BladeGraphics\src\windows\DisplayDriver.cpp)
      * [GraphicsLink.cpp](.\BitBlade\BladeGraphics\src\windows\GraphicsLink.cpp)
      * [ImageLoader.cpp](.\BitBlade\BladeGraphics\src\windows\ImageLoader.cpp)
      * [PixelOperations.cpp](.\BitBlade\BladeGraphics\src\windows\PixelOperations.cpp)
    * [BladeGraphics.cpp](.\BitBlade\BladeGraphics\src\BladeGraphics.cpp)
    * [BladeGraphicsProcessor.cpp](.\BitBlade\BladeGraphics\src\BladeGraphicsProcessor.cpp)
    * [ImageData.cpp](.\BitBlade\BladeGraphics\src\ImageData.cpp)
    * [MasterSpriteData.cpp](.\BitBlade\BladeGraphics\src\MasterSpriteData.cpp)
    * [SpriteInstance.cpp](.\BitBlade\BladeGraphics\src\SpriteInstance.cpp)
  * [CMakeLists.txt](.\BitBlade\BladeGraphics\CMakeLists.txt)
  * [main.cpp](.\BitBlade\BladeGraphics\main.cpp)
* [BladeTestGame/](.\BitBlade\BladeTestGame)
  * [components/](.\BitBlade\BladeTestGame\components)
    * [include/](.\BitBlade\BladeTestGame\components\include)
      * [Hero.h](.\BitBlade\BladeTestGame\components\include\Hero.h)
      * [ObjectUpdatePrinter.h](.\BitBlade\BladeTestGame\components\include\ObjectUpdatePrinter.h)
      * [QuitTimer.h](.\BitBlade\BladeTestGame\components\include\QuitTimer.h)
    * [src/](.\BitBlade\BladeTestGame\components\src)
      * [Hero.cpp](.\BitBlade\BladeTestGame\components\src\Hero.cpp)
      * [ObjectUpdatePrinter.cpp](.\BitBlade\BladeTestGame\components\src\ObjectUpdatePrinter.cpp)
    * [CMakeLists.txt](.\BitBlade\BladeTestGame\components\CMakeLists.txt)
  * [include/](.\BitBlade\BladeTestGame\include)
    * [TestGame.h](.\BitBlade\BladeTestGame\include\TestGame.h)
  * [src/](.\BitBlade\BladeTestGame\src)
    * [TestGame.cpp](.\BitBlade\BladeTestGame\src\TestGame.cpp)
  * [CMakeLists.txt](.\BitBlade\BladeTestGame\CMakeLists.txt)
  * [main.cpp](.\BitBlade\BladeTestGame\main.cpp)
* [Tests/](.\BitBlade\Tests)
  * [BladeGameEngine/](.\BitBlade\Tests\BladeGameEngine)
    * [include/](.\BitBlade\Tests\BladeGameEngine\include)
      * [DataPoolTypes_test.h](.\BitBlade\Tests\BladeGameEngine\include\DataPoolTypes_test.h)
      * [DebugComponents.h](.\BitBlade\Tests\BladeGameEngine\include\DebugComponents.h)
      * [GameTestRunner.h](.\BitBlade\Tests\BladeGameEngine\include\GameTestRunner.h)
      * [GfxTestPacker_test.h](.\BitBlade\Tests\BladeGameEngine\include\GfxTestPacker_test.h)
    * [src/](.\BitBlade\Tests\BladeGameEngine\src)
      * [BitBladeGame_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\BitBladeGame_testDriver.cpp)
      * [DataCluster_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\DataCluster_testDriver.cpp)
      * [DataPool_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\DataPool_testDriver.cpp)
      * [DebugComponents.cpp](.\BitBlade\Tests\BladeGameEngine\src\DebugComponents.cpp)
      * [Event_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\Event_testDriver.cpp)
      * [GfxPacking_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\GfxPacking_testDriver.cpp)
      * [TransformComponent_testDriver.cpp](.\BitBlade\Tests\BladeGameEngine\src\TransformComponent_testDriver.cpp)
    * [CMakeLists.txt](.\BitBlade\Tests\BladeGameEngine\CMakeLists.txt)
  * [CMakeLists.txt](.\BitBlade\Tests\CMakeLists.txt)
* [CMakeLists.txt](.\BitBlade\CMakeLists.txt)


## License

No license at this time.

---


Copyright 2024 - Gregory Maynard