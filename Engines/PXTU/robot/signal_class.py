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
    
    def __init__(self,ip_addr,dst_port):
        
        #socket members
        socket_config._client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #creating a socket for UDP
        socket_config._client_sock.setblocking(0) #making non-blocking
        socket_config._client_sock.settimeout(5) #setting the timeout to 5 secs

        host = ip_addr; port = dst_port #For external communication TODO: Make this XML readable
        socket_config._addr = (host,port)

    def __del__(self):

        socket_config._client_sock.close()
        

class signal(socket_config):    

    def __init__(self,name,address,partition):        
        
        #signal members
        self.name = name
        self.__address = address
        self.__partition = partition
        self.__value = 0    

    def read(self):
        '''
        Reads the value from the address.
        Args: None.
        Return: data read from the signal
        '''

        #print "Reading " + self.name + '...'       
        payload = self.construct_payload(1) #Constructing the payload
        
        send = socket_config._client_sock.sendto(payload , socket_config._addr)

        try:
                self._data,self._addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
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

        print "Writing " + str(new_value) + " to " + self.name + '...'
        payload = self.construct_payload(2,new_value)

        send = socket_config._client_sock.sendto(payload , socket_config._addr)

        try:
                self._data,self._addr2 = socket_config._client_sock.recvfrom(1024) #receiving the address from the server
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
            command = (self.__address << 3) | cmd
        elif cmd == 2:
            command = (value << 11) | (self.__address << 3) | cmd

        command = str(command)
        
        return command+'\0'
        
    
if __name__ == "__main__":
        
    new_signal = signal('naveen',345,'part')
    

        
        
        

        

        

        
