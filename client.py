#!usr/bin/python
import random
import socket
import sys
import time
from Tkinter import *


def set_time(which):
	global start_time, end_time
	if which == 's':
		start_time = time.time()
	if which == 'e':
		end_time = time.time()

def calc_time():
	global start_time, end_time
	result = end_time - start_time
	return round(result,3)

class GuiFramework:
	
	#define function for making frame to hold all the other elements
	def makeFrame(self, parent, bckgrnd, rel, a, b, ht, width=None):
      		parent.fram = Frame(parent, bg = bckgrnd, relief = rel, height = ht)
      		if width:
         		parent.fram.config(width=width)
      		parent.fram.place(x = a, y = b)
      		return parent.fram

	#define function for making labels
   	def makeLabel(self, parent, rel, brdr, ht, wd, r, c, stky, rs, cs, txt, bckgrnd=None):
      		parent.lbl = Label(parent, relief = rel, height = ht, width = wd, bd = brdr)
      		if txt:
         		parent.lbl.config(text = txt)
      		if bckgrnd:
         		parent.lbl.config(background = bckgrnd)
      		parent.lbl.grid(row = r, column = c, rowspan = rs, columnspan = cs, sticky = stky)
      		return parent.lbl

	#define entry function for creating entry widgets
	def makeEntry(self, parent, rel, wd, brdr, r, c, cs, stky, txtvar):
      		parent.en = Entry(parent, relief = rel, width = wd, bd = brdr)
      		parent.en.grid(row = r, column = c, columnspan = cs, sticky = stky)
      		if txtvar:
         		parent.en.config(textvariable = txtvar)
	 		parent.en.delete(0,END)
	 		parent.en.insert(END, " 0")
      		return parent.en

	#function for button widgets on parent
	def makeButton(self, parent, txt, wd, brdr, a, b, comm=None): 
      		parent.bttn = Button(parent, text = txt, width = wd, bd = brdr)
      		if comm:
         		parent.bttn.config(command = comm)
      		parent.bttn.place(x = a, y = b)
      		return parent.bttn

	#function for calculating and displaying client and server time for all the calculations and response time	
	def show_time(self, parent, a, b, dt=None):
      		parent.static_time = self.makeFrame(parent, None, "ridge", a, b, "40", "100")
      		for i in range(3):
         		if i == 0:
            			y = 2
         		else:
            			y = 5
 			for j in range(y):
            			if i == 0:
               				w = "14"
               				if j == 0:
                 				txt = "Client Total Time: "
               				elif j == 1:
						if dt:
                     					txt = str(calc_time())
						else:
							txt = None
            			else:
					w = "10"
					if i == 1:
						if j == 0:
							txt = "Server Time: "
						if j == 1:
							txt = "Real"
						if j == 2:
							txt = "User"
						if j == 3:
							txt = "System"
						if j == 4:
							txt = "User + Sys"	  
	        			elif i == 2:
						if dt:
							if j == 0:
								txt = None
							if j == 4: # convert int time to real
								txt = str(float(dt[len(dt)-1]) + float(dt[len(dt)-2]))
		  					if j == 3: 
		     						txt = dt[len(dt) - 1]
							if j == 2:
								txt = dt[len(dt) - 2] 
							if j == 1:
								txt = dt[len(dt) - 3] 
						else:
							txt = None
				parent.static_time.st1 = self.makeLabel(parent.static_time, "ridge", "2", "1", w, i, j, NSEW, 1, 1, txt , "#FFFFFF")
                  
	#send response to the server for displaying the starting time in the GUI
	def send_static_parameters(self):
      		try:
        		set_time('s')
      			S.send('s')
      		except socket.error:
			error = Toplevel()
			error.e1 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
      		static_data = S.recv(MSGSIZE)
      		set_time('e')
      		static_data = static_data.split(' ')
      		return static_data
     
	#function to send response to server and calculate all the global parameters to be displayed dynamically
	def calculate(self, parent, default_data):
		self.exit(parent)
		global c1, c2, c3, c4
      		try:
			set_time('s')
      			S.send('c 2yr '+str(c1.get())+' 5yr '+str(c2.get())+' 10yr '+str(c3.get())+' 30yr '+str(c4.get()))
      		except socket.error:
			error = Toplevel()
			error.e2 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
      		dynamic_data = S.recv(MSGSIZE)
      		set_time('e')
		dynamic_data = dynamic_data.split(' ')
      		self.display_right_table(default_data, dynamic_data)

	#function to send response to server to bump the entire tradingbook by 50% up
	def shift_50_up(self, parent, default_data):
		self.exit(parent)
      		try:
        		set_time('s')
      			S.send('u')
      		except socket.error:
			error = Toplevel()
			error.e2 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
      		up_data = S.recv(MSGSIZE)
      		set_time('e')
		up_data = up_data.split(' ')
      		self.display_right_table(default_data, up_data)

	#function to send response to server to bump the entire tradingbook by 50% down
	def shift_50_down(self, parent, default_data):
		self.exit(parent)
      		try:
        		set_time('s')
      			S.send('d')
      		except socket.error:
			error = Toplevel()
			error.e2 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
      		down_data = S.recv(MSGSIZE)
      		set_time('e')
		down_data = down_data.split(' ')
		self.display_right_table(default_data, down_data)

	#send response to server to draw graph
	def call_graph_function(self):
		try:
      	  		S.send('g')
      		except socket.error:
			error = Toplevel()
			error.e2 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
		data = S.recv(MSGSIZE)
		gp.plot_graph(data)
		
	#send response to server to fetch dynamic values to display the table
	def send_dynamic_parameters(self):
      		try:
        		set_time('s')
      			S.send('i')
      		except socket.error:
			error = Toplevel()
			error.e2 = self.makeLabel(error, None, "2", "1", 5, None, None, NSEW, 1, 1, "No Server Response" , None)
			time.sleep(3)
			sys.exit()
        	data = S.recv(MSGSIZE)
		set_time('e')
		data = data.split(' ')
      		self.display_right_table(data)

	
   	def __init__( self, parent ):

      		self.myParent = parent
      		parent.top = self.makeFrame(parent, "WHITE", "ridge", 10, 10, "2", "600")
      		parent.left = self.makeFrame(parent, None, "ridge", 10, 46, "400")
      
      		parent.static_button = self.makeButton(parent, "Display static Information", "20", "3", 695, 580, lambda: self.display_left_table("Go"))
 
      		parent.top.header = self.makeLabel(parent.top, "ridge", "2", "2", "109", 0, 10, None, 1, 60, "Daily Change by Issuer (amounts in 000's)", "white")
      		parent.left.top_row1 = self.makeLabel(parent.left, "ridge", "2", "2", "10", 1, 0, NSEW, 1, 1, None, "#FFFFFF")
      		parent.left.top_row2 = self.makeLabel(parent.left, "ridge", "2", None, "15", 1, 1, NSEW, 1, 3, "Opening Position", "#FFCC99")
      		parent.left.top_row3 = self.makeLabel(parent.left, "ridge", "2", None, "15", 1, 4, NSEW, 1, 3, "Closing Position", "#FFFF99")
      		parent.left.top_row4 = self.makeLabel(parent.left, "ridge", "2", None, "15", 1, 7, NSEW, 1, 3, "Intra-Day Change", "#CCFFCC")
      		self.display_left_table()

      		parent.show_dynamic_content = self.makeButton(parent, "Show dynamic Content", "18", "3", 420, 650, lambda: self.send_dynamic_parameters())
      		parent.show_graph = self.makeButton(parent, "Show Graph", "15", "3", 610, 650, lambda: self.call_graph_function())
      		parent.show_graph = self.makeButton(parent, "Close", "10", "3", 775, 650, lambda: self.exit(parent, "main"))
    

	def display_left_table(self, dt=None):

      		left_counter = 0
      		if dt:
         		data = self.send_static_parameters()
      			self.show_time(self.myParent, 10, 580, data)
		else:
			self.show_time(self.myParent, 10, 580)
      		for i in range(20):
         		for j in range(10):
               			if i == 0:
                  			if j == 0:
                     				self.myParent.left.l1 = self.makeLabel(self.myParent.left, "ridge", "2", None, "12", 5+i, j, NSEW, None, None, "Ticker", "#FFFFFF")
                  			if j in [1,4,7]:
                     				self.myParent.left.l2 = self.makeLabel(self.myParent.left, "ridge", "2", None, "12", 5+i, j, NSEW, None, None, "Notional", "#FFFFFF")
                  			if j in [2,5,8]:
                     				self.myParent.left.l3 = self.makeLabel(self.myParent.left, "ridge", "2", None, "8", 5+i, j, NSEW, None, None, "Risk", "#FFFFFF")
                  			if j in [3,6,9]:
                     				self.myParent.left.l4 = self.makeLabel(self.myParent.left, "ridge", "2", None, "8", 5+i, j, NSEW, None, None, "LGD", "#FFFFFF")    
	       			else:
                  			if j == 0:
                     				if i == 1:
                        				var = "GM"
                     				if i == 2:
                        				var = "FG"
                     				if i == 3:
                        				var = "XY"
		     				if i == 4:
                        				var = "TT"
                     				if i == 5:
                        				var = "YU"
                     				if i == 6:
                        				var = "Quality:"
                     				if i == 7:
                        				var = "AAA"
                     				if i == 8:
                        				var = "AA"
                     				if i == 9:
                        				var = "A"
                     				if i == 10:
                        				var = "BBB"
                     				if i == 11:
                        				var = "BB"
                     				if i == 12:
                        				var = "B"
                     				if i == 13:
                        				var = "CCC"
                     				if i == 14:
                        				var = "CC"
		     				if i == 15:
							var = "C"
		     				if i == 16:
                        				var = "D"
		     				if i == 17:
							var = None
                     				if i == 18:
                        				var = "Investment Grade"
                     				if i == 19:
                        				var = "Junk"
                     				self.myParent.left.l5 = self.makeLabel(self.myParent.left, "ridge", "2", None, "12", 5+i, j, NSEW, None, None, var, "#FFFFFF")
                  			else:
						if dt:
		     					if len(data) > 0 and not i == 17 and not i == 6:
		        					self.myParent.left.l5 = self.makeLabel(self.myParent.left, "ridge", "2", None, "6", 5+i, j, NSEW, None, None, data[left_counter], "#FFFFFF")
								left_counter += 1
		     				else:
		        				self.myParent.left.l5 = self.makeLabel(self.myParent.left, "ridge", "2", None, "6", 5+i, j, NSEW, None, None, None, "#FFFFFF")
		     

		for i in range(3):
         		k = 1
         		for j in range(4):
            			if j == 0:
               				if i == 0:
                  				var = "VaR - Credit Spread"
               				if i == 1:
                  				var = "VaR - Interest Rate"
               				if i == 2:
                  				var = "Total - total"
               				self.myParent.left.l1 = self.makeLabel(self.myParent.left, "ridge", "2", None, "18", 50+i, j, NSEW, None, 1, var, "#FFFFFF")
            			else:
	       				if dt:
						if len(data) > 0:
                  					self.myParent.left.l1 = self.makeLabel(self.myParent.left, "ridge", "2", None, "18", 50+i, k, NSEW, None, 3, data[left_counter], "#FFFFFF")
		  					left_counter += 1
	       				else:
                  				self.myParent.left.l1 = self.makeLabel(self.myParent.left, "ridge", "2", None, "18", 50+i, k, NSEW, None, 3, None, "#FFFFFF")
               				k += 3


	def display_right_table(self, def_data, msg=None):
		parent = Toplevel()
      		right_counter = 0
		msg_counter = 0
		parent.geometry("%dx%d%+d%+d" % (850, 550, 100, 100))
		parent.title("Client Dynamic Information")
      		parent.top1 = self.makeFrame(parent, "WHITE", "ridge", 20, 20, 2, "100")
      		parent.right = self.makeFrame(parent, "WHITE", "ridge", 20, 55, 2, "400")
      		parent.top1.header1 = self.makeLabel(parent.top1, "ridge", "2", "2", "99", 0,  0, None, 1, 4, "Risk by Maturity Bucket (amounts in 000's)", "white")
		parent.top_row1 = self.makeLabel(parent.right, "ridge", "2", "2", "14", 2, 0, NSEW, 1, 2, "Book", "#FFFFFF")
		parent.top_row2 = self.makeLabel(parent.right, "ridge", "2", "2", "12", 2, 2, NSEW, 1, 2, "2YR", "#FFCC99")
		parent.top_row3 = self.makeLabel(parent.right, "ridge", "2", "2", "12", 2, 4, NSEW, 1, 2, "5YR", "#FFFF99")
		parent.top_row4 = self.makeLabel(parent.right, "ridge", "2", "2", "12", 2, 6, NSEW, 1, 2, "10YR", "#CCFFCC")
		parent.top_row5 = self.makeLabel(parent.right, "ridge", "2", "2", "12", 2, 8, NSEW, 1, 2, "30YR", "#CCFFFF")

		for i in range(5):
         		for j in range(9):
            			if i == 0:
					if j == 0:
						parent.l1 = self.makeLabel(parent.right, "ridge", "1", None, "14", 5+i, j, NSEW, None, 2, None, "#FFFFFF")
		  			if j in [1,3,5,7]:
                      				parent.l4 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, None, 1, "Risk", "#FFFFFF")
		  			elif j in [2,4,6,8]:
		      				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, None, 1, "Mkt Value", "#FFFFFF")
            			
				elif i == 1:
					if j == 0:
						parent.l1 = self.makeLabel(parent.right, "ridge", "1", None, "14", 5+i, j, NSEW, None, 2, "Closing Position", "#FFFFFF")
	      	      			elif j in [1,3,5,7]:
						if def_data[right_counter]:
		      	   				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, 1, 1, def_data[right_counter], "#FFFFFF")
							right_counter += 1
						else:
							parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, 1, 1, None, "#FFFFFF")
		      			elif j in [2,4,6,8]:
						if def_data[right_counter]:
		      	   				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1, def_data[right_counter], "#FFFFFF") 	
		      	   				right_counter += 1 
						else:
							parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1, None, "#FFFFFF") 

	    			elif i == 2:
					if j == 0:
						parent.l1 = self.makeLabel(parent.right, "ridge", "1", None, "14", 5+i, j, NSEW, None, 2, "2yr Hedge", "#FFFFFF")
		      			elif j in [2,4,6,8]:
						if def_data[right_counter]:
		           				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1,def_data[right_counter], "#FFFFFF")
							right_counter += 1
						else:
							parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1,None, "#FFFFFF")
		      			elif j in [1,3,5,7]:
		           			parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, 1, 1, None, "#FFFFFF")
 	    			elif i == 3:
					if j == 0:
						parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "14", 5+i, j, NSEW, None, 2, "Shifted Position", "#FFFFFF")
		      			else:
						if msg:
		           				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1, msg[msg_counter], "#FFFFFF")
							msg_counter += 1
						else:
							parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1,None, "#FFFFFF")
		      		
				elif i == 4:
					if j == 0:
						parent.l1 = self.makeLabel(parent.right, "ridge", "1", None, "14", 5+i, j, NSEW, None, 2, "Shifted 2yr Hedge", "#FFFFFF")
					elif j in [2,4,6,8]:
						if msg:
		           				parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1, msg[msg_counter], "#FFFFFF")
							msg_counter += 1
						else:
							parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "10", 5+i, j+1, NSEW, 1, 1,None, "#FFFFFF")
		      			else:
		           			parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, 1, 1, None, "#FFFFFF")

				else:
		  			parent.l5 = self.makeLabel(parent.right, "ridge", "1", None, "6", 5+i, j+1, NSEW, 1, 1, None, "#FFFFFF")						
			              

		for i in range(2):
         		k = 1
         		for j in range(9):
            			if j == 0:
               				if i == 0:
                  				var = "Yield Curve"
               				if i == 1:
                  				var = "Bump Curve"
               				parent.l1 = self.makeLabel(parent.right, "ridge", "1", None, "14", 15+i, j, NSEW, None, 2, var, "#FFFFFF")
            			elif j in [1,3,5,7]:
                     			parent.l2 = self.makeLabel(parent.right, "raised", "1", None, "6", 15+i, j+1, NSEW, None, 1, None)
	       			else:
					if i == 0:
		     				parent.l2 = self.makeLabel(parent.right, "ridge", "1", None, "10", 15+i, j+1, NSEW, 1, 1, k, "#FFFFFF")
		     				k += 1
					elif i == 1:
		     				if j == 2:
		     					parent.l1 = self.makeEntry(parent.right, "ridge", "10", "1", 15+i, j+1, 1, NSEW, c1)
		     				if j == 4:
		     					parent.l1 = self.makeEntry(parent.right, "ridge", "10", "1", 15+i, j+1, 1, NSEW, c2)
		     				if j == 6:
		     					parent.l1 = self.makeEntry(parent.right, "ridge", "10", "1", 15+i, j+1, 1, NSEW, c3)
	             				if j == 8:
		     					parent.l1 = self.makeEntry(parent.right, "ridge", "10", "1", 15+i, j+1, 1, NSEW, c4)


		parent.dynamic_button = self.makeButton(parent, "Calculate", "10", "3", 705, 270, lambda: self.calculate(parent, def_data))
		parent.shift_up = self.makeButton(parent, "Shift 50% Up", "13", "3", 380, 320, lambda: self.shift_50_up(parent, def_data))
		parent.shift_down = self.makeButton(parent, "Shift 50% Down", "13", "3", 530, 320,  lambda: self.shift_50_down(parent, def_data))
		parent.back_to_original = self.makeButton(parent, "Back to Original", "13", "3", 682, 320, lambda: self.exit(parent, "original"))
		if msg:
			self.show_time(parent, 20, 400, msg)
		else:
			self.show_time(parent, 20, 400)
		parent.close_window = self.makeButton(parent, "Close", "13", "3", 680, 430, lambda: self.exit(parent))

	#function to destroy the frame in order to create a new one
	def exit(self, parent, msg=None):
		if msg == "original":
			self.send_dynamic_parameters()
		if msg == "main":
      			S.send('x')
			S.close()
      		parent.destroy() 
   
if __name__ == "__main__":
	#set up server
	import os
	os.system("chmod 700 run.sh")
	os.system("./run.sh &")
	time.sleep(0.01)

	#set up client connection
	HOST = socket.gethostname()
	PORT = 0x1234
	MSGSIZE = 8192
	S = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	S.connect((HOST,PORT))

	root = Tk()
	#global parameters to hold n-yr bucket's values
	c1 = StringVar()
	c2 = StringVar()
	c3 = StringVar() 
	c4 = StringVar()
	start_time = 0.00
	end_time = 0.00
	import graph3 as gp

	root.title('Client')
	root.geometry("%dx%d%+d%+d" % (900, 700, 0, 0))
	GuiFrame = GuiFramework(root)
	root.mainloop()
