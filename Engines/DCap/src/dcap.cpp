
#include "dcap.h"

using namespace std;



int main()
{

	int time_s = 1 * 1000000;
	int* base = get_base();

	int counter = 0;

	ofstream outfile;
	outfile.open("note.csv");

	// get mapping for numbers to names
	//outfile << "swarna,shree,chandrashekeran;"<<'\n';

	vector<int> v {1,2,3,4,5};
	int i = 0;

	// write the headings to the csv first
	
	while(1) // check for stop flag
	//for(int i = 0; i <= 50; i++)
	{
		ostringstream row;
		row << counter*10 << ',';
		for(int x : v)
		{						
			row << *(base+x) << ",";
		}

		row << '\n';		
		cout << row.str(); // Debug
		outfile << row.str();
		usleep(time_s);
		counter++;
		
		if(counter==5)
			break;
	}
		
	//outfile << "naveen,chakravarthy,jakka,"<<'\n';

	outfile.close();
	return 0;
}
