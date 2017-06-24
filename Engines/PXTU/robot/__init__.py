from base import Base, Record
#from base import Record

# Use base equivalent objects here
__pxtu_sock = signal_class.socket_config("192.168.0.108",2017,2018) # TODO: Read this from config file
base = Base()
record = Record()

# To eliminate stray references
del Base 
del Record
