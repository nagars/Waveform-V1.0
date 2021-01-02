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
    2. Encoder
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
- LCD screeb to show which signal has been selected
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

    A basic knowledge of how MCU's work, their hardware peripherals, working of interrupts and understanding of communication protocols is necessary.
    A major part of working with any MCU is understanding how to read and write to registers, access hardware etc.
      
- Analog Circuit Design / Analog Signals
  
    Knowledge regarding various signal types will be necessary. How Frequency, Ampitude and DC Offset affect a signal. How Opamps work, Thevenin and Norton theorem's, Linear regulators etc.
    
- EagleCAD

    EagleCAD is a what I designede the PCB in. It's popular among hobbyists because the free version is only limited by board size and not by the number of nodes as is with Orcad PCB Designer for example.
    It's a good place to start if you're looking to learn PCB designing.
    
    
## Design Breakdown
I will breakdown the software and hardware for each sub-system of my design.

### PIC MCU
I used the [PIC24FJ64GA002](https://www.microchip.com/wwwproducts/en/PIC24FJ64GA002). Given the simple requirements of this project, almost any mainstream MCU will do.
Primarily I prioritised the following:

- Enough GPIO's (12 for the DAC, 3 for encoder inputs, 3 for the shift register, 2 for the LCD)
- 1 SPI (For the LCD)
- Fast speed (This one goes up to 32Mhz with an internal clock + PLL)
- DIP packaging to make a novice's life easier

## Encoder



## Issues
Given that this was my first major project, a whole host of issues were expected.

1. Diode D1 overlaps the R - 2R ladder.
2. Diodes D2 and D3 do not fit in the space I provided.
3. The power Jack is too large to fit on the Board.
4. Higher frequencies result in the signal getting distorted. Issue is probably a combination of building my own DAC and of how I adjust the frequency in code explained above.
5. LCD used just looks plan awful hanging off the board.

## Future Scope
For now, this project is on pause. Between work, family and other projects I don't have any immediate plans to work on a version 2.0. However, the changes I will make when I get back to it
will probably involve:

1. Replacing the PIC with an STM32. It's faster with clock going as far up as 40Mhz, it executes one instruction per cycle unlike the PIC with 2. The biggest factor
is the cost. The Pickit 4 debugger is around 7000 rs while an STM32 Nucleo evaluation board with the mcu and debugger is less than 2000 rs.

2. It goes without saying that I will switch to SMD parts to help with form factor. 

3. Finally, I'll abandon the R - 2R ladder and use the internal DAC of the MCU. Or maybe an external one with more bits. 

4. Implement an external oscillator for the MCU

5. Potentially implement digital potentiometers for signal adjustment
