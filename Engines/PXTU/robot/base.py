import modules
import signal_class

class Base(object):

    def __init__(self):
        
        print "Initializing base..."

        self.socket_this = signal_class.socket_config()
        self.ir = modules.IR()
        self.speed = modules.Speed()
        self.power = modules.Power()
        self.usr = modules.USR()
        self.angle = modules.Angle()
        self.config = modules.Config()
        

        print "Done initializing Base"
