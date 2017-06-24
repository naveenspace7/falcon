from base import Base, Record
#from base import Record

# Use base equivalent objects here
base = Base()
record = Record()

# To eliminate stray references
del Base 
del Record
