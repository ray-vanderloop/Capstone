#!/usr/bin/python
import subprocess
import tkinter as tk
from subprocess import call
import time
from threading import Timer
from subprocess import Popen, PIPE
 
root = tk.Tk()
frame = tk.Frame(root)
frame.pack()

cmd = "blinkingLed.c"

print ("Hey this is Python Script Running\n")

subprocess.call(["gcc", cmd, "-o", "led", "-lwiringPi"]) #For Compiling

def blinkLed():
    subprocess.call(["sudo", "./led"])
    
def kill_proc():
    proc.kill()

proc = Popen("count_to_10.exe", shell=True)
t = Timer(60, kill_proc)
t.start()
proc.wait()

button = tk.Button(frame, text="QUIT", fg="red", command=quit)
button.pack(side=tk.LEFT)
slogan = tk.Button(frame, text="Blink", command=blinkLed)
slogan.pack(side=tk.LEFT)

root.mainloop()