#!/usr/bin/env python3
###################################################################
#
#   CSSE1001/7030 - Assignment 2
#
#   Student Username: s4372218
#
#   Student Name: Akira Kitamura
#
###################################################################

#####################################
# Support given below - DO NOT CHANGE
#####################################

from assign2_support import *

#####################################
# End of support 
#####################################

# Add your code here

from statistics import *

"""set up AnimalData to prepaire for using Animal data in GUI layout"""
class AnimalData(object):
    
    def __init__(self):
        self._data= {}
        self._filename = []
        self._select =[]        

    """set up load file function"""    
    def load_data(self, filename):
        #call AnimalDataSet class from support.py
        dataset = AnimalDataSet(filename)
        #set up a dictionary {animal name: animaldataset}
        self._data[dataset.get_name()]= dataset
        self._filename.append(dataset.get_name())
        #set up select list to search weather it is activating or not
        #Default is "T"
        self._select.append("T")
        
    """set up animal name list"""
    def get_animal_names(self):
        return self._filename

    """set up getting a particular value of the dictionary"""
    def get_animal(self, animal):
        return self._data.get(animal)
    
    """set up a function for serching the condition whether "T" or "F" """
    def is_selected(self, index):
        if self._select[index]=="T":
            return True
        else:
            return False

    """set up to change "T" """
    def select(self, index):
        self._select[index] = "T"
        
    """set up to change "F" """
    def deselect(self, index):
        self._select[index] = "F" 

    """set up to get ranges of animal data of "T" by comparing data""" 
    def get_ranges(self):
        emptytuple = ()
        #set up default, infinite nubers and 0
        min_height = float("inf")
        max_height = 0
        min_weight = float("inf")
        max_weight = 0
        #check how many numbers(-1) of animal data are loaded
        index = len(self._filename)-1
        #if true, it means animal dataset exists
        #if "T", serch max and min of height and weight
        while index >= 0:
            if self._select[index] == "T":
                i = self._filename[index]
                heights = self._data[i].get_height_range()
                weights = self._data[i].get_weight_range()
                if heights[0] < min_height:
                    min_height = heights[0]
                if heights[1] > max_height:
                    max_height = heights[1]
                if weights[0] < min_weight:
                    min_weight = weights[0]
                if weights[1] > max_weight:
                    max_weight = weights[1]
            index += -1

        #if data do not exist, return None
        if min_height == 0:
            min_height = None
            max_height = None
            min_weight = None
            max_weight = None

        emptytuple = min_height, max_height, min_weight, max_weight
        return emptytuple

    """set up animal information of animal name, numbers and condition"""
    def to_tabbed_string(self, index):
        #use get_animal_names function to get index's animal name
        animalname = self.get_animal_names()[index]
        #use get_animal function to use AnimalDataSet.get_data_points function
        numbers = len(self.get_animal(animalname).get_data_points())
        if self._select[index] == "T":
            return str(animalname)+"\t".expandtabs()+str(numbers)+"\t".expandtabs()+"Visible"
        else:
            return str(animalname)+"\t".expandtabs()+str(numbers)+"\t".expandtabs()+"Hidden"
       

"""set up Plotter class to draw plots of the animal, mouse chaser and
the coordinates of the point.
inheriting from tk.Canvas"""
class Plotter(tk.Canvas):
    """set up init to define basic data of canvas and to use functions from AnimalDataPlot class"""
    def __init__(self, frame4, animaldata, label2):
        super().__init__(frame4, bg="white", relief=tk.SUNKEN)
        self._animaldata=animaldata
        self._label2=label2
        #set up <Motion>, <Leave> and <Configure> for the mouse chaser and resize canvas
        self.bind("<Motion>", self.motion)
        self.bind("<Leave>", self.leave)
        self.bind("<Configure>", self.configure)
        self._coordinates=None

        

    """set up for checking weather animaldata data is loaded or not"""
    def existdata(self):
        if self._animaldata.get_animal_names():
            return True
        else:
            return False


    """set up the mouse chaser command"""
    def motion(self, e):
        self.delete(tk.ALL)
        #if no data is exist, the mouse chaser does not work
        if self.existdata() == False:
            pass
        #if animal data is exist and coordinates is on the canvas, it works
        else:
            if self._coordinates is not None:
                self.delete(tk.ALL)
                self._label2.config(text="Height: {0:0.2f} cm, Weight: {1:0.2f} kg".format\
                                    (self._coordinates.get_height(e.x),\
                                     self._coordinates.get_weight(e.y)))
                #tag lines as "line" for leave function
                self._x=self.create_line((e.x, 0),(e.x, self.winfo_height()),tag="line")
                self._y=self.create_line((0, e.y),(self.winfo_width(),e.y),tag="line")
                self.resize(self._animaldata)


    """set up leave function for deleting lines"""
    def leave(self, even):
        self.delete("line")

    """set up configure function to resize the canvas"""
    def configure(self, e):
        self.delete(tk.ALL)
        self.resize(self._animaldata)
        


    """set up canvas size depending on ranges by using CoordinateTranslactor from suppot.by."""
    def resize(self,animaldata):
        xrange_yrange = self._animaldata.get_ranges()
        if xrange_yrange:
            self._coordinates=CoordinateTranslator(self.winfo_width(),\
                                                    self.winfo_height(),\
                                                    xrange_yrange[0],\
                                                    xrange_yrange[1],\
                                                    xrange_yrange[2],\
                                                    xrange_yrange[3])
            #make sure to do drawplot function, after resizing canvas
            self.drawplot()


    """set up drawing plots on canvas"""
    def drawplot(self):
        plots = []
        index = 0
        #get animal name list from AnimalData class
        animallist = self._animaldata.get_animal_names()
        #create loop for getting animal data points of each animal
        for i in animallist:
            animaldataset = self._animaldata.get_animal(i)
            animaldatasetpoints = animaldataset.get_data_points()
            #check the condition of the animal
            if self._animaldata.is_selected(index) == True:
                #create loop for drawing each rectangle
                for v in animaldatasetpoints:
                    eachcoordinate = self._coordinates.get_coords(v[0], v[1])
                    plots.append(self.create_rectangle\
                                  (eachcoordinate[0],\
                                   eachcoordinate[1],\
                                   eachcoordinate[0]+5,\
                                   eachcoordinate[1]+5,\
                                   outline=COLOURS[index],\
                                   fill=COLOURS[index]))
            index += 1



"""set up SelectionBox class to look at animal name, numbers and condition
inheriting from tk.Listbox"""        
class SelectionBox(tk.Listbox):
    def __init__(self, frame3, animaldata):
        super().__init__(frame3)
        self._animaldata = animaldata

    """insert animal information into the Listbox"""    
    def insertanimal(self):
        #delete listbox contents for insert animal information each time
        self.delete(0, tk.END)
        #check how many kinds of animals are loaded
        animalkinds = len(self._animaldata.get_animal_names())
        for i in range(animalkinds):
            animalinfo=self._animaldata.to_tabbed_string(i)
            self.insert(tk.END, animalinfo)
            #change the colours of each animal
            self.itemconfig(tk.END, fg=COLOURS[i])
        
        

        

        
"""set up AnimalDataPlotApp class as the top-level class for managing other classes""" 
class AnimalDataPlotApp(object):

    def __init__(self, master):
        master.title("AnimalDataPlotApp")
        self._master = master

        # set up four Frames
        self._frame1 = tk.Frame(master)
        self._frame1.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)

        self._frame2 = tk.Frame(master)
        self._frame2.pack(side=tk.RIGHT,fill=tk.BOTH, expand=1)

        self._frame3 = tk.Frame(self._frame1, relief=tk.SUNKEN)
        self._frame3.pack(side=tk.BOTTOM,fill=tk.BOTH, expand=1)

        self._frame4 = tk.Frame(self._frame2)
        self._frame4.pack(side=tk.BOTTOM,fill=tk.BOTH, expand=1)
        

        # set up two Labels (later)
        self._label1 = tk.Label(self._frame1, text ="Animal Data Sets")
        self._label1.pack(side=tk.TOP)
        
        self._label2 = tk.Label(self._frame2)
        self._label2.pack(side=tk.TOP)
        


        # set up three Buttons
        self._button1 = tk.Button(self._frame1, text ="Select", command=self.select)
        self._button2 = tk.Button(self._frame1, text ="Deselect", command=self.deselect)
        self._button3 = tk.Button(self._frame1, text ="Summary", command=self.summary)
        
        self._button1.pack(side=tk.LEFT, anchor="nw",expand=1,fill=tk.X)
        self._button2.pack(side=tk.LEFT, anchor="nw",expand=1,fill=tk.X)
        self._button3.pack(side=tk.LEFT, anchor="nw",expand=1,fill=tk.X)

        
        # set up file menu
        menubar = tk.Menu(master)
        # set up menubar for telling master what it's menu
        master.config(menu=menubar)
        filemenu = tk.Menu(menubar)
        menubar.add_cascade(label="File", menu=filemenu)
        filemenu.add_command(label="Open", command=self.open_file)

        
        # set up conponents of other classes
        self._animaldata= AnimalData()
        self._plotter=Plotter(self._frame4, self._animaldata, self._label2)
        self._plotter.pack(fill=tk.BOTH, expand =1)
        self._selectionbox = SelectionBox(self._frame3, self._animaldata)
        self._selectionbox.pack(expand=1,fill=tk.BOTH)
        self.summarywindow = None
        self._selectionbox.bind("<<ListboxSelect>>", self.summary2)

        


    """set up open file function for using loaded data"""
    def open_file(self):
        filename = filedialog.askopenfilename()
        base = os.path.basename(filename)
        newfilename = base.replace(".csv", "")
        animalname = self._animaldata.get_animal_names()
        try:
            filename[-4:] == ".csv"
            #compare loaded animal names with new animal name for prevent overlapping
            if newfilename in animalname:
                pass
            #if not overlapping, then load and draw
            else:
                self._animaldata.load_data(filename)
                #make sure to refresh
                self._plotter.delete(tk.ALL)
                self._plotter.resize(self._animaldata)
                self._selectionbox.insertanimal()
        except ValueError:
            #error display
            messagebox.showerror("File Error",filename+ " is an invalid　Animal Data file")
        except FileNotFoundError:
            pass

    """ set up select the button command to activate the animal"""
    def select(self):
        if self._selectionbox.curselection() == ():
            messagebox.showerror("Error Message", "No data set is selected.")
        else:
            #check which animal index is selected by using curselection function
            index = self._selectionbox.curselection()[0]
            print(self._selectionbox.curselection())
            self._animaldata.select(index)
            self._selectionbox.insertanimal()
            self._plotter.delete(tk.ALL)
            self._plotter.resize(self._animaldata)
            
    """ set up deselect the button command not to activate the animal"""
    def deselect(self):
        self._plotter.delete(tk.ALL)
        if self._selectionbox.curselection() == ():
            messagebox.showerror("Error Message", "No data set is selected.")
        else:
            index = self._selectionbox.curselection()[0]
            self._animaldata.deselect(index)
            self._selectionbox.insertanimal()
            self._plotter.delete(tk.ALL)
            self._plotter.resize(self._animaldata)

    """When click the summary button choosing an animal on the listbox,
    then the animal summary window will show up"""
    def summary(self):
        if self._selectionbox.curselection() == ():
            messagebox.showerror("Error Message", "No data set is selected.")
        else:
            #if summary file does not exist, create it
            if self.summarywindow == None or not self.summarywindow.winfo_exists():                
                self.summarywindow = SummaryWindow(self._master, self._animaldata, self._selectionbox)
                self.summarywindow.command()
            #if already summary file exists, then destroy it and create again
            elif self.summarywindow.winfo_exists():
                self.summarywindow.destroy()
                self.summarywindow = SummaryWindow(self._master, self._animaldata, self._selectionbox)
                self.summarywindow.command()

    """If summary already show up and then the user click a listbox,
    then change the contents of summary window depending on animal."""
    def summary2(self, a):
        try:
            event =a.widget
            index = event.curselection()[0]
            if self._animaldata.is_selected(index) == True:
                for widget in self.summarywindow.winfo_children():
                    widget.destroy()
                self.summarywindow.command()
        except Exception:
            pass
    
        
                
        
        
        
"""set up SummaryWindow class to show summary data of selected animal
inherit from Toplevel"""
class SummaryWindow(tk.Toplevel):
    
    """set up references classes"""
    def __init__(self, master, animaldata, selectionbox):
        super().__init__(master)
        self.title("Animaldata Summary")
        self._animaldata = animaldata
        self._selectionbox = selectionbox
        
    """check the index of the animal on the listbox"""
    def index(self):
        return self.index

    """set up main command to show summary data"""
    def command(self):
        self.index = self._selectionbox.curselection()[0]
        #set up animal name for 1st line
        self.animalname = self._animaldata.get_animal_names()[self.index]
        
        #set up the numbers of points for 2nd line
        self.animaldataset = self._animaldata.get_animal(self.animalname)
        self.alldata = self.animaldataset.get_data_points()
        self.points = len(self.alldata)
        
        #set up means and stds for 3rd, 4th, 5th and 6th line with statistics library
        self.allweightlist = []
        self.allheightlist = []
        for v in range(self.points):
            self.allweightlist.append(self.alldata[v][1])
            self.allheightlist.append(self.alldata[v][0])

        
        self.label1= tk.Label(self, text="Animal: {0}".format(self.animalname))
        self.label1.pack()
        self.label2= tk.Label(self, text="Data points: {0}".format(self.points))
        self.label2.pack()
        self.label3= tk.Label(self, text="Weight mean: {0:.2f}".format(mean(self.allweightlist)))
        self.label3.pack()
        self.label4= tk.Label(self, text="Height mean: {0:.2f}".format(mean(self.allheightlist)))
        self.label4.pack()
        self.label5= tk.Label(self, text="Weight std dev: {0:.2f}".format(stdev(self.allweightlist)))
        self.label5.pack()
        self.label6= tk.Label(self, text="Height std dev: {0:.2f}".format(stdev(self.allheightlist)))
        self.label6.pack()

        


    




##################################################
# !!!!!! Do not change (or add to) the code below !!!!!
# 
# This code will run the interact function if
# you use Run -> Run Module  (F5)
# Because of this we have supplied a "stub" definition
# for interact above so that you won't get an undefined
# error when you are writing and testing your other functions.
# When you are ready please change the definition of interact above.
###################################################

def main():
    root = tk.Tk()
    app = AnimalDataPlotApp(root)
    root.geometry("800x400")
    root.mainloop()

if __name__ == '__main__':
    main()
