# Waveform-V1.0 (May 2018 - August 2018)

## IMPORTANT NOTE
### **This project is incomplete.** 
Though all the peripherals have been implemented and tested, I abandoned compiling them all into one homogenous project. At the time my intention
was to revisit the core design and implement a version 2.0. The reasons why have been listed further down. Effectively, the only features missing are:

- The ability to select the required waveform from a menu on the LCD using the encoder
- The ability to select frequency of the signal via the encoder. [Removed because it was causing signal integrity issues]

## Inspiration
Back in May of 2018, fresh out of college I decided to gain some experience on the PIC24 line of MCU's. Microchip has good hobbyists support through forums and documentation. 
But the biggest reason was that their MCU's come in DIP packaging which meant I could just use a breadboard for prototyping. 

**This would become my first major embedded systems project and my first time working with a PIC outside of college. It would also be my second PCB design on EagleCAD. 
Please bear that in mind if you want to use this for reference.**

## Table of Contents
1. Objective
    1. Feature List
    2. Knowledge Base Required
2. Design Breakdown
    1. PIC MCU
    2. Rotary Encoder
    3. DAC
    4. DMA
    5. LCD
    6. Negative Voltage Generator
    7. Opamp Stage
    8. Voltage Regulator
3. Issues
4. Future Scope

## Objective
The objective of this project was to make a Waveform Generator based on a PIC24. Signals to be generated included SINE, SAWTOOTH, SQUARE and TRIANGLE. 
The device would have to include Frequency, Amplitude and DC Offset adjustment. It was to run on a 12V power supply.

### Feature List

- Device should run on 12V power supply with a DC jack
- LCD screen to show which signal has been selected
- Encoder to select signal via LCD
- Encoder to set Frequency of signal
- Pushbutton to enable / disable signal output (Included within Encoder)
- MCU Debug interface for on board debugging
- Resistor Ladder based DAC implementation
- Amplitude adjustment to max +-10V
- Frequency adjustment to max 1Mhz
- Offset adjustment to max +-10V
- Power switch and LED indicator

## Knowledge Base Required

- MPLAB IDE
  
    MPLAB is microchips IDE for PIC and now several AVR MCU's. It has all the standarc IDE features including project management and Debugging. Microchip has it's own HAL
    called Harmony. I deliberately chose not to use it as I wanted a deeper understanding of the PIC's features.
      
- C programming language

    A basic understanding of c is necessary. How to use For loops, switch cases etc.
    
- Microcontroller basics

    A basic knowledge of how MCU's work, their hardware peripherals, working of interrupts and understanding of communication protocols is necessary. A major part of working with any MCU is understanding how to read and write to registers, access hardware etc.
      
- Analog Circuit Design / Analog Signals

    Knowledge regarding various signal types will be necessary. How Frequency, Ampitude and DC Offset affect a signal. How Opamps work, Thevenin and Norton theorem's, Linear regulators etc.
    
- EagleCAD

    EagleCAD is a what I designede the PCB in. It's popular among hobbyists because the free version is only limited by board size and not by the number of nodes as is with Orcad PCB Designer for example. It's a good place to start if you're looking to learn PCB designing.
    
    
## Design Breakdown
I will breakdown the software and hardware for each sub-system of my design.

### PIC MCU
I used the [PIC24FJ64GA002](https://www.microchip.com/wwwproducts/en/PIC24FJ64GA002). Given the simple requirements of this project, almost any mainstream MCU will do.
Primarily I prioritised the following:

- Enough GPIO's (12 for the DAC, 3 for encoder inputs, 3 for the shift register, 2 for the LCD)
- 1 SPI (For the LCD)
- Fast speed (This one goes up to 32Mhz with an internal clock + PLL)
- DIP packaging to make a novice's life easier

## Rotary Encoder
I decided to use a relative rotary encoder with an internal pushbutton. The intention was to use the rotary encoder to select the signal type along with it's various parameters. The inputs would be finalised by pressing the encoder button. A good resource for encoders can be found [here](https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/).

The pushbutton along with the encoder outputs were driven to input GPIO's. I poll the encoder outputs using timer1 triggering an interrupt every 10ms. 

My code logic is simple. I read both outputs and place them into a single variable. The 0th and 1st bits represent each signal respectively. I enter a switch case where I compare the current configuration to the last polled one. Depending on which output has changed first, I determine if the encoder was rotated clockwise or counter-clockwise.

The intention was for the encoder to be used to select the signal to output along with its frequency. Selecting the option could be done by hitting the pushbutton in the encoder.

## DAC
DAC stands for **Digital to Analog Converter**. In short, it converts a digital number to an analog signal whose voltage varies depending on the value of said number. Say your DAC is 8 bits, it can handle numbers between 0 to 255. A value of 255 will output Vmax where as 128 will output 0.5Vmax. 

I did not use the inbuilt DAC. I wanted to build my own using an R - 2R resistor ladder. There is no benefit to this, absolutely none but I did it anyway just to see how good or bad the results would be. You can find a good resource on it [here](https://www.electronics-tutorials.ws/combination/r-2r-dac.html).

Note: Most standard resistors have an error of between 10% to 1%. I ordered those with 0.1% error as the DAC is heavily dependent on equal values amongst its resistors. 

I decided to create a lookup table for the signals. It is an array of values that generate your signal on your DAC when you parse through the array. There are a bunch of tools to make lookup tables [like](https://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml).

I assigned the DAC pins to PORTB. Then I used the DMA module to transfer the array to PORTB. More information on that below. Depending on which gpio was set or cleared, the DAC put out a certain value between 0 and 5V. 

## DMA
DMA stands for **Direct Memory Access**. Basically it is an effective way of trasnfering data from one memory address to another, one peripheral to memory address or one memory address to a peripheral. Depending on the MCU, you may even transfer data from oen peripheral to another. The advantage of using the DMA is that the processor has no role in this transfer. As a result it can go ahead and do other tasks without wasting time and resources on a memory transfer. The DMA runs intependent of the processor.

I used the DMA to transfer 256 bytes from a specific lookup table address to the PORTB address. Upon completion, the DMA would trigger an interrupt where in I would trigger the same DMA transaction again resultiing in a repeating signal generated at the DAC. 

There is a fundemental flaw in this method. How do you control the frequency of the signal? Instead of changing the rate of transfer to the DAC, I decided to reduce the number of data points of the signal to increase the frequency of the signal. So say for a 1 hz signal I would put out all 256 data points from the lookup table, to increase frequency I would reduce the number of data points. I did this by changing how I traverse the array. I would increase the number of elements to increment by. Thus, at higher frequencies the signal would be completely compromised. Even for a novice, this was a bad idea.

My plan now is to use a timer to trigger a single DMA transfer of 1 element in the array. With each trigger of the timer interrupt, I send out the next element to the port of the DAC. Now, I can control the frequency by adjusting the timer. I also found an [application note](http://ww1.microchip.com/downloads/en/Appnotes/00003312A.pdf) that does the same with the addded functionality of using a potentiometer to control the frequency.

 ## LCD
 I used a standard 16x2 LCD interfaced with a parallel port in 8 bit mode. The user would use the rotary encoder to select a highlighted option on the screen. A blinking option would mean you could change that options value. A good resource for interfacing an LCD like this can be found [here](https://circuitdigest.com/microcontroller-projects/16x2-lcd-interfacing-with-pic-microcontroller).
    
 ## Negative Voltage Generator
 
    
    7. Opamp Stage
    8. Voltage Regulator

## Issues
Given that this was my first major project, a whole host of issues were expected.

1. Diode D1 overlaps the R - 2R ladder.
2. Diodes D2 and D3 do not fit in the space I provided.
3. The power Jack is too large to fit on the Board.
4. Higher frequencies result in the signal getting distorted. Issue is probably a combination of building my own DAC and of how I adjust the frequency in code explained above.
5. LCD used just looks plan awful hanging off the board.
6. Very very poor frequency control. 

## Future Scope
For now, this project is on pause. Between work, family and other projects I don't have any immediate plans to work on a version 2.0. However, the changes I will make when I get back to it
will probably involve:

1. Replacing the PIC with an STM32. It's faster with clock going as far up as 40Mhz, it executes one instruction per cycle unlike the PIC with 2. The biggest factor
is the cost. The Pickit 4 debugger is around 7000 rs while an STM32 Nucleo evaluation board with the mcu and debugger is less than 2000 rs.

2. It goes without saying that I will switch to SMD parts to help with form factor. 

3. Finally, I'll abandon the R - 2R ladder and use the internal DAC of the MCU. Or maybe an external one with more bits. 

4. Implement an external oscillator for the MCU to avoid using a PLL.

5. Potentially implement digital potentiometers for signal adjustment.
