'''
The module defines the signal class.
All the signals are instances of this class.

TO-DO:
# Clean the code
'''

###IMPORTS###
import socket

class socket_config(object):

    _client_sock = 0
    _addr = 0
    _addr_dcap = 0
    
    def __init__(self,ip_addr,dst_port,dst_port_dcap):
        
        #socket members
        socket_config._client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #creating a socket for UDP
        socket_config._client_sock.setblocking(0) #making non-blocking
        socket_config._client_sock.settimeout(5) #setting the timeout to 5 secs

        host = ip_addr; port = dst_port #For external communication TODO: Make this XML readable
        socket_config._addr = (host,port)

        port = dst_port_dcap #For external communication TODO: Make this XML readable
        socket_config._addr_dcap = (host,port)

    def __del__(self):

        socket_config._client_sock.close()     

class signal(socket_config):    

    def __init__(self,name,address,partition,minVal,maxVal,dType,desc=None):
        
        #signal members
        self.name = name
        self.__address = address
        self._address = address
        self.__partition = partition
        self.__value = 0
        self.__desc = desc
        self.__dType = dType
        self.__minVal = minVal
        self.__maxVal = maxVal

    def check_range(self,val):
        
        if val >= self.__minVal and val <= self.__maxVal:
            return True
        else:
            return False

    @property
    def value(self):
        '''
        Sends a read command to the RTU
        '''
        self.__value = self.read()
        return self.__value

    @value.setter
    def value(self,val):
        '''
        Sends a write command to the RTU
        '''        
        retData = self.write(val)
        print retData
        self.__value = val

    def read(self):
        '''
        Reads the value from the address.
        Args: None.
        Return: data read from the signal
        '''

        #print "Reading " + self.name + '...'       
        payload = '{' + self.construct_payload(1) + '}\0' #Constructing the payload        
        send = socket_config._client_sock.sendto(payload , socket_config._addr)
        try:
                self._data,self._addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
                self._data = self._data[1:-2]
                #print "Data received from the server: " + str(self.__data)                    
        except socket.timeout:
                print "Timeout!"
                return None

        return int(self._data)

    def write(self,new_value):
        '''
        Writes the new value into the address.
        Args: new_value to be written.
        Return: data written into the signal
        '''

        #print "Writing " + str(new_value) + " to " + self.name + '...' # Debug
        valid = self.check_range(new_value)

        if valid:
            payload = '{' + self.construct_payload(2,new_value) + '}\0'
            send = socket_config._client_sock.sendto(payload , socket_config._addr)

            try:
                    self._data,self._addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
                    #print "Data received from the server: " + str(self._data) # Debug
            except socket.timeout:
                    print "Timeout!"
                    return None
            return self._data
        else:
            return "Out of range value given"

    def construct_payload(self,cmd,value=0):
        '''
        Encodes the data.
        Args: cmd = write(2) or read(1)
              value = 0 (default) or value (to be written)
        Return: data written into the signal
        '''

        if cmd == 1:
            command = (self.__address << 3) | cmd
        elif cmd == 2:
            command = 0
            if value < 0:
                # remove the sign                
                value = abs(value)
                command |= (1 << 27)                        
            command |= (value << 11) | (self.__address << 3) | cmd                                                    
            
        command = str(command)
        
        return command

    @property
    def desc(self):

        return self.__desc                    

    def __repr__(self):

        display_string = "<signal-object: name=%s, partition=%s, address=%d, value=%d>"%(
            self.name,self.__partition,self.__address,self.__value)
        
        return display_string

    #TODO: return the signal name with its description. Nice to have.
##    def __str__(self):
##
##        return ''

class engine_signal(signal):

    def __init__(self,name,address,partition,desc=None):
        signal.__init__(self,name,address,partition,desc=None)

    @property
    def status(self):
        '''
        Engine's running state: 1 - paused, 0 - running
        '''        
        try:
            #print super(engine_signal,self).read() # Debug
            value = super(engine_signal,self).read() >> 15
            return int(value)
        except(TypeError):
            print "Not connected"
            return None

    def read(self):
        raise AttributeError("Please use 'status' to read the state of the engine")

    def write(self):
        raise AttributeError("Please use 'state' to write the state of the engine")

    def state(self,run_state):
        try:
            #print "Value got = ",super(engine_signal,self).read() # Debug
            value = ((super(engine_signal,self).read() & 32767) | (run_state << 15))
            super(engine_signal,self).write(value)
            #return "Set value: "+str(value) # Debug
        except(TypeError):
            print "Not connected"
            return None
    
if __name__ == "__main__":
        
    new_signal = signal('naveen',345,'part') #won't work
    

        
        
        

        

        

        
