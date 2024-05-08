# SynthBoard

## Overview
The SynthBoard is an open-source embedded audio synthesizer that merges the features of traditional synthesizers with the flexibility of digital instruments, specifically designed for live performances and music experimentation. This device was developed as part of a capstone project by a team of engineering students, aiming to create a cost-effective, yet powerful musical instrument that provides a unique platform for artists and enthusiasts to explore sound design.

The STM32F407G-DISC1 microcontroller is at the heart of the SynthBoard, chosen for its rich set of features including ample GPIO pins, analog inputs, and built-in DAC capabilities. The device firmware was developed in C++ using STM32CubeIDE, leveraging the microcontroller's features to achieve low-latency audio processing and responsive user interactions.

Demo of the SynthBoard in action: https://youtu.be/wo0ibzK1-dg

### Project Highlights
- **Grid Layout Button Matrix**: Allows users to trigger sounds and control sequences with tactile feedback.
- **Configurable LED Lighting**: Each button is backlit, providing visual feedback that enhances user interaction.
- **Advanced Waveform Generation**: Users can generate and manipulate a variety of waveforms to create unique sounds.
- **Dynamic Sound Control**: Through onboard potentiometers, users can adjust sound parameters like volume, pitch, and filters.
- **Polyphonic Capabilities**: Supports playing multiple notes simultaneously, ideal for complex compositions.
- **Extensive Connectivity**: Includes an AUX output for easy integration with other audio equipment.

<p align="center">
  <img src="https://github.com/Galc3882/SynthBoard/assets/86870298/64dfb12f-0615-463f-b2a6-bb43d3244476" alt="cer" height="600"/>
</p>

## Hardware Design
### Case and Structural Design
The SynthBoard's case was meticulously designed using Autodesk Fusion360, inspired by similar devices in the market but optimized for our unique requirements. The final design is a robust, compact enclosure that houses all electronic components securely while providing easy access to controls for live performance.

![allen_mie drawio](https://github.com/Galc3882/SynthBoard/assets/86870298/6fad9374-368e-478d-8fac-e2e7b6ea0cf6)

#### Specifications:
- **Dimensions**: 210 mm x 121 mm x 46.5 mm, designed to fit within the maximum build volume of the Creality Ender 3 S1 Pro 3D printer.
- **Material**: The case components were printed using PLA due to its ease of printing and sufficient durability for handheld devices.
- **Assembly**: The case uses screws and clips designed for easy assembly and maintenance, allowing users to replace components if necessary.

### Button Matrix and LED Configuration
The button matrix is a core component of the SynthBoard, allowing users to trigger and modify sounds interactively. Each button is paired with an RGB LED, which is controlled dynamically to reflect the device's status and provide feedback on user actions.

#### Details:
- **Matrix Layout**: 4x4 grid of tactile buttons, each with an associated WS2812B RGB LED.
- **LED Effects**: Configurable lighting patterns and colors to indicate different states and functions, enhancing the visual appeal and user experience.

### Audio Output Components
To handle audio output, the SynthBoard is equipped with an AUX port that supports external speakers and headphones. This flexibility allows the device to be used in various environments, from studio sessions to live performances.

#### Features:
- **Integrated DAC**: Utilizes the microcontroller's built-in DAC to convert digital audio signals into analog, ensuring high-quality sound output.
- **Potentiometer Controls**: Eight rotary knobs are used to adjust volume, tone, and effects in real-time, providing extensive control over the audio output.

## Software Design and Implementation

### Program Flow
The SynthBoard operates through a well-defined program flow that includes initializing the hardware, processing user inputs, generating audio, and managing LED outputs. This process ensures a seamless and interactive user experience.

![SystemArchitectureReport](https://github.com/Galc3882/SynthBoard/assets/86870298/42b7b1fb-a5f7-4840-a3b3-941ab5d2a78a)

#### Key Components:
- **Input Handling**: Scans the button matrix and potentiometers to detect user interactions and adjust settings accordingly.
- **Sound Generation**: Uses digital signal processing techniques to create and manipulate waveforms in real-time based on user inputs.
- **Output Management**: Controls the LEDs and audio output to reflect changes and provide feedback.

### WS2812B LED Control

#### Overview
For the visual feedback component of the SynthBoard, we utilized WS2812B LEDs controlled via Direct Memory Access (DMA) to minimize CPU overhead and ensure smooth lighting effects. The LEDs are interfaced using the SPI peripheral configured through STM32CubeIDE to manage the precise timing requirements of the WS2812B protocol.

#### LED Protocol Implementation
- **Encoding Bits**: The WS2812B protocol requires specific timings for logical '0' and '1':
  - A logical '0' is encoded as a high pulse of 0.35 microseconds followed by a low pulse of 0.90 microseconds.
  - A logical '1' is encoded as a high pulse of 0.90 microseconds followed by a low pulse of 0.35 microseconds.
- **Data Transmission**: Using an 8-bit SPI protocol, a '0' or '1' is transmitted as `0x80` or `0xFC`, respectively. This approach simplifies the implementation by using standard SPI hardware to emulate the single-wire protocol of the LEDs.

#### Functionality
- **Dynamic Control**: The `ws2812_pixel` function wraps the SPI transmissions, enabling individual control over each LED's color and brightness. This allows for complex lighting patterns and feedback that enhances user interaction with the SynthBoard.

### Potentiometer Integration

#### Configuration
Each of the eight potentiometers is directly connected to one of the Analog-to-Digital Converter (ADC) channels on the STM32 microcontroller. This setup bypasses the need for a multiplexer, simplifying the hardware design and increasing the responsiveness of the controls.

#### ADC Setup
- **Initialization**: Configured for continuous conversion mode with an 8-bit resolution, allowing a range of 0-255, which adequately captures the variations in potentiometer settings from minimum to maximum.
- **Efficiency**: The ADC readings are managed via DMA to offload the CPU from continuous polling, enhancing the overall system efficiency.

#### Operational Details
- **Continuous Monitoring**: The ADC continuously converts the analog voltages from the potentiometers into digital values stored in `adc1Buffer`.
- **Data Handling**: The `getInputs` function periodically updates the control parameters based on the latest ADC values, facilitating real-time audio manipulation based on user input.

<p align="center">
  <img src="https://github.com/Galc3882/SynthBoard/assets/86870298/2afe0608-d89f-49a0-b0b7-8233ecd27923" alt="cer" height="400"/>
</p>

### Audio Processing Techniques

#### Waveform Generation
- **Class Design**: The `Oscillator` class handles the generation of different waveforms—sine, square, sawtooth, and triangle. Each waveform generation method computes the appropriate values based on phase accumulators and waveform-specific properties.
- **ADSR Envelope**: The `ADSREnvelope` class dynamically adjusts the amplitude of the sound through its Attack, Decay, Sustain, and Release phases, crucial for shaping the character of the sound.

#### Sound Synthesis
- **Synthesizer Class**: Centralizes the waveform generation methods, facilitating the creation of complex sounds by combining fundamental tones with harmonics.
- **Low-Pass Filter**: Implemented to shape the sound by attenuating higher frequencies, making the output more mellow and less harsh.

#### Performance Optimization
- **Inline Functions and Macros**: Critical for reducing function call overhead in performance-critical parts of the audio processing loop.
- **Integer Arithmetic and Bitwise Operations**: Utilized to enhance execution speed by avoiding floating-point computations and simplifying multiplicative and divisional operations.

## License

This project is licensed under the MIT License. For more details, see the `LICENSE.md` file in the repository.
