from assign1_support import *

display_with_padding("print")
display_with_padding("dadasd")
display_with_padding("aaaaa")
print("")

display_with_padding("hello")
display_with_padding("lol")
display_with_padding("matt")
print('')
for i in range(0,11):
    print(i)




def interact():
    print("Welcome to the Summariser")
    print("Please enter the data source file: animal_heights.csv")
##    data = load_data('animal_heights.csv')
    command = input("Command: ")
    if command == "a":
        print("got it")

print(interact())
