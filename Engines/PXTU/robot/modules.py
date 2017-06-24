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

class record(object):
    
    recording = False
    
    def start(self, sig_list, rate = 100, file_name = None):
        if recording != True:
            if sig_list == [] or sig_list == None:
                print "Warning: signal list is empty"
            x = lambda s: s[:s.index('.')-1]
            if file_name == None:
                file_name = x(str(time.time()))
            
            temp_element = payload_package(1, sig_list, rate, file_name)
            self.element = temp_element.payload
            record.recording = True
            print self.element
            # send self.element
        else:
            print "Warning: Recorder is already running"

    def stop(self):
        if recording != False:
            temp_element = payload_package(0)
            self.element = temp_element.payload
            record.recording = False
            print self.element
            # send self.element
        else:
            print "Warning: Recorder is stopped already"
        
        
        

class payload_package():

    def __init__(self, cmd, sig_list = None, rate = None, file_name = None):
        print "payload_package"
        self.payload = ''        
        self.cmd = cmd
        self.payload += str(self.cmd)
        if cmd != 0:
            self.file_name = file_name
            self.payload += ',['
            for each_signal in sig_list:
                self.payload += str(each_signal._address) + ','
            self.payload = self.payload[:-1]
            self.payload += '],'
            self.payload += str(rate) + ','
            self.payload += file_name + '.csv'
        self.payload = '{' + self.payload + '}'

    @property
    def payload(self):
        return self.payload
        
    
        


        
