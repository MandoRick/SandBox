from tkinter import *

GBP = 1.00
USD = 1.40
EUR = 1.14
BRL = 4.77
JPY = 151.05
TRY = 5.68
Rate1 = 3.5
Rate2 = 3
Rate3 = 2.5
Rate4 = 2
Rate5 = 1.5


# on change dropdown value
def change_dropdown(*args):
    print( tkvar.get() )

def clickCalculate():
    print("Calculate!")
    global entry    
    string= entry.get()
    var = float(string)    

    
    if tkvar.get() == 'USD':              
        newLabel1 = var * USD
        print(newLabel1)
    if tkvar.get() == 'EUR':         
        newLabel1 = var * EUR
        print(newLabel1)
    if tkvar.get() == 'BRL':        
        newLabel1 = var * BRL
        print(newLabel1)
    if tkvar.get() == 'JPY':      
        newLabel1 = var * JPY
        print(newLabel1)
    if tkvar.get() == 'TRY':     
        newLabel1 = var * TRY
        print(newLabel1)        
    if var >= 0.01 and var <= 300:
        newLabel2 = newLabel1 + (newLabel1 / 100 * 3.5)
    if var >= 300.01 and var <= 750:
        newLabel2 = newLabel1 + (newLabel1 / 100 * 3)
    if var >= 750.01 and var <= 1000:
        newLabel2 = newLabel1 + (newLabel1 / 100 * 2.5)
    if var >= 1000.01 and var <= 2000:
        newLabel2 = newLabel1 + (newLabel1 / 100 * 2)
    if var >= 2000.01 and var <= 2500:
        newLabel2 = newLabel1 + (newLabel1 / 100 * 1.5) 
    text3 = Label(root, text=newLabel1, font=("Courier 16 bold"), bg="#202020", fg="#0088FF")
    text3.place(x=320,y=400)
    text4 = Label(root, text=newLabel2, font=("Courier 16 bold"), bg="#202020", fg="#0088FF")
    text4.place(x=320,y=430)
    
root = Tk()
root.configure(bg='#202020')
root.geometry("800x550")
root.title('Currency Calculator')
mainframe = Frame(root)
mainframe.grid(column=0,row=0, sticky=(N,W,E,S) )
mainframe.columnconfigure(0, weight = 1)
mainframe.rowconfigure(0, weight = 1)
mainframe.pack(pady = 100, padx = 100)
# Create a Tkinter variable
tkvar = StringVar(root)
# Dictionary with options
choices = {'USD','EUR','BRL','JPY','TRY'}
tkvar.set('USD') # set the default option
popupMenu = OptionMenu(mainframe, tkvar, *choices)
Label(mainframe, text="CURRENCY", font=("Courier 16 bold"), bg="#202020", fg="#0088FF").grid(row = 1, column = 1)
popupMenu.grid(row = 2, column =1)
# link function to change dropdown
tkvar.trace('w', change_dropdown)
text1 = Label(root, text="Result:", font=("Courier 12 bold"), bg="#202020", fg="#0088FF")
text1.place(x=200,y=405)
text2 = Label(root, text="With %:", font=("Courier 12 bold"), bg="#202020", fg="#0088FF")
text2.place(x=200,y=435)   
label=Label(root, text="QTY? 0.01 to 2500 GBP", font=("Courier 10 bold"), bg="#202020", fg="#0088FF")
label.pack()
entry= Entry(root, width= 40)
entry.focus_set()
entry.pack()
myButton = Button(root, text="CALCULATE", font=("Courier 16 bold"), command=clickCalculate, bg="#202020", fg="#0088FF")
myButton.pack(pady=25)
root.mainloop()
