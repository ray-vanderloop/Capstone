#!/usr/bin/env python

from tkinter import *
import tkinter as tk
from pandas import DataFrame
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pylab as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg


class capstoneGUI:
    
    def __init__(self, master):
        
        self.master = master
        
        self.flighttime = Label(master, text="Flight Time: ", bg="gray", fg="white", anchor="w")
        self.flighttime.pack()
        self.flighttime.place(x=400, y=0)
        self.flighttime.config(width=35)
        self.flighttime.config(font=("Courier", 15))
        self.flighttime.config(padx = 10, pady = 10)
        
        self.duration = Label(master, text="Duration: ", bg="#4f2683", fg="white", anchor="w")
        self.duration.pack()
        self.duration.place(x=400, y=45)
        self.duration.config(width=35)
        self.duration.config(font=("Courier", 15))
        self.duration.config(padx = 10, pady = 10)
        
        self.maxspeed = Label(master, text="Max. Speed: ", bg="gray", fg="white", anchor="w")
        self.maxspeed.pack()
        self.maxspeed.place(x=400, y=90)
        self.maxspeed.config(width=35)
        self.maxspeed.config(font=("Courier", 15))
        self.maxspeed.config(padx = 10, pady = 10)
        
        self.avgspeed = Label(master, text="Avg. Speed: ", bg="#4f2683", fg="white", anchor="w")
        self.avgspeed.pack()
        self.avgspeed.place(x=400, y=135)
        self.avgspeed.config(width=35)
        self.avgspeed.config(font=("Courier", 15))
        self.avgspeed.config(padx = 10, pady = 10)
        
        self.drogue= Label(master, text="Drogue Time: ", bg="gray", fg="white", anchor="w")
        self.drogue.pack()
        self.drogue.place(x=400, y=180)
        self.drogue.config(width=35)
        self.drogue.config(font=("Courier", 15))
        self.drogue.config(padx = 10, pady = 10)
        
        self.main = Label(master, text="Main Time: ", bg="#4f2683", fg="white", anchor="w")
        self.main.pack()
        self.main.place(x=400, y=225)
        self.main.config(width=35)
        self.main.config(font=("Courier", 15))
        self.main.config(padx = 10, pady = 10)
        
        self.apogee = Label(master, text="Apogee Time: ", bg="gray", fg="white", anchor="w")
        self.apogee.pack()
        self.apogee.place(x=400, y=270)
        self.apogee.config(width=35)
        self.apogee.config(font=("Courier", 15))
        self.apogee.config(padx = 10, pady = 10)
        
        self.graph= Label(master, bg="white", fg="black", anchor="w")
        self.graph.pack()
        self.graph.place(x=0, y=0)
        self.graph.config(width=32, height=13)
        self.graph.config(font=("Courier", 15))
        self.graph.config(padx = 10, pady = 10)





    
root = tk.Tk()

root.geometry("800x480+0+0")
#root.attributes('-type','dock')
#root.focus_force()

theGUI = capstoneGUI(root)
root.mainloop()
