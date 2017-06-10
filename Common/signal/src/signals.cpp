#include <iostream>
#include <stdio.h>
#include "signals.h"
#include <cstring>
#include <stdlib.h>

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
}

int signals::get()
{
	int value = *((int*)base+address);
	return value;
}

void signals::set(int value)
{
	*((int*)base+address) = value;
}
