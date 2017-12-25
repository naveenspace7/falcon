#include "ecap.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <stdlib.h>

using namespace std;

int *rec_status = base + 1;
const string tcpdump_cmd = "sudo tcpdump port %i -i wlan0 -w %s.pcap";
//char tcpdump_cmd_final = tcpdump_cmd.c_str();
char tcpdump_cmd_final[] = "sudo tcpdump port %i -i wlan0 -w %s.pcap";
char tcpdump_cmd_final_1[100];
bool shell_cmd_ready = false;
bool tcpdump_cmd_running = false;
map<int,int> mapping;
vector<int> pids;
vector<vector<string>> lines_from_file;
const string ps_cmd = "ps -aux | grep tcpdump >> cache.pids";

string file_name = "cache.pids";

void tcpdump_run()
{
    while(1)
    {
        if(shell_cmd_ready && !tcpdump_cmd_running)
        {
            // Run the tcpdump command now
            tcpdump_cmd_running = true;
            cout << "tcpdump_cmd_final:" << tcpdump_cmd_final << endl;
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
        //cout << "temp1:" << *rec_status << endl;
        if(*rec_status & 1 == 1) // Record something
        {
            if(*rec_status == 3 || *rec_status == 1) // DCap or RTU Ethernet recording
            {
                cout << "Start recording now." << endl;
                sprintf(tcpdump_cmd_final, tcpdump_cmd.c_str(), mapping[*rec_status],"somefile");
                cout << "The final command that is going to run: " << tcpdump_cmd_final << endl;
                shell_cmd_ready = true;
                while(!tcpdump_cmd_running); // Wait until tcpdump starts
                sleep(2);
                convert_text_to_vectors();// Fetch the PIDs of the tcpdump
                shell_cmd_ready = false;
            }

            *rec_status = 0; // Reset the status to None
        }

        else if(*rec_status == 2)
        {
            if(tcpdump_cmd_running)
            {
                terminate_tcpdump();
                cout << "Stopped recording now." << endl;
            }

            *rec_status = 0; // Reset the status to None
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
    system("rm cache.pids");
    system(ps_cmd.c_str());
    fstream pids_file;

    // cout << "Done0 " << file_name << endl;

    pids_file.open(file_name, ios::in);
    string each_line;

    // First line, second line and third line
    for(int count = 1; count <= 4; count++)
    {
        getline(pids_file, each_line);
        {
            // cout << "each_line:" << each_line << endl;
            istringstream iss(each_line);
            vector<string> container;
            for(string temp; iss >> temp; )
                container.push_back(temp);
            lines_from_file.push_back(container);
        }
    }

    // cout << "Done1" << lines_from_file[0][1] << endl;

    // for(vector<vector<string>>::iterator i = lines_from_file.begin(); i != lines_from_file.end(); i++)
    // {
    //     for(vector<string>::iterator j = i->begin(); j != i->end(); j++)
    //         cout << " element :" << *j << endl;
    // }

    // Fetches the actual PIDs from the text file.
    for(vector<string> i: lines_from_file)
    {
        cout << "writing pid " << i[1] << endl;
        pids.push_back(stoi(i[1]));
    }

    cout << "Done2" << endl;
}

// Kill all the tcpdump instances from PIDs
void terminate_tcpdump()
{
    for(vector<int>::iterator i = pids.begin(); i != pids.end(); i++)
        cout << "Pids: " << *i << endl;

    for(int pid: pids)
    {
        cout << "kill process with PID: " << pid << endl;
        kill(pid, SIGTERM);
    }
        
}