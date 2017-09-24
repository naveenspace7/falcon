'''
TODO: Read the IP address and port number from config file.
'''
### IMPORTS ###
import modules, time, sqlite3, socket
import signal_class
from signal_class import socket_config, signal, engine_signal

class Base(object):

    def __init__(self):
        
        print "Initializing Base..."

        #self.__pxtu_sock = signal_class.socket_config("192.168.0.108",2017,2018) # TODO: Read this from config file
        self.ir = modules.IR()
        self.speed = modules.Speed()
        self.power = modules.Power()
        self.usr = modules.USR()
        self.angle = modules.Angle()
        self.config = modules.Config()

        print "Done initializing Base"

class Record(socket_config):
    
    recording = False

    def __init__(self):
        pass
    
    def start(self, sig_list, rate = 100, file_name = None):
        '''
        Starts recording
        @sig_list: list of signals to be recorded
        @rate: rate of recording (optional) - default to 100Hz
        @file_name: output file name (optional)

        returns: if it is recording or not
        '''
        
        if Record.recording != True:            
            # Give a file name corresponding to time if no file name is specified
            if file_name == None:
                get_filename = lambda s: "Recording_" + s[:s.index('.')-1]
                file_name = get_filename(str(time.time()))
                # Recording_12345
            
            temp_element = payload_package(1, sig_list, rate, file_name)
            self.element = temp_element.payload            
            print self.element
            # send self.element
            socket_config._client_sock.sendto(self.element , socket_config._addr_dcap)
            # receive response here and only if is Done make the recording to true
            try:
                _data,_addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
                if _data == "Done": Record.recording = True
                print "Success"
                #print "Data received from the server: " + str(self.__data)
            except socket.timeout:
                print "Timeout!"
                return None            
        else:
            print "Warning: Recorder is already running"

        return Record.recording

    def stop(self):
        '''
        Stops the recording
        '''
        if Record.recording != False:
            temp_element = payload_package(0)
            self.element = temp_element.payload            
            print self.element
            # send self.element
            socket_config._client_sock.sendto(self.element , socket_config._addr_dcap)
            try:
                _data,_addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
                if _data == "Done": Record.recording = False
                print "Success"
                #print "Data received from the server: " + str(self.__data)
            except socket.timeout:
                print "Timeout!"
                return None            
        else:
            print "Warning: Recorder is stopped already"

        return Record.recording
        
class payload_package():

    def __init__(self, cmd, sig_list = None, rate = None, file_name = None):
        # Packing the payload string here
        self.payload = ''        
        self.cmd = cmd
        self.payload += str(self.cmd)
        if cmd != 0:
            self.file_name = file_name
            self.payload += ';['
            for each_signal in sig_list:
                self.payload += str(each_signal._address) + ','
            self.payload = self.payload[:-1]
            self.payload += '];'
            self.payload += str(rate) + ';'
            if '.csv' not in file_name:
                self.payload += "" + file_name + ".csv"
            else:
                self.payload += "'" + file_name + "'"
        self.payload = '{' + self.payload + '}\0'        
        # {1,[23,44,56,22],100,'Recording_1234.csv'}

    @property
    def payload(self):
        return self.payload

##class Engines(object):
##
##    def __init__(self):
##
##        print "Building Engine symbols...",
##        connection = sqlite3.connect(modules.db_path)
##        cursor = connection.execute("SELECT t1.Name, t1.Address, t1.Parent, t1.Description FROM signal AS t1 WHERE t1.parent = 'engine'")
##        count = 0
##
##        for each in cursor:            
##            temp_string = "self.{} = engine_signal('{}',{},'{}','{}')".format(each[0],each[0],each[1],each[2],each[3])
##            #print temp_string # Debug
##            exec(temp_string)
##            count += 1
##
##        if count == 0:
##            print "Warning: No symbols found"
##        else:
##            print str(count) + ' Engine Symbols added successfully\n'
##
##        connection.close()

class Signals(socket_config):

    def __init__(self):
        self.done = True
        
    @staticmethod
    def read(signals_list):
        payload = '{'
        for each_signal in signals_list:
            payload += Signals.construct_payload(each_signal, 1)+','
        payload = payload[:-1] + '}\0'
        print payload #debug
        send = socket_config._client_sock.sendto(payload , socket_config._addr)

        try:
            _data,_addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server            
            _data = _data[1:-2]            
            _data = _data.split(',')
            return_string = ""
            for index, each_signal in enumerate(signals_list):
                return_string += each_signal.name + ':' + _data[index] + ','
            print return_string[:-1]
            #print "Data received from the server: " + str(_data)  #make this proper                  
        except socket.timeout:
            print "Timeout!"
            return None
        
    @staticmethod
    def write(signals_list, values_list = None):
        payload = '{'
        if values_list == None:        
            for each_signal_tuple in signals_list:
                payload += Signals.construct_payload(each_signal_tuple[0], 2, each_signal_tuple[1])+','
        else:
            if len(signals_list) != len(values_list):
                print "Lengths don't match"
            else:
                for ele_no in range(0,len(signals_list)):
                    payload += Signals.construct_payload(signals_list[ele_no], 2, values_list[ele_no]) + ','
        payload = payload[:-1] + '}\0'
        print payload #debug
        
        send = socket_config._client_sock.sendto(payload , socket_config._addr)

        try:
            _data,_addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
            print "Data received from the server: " + str(_data)                    
        except socket.timeout:
            print "Timeout!"
            return None

    @staticmethod
    def construct_payload(signal,cmd,value=0):
        '''
        Encodes the data.
        Args: cmd = write(2) or read(1)
              value = 0 (default) or value (to be written)
        Return: data written into the signal
        '''
        if cmd == 1:
            command = (signal._address << 3) | cmd
        elif cmd == 2:
            command = (value << 11) | (signal._address << 3) | cmd

        command = str(command)
        
        return command
        
