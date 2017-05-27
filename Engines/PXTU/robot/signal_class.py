'''
The module defines the signal class.
All the signals are instances of this class.

TO-DO:
# Clean the code
'''

###IMPORTS###
import socket

class signal():    

    def __init__(self,name,address,partition):

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
        host = "192.168.0.104"; port = 2017 #For external communication
        self.__addr = (host,port)

        self.__message = "Naveen and Swathi" #Sending this message
        

    def __del__(self):
        
        self.__client_sock.close()
        

    def read(self):
        '''
        Reads the value from the address.
        Args: None.
        Return: data read from the signal
        '''

        #print "Reading " + self.name + '...'       
        payload = self.construct_payload(1) #Constructing the payload
        
        send = self.__client_sock.sendto(payload , self.__addr)

        try:
                self.__data,self.__addr2 = self.__client_sock.recvfrom(1024) #receiving the address from the server
                #print "Data received from the server: " + str(self.__data)                    
        except socket.timeout:
                #print "Timeout!"
                return None

        return int(self.__data)

    def write(self,new_value):
        '''
        Writes the new value into the address.
        Args: new_value to be written.
        Return: data written into the signal
        '''

        print "Writing " + str(new_value) + " to " + self.name + '...'
        payload = self.construct_payload(2,new_value)

        send = self.__client_sock.sendto(payload , self.__addr)

        try:
                self.__data,self.__addr2 = self.__client_sock.recvfrom(1024) #receiving the address from the server
                print "Data received from the server: " + str(self.__data)                    
        except socket.timeout:
                print "Timeout!"
                return None

        return self.__data

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
    

        
        
        

        

        

        
