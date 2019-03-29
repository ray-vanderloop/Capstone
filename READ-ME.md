Sounding Rocket Ground Station
This read me file is for the Sounding Rocket Ground Station by Eiley Tarlton, Kelsey Hynes and Rachel Vanderloop.

Getting Started
Download a copy of packet_process.c, packet_process.h, packet.h and msg_queue.h to run the processing part of the code. For the matlab section of the code you will need to download PacketGenerator_Matlab.m or Capstone_no_dropped_packet.m

If you would like to view the GUI download final-gui.py


Prerequisites
In order to run the packet processing portion of the code you will need matlab

In order to run the GUI you will need python, numpy and matplotlib

Running the tests
  To run the matlab code, please follow the following tutorial, as the serial port will be different for everyone's computer
    https://www.mathworks.com/help/matlab/matlab_external/getting-started-with-serial-i-o.html
    This link will tell you how to create a serial object at the desired port location and open said object.
    Next you will run the matlab code provided and then run the c code as follows.
  To view the c code without the GUI type the following in the terminal
    gcc packet_proccess.c -o output -pthread -lwiringPi
    ./output

  To view the GUI type the following in the terminal
    python3 final-gui.py

Built With
  python
  matlab
  visual studios

Authors
Eiley Tarlton - Packet Processing
Kesley Hynes - Hardware
Rachel Vanderloop - GUI


Acknowledgments
  Thanks to everyone who helped out:
    Gabriel Harris
    Professor McIsaac
    Chelsea Everest
