# LED-WiFi-Gradcap
Code for the obnoxious 14x14 LED matrix I soldered onto my graduation cap. Hosts a WiFi access point and webserver to control animations!

In May 2022, I graduated from the University of Central Florida, and decided to decorate my graduation cap with individually-addressable RGB LED strips, making a matrix. It's controlled by an ESP32 and powered by a phone battery bank in my pocket. The ESP32 hosts a Wi-Fi access point and webserver, allowing me to connect with my phone to control the matrix. Various images and scrolling text (including several computer science jokes) appear on on the matrix.

Please note that my code started off nice and organized... it did not finish that way. I started writing the code for this less than 32 hours before walking.

Video of the animations:
[![Link to YouTube video of RGB Matrix](https://img.youtube.com/vi/TQ12iKv7iug/0.jpg)](https://www.youtube.com/watch?v=TQ12iKv7iug)

Screenshot of the webpage that the webserver on the ESP32 hosts to control the matrix:
![Screenshot of user controls](https://pasteimg.com/images/2022/06/12/IMG_1258.jpg)

It made it quite easy for my family to spot me at graduation
![enter image description here](https://pasteimg.com/images/2022/06/12/IMG_20220507_184129__01.jpg)

Required components:
- ESP32 Dev board
- WS2812B RGB LEDs
- A USB cable you don't mind chopping up
- Soldering tools + some skill
