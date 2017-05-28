import modules
import signal_class

class Base(object):

    def __init__(self):
        
        print "Initializing base..."

        self.socket_this = signal_class.socket_config()
        self.ir = modules.IR(self.socket_this)
        self.speed = modules.Speed(self.socket_this)
        self.power = modules.Power(self.socket_this)
        self.usr = modules.USR(self.socket_this)
        self.angle = modules.Angle(self.socket_this)
        self.config = modules.Config(self.socket_this)
        

        print "Done initializing Base"
