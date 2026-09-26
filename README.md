# mliner
Message dispatcher for low-cost controller net

## Basics
Aim of project is easy to implement message dispatch system for low-cost controllers. You can create and test main logic on PC(using zmq for test transmiting), then implement it on controllers. The method have to save much time of developing.

System was developed for IMU net which can be used for motion capture and processing. IMU net consists of several measurement units and processing uints. Each of net node can transmit and receive data using mline: so you can rapidly increase or decrease numbers of net nodes. Cause of SPI system must have main device which regulates data transfer.

Another issue is automation of processing data division. Main device know about free time on different nodes, so, it's possible to arrange similar processes between nodes. OR you can create you full process logic, but separate it on different source files. Based on processing power of your controllers you can build different parts of the logic for different physical controllers. So you can upload large code on your small set of controllers.  

For PC, you can use cmake for building and implementing.

# Hardware

Hardware is based on DMA SPI. Drivers can be found in src/spi. They are written for stm32f103x and stm32f74x. 

# Software
Software build for Embox OS and for STM32 LL library. You can rewrite drivers for another OS(FreeRTOS, uCOS, Nuttx, etc.), but you have to start before using Mline function like initSpiMlinerMain(name can be different) in src/spi/spi_impl_maindev.c

# Roadmap

- Add examples for stm32
- Using different interfaces simultaneously on one device: for example, controller can send messages both via USB and SPI using address
- Add tests for transmit speed

# mliner
Диспетчер сообщений для сетей из дешевых микроконтроллеров

## Базовые

Цель проекта - система передачи сообщений, которую легко имплементировать. Вы можете создавать и тестировать основную логику процесса на ПК (используя zmq для симуляции передачи данных), а далее адаптировать их под контроллеры. Этот метод должен сэкономить значительное время разработки.

Система была разработана для сети IMU, которая предназначается для захвата и анализа движений. Сеть IMU состоит из нескольких измерительных устройств и устройств обработки данных. Каждый узел сети может передавать и принимать данные, используя Mline: так что возникает возможность быстрого увеличения или уменьшения количества узлов в сети. Из-за использования SPI система включать в себя главное устройство, который регулирует передачу данных.

Другая проблема - это автоматизация распределения нагрузки по обработке данных. Главному устройству известно насколько много свободного времени у устройства в узле, поэтому есть возможность перераспределять нагрузку между узловыми устройствами. ИЛИ ты можешь разрабатывать всю процедурную логику, но размещать их в разных файлах исходного кода. Ориентируясь на обрабатывающие мощности Ваших контроллеров вы можете распределить логику на всю сеть независимых физически устройств. Получается вы можете загрузить большой код на вашу сеть малых контроллеров.

Для ПК вы можете использовать cmake для сборки и встраивания.

# Аппаратное обеспечение

Аппаратное обеспечение использует DMA SPI. Драйверы могут быть найдены в src/spi. Они написаны для stm32f103x и stm32f74x.

# Программное обеспечение

Программное обеспечение создано для Embox OS и для STM32 LL library. Вы можете переписать драйверы для другой ОС (FreeRTOS, uCOS, Nuttx, etc.), но Вам нужно запустить перед использованием Mline такую функцию как, например, initSpiMlinerMain(имя не имеет значения) в src/spi/spi_impl_maindev.c

# Проектные цели

- Добавить примеры для stm32
- Использовать разные интерфейсы одновременно на одном устройстве: например, контроллер может отправлять сообщения через USB и SPI, используя адреса
- Добавить тесты скорости передачи данных
