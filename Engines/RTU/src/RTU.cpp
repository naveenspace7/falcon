#include "RTU.h"
#include "Message.h"

using namespace std;
using namespace pugi;

// Actual code for handling the requests: will be a thread
void perform_action(pair<int, int> payload_pair)
{
  int payload_pointer = payload_pair.first;
  int command = payload_pair.second;
  int rec_addr = Message::obtain_address(command);

  mx.lock();
  switch ((int)command & 7) // Checking the command bits
  {
    case 1: //Read command
    {
      read_flag = true;
      string log_msg;
      log_msg = "Cmd=R, Name:" + sig_map[rec_addr];
      syslog(LOG_INFO, "%s", log_msg.c_str());
      int temp_val = *(shm + rec_addr); // Reading the value from the SHM
      log_msg = "Read Value: " + to_string(temp_val);
      syslog(LOG_INFO, "%s", log_msg.c_str());
      read_values[payload_pointer] = to_string(temp_val); // Put it into the right place
      break;
    }

    case 2: // Write Command
    {
      bitset<32> incoming_cmd = command;
      read_flag = false;
      string log_msg;
      log_msg = "Cmd=W, Name:" + sig_map[rec_addr] + ", ";
      int temp_new_val = ((command & VAL) >> VALOFF); //Obtaining the new value to be written from the payload
      if (incoming_cmd.test(SIGN) == 1)
        temp_new_val = 0 - temp_new_val;
      log_msg += "Value:" + to_string(temp_new_val);
      syslog(LOG_INFO, "%s", log_msg.c_str());
      *(shm + rec_addr) = temp_new_val; //writing the new value into SHM
      break;
    }
  }
  mx.unlock();
}

// Pack the read values into a ready to be sent form
string pack_read_payload()
{
  string return_str = "{";
  for(auto& each_content: read_values)
    return_str += each_content + ",";
  return_str += "}";
  return return_str;
}

int main(int argc, char *argv[])
{
  // TODO: Create seg handler to delete the SHM.
  openlog("Falcon:RTU", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

  // TODO: Move the below Daemon code to a function
  #if DEBUG == 0 // MAKE ME A DAEMON
  RunAsDaemon();
  #endif

  syslog(LOG_INFO, "Starting RTU Engine");
  // Daemon code ends here and actual code start from here

  shm = get_base(); // Obtain the base address of the shared memory
  signals::make_map(sig_map, name_addr); // Signal address to signal name map

  //thread TCPthread(background_monitor, cref(sig_map));	
  string buff; // Buffer to store the incoming request

  while(1)
  {		
    // Blocking socket - receive
    buff = new_sock->sock_recv();
    if (buff == "ERROR")
      return -1;

    Message *rxRequest = new Message(buff); // Creating new object for storing the RX stuff.

    vector<thread> threads;

    read_values.resize(rxRequest->getNumberOfRequests()); // Make the size to the received length

    string decoded = "Decoded:" + buff;
    syslog(LOG_INFO, "%s", decoded.c_str());

    // Make every read/write operation into threads for the sake of concurrency
    for (vector<pair<int, int>>::iterator ele = rxRequest->requests.begin(); ele != rxRequest->requests.end(); ele++)    
      threads.push_back(thread(perform_action, *ele));
    
    // Join all the started threads
    for (auto& each_thr : threads)
      each_thr.join();

    // Send reply for the request received from PXTU
    if(read_flag)
      new_sock->sock_send(pack_read_payload());
    else
      new_sock->sock_send(string("WDone"));

    delete rxRequest;
  }
  return 0;
}

// Forking done for creating a Daemon
void RunAsDaemon()
{
  pid_t pid, sid;
  pid = fork();
  if(pid < 0)
    exit(EXIT_FAILURE);
  if(pid > 0)
    exit(EXIT_SUCCESS);
  umask(0); 
  sid = setsid();
  if(sid < 0)
    exit(EXIT_FAILURE);
  if((chdir("/")) < 0)
    exit(EXIT_FAILURE);
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}