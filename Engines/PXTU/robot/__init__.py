from base import Base, Record
#from base import Record

import xml.etree.ElementTree as ET

# Use base equivalent objects here
__confPath = "E:\\Falcon\\Common\\Signals\\signals.xml"

root = ET.parse(__confPath)
s = root.find('config').findall('param')
__network_conf = [s[0].attrib['address'],int(s[1].attrib['value']),int(s[2].attrib['value'])]
#### Network config parameters ####

__pxtu_sock = signal_class.socket_config(__network_conf[0],__network_conf[1],__network_conf[2])
base = Base()
record = Record()

# To eliminate stray references
del s, root, ET
del Base, Record
