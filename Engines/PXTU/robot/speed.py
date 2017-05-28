'''
This module creates the instances of the Speed partition
'''

###IMPORTS###
import sqlite3
from signal_class import signal

class Speed(object):

    def __init__(self):

        print "Building Speed symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'speed'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}')".format(each[0],each[0],each[1],each[2])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Speed Symbols added successfully\n'

        connection.close()

###CLEAN-UP###    
#del cursor,connection,sqlite3,signal,count,temp_string,each
