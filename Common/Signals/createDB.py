### IMPORTS ###
import os, sqlite3

def create_database():

    csv_file = open("signals.csv",'r')

    insert_string = "INSERT INTO signal (Name, Address, Parent, Min, Max, DataType, Type, Description) VALUES ('{}', {}, '{}', {}, {}, '{}', '{}', '{}');"

    connection = sqlite3.connect("E:\\Falcon\\Common\\Signals\\signals_database.db") #TODO: Don't hardcode the path, should be either config readable
    
    connection.execute("DELETE FROM signal")

    line = csv_file.readline()

    while(True):

        line = csv_file.readline().strip('\n')

        if line == '':
            break

        elements = line.split(',')    

        query_string = insert_string.format(elements[0],elements[1],elements[2],\
                                            elements[3],elements[4],elements[5],elements[6],elements[7])

        print query_string
        
        connection.execute(query_string)        

    print "Updated database."
    connection.commit()    
    connection.close()
    print "Closed the database."
    csv_file.close()
    print "Closed the CSV file."

create_database()
