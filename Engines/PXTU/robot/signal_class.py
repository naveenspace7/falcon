'''
The module defines the signal class.
All the signals are instances of this class.

TO-DO:
# Clean the code
'''

###IMPORTS###
import socket

class socket_config(object):

    def __init__(self):
        
        #socket members
        self._client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #creating a socket for UDP
        self._client_sock.setblocking(0) #making non-blocking
        self._client_sock.settimeout(5) #setting the timeout to 5 secs

        host = "192.168.0.102"; port = 2017 #For external communication TODO: Make this XML readable
        self._addr = (host,port)
        

class signal(socket_config):    

    def __init__(self,name,address,partition):
        print 'signal created'
        '''
        #signal members
        self.name = name
        self.__address = address
        self.__partition = partition
        self.__value = 0

        #socket members
        self.__client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #creating a socket for UDP
        self.__client_sock.setblocking(0) #making non-blocking
        self.__client_sock.settimeout(5) #setting the timeout to 5 secs
        
        #Address of the Server
        #host = "127.0.0.1"; port = 12345 #For local host communication
        host = "192.168.0.104"; port = 2017 #For external communication TODO: Make this XML readable
        self.__addr = (host,port)

        self.__message = "Naveen and Swathi" #Sending this message
        '''
        

##    def __del__(self):
##        
##        self._client_sock.close()
##        

    def read(self):
        '''
        Reads the value from the address.
        Args: None.
        Return: data read from the signal
        '''

        #print "Reading " + self.name + '...'       
        payload = self.construct_payload(1) #Constructing the payload
        
        send = self._client_sock.sendto(payload , self._addr)

        try:
                self._data,self._addr2 = self._client_sock.recvfrom(1024) #receiving the address from the server
                #print "Data received from the server: " + str(self.__data)                    
        except socket.timeout:
                #print "Timeout!"
                return None

        return int(self._data)

    def write(self,new_value):
        '''
        Writes the new value into the address.
        Args: new_value to be written.
        Return: data written into the signal
        '''

        print "Writing " + str(new_value) + " to " + self.name + '...'
        payload = self.construct_payload(2,new_value)

        send = self._client_sock.sendto(payload , self._addr)

        try:
                self._data,self._addr2 = self._client_sock.recvfrom(1024) #receiving the address from the server
                print "Data received from the server: " + str(self._data)                    
        except socket.timeout:
                print "Timeout!"
                return None

        return self._data

    def construct_payload(self,cmd,value=0):
        '''
        Encodes the data.
        Args: cmd = write(2) or read(1)
              value = 0 (default) or value (to be written)
        Return: data written into the signal
        '''

        if cmd == 1:
            command = (self._addr << 3) | cmd
        elif cmd == 2:
            command = (value << 11) | (self._addr << 3) | cmd

        command = str(command)
        
        return command+'\0'
        
    
if __name__ == "__main__":
        
    new_signal = signal('naveen',345,'part')
    

        
        
        

        

        

        
