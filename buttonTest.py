from tkinter import *

class Window(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master)        
        self.master = master

        # widget can take all window
        self.pack(fill=BOTH, expand=1)

        # create button, link it to clickExitButton()
        exitButton = Button(self, text="Exit", command=self.clickExitButton)
        addUserButton = Button(self, text="Add User", command=self.clickAddUserButton)
        addBookButton = Button(self, text="Add Book", command=self.clickAddBookButton)

        # place button at (0,0)
        addUserButton.place(x=0, y=10)
        addBookButton.place(x=0, y=50)
        exitButton.place(x=0, y=90)
        

    def clickExitButton(self):
        exit()
        
    def clickAddUserButton(self):
        exit()
        
    def clickAddBookButton(self):
        exit()
        
root = Tk()
app = Window(root)
root.wm_title("Tkinter button")
root.geometry("320x200")
root.mainloop()