'''
This module creates the instances of the Power partition
'''

###IMPORTS###
import sqlite3
from signal_class import signal

print "Building Wheel Power symbols..."
connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'power'")
count = 0

for each in cursor:        
    temp_string = "{} = signal('{}',{},'{}')".format(each[0],each[0],each[1],each[2]) #replace this with signal class
    #print temp_string #debug
    exec(temp_string)
    count += 1

if count == 0:
    print "Warning: No symbols found"
else:
    print str(count) + ' Power Symbols added successfully\n'

connection.close()

###CLEAN-UP###    
del cursor,connection,sqlite3,signal,count,temp_string,each
