# LED Octapad

<p float='left'>
<img src = "https://github.com/DangerousTim/LED-Octapad/blob/master/Media/closeupnice.jpg" width=400>
<img src = "https://github.com/DangerousTim/LED-Octapad/blob/master/Media/nice2.jpg" width=400>
</p>

Well technically, an LED hexapad, but Octapad kinda stuck early on and no one wanted to change that name.

The LED Octapad is, at its heart, a MIDI controller, but specced up with _324_ individually controlled RGB LEDs. This means the pads are dynamic, reacting in real time to your touch, while also sending MIDI signals to your computer to play, say a virtual drum machine, or a virtual piano.

## Project Members
Abhishek Dixit, [Aditya Bidwai](https://github.com/adbidwai), Advait Kulkarni, [Tanmay Bhonsale](https://github.com/DangerousTim), Tanmay Devale, Vedant Shah, Yash Jangir


## Uploading the code

### The touch controller

The project has 2 microcontrollers: One to handle the touch responses, and one to handle the precarious task of handling 300+ neopixels. We've used a teensy LC for the former. Be sure to have installed the teensy loader first. To set it up:
1. Load [midiTouchTeensy.ino](https://github.com/DangerousTim/LED-Octapad/blob/master/midiTouchTeensy/midiTouchTeensy.ino)
2. In the arduino IDE, select Tools > USB type > "Serial + MIDI"
3. Upload.

### The NeoPixel controller 

An UNO handles 6 separate strips of neopixels - one in each hexagon. Only one Adafruit_NeoPixel object is instantiated though, and different strips are handled using Adafruit_Neopixel.setPin() method. The strip is arranged concentrically inside each hexagon.

To set it up:
1. Load [ledSlaveUNO.ino](https://github.com/DangerousTim/LED-Octapad/blob/master/ledSlaveUNO/ledSlaveUNO.ino)
2. Upload.
