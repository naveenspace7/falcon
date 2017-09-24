'''
This module creates the instances of the Modules partition
'''

###IMPORTS###
import sqlite3, time
from signal_class import signal
from collections import namedtuple
#print "Importing modules..."

db_path = "E:\\Falcon\\Common\\Signals\\signals_database.db" # TODO: Remove the hardcoded path. Read it from the XML
queryString = "SELECT t1.Name as name, t1.Address as address, t1.Parent as parent, t1.Min as min, t1.Max as max, "
queryString += "t1.DataType as dType, t1.Description as desc FROM signal AS t1 WHERE parent = '{}'"

def factory(someTuple):
    '''
    Input: list of tuples
    Output: list of dictionaries
    '''
    dictReturn = []
    #s = (u'ir_front', 51, u'ir', 0, 0, u'discrete', u'Set of ir sensor readings on the front')
    element = ["name", "address", "parent", "min", "max", "dType", "desc"]
    if len(element) != len(someTuple[0]):
        print "Error"
        return None

    for eachTuple in someTuple:
        tempDict = {}
        for index in range(len(element)):            
            tempDict[element[index]] = eachTuple[index]
        dictReturn.append(tempDict)
        
    #print dictReturn
    return dictReturn        
                
class IR(object):

    def __init__(self):

        print "Building IR symbols...",
        connection = sqlite3.connect(db_path)
        query = queryString.format('ir')
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)
        #print "Cursor",cursor
        count = 0

        for each in cursor:            
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' IR Symbols added successfully'

        connection.close()


class Speed(object):

    def __init__(self):

        print "Building Speed symbols...",
        connection = sqlite3.connect(db_path)                
        query = queryString.format('speed')
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Speed Symbols added successfully'

        connection.close()


class Power(object):

    def __init__(self):

        print "Building Wheel Power symbols...",
        connection = sqlite3.connect(db_path)
        query = queryString.format('power')
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)        
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Power Symbols added successfully'

        connection.close()

class USR(object):

    def __init__(self):
        print "Building USR symbols...",
        group = "usr"
        connection = sqlite3.connect(db_path)        
        query = queryString.format(group)
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)        
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' USR Symbols added successfully'

        connection.close()

class Config(object):

    def __init__(self):

        print "Building CONFIG symbols...",
        group = "config"
        connection = sqlite3.connect(db_path)
        query = queryString.format(group)
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string #debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Config Symbols added successfully'

        connection.close()

class Angle(object):

    def __init__(self):
        
        print "Building Angle symbols...",
        group = "angle"
        connection = sqlite3.connect(db_path)
        query = queryString.format(group)
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)
        
        count = 0

        for each in cursor:        
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string #debug
            exec(temp_string)
            count += 1
            
        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Angle Symbols added successfully'

        connection.close()

class Engines(object):

    def __init__(self):

        print "Building Engine symbols...",
        group = "engine"
        connection = sqlite3.connect(db_path)        
        query = queryString.format(group)
        #print query
        cursor = connection.execute(query).fetchall()        
        cursor = factory(cursor)
        count = 0

        for each in cursor:            
            temp_string = "self.{} = signal('{}',{},'{}',{},{},'{}','{}')".format(each["name"],each["name"],each["address"],each["parent"],
                                                                       each["min"],each["max"],each["dType"],each["desc"])
            #print temp_string # Debug
            exec(temp_string)
            count += 1

        if count == 0:
            print "Warning: No symbols found"
        else:
            print str(count) + ' Engine Symbols added successfully\n'

        connection.close()
