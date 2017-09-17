#include "signals.h"

using namespace std;
using namespace pugi;

signals::signals(string name,int*&Base)
{
	xml_document doc;
	xml_parse_result result = doc.load_file("../../../Common/Signals/signals.xml");
	//address = base + address;	
	base = Base;

	if(!result)
		cout << "Error in reading the file.";		

	string path_string = "/signals/signal[@name='" + name + "']";	
	char* path_string_1 = new char[path_string.length()+1];
	strcpy(path_string_1, path_string.c_str());	

	xpath_node_set sig_node = doc.select_nodes(path_string_1);
	
	//xpath_node_set sig_node = doc.select_nodes("/signals/signal[@name='power_l']"); //debug

	for(pugi::xpath_node_set::const_iterator it = sig_node.begin(); it != sig_node.end(); ++it)
	{
		pugi::xpath_node node = *it;
		address = atoi(node.node().attribute("address").value());
		//cout << "Address: " << address << endl; //debug
	}

	delete path_string_1;
}

int signals::get()
{
	int value = *((int*)base+address);
	return value;
}

// Set will depend on the value of the lock
void signals::set(int value)
{
	// Checks if the LOCK variable is 0 and only then it can write
	if(*(int*)base == 0)
		*((int*)base+address) = value;
}

// Force will bypass the lock checking
void signals::force(int value)
{
	*((int*)base + address) = value;
}

// Populate the map with values read from the XML file
void signals::make_map(map<int,string>& sig_map, map<string,int>& sig_map2)
{
	/*
	Create something like this:
	sig_map[21] = "usr_right";
	sig_map[22] = "usr_left";	
	sig_map[52] = "ir_rear";*/

	int address;
	string name;

	// Open XML file
	xml_document doc;
	xml_parse_result result = doc.load_file("/home/pi/Falcon/Common/Signals/signals.xml");

	if(!result)
		cout << "Error reading the XML signals file" << endl;

	xml_node signals = doc.child("robot").child("signals");
	for(xml_node signal = signals.first_child(); signal; signal = signal.next_sibling())
	{
		// Operate on signal
		address = atoi(signal.attribute("address").value());
		name = signal.attribute("name").value();
		sig_map[address] = name;
		sig_map2[name] = address;
	}
}