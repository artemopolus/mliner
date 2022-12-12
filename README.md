# mliner
Dispatcher for low-cost controller net

## Basics
Aim of project is easy to implement dispatch system for low-cost controllers. You can create and test main logic on PC(using zmq for test transmiting), then implement it on controllers. The method have to save much time of developing.

System was developed for IMU net which can be used for motion capture and processing. IMU net consist of several measurement units and processing uints. Each of net node can transmit and receive data using mline: so you can rapidly increase or decrease numbers of net nodes. Cause of SPI system must have main device which regulates data transfer.

Another issue is automation of processing data division. Main device know about free time on different nodes, so, it's possible to arrange similar processes between nodes. OR you can create you full process logic, but separate it on different source files. Based on processing power of your controllers you can build different parts of the logic for different physical controllers. So you can upload large code on your small set of controllers.  

For PC, you can use cmake for building and implementing.

# Hardware

Hardware is based on DMA SPI. Drivers can be found in src/spi. They are written for stm32f103x and stm32f74x. 

# Software
Software build for Embox OS and for STM32 LL library. You can rewrite drivers for another OS(FreeRTOS, uCOS, Nuttx, etc.), but you have to start before using Mline function like initSpiMlinerMain(name can be different) in src/spi/spi_impl_maindev.c

# Roadmap

- Add examples for stm32
- Using different interfaces simultaneously on one device: controller can send messages both via USB and SPI using address
- Add tests for transmit speed

