from tkinter import *
import tkinter as tk
import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import matplotlib
import math
matplotlib.use("TkAgg")
from pylab import rcParams

class Page(tk.Frame):
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)
    def show(self):
        self.lift()

class Page1(Page):
   def __init__(self, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       
       flighttime = tk.Label(self, text="Flight Time: ", bg="gray", fg="white", anchor="w")
       #flighttime.pack()
       flighttime.place(x=400, y=0)
       flighttime.config(width=35)
       flighttime.config(font=("Courier", 15))
       flighttime.config(padx = 10, pady = 10)
       
       self.duration = tk.Label(self, text="Duration: ", bg="#4f2683", fg="white", anchor="w")
       #self.duration.pack()
       self.duration.place(x=400, y=45)
       self.duration.config(width=35)
       self.duration.config(font=("Courier", 15))
       self.duration.config(padx = 10, pady = 10)
        
       self.maxspeed = tk.Label(self, text="Max. Speed: ", bg="gray", fg="white", anchor="w")
       #self.maxspeed.pack()
       self.maxspeed.place(x=400, y=90)
       self.maxspeed.config(width=35)
       self.maxspeed.config(font=("Courier", 15))
       self.maxspeed.config(padx = 10, pady = 10)
        
       self.avgspeed = tk.Label(self, text="Avg. Speed: ", bg="#4f2683", fg="white", anchor="w")
       #self.avgspeed.pack()
       self.avgspeed.place(x=400, y=135)
       self.avgspeed.config(width=35)
       self.avgspeed.config(font=("Courier", 15))
       self.avgspeed.config(padx = 10, pady = 10)
        
       self.drogue= tk.Label(self, text="Drogue Time: ", bg="gray", fg="white", anchor="w")
       #self.drogue.pack()
       self.drogue.place(x=400, y=180)
       self.drogue.config(width=35)
       self.drogue.config(font=("Courier", 15))
       self.drogue.config(padx = 10, pady = 10)
        
       self.main = tk.Label(self, text="Main Time: ", bg="#4f2683", fg="white", anchor="w")
       #self.main.pack()
       self.main.place(x=400, y=225)
       self.main.config(width=35)
       self.main.config(font=("Courier", 15))
       self.main.config(padx = 10, pady = 10)
        
       self.apogee = tk.Label(self, text="Apogee Time: ", bg="gray", fg="white", anchor="w")
       #self.apogee.pack()
       self.apogee.place(x=400, y=270)
       self.apogee.config(width=35)
       self.apogee.config(font=("Courier", 15))
       self.apogee.config(padx = 10, pady = 10)
       
       def cm2inch(value):
            return value/2.54
        
       fig = plt.figure(figsize=(cm2inch(10.16), cm2inch(8.05)))
       plt.ion()
       t=[1,2,3,4,5,6,7]
       s=[0,0,0,10000,9500,3000,0]
       flightstages=['Ignition','Signal', 'Blast\nOff', 'Apogee', 'Drogue', 'Main', 'Touch\nDown']
       plt.xticks(t,flightstages)
       plt.tick_params(axis='x', which='major', labelsize=7)
       
       for i_t, i_s in zip(t,s):
           plt.text(i_t, i_s, '{}'.format(i_s))
       
       plt.plot(t,s)
       canvas = FigureCanvasTkAgg(fig, master=self)
       plot_widget = canvas.get_tk_widget()
       plot_widget.place(x=0,y=0)
       
       self.gps = tk.Label(self, text="GPS Coordinates of Rocket: \n\n\n\n\n\n\n\n\n\n\n", bg="gray", fg="white", anchor="nw")
       #self.apogee.pack()
       self.gps.place(x=0, y=315)
       self.gps.config(width=32)
       self.gps.config(font=("Courier", 15))
       self.gps.config(padx = 10, pady = 10)       

class Page2(Page):
   def __init__(self, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       
       p_ignition = tk.Label(self).grid(row=0)
       p_signal = tk.Label(self).grid(row=0)
       p_blast = tk.Label(self).grid(row=0)
       p_apogee = tk.Label(self).grid(row=0)
       p_drogue = tk.Label(self).grid(row=1)
       p_main = tk.Label(self).grid(row=1)
       p_touch = tk.Label(self).grid(row=1)
       p_total = tk.Label(self).grid(row=1)
       
       p_ignition = Entry(self)
       p_signal = Entry(self)
       p_blast = Entry(self)
       p_apogee = Entry(self)
       p_drogue = Entry(self)
       p_main = Entry(self)
       p_touch = Entry(self)
       p_total = Entry(self)
       
       p_ignition.grid(row=1,column=1)
       p_signal.grid(row=1,column=2)
       p_blast.grid(row=1,column=3)
       p_apogee.grid(row=1,column=4)
       p_drogue.grid(row=2,column=1)
       p_main.grid(row=2,column=2)
       p_touch.grid(row=2,column=3)
       p_total.grid(row=2,column=4)

class MainView(tk.Frame):
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)

        p1 = Page1(self)
        p2 = Page2(self)
       
        buttonframe = tk.Frame(self)
        container = tk.Frame(self)
        buttonframe.pack(side="bottom", expand=False)
        buttonframe.pack(expand=False)
        buttonframe.configure(width=50)
        container.pack(side="top", fill="both", expand=True)

        p1.place(in_=container, x=0, y=0, relwidth=1, relheight=1)
        p2.place(in_=container, x=0, y=0, relwidth=1, relheight=1)
       
        b1 = tk.Button(buttonframe, text="Page 1", command=p1.lift,width=47,bg="#4f2683",fg="white")
        b2 = tk.Button(buttonframe, text="Page 2", command=p2.lift,width=47,bg="#4f2683",fg="white")
        b2.pack(side="right")
        b1.pack(side="right")

        p1.show()

if __name__ == "__main__":
    root = tk.Tk()
    main = MainView(root)
    main.pack(side="top", fill="both", expand=True)
    root.wm_geometry("800x480+0+0")
    root.mainloop()
