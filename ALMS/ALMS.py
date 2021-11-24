from tkinter import *
import csv

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

def clickSearchBookButton():
    print("Search Book Button button pressed")
    root2 = Tk()    
    root2.configure(bg='#202020')
    root2.title('Book Search')
    root2.geometry("800x550")
    w = Label(root2, text ='Book Search', font=("Courier 18 bold"), bg="#202020", fg="#0088FF") 
    w.pack()
    with open('bookData.csv', 'r') as csv_file:        
        csv_reader = csv.reader(csv_file, delimiter=',')        
        for row in csv_reader:
            msg = Message(root2, text = (row[0], row[1], row[2], row[3], row[4]), font=("Courier 12 bold"), bg="#202020", fg="#0088FF", width=700)
            msg.pack()
   

def clickSearchItemButton():
    print("Search Item Button button pressed")
    root3 = Tk()    
    root3.configure(bg='#202020')
    root3.title('Item Search')
    root3.geometry("800x550")
    w = Label(root3, text ='Item Search', font=("Courier 18 bold"), bg="#202020", fg="#0088FF") 
    w.pack()
    with open('itemData.csv', 'r') as csv_file:        
        csv_reader = csv.reader(csv_file, delimiter=',')        
        for row in csv_reader:
            msg = Message(root3, text = (row[0], row[1], row[2], row[3]), font=("Courier 12 bold"), bg="#202020", fg="#0088FF", width=700)
            msg.pack() 

def clickSearchUserButton():
    print("Search User Button button pressed")
    root4 = Tk()    
    root4.configure(bg='#202020')
    root4.title('User Search')
    root4.geometry("800x550")
    w = Label(root4, text ='User Search', font=("Courier 18 bold"), bg="#202020", fg="#0088FF") 
    w.pack()
    with open('userData.csv', 'r') as csv_file:        
        csv_reader = csv.reader(csv_file, delimiter=',')        
        for row in csv_reader:
            msg = Message(root4, text = (row[0], row[1], row[2], row[3], row[4]), font=("Courier 12 bold"), bg="#202020", fg="#0088FF", width=700)
            msg.pack() 

def clickAddUserButton():
    print("Add User Button button pressed")
    root5 = Tk()    
    root5.configure(bg='#202020')
    root5.title('Add User')
    root5.geometry("800x550")
    
def drawMainMenu():
    searchBookButton = Button(root, text="Search Book", font=("Courier 18 bold"), bg="#202020", fg="#0088FF", command=clickSearchBookButton)
    searchItemButton = Button(root, text="Search Item", font=("Courier 18 bold"), bg="#202020", fg="#0088FF", command=clickSearchItemButton)
    searchUserButton = Button(root, text="Search User", font=("Courier 18 bold"), bg="#202020", fg="#0088FF", command=clickSearchUserButton)
    addUserButton = Button(root, text="Add User", font=("Courier 18 bold"), bg="#202020", fg="#0088FF", command=clickAddUserButton)
    # place buttons        
    searchBookButton.place(x=100, y=150)
    searchItemButton.place(x=400, y=150)
    searchUserButton.place(x=100, y=250)
    addUserButton.place(x=400, y=250)

def clearnLoginScreen():
    myLabel = Label(root, text="Login Success!", font=("Courier 16 bold"), bg="#202020", fg="#0088FF")
    myLabel.pack()                
    usernameInput.pack_forget()
    passwordInput.pack_forget()
    myLabel.pack_forget()
    label1.pack_forget()
    label2.pack_forget()
    label3.pack_forget()
    myButton.pack_forget()
    root.configure(bg='#202020')
    root.geometry("1000x800")
    label4=Label(root, text="Automated Library Management System", font=("Courier 22 bold"), bg="#202020", fg="#0088FF")
    label4.pack(pady=10)

def myClick():
    username = usernameInput.get()
    password = passwordInput.get()
    if username == 'user' and password == '123':
        clearnLoginScreen()
        drawMainMenu()

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
root.configure(bg='#202020')
root.geometry("800x550")
root.title('Automated Library Management System')
label1=Label(root, text="Automated Library Management System Login", font=("Courier 22 bold"), bg="#202020", fg="#0088FF")
label1.pack(pady=55)
label2=Label(root, text="Username", font=("Courier 18 bold"), bg="#202020", fg="#0088FF")
label2.pack()
usernameInput = Entry(root, font=("Courier 12 bold"), width=50, bg="#202020", fg="#0088FF", borderwidth=5)
usernameInput.pack(pady=25)
label3=Label(root, text="Password", font=("Courier 18 bold"), bg="#202020", fg="#0088FF")
label3.pack()
passwordInput = Entry(root, font=("Courier 12 bold"), width=50, bg="#202020", fg="#0088FF", borderwidth=5)
passwordInput.pack(pady=25)
myButton = Button(root, text="Sign In", font=("Courier 16 bold"), command=myClick, bg="#202020", fg="#0088FF")
myButton.pack(pady=25)
root.mainloop()
