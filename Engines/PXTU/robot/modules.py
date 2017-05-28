'''
This module creates the instances of the USR partition
'''

###IMPORTS###
import sqlite3
from signal_class import signal

#print "Importing modules..."

class IR(object):

    def __init__(self,sockt):

        print "Building IR symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db") # TODO: Remove the hardcoded path.
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'ir'")
        count = 0

        for each in cursor:            
            temp_string = "self.{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2]) #replace this with signal class
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' IR Symbols added successfully\n'

        connection.close()


class Speed(object):

    def __init__(self,sockt):

        print "Building Speed symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'speed'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Speed Symbols added successfully\n'

        connection.close()


class Power(object):

    def __init__(self,sockt):

        print "Building Wheel Power symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'power'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2]) #replace this with signal class
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Power Symbols added successfully\n'

        connection.close()

class USR(object):

    def __init__(self,sockt):
        print "Building USR symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'usr'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2]) #replace this with signal class
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' USR Symbols added successfully\n'

        connection.close()

class Config(object):

    def __init__(self,sockt):

        print "Building CONFIG symbols..."

        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'config'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Config Symbols added successfully\n'

        connection.close()

class Angle(object):

    def __init__(self,sockt):
        
        print "Building Angle symbols..."
        connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db")
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'angle'")
        count = 0

        for each in cursor:        
            temp_string = "{} = signal('{}',{},'{}',sockt)".format(each[0],each[0],each[1],each[2])
            #print temp_string #debug
            exec(temp_string)
            count += 1
            
        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Angle Symbols added successfully\n'

        connection.close()
        
