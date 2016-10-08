#!/usr/bin/env python3
###################################################################
#
#   CSSE1001/7030 - Assignment 1
#
#   Student Username: s4372218
#
#   Student Name: Akira Kitamura
#
###################################################################

#####################################
# Support given below - DO NOT CHANGE
#####################################

from assign1_support import *

#####################################
# End of support 
#####################################

# Add your code here

# I have to write something to discribe this.


def load_data(filename):
    """
    Open file and return a list separated each line data.

    load_data(filename) -> [('str', [floats]),('str', [floats]),('str',[folats])]
    """

    file = open(filename,'r')
    a = []
    for i in file:
        b = i.split(',')
        c = []
        for n in b[1:]:
            c += [float(n)]
        a.append((b[0],c))

    file.close()
    return a


def get_ranges(data):
    """
    Arrange numbers of list from minimum to maximum
    and then return minimum and muximum numbers.

    get_ranges([floats]) -> (minimum number, maximum number)
    """

    data.sort()
    a = data
    return a[0], a[-1]


def get_mean(data):
    """
    Maltiple by data
    and divide it by the numbers of the elements.

    get_mean([floats]) -> average number
    """
    
    sum = 0
    for i in data:
        sum += i
    return sum/len(data)



def get_median(data):
    """
    Arrange list of numbers
    and return the number of center of the list depending on the numbers of elements;odd and even

    get_median([floats]) -> middle number
    """
    
    data.sort()
    a = data
    if len(a)%2 == 0:
        return (a[len(data)//2] + a[len(data)//2 -1])/2
    else:
        return a[len(data)//2 ]


def get_std_dev(data):
    """
    Return the standard deviation following the formula
    (Substract average number from each values and to the power of 2
    and add all of them then divide it the numvbers of elements
    finally the number to the power of 1/2)

    get_std_dev([floats]) -> standard deviation number
    """

    a = 0
    for i in data:
        a += (i - get_mean(data))**2
    b = a / len(data)
    c = b**(1/2)
    return c


def data_summary(data):
    """
    Output the list of each function numbers which were done above.
    Make all elements that we need
    and put them in tuple depending on each person
    then put in one list

    data_summary([('str', [floats]),('str', [floats]),('str',[folats])])
    -> [('str', floats), ('str', floats),('str', floats)]
    """
    
    summary = []
    for i in data:
        a = i[0]
        b = len(i[1])
        c = get_mean(i[1])
        d = get_median(i[1])
        e = i[1][0]
        f = i[1][-1]
        g = get_std_dev(i[1])
        h = (a,b,c,d,e,f,g)
        summary.append(h)
    return summary


def display_set_summaries(data):
    """
    Output data summaries as a table.
    Make the list of the each line's name
    and print str and number from each tuple in turn.

    display_set_summaries([('str', floats), ('str', floats),('str', floats)])
    -> table
    """
    
    print("Set Summaries")
    print("")
    
    a = ["", "Count:","Mean:","Median:","Minimum:","Maximum:","Std Dev:"]
    display_with_padding(a[0])
    for n in range(len(data)):
        display_with_padding(data[n][0])
    print("")
    
    for i in range(1,7):
        display_with_padding(a[i])
        for n in range(len(data)):
            display_with_padding(round(data[n][i], 2))
        print("")
        




def interact():
    """
    Make user interface function.
    Print welcome message and promote user to enter a filename.
    If the filename is animal_heights.csv, promote user to enter a command.
    Output something depending on the command.
    """

    
    filename = input("Welcome to the Statistic Summariser\n\nPlease enter the data source file: ")
    data = load_data(filename)
    while True:
        print("")
        command = input("Command: ")
        suma = data_summary(data)
        args = command.split()

        if args[0] == "summary" and len(args) == 1:
            display_set_summaries(suma)

        elif args[0] == "sets" and len(args) >= 2:
            new_suma = []
            for i in args[1:]:
                new_suma.append(suma[int(i)])
            display_set_summaries(new_suma)
            

        elif args[0] == "q" and len(args) == 1:
            break

        elif args[0] == "comp" and len(args) == 3:
            set1 = int(args[1])
            set2 = int(args[2])
            display_comparison(data_comparison(data, set1, set2))

        else:
            print("Unknown command: "+ command)
            
        




def data_comparison(data, set1, set2):
    """
    Make a function to compare set1 with set2 about mean and standard deviation.
    Firstly, make 3 elements of name, mean and standard deviation.
    then, put them together into 1 blanket.

    data_comparison([('str', [floats]),('str',[folats])])
    -> ((names),(means),(standard deviations))
    """
    
    a = ()
    b = ()
    c = ()
    d = ()

    a = data[set1][0], data[set2][0]

    sum1 = 0
    sum2 = 0
    shared_mean = 0
    for i in data[set1][1]:
        sum1 += i
    for n in data[set2][1]:
        sum2 += n
    shared_mean = (sum1 + sum2) /(len(data[set1][1]) + len(data[set2][1]))
    element1 = get_mean(data[set1][1])-shared_mean
    element2 = get_mean(data[set2][1])-shared_mean
    b = shared_mean, element1, element2

    x = 0
    for i in data[set1][1]:
        x += (i - shared_mean)**2
    for n in data[set2][1]:
        x += (n - shared_mean)**2
    shared_std_dev = (x / (len(data[set1][1]) + len(data[set2][1])))**(1/2)
    c = shared_std_dev, get_std_dev(data[set1][1]), get_std_dev(data[set2][1])

    d = a, b, c
    return d


def display_comparison(data):
    """
    Output comparison data as a table.
    Make each line

    display_comparison((names),(means),(standard deviations))
    -> a table
    """

    print("Comparison between " + data[0][0] + " and " + data[0][1])
    print("")
    
    s = "S"
    display_with_padding(" ")
    display_with_padding("Mean")
    display_with_padding("Std Dev")
    print("")
    
    display_with_padding("Shared (S)")
    display_with_padding(round(data[1][0], 2))
    display_with_padding(round(data[2][0], 2))
    print("")
    
    display_with_padding(data[0][0])
    if data[1][1] >= 0:
        display_with_padding(s +'+'+ str(round(data[1][1], 2)))
    else:
        display_with_padding(s + str(round(data[1][1], 2)))
    display_with_padding(round(data[2][1], 2))
    print("")

    display_with_padding(data[0][1])
    if data[1][2] >= 0:
        display_with_padding(s +"+"+ str(round(data[1][2], 2)))
    else:
        display_with_padding(s + str(round(data[1][2], 2)))
    display_with_padding(round(data[2][2], 2))
    print("")






#def interact():
    #I just make a function of add numbers
    #pass


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

if __name__ == '__main__':
    interact()
