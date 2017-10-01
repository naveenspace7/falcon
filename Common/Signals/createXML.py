import xml.etree.ElementTree as ET
import os

os.chdir('E:\\Falcon\\Common\\Signals')

def create_xml():

    csv_file = open('E:\\Falcon\\Common\\Signals\\signals.csv')    
    csv_file.readline()
   
    robot = ET.Element("robot")

    signal = ET.SubElement(robot,"signals")
    config = ET.SubElement(robot,"config")

    ET.SubElement(config,"param",name="ip_address",address="192.168.0.143")
    ET.SubElement(config,"param",name="port",value="2017")
    ET.SubElement(config,"param",name="port_dcap",value="2018")
    ET.SubElement(config,"param",name="db_path",value="E:\\Falcon\\Common\\Signals\\signals_database.db")
    
    no = 0
    
    while(True):
        
        line = csv_file.readline().strip()

        if line=='':
            break
        
        line = line.split(',') #list                
        signal_no = "signal"
        ET.SubElement(signal,signal_no,name=line[0],address=line[1])     

    tree = ET.ElementTree(robot)    
    tree.write("signals.xml", encoding="utf-8", xml_declaration=True)
    csv_file.close()    

create_xml()
