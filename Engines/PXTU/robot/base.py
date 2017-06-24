'''
TODO: Read the IP address and port number from config file.
'''
### IMPORTS ###
import modules, time
import signal_class
from signal_class import socket_config

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
#class Record(object):
    
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
            Record.recording = True
            print self.element
            # send self.element
            socket_config._client_sock.sendto(self.element , socket_config._addr_dcap)

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
            Record.recording = False
            print self.element
            # send self.element
            socket_config._client_sock.sendto(self.element , socket_config._addr_dcap)
            
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
            self.payload += ',['
            for each_signal in sig_list:
                self.payload += str(each_signal._address) + ','
            self.payload = self.payload[:-1]
            self.payload += '],'
            self.payload += str(rate) + ','
            if '.csv' not in file_name:
                self.payload += "'" + file_name + ".csv\'"
            else:
                self.payload += "'" + file_name + "'"
        self.payload = '{' + self.payload + '}\0'
        # {1,[23,44,56,22],100,'Recording_1234.csv'}

    @property
    def payload(self):
        return self.payload
