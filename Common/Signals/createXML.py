import xml.etree.ElementTree as ET
import os, shutil

os.chdir('E:\\Falcon\\Common\\Signals')

def create_xml():

    csv_file = open('E:\\Falcon\\Common\\Signals\\signals.csv')    

    csv_file.readline()
   
    robot = ET.Element("robot")

    signal = ET.SubElement(robot,"signals")    

    no = 0
    
    while(True):
        
        line = csv_file.readline().strip()

        if line=='':
            break
        
        line = line.split(',') #list        
        
        signal_no = "signal"

        ET.SubElement(signal,signal_no,name=line[0],address=line[1])     

    tree = ET.ElementTree(robot)

    
    
    
    tree.write("signals.xml")

    csv_file.close()

    shutil.copy('E:\\Falcon\\Common\\Signals\\signals.xml','E:\\Falcon\\Common\\Utils\\signal\\src')

create_xml()
