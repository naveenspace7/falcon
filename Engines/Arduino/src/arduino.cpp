#include "engineFrame.h"
#include <iostream>
#include "arduino.h"
#include <signal.h>

using namespace std;

void compute(int signum)
{
	Engine.setLock(1);

 	cout << "Compute function called inside arduino\n";

 	Engine.setLock(0);
}

void arduino::serial(const char* dev_name, int in_baud) // : baud(in_baud)
{	
	baud = in_baud;
	Open(dev_name); //TODO: Device name should be read from XML
}

void arduino::Close()
{
	close(fd);
}

int arduino::Send(int n) //(const int fd, const char *s)
{
	//n is a number but we need the string version of it
	char temp_buff[10];
	sprintf(temp_buff,"%d\r",n);	  		
	int len = len_of(temp_buff);
	return write (fd, temp_buff, len) ;
}

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

arduino::arduino() : engineFrame("arduino_engine",1)
{ 
 serial("/dev/ttyUSB0",9600);
}

int arduino::get_ser_data()
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
	int reply = atoi(read_buffer);
	return reply;
}

int arduino::query(int x)
{
	Send(x);
	int c = get_ser_data();
	cout << "Rx data: " << c << endl;
	return c;
}

void arduino::test()
{
	// Use this only for tests	
}

int arduino::ping()
{	
	srand(static_cast<int>(getpid()));
	vector<int> vec;

	for(int i = 1; i <= 3; i++)	
		vec.push_back((rand()%100 + 1)+300);	

	for(const auto i: vec)
		query(i);
}

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
}

int main()
{
 arduino Engine;
 signal(SIGINT,compute);
 //Engine.init();
 cout << "Engine ready to run:\n";
 while(1);
 return 0;
}
