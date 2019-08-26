/*
gpio export 12 out
gpio export 16 out
gpio export 20 out
gpio export 21 out
#
gpio -g write 12 1
gpio -g write 16 1
gpio -g write 20 1
gpio -g write 21 1
#
gpio -g write 12 0
gpio -g write 16 0
gpio -g write 20 0
gpio -g write 21 0
*/
/*
gpio readall
	 +-----+-----+---------+------+---+---Pi 3B--+---+------+---------+-----+-----+
	 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |		#Für das Relais genutze GPIO Ports#
	 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
	 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
	 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
	 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
	 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 0 | IN   | TxD     | 15  | 14  |
	 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  | 15  |
	 |  17 |   0 | GPIO. 0 |  OUT | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
	 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
	 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
	 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
	 |  10 |  12 |    MOSI |  OUT | 0 | 19 || 20 |   |      | 0v      |     |     |
	 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
	 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
	 |     |     |      0v |      |   | 25 || 26 | 0 | IN   | CE1     | 11  | 7   |
	 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
	 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
	 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | OUT  | GPIO.26 | 26  | 12  |					X
	 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
	 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | OUT  | GPIO.27 | 27  | 16  |					X
	 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | OUT  | GPIO.28 | 28  | 20  |					X
	 |     |     |      0v |      |   | 39 || 40 | 0 | OUT  | GPIO.29 | 29  | 21  |					X
	 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
	 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
	 +-----+-----+---------+------+---+---Pi 3B--+---+------+---------+-----+-----+
*/
//C_C++ Header
//#include <dirent.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include unistd.h für sleep 
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
//#include <iomanip>
#include <iostream>
//#include <vector>
//#include <sys/time.h>
//#include <sstream>
#include <fstream>
//#include <pthread.h>
//#include <thread>
//#include <time.h>
#include <errno.h>
#include <signal.h>
//#include <malloc.h>
//#include <errno.h>
//#include <sys/mman.h>
//#include <csignal>
//#include <iterator>
//#include <pthread.h>
//Header für setrlimit (ulimits setzen)
//#include <sys/resource.h>
#include <cstring>		//strlen
#include <algorithm>	//string erase
//GPIO wiring PI Header
#include <wiringPi.h>


using namespace std;

extern bool zu_kalt;
extern bool zu_warm;
extern string wiring_pi_nummer;

void gpio_control()
	{
		//cout<<"GPIO Control Object File"<<endl;
		//cout<<"bool zu_kalt#"<<zu_kalt<<"#"<<endl;
		//cout<<"bool zu_warm#"<<zu_warm<<"#"<<endl;
		//cout<<"wiring_pi_nummer#"<<wiring_pi_nummer<<"#"<<endl;
		
		int wiringPiSetup_api_return_code = 0;
		wiringPiSetup_api_return_code = wiringPiSetup();
		
		//cout<<"wiringPiSetup_api_return_code: "<<wiringPiSetup_api_return_code<<endl;
		
		if (wiringPiSetup_api_return_code == -1)
		{	
			cout<<"GPIO wiringPi Error"<<endl;
			exit(-1);
		}

		
		//pinout, relai hängt an GPIO 12,16,20,21
 
		//Phy PIN 32 = wPI 26 = GPIO 12 für den gebrauch exportieren
		pinMode(26, OUTPUT);
	
		//int digitalRead (int pin) HIGH or LOW (1 or 0) 
		int bit_pin_zustand_26 = - 1 ;

		bit_pin_zustand_26 = digitalRead(26); 
		//cout<<"bit_pin_zustand_26 zustand: "<<bit_pin_zustand_26<<endl;
		
		if (zu_kalt == true)
		{
			//Relais schalten sodass strim fließt
			digitalWrite(26, 1);
			bit_pin_zustand_26 = digitalRead(26); 
			cout<<"ZU KALT. HEIZUNG AN"<<endl;
			cout<<"bit_pin_zustand_26 zustand: "<<bit_pin_zustand_26<<endl;
		}
		
		if (zu_warm == true)
		{
			digitalWrite(26, 0);
			bit_pin_zustand_26 = digitalRead(26); 
			cout<<"ZU WARM. HEIZUNG AUS"<<endl;
			cout<<"bit_pin_zustand_26 zustand: "<<bit_pin_zustand_26<<endl;
		}
			
	}
	