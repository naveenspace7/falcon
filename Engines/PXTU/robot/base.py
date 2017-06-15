'''
TODO: Read the IP address and port number from config file.
'''
### IMPORTS ###
import modules
import signal_class

class Base(object):

    def __init__(self):
        
        print "Initializing Base..."

        self.__pxtu_sock = signal_class.socket_config("192.168.0.102",2017) # TODO: Read this from config file
        self.ir = modules.IR()
        self.speed = modules.Speed()
        self.power = modules.Power()
        self.usr = modules.USR()
        self.angle = modules.Angle()
        self.config = modules.Config()        

        print "Done initializing Base"
