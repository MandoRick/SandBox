from tkinter import *
import tkinter
import tkinter.messagebox

class Window(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master)        
        self.master = master
        self.pack(fill=BOTH, expand=1)
        # create buttons
        exitButton = Button(self, text="Exit", command=self.clickExitButton)
        searchBookButton = Button(self, text="Search Book", command=self.clickSearchBookButton)
        searchItemButton = Button(self, text="Search Item", command=self.clickSearchItemButton)
        searchUserButton = Button(self, text="Search User", command=self.clickSearchUserButton)
        addUserButton = Button(self, text="Add User", command=self.clickAddUserButton)
        # place buttons
        exitButton.place(x=300, y=550)
        searchBookButton.place(x=100, y=50)
        searchItemButton.place(x=400, y=50)
        searchUserButton.place(x=100, y=150)
        addUserButton.place(x=400, y=150)
        
    def clickSearchBookButton(self):
        print("admin name:", a1.firstname, a1.lastname,)
        print("admin email:", a1.email)
        print("admin pass:", a1.password)
        print("admin name:", a2.firstname, a2.lastname,)
        print("admin email:", a2.email)
        print("admin pass:", a2.password)
        
        
    def clickSearchItemButton(self):
        print("Item Number:", i1.idnum)
        print("Item Status:", i1.status)
        print("Item Location:", i1.location)
        print("Item Title:", i1.title)
        print("Item Author:", i1.author)
        print("Item Category:", i1.category)
        print("Item Number:", i2.idnum)
        print("Item Status:", i2.status)
        print("Item Location:", i2.location)
        print("Item Title:", i2.title)
        print("Item Author:", i2.author)
        print("Item Category:", i2.category)
        
    def clickSearchUserButton(self):
        print("name: ", u1.firstname, u1.lastname)
        print("postcode: ", u1.postcode)
        print("phone: ", u1.phonenum)
        print("email: ", u1.email)
        print("name: ", u2.firstname, u2.lastname)
        print("postcode: ", u2.postcode)
        print("phone: ", u2.phonenum)
        print("email: ", u2.email)
        
    def clickAddUserButton(self): 
        def buttonClick():
            tkinter.messagebox.showinfo('success!', 'user added successfully!')
            #tkinter.messagebox.showwarning('title', 'message')
            #tkinter.messagebox.showerror('title', 'message')
 
        root=tkinter.Tk()
        root.title('GUI')  
        root.geometry('200x200')  
        root.resizable(False, False)  
        tkinter.Button(root, text='add user',command=buttonClick).pack()
        root.mainloop()
        
    def clickExitButton(self):
        exit()        







class User:
    def __init__(self, firstname, lastname, postcode, phonenum, email):
        self.firstname = firstname
        self.lastname = lastname
        self.postcode = postcode
        self.phonenum = phonenum
        self.email = email
        
class Admin:
    def __init__(self, firstname, lastname, password, email):
        self.firstname = firstname
        self.lastname = lastname
        self.password = password
        self.email = email
        
class Item:
    def __init__(self, idnum, status, location, title, author, category):
        self.idnum = idnum
        self.status = status
        self.location = location
        self.title = title
        self.author = author
        self.category = category

class Borrow:
    def __init__(self, idnum, status, location, title, author, category):
        self.idnum = idnum
        self.status = status
        self.location = location
        self.title = title
        self.author = author
        self.category = category

class Return:
    def __init__(self, idnum, status, location, title, author, category):
        self.idnum = idnum
        self.status = status
        self.location = location
        self.title = title
        self.author = author
        self.category = category

u1 = User("Richard", "Bakker", "B980BQ", "01527454343", "atorque@hotmail.co.uk")
u2 = User("Jim", "Jones", "B970BE", "01523455512", "jjones@hotmail.co.uk")
a1 = Admin("John", "Doe", "1234", "johnDoe@hotmail.com")
a2 = Admin("Jane", "Doe", "5678", "janeDoe@hotmail.com")
i1 = Item("123456789", "out", "out", "to kill a mockingbird", "Harper Lee", "Southern Gothic")
i2 = Item("987654321", "in", "section 3, shelf 1", "the animal farm", "George Orwell", "Political Satire")
b1 = Borrow("123456789", "out", "out", "to kill a mockingbird", "Harper Lee", "Southern Gothic")
b2 = Borrow("987654321", "in", "section 3, shelf 1", "the animal farm", "George Orwell", "Political Satire")
r1 = Return("123456789", "out", "out", "to kill a mockingbird", "Harper Lee", "Southern Gothic")
r2 = Return("987654321", "in", "section 3, shelf 1", "the animal farm", "George Orwell", "Political Satire")

root = Tk()
app = Window(root)
root.wm_title("Automated Library Management System")
root.geometry("600x600")
root.mainloop()