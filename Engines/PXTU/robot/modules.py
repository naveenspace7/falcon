'''
This module creates the instances of the Modules partition
'''

###IMPORTS###
import sqlite3, time
from signal_class import signal

#print "Importing modules..."

db_path = "E:\\Falcon\\Common\\Signals\\signals_database.db" # TODO: Remove the hardcoded path. Read it from the XML

class IR(object):

    def __init__(self):

        print "Building IR symbols..."
        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'ir'")
        count = 0

        for each in cursor:            
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' IR Symbols added successfully\n'

        connection.close()


class Speed(object):

    def __init__(self):

        print "Building Speed symbols..."
        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT * FROM signal WHERE signal.parent = 'speed'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Speed Symbols added successfully\n'

        connection.close()


class Power(object):

    def __init__(self):

        print "Building Wheel Power symbols..."
        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'power'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Power Symbols added successfully\n'

        connection.close()

class USR(object):

    def __init__(self):
        print "Building USR symbols..."
        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'usr'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' USR Symbols added successfully\n'

        connection.close()

class Config(object):

    def __init__(self):

        print "Building CONFIG symbols..."

        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'config'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Config Symbols added successfully\n'

        connection.close()

class Angle(object):

    def __init__(self):
        
        print "Building Angle symbols..."
        connection = sqlite3.connect(db_path)
        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'angle'")
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
            #print temp_string #debug
            exec(temp_string)
            count += 1
            
        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Angle Symbols added successfully\n'

        connection.close()
