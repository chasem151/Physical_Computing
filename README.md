CS410 Assignment 3: Fall 2020

Teammates -- UID:

Chase Maivald,
Kaelyn Shinbashi,
Jared Yellen
--

Setup:

`<make clean>`
>Cleans working dir from all executable, object, library files

`<make all>`
>Compiles webserv.c, authentication.c, my-histogram.c, into their respective executables

Usage:

`<./webserv port-number usesthreads>` 
>Binds a web server to port-number in the range 5000-65536 followed by the usedthreads flag=0-fork() child creation/1-clone() child forking

`<./webserv2 port-number usesthreads>` 
>Accepts 4-digit passwords before the binding of the server to unlock the door... incorrect passwords will be captured by the camera!!
>Binds a web server to port-number in the range 5000-65536 followed by the usedthreads flag=0-fork() child creation/1-clone() child forking

`<python3 gotwoangle.py>`
>With pins 11 & 12 as Analog PWM outputs, the two servos rotate & wait in conjunction... with more powerful motors and some command strips, these will unlock a door of choice

`<python3 stream.py>`
>Records infrared 640x480 resolution (5 megapixel) @24 FPS bound to port-number 8080 with GET requests... will later handle TensorFlow Lite object detection
>https://randomnerdtutorials.com/video-streaming-with-raspberry-pi-camera/ for base-layer to this video server

1. List Directories 

2. Displays HTML files 

3. Displays .jpeg and .gif images

4. Executes static .cgi scripts

5. Thread management 

6. Physical Computing with Raspberry Pi 3B+ Board 

>4 count of 9 grams per Analog servo
https://www.amazon.com/MakerDoIt-Micro-Arduino-Walking-Control/dp/B07C5PGD3Q/ref=sr_1_1_sspa?dchild=1&keywords=servo+motor+raspberry+pi&qid=1607822289&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyVzhKOVRLM01ZTlQyJmVuY3J5cHRlZElkPUEwMDMyODUyMzYxMTlJUUpZVkMzViZlbmNyeXB0ZWRBZElkPUEwNzUzNDYzMzRLMlYyQ1UyMlJEVCZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=

>Keypad via USB
https://www.amazon.com/Numeric-Jelly-Comb-Portable-Computer/dp/B01NBSN2DC/ref=sr_1_1_sspa?dchild=1&keywords=keypad&qid=1608271220&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzRzhSVlgxVktKSTBTJmVuY3J5cHRlZElkPUEwNDU2NjI2VFpTRDk1UUVOMUEyJmVuY3J5cHRlZEFkSWQ9QTA1NzIwODcyWEtMNVZIR1dJNUU2JndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==

>RPI 3B+ Board
https://www.amazon.com/CanaKit-Raspberry-Power-Supply-Listed/dp/B07BC6WH7V/ref=sr_1_4?dchild=1&keywords=raspberry+pi+3b%2B&qid=1608271307&sr=8-4



