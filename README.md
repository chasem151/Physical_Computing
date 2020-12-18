CS410 Assignment 3

Teammates -- UID:

Chase Maivald -- U18719879,
Kaelyn Shinbashi -- U6256519,
Jared Yellen -- U62909356
--

Setup:

`<make clean>`
>Cleans working dir from all executable, object, library files

`<make all>`
>Compiles webserv.c, authentication.c, my-histogram.c, into their respective executables

Usage:

`<./webserv port-number usesthreads>` 
>Binds a web server to port-number in the range 5000-65536 followed by the usedthreads flag=0-fork() child creation/1-clone() child forking
>Accepted passkeys before the binding of the server will unlock the door... others will be captured by the camera!!

`<python3 gotwoangle.py>`
>With pins 11 & 12 as Analog PWM outputs, the two servos rotate & wait in conjunction... with more powerful motors and some command strips, these will unlock a door of choice

`<python3 stream.py>`
>Records infrared 640x480 resolution (5 megapixel) @24 FPS bound to port-number 8080 with GET requests... will later handle TensorFlow Lite object detection

1. List Directories 

2. Displays HTML files 

3. Displays .jpeg and .gif images

4. Executes static .cgi scripts

5. Thread management 

6. Physical Computing with Raspberry Pi 3B+ Board 




