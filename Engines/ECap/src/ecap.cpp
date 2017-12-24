#include "ecap.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <stdlib.h>

using namespace std;

int *rec_status = base;
string tcpdump_cmd = "sudo tcpdump port %i -i wlan0 -w %s.pcap";
char tcpdump_cmd_final = tcpdump_cmd.c_str();
bool shell_cmd_ready = false;
bool tcpdump_cmd_running = false;
map<int,int> mapping;
vector<int> pids;
vector<vector<string>> lines_from_file;
string ps_cmd = "ps -aux | grep tcpdump >> cache.pids";

string file_name = "cache.pids"

void tcpdump_run()
{
    while(1)
    {
        if(shell_cmd_ready)
        {
            // Run the tcpdump command now
            tcpdump_cmd_running = true;
            system(tcpdump_cmd_final);
            tcpdump_cmd_running = false;
        }
    }

}

int main()
{
    mapping[1] = 2017;
    mapping[3] = 2018;

    thread tdump_thread(tcpdump_run);

    while(1) // Run the main thread continually
    {
        cout << "temp1:" << *(base + 1) << endl;
        if(*rec_status & 1 == 1) // Record something
        {
            if(*rec_status == 3 || *rec_status == 1) // DCap or RTU Ethernet recording
            {
                sprintf(tcpdump_cmd_final,mapping[*rec_status],"somefile");
                cout << "The final command that is going to run: " << tcpdump_cmd_final << endl;
                shell_cmd_ready = true;
                while(!tcpdump_cmd_running); // Wait until tcpdump starts
                convert_text_to_vectors();// Fetch the PIDs of the tcpdump
            }

            *rec_status = 0; // Reset the status to None
        }

        else if(*rec_status == 2)
        {
            if(tcpdump_cmd_running)
            {
                terminate_tcpdump();
            }
        }
    }

    tdump_thread.join();
    
    return 0;
}

// Opens the text file and obtains the valid PIDs and stores them
void convert_text_to_vectors()
{
    pids.clear();
    lines_from_file.clear();
    // Execute ps_cmd command
    fstream pids_file;
    pids_file.open(file_name, ios::in);
    string each_line;

    // First line, second line and third line
    for(int count = 1; count <= 3; count++)
    {
        getline(pids_file, each_line);
        {
            istringstream iss(each_line);
            vector<string> container;
            for(string temp; iss >> temp; )
                container.push_back(temp);
            lines_from_file.push_back(container);
        }
    }

    // Fetches the actual PIDs from the text file.
    for(vector<string> i: lines_from_file)
    {
        if(i[0] == "root")
            pids.push_back(stoi(i[1]);
    }
}

// Kill all the tcpdump instances from PIDs
void terminate_tcpdump()
{
    for(int pid: pids)
        kill(pid);
}