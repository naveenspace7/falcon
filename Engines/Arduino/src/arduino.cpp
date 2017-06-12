#include "arduino.h"

using namespace std;

/* This function runs only when called by the master.
   Should run twice in every frame */
void compute(int signal_number)
{
	Engine.compute_start();     // Consumes the start time and locks the SHM
	
	// Alternates between Read and Write depending on variable Engine.rw
	if(Engine.rw) // Reading data from Arduino base
	{
		Engine.obtain_datablock();
		cout << "Compute function called inside read arduino\n";
	}

	else // Writing data to Arduino base
	{		
		cout << "Compute function called inside write arduino\n";
		// TODO: Obtain some confirmation that data has been written to Base
	}
 	
 	Engine.compute_end();		// Consumes the stop time and unlocks the SHM
 	Engine.rw = !Engine.rw;		// Toggle between Read and Write
}

/* Obtains the data block from the Arduino base
   This function packs the data into vector and writes into the signal SHM. */
void arduino::obtain_datablock()
{
	string serial_data = "";

	serial_data = query(1);      // Query the Arduino base for data block
	serial_data = "";            // HACK: query is adding bad data into this

	if(serial_data != "")        // Ignore if the string is blank
	{		
		vector<int> sensor_values = decode_string(serial_data);

		current_timestamp = sensor_values[0]; // Obtain the timestamp

		verify_timestamp();
		
		// Setting the signals into the SHM
		timestamp_arduino->set(current_timestamp);
		usr_fr->set(sensor_values[1]);
		usr_rt->set(sensor_values[2]);
		usr_rr->set(sensor_values[3]);
		usr_lt->set(sensor_values[4]);
		angle->set(sensor_values[5]);	
	}

	else
		cout << "No data received from Arduino\n";	
}

/* Takes the string received from Arduino base, strips it 
   and packs them into vector.*/
vector<int> arduino::decode_string(const string& rx_string)
{
	/*
	Format of string data received: ";14;23;45;26;\0"
	Example: ";14;23;45;26;\0" ---> {14,23,45,26}
	*/
	cout << "Decoding string: " << rx_string << endl; // Debug
	// str_pt - index pointer, start - word start index, end - word end index
	int str_pt = 0, start = 0, end = 0;
	// Obtaining word - true:word started false: word not started 
 	bool s_bool = false;
	string sub_str;   // Substring holder
	vector<int> value_vector;
 	while(1)
 	{
  		if(rx_string[str_pt] != '\0')
  		{
   			if(rx_string[str_pt] == ';' && s_bool == true)
   			{
			    end = str_pt - 1;
			    sub_str = rx_string.substr(start,end-start+1);
				//cout << sub_str << endl; // Debug
    			value_vector.push_back(stoi(sub_str));
    			s_bool = false;
   			}
   		else if(rx_string[str_pt] == ';' && s_bool == false)
   		{
	    	start = str_pt + 1;
	    	s_bool = true;
	    	str_pt++;
   		}
   		else
    		str_pt++;
  		}
  		else
   			break;
 	}
 	return value_vector;
}

/* Checking the timestamp of current transaction.
   Reports when stale data is received */
void arduino::verify_timestamp() // Get the timestamp and compare it with the present timestamp.
{	
	if(timestamp >= current_timestamp)
	{
		cout << "Stale data received from Arduino base.\n";
		stale_counter++;

		if(stale_counter % 5 == 0) // If stale data is received more than 5 times raise an error
		{
			cout << "ERROR: Looks like Arduino base is down.\n";
			// TODO: take any corrective steps HERE
		}
	}

	else // Updated data received
	{
		timestamp = current_timestamp; // Updating the timestamp
		stale_counter = 0;
	}
}

/* Defining serial port and opening */
void arduino::serial(const char* dev_name, int in_baud) // : baud(in_baud)
{	
	baud = in_baud;
	Open(dev_name); //TODO: Device name should be read from XML
}

void arduino::Close()
{
	close(fd);
}

/* Send a number to the destination Arduino base */
int arduino::Send(int n)
{
	//n is a number but we need the string version of it
	char temp_buff[10];
	sprintf(temp_buff,"%d\r",n);	  		
	int len = len_of(temp_buff);
	return write (fd, temp_buff, len);
}

/* Measure the length of the char array by count chars until '\0' */
int arduino::len_of(char* temp)
{
	int len = 0;
	while(1)
	{
		if(*(temp+len) != '\0')
			len++;
		else
			break;
	}
	return len+1;
}

/* Configure the serial connection */
int arduino::Open(const char* device)
{
	cout << "Device: " << device << endl;
	struct termios options;
	speed_t myBaud;

	switch (baud)
	{
		case      50:	myBaud =      B50 ; break ;
		case      75:	myBaud =      B75 ; break ;
		case     110:	myBaud =     B110 ; break ;
		case     134:	myBaud =     B134 ; break ;
		case     150:	myBaud =     B150 ; break ;
		case     200:	myBaud =     B200 ; break ;
		case     300:	myBaud =     B300 ; break ;
		case     600:	myBaud =     B600 ; break ;
		case    1200:	myBaud =    B1200 ; break ;
		case    1800:	myBaud =    B1800 ; break ;
		case    2400:	myBaud =    B2400 ; break ;
		case    4800:	myBaud =    B4800 ; break ;
		case    9600:	myBaud =    B9600 ; break ;
		case   19200:	myBaud =   B19200 ; break ;
		case   38400:	myBaud =   B38400 ; break ;
		case   57600:	myBaud =   B57600 ; break ;
		case  115200:	myBaud =  B115200 ; break ;
		case  230400:	myBaud =  B230400 ; break ;
		case  460800:	myBaud =  B460800 ; break ;
		case  500000:	myBaud =  B500000 ; break ;
		case  576000:	myBaud =  B576000 ; break ;
		case  921600:	myBaud =  B921600 ; break ;
		case 1000000:	myBaud = B1000000 ; break ;
		case 1152000:	myBaud = B1152000 ; break ;
		case 1500000:	myBaud = B1500000 ; break ;
		case 2000000:	myBaud = B2000000 ; break ;
		case 2500000:	myBaud = B2500000 ; break ;
		case 3000000:	myBaud = B3000000 ; break ;
		case 3500000:	myBaud = B3500000 ; break ;
		case 4000000:	myBaud = B4000000 ; break ;

		default:
		  return -2 ;
	}

	if ((fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
		cout << "Failed to open\n";   

	fcntl (fd, F_SETFL, O_RDWR) ;

	// Get and modify current options:

	tcgetattr (fd, &options) ;

	cfmakeraw   (&options) ;
	cfsetispeed (&options, myBaud) ;
	cfsetospeed (&options, myBaud) ;

	options.c_cflag |= (CREAD | CLOCAL);

	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;

	options.c_iflag &= ~(ICANON|ECHO|ECHOE|ISIG);
	tcsetattr (fd, TCSANOW, &options) ;
	usleep (10000) ;	// 10mS
	return 1;
}

/* Instantiating the new engine */
arduino::arduino() : engineFrame("arduino_engine",1) // 1 is the engine ID
{ 
 serial("/dev/ttyUSB0",9600); // Giving the device file and baud rate
 timestamp = 0;               // Resetting the timestamp
}

/* Obtain the string data from the Arduino base */
string arduino::get_ser_data()
{
	char read_buffer[10];
	int result;
	for(int try_count = 0; try_count <= 5; try_count++)
	{
		if(ioctl(fd, FIONREAD, &result) == -1)
			result = -1;
		else
		{			
			if(result > 0)
			{	
				cout << "result: " << result << endl;			
				read(fd,&read_buffer,result);				
				break;
			}
		}		
		usleep(10000);
	}	
	//int reply = atoi(read_buffer);
	string reply = string(read_buffer);
	return reply;
}

/* Send a request ID and wait for the response */
string arduino::query(int x)
{
	Send(x);
	string c = get_ser_data();
	cout << "Rx data: " << c << endl; // Debugging
	return c;
}

void arduino::test()
{
	// Use this only for tests	
}

/* Periodically run this to test the connection to the base */
int arduino::ping()
{	
	srand(static_cast<int>(getpid()));
	vector<int> vec;

	for(int i = 1; i <= 3; i++)	
		vec.push_back((rand()%100 + 1)+300);	

	for(const auto i: vec)
		query(i);
}

/* Initialize all the signals */
void arduino::init()
{
	usr_rt = make_shared<signals> ("usr_right",base);
	usr_lt = make_shared<signals> ("usr_left",base); 
	usr_fr = make_shared<signals> ("usr_front",base);
	usr_rr = make_shared<signals> ("usr_rear",base);

	wheel_r = make_shared<signals> ("wheel_r",base);
	wheel_l = make_shared<signals> ("wheel_l",base);

	power_l = make_shared<signals> ("power_l",base);
	power_r = make_shared<signals> ("power_r",base);

	ir_fr = make_shared<signals> ("ir_front",base);
	ir_rr = make_shared<signals> ("ir_rear",base);

	angle = make_shared<signals> ("angle",base);
	angle_cmd = make_shared<signals> ("angle_cmd",base);

	timestamp_arduino = make_shared<signals> ("timestamp",base);

	cout << "Signals init done. Engine ready to run.\n";
}

int main()
{
 signal(SIGINT,compute);
 Engine.init();
 while(1); // Keep the process alive and do nothing until compute is called
 return 0;
}
