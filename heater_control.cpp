//Raspberry Pi GPIO Heizungssteuerung

//DEV COMPILING: 
//Mehrzeiler
//clear; rm -frv gpio_control.o; clear; g++ -g -fPIC -Wall -c gpio_control.cpp;date

//Einzeiler
//clear; rm -frv gpio_control.o; g++ -g -fPIC -Wall -c gpio_control.cpp; rm -frv heater_control ;g++ -g -fPIC heater_control.cpp -o heater_control gpio_control.o -lwiringPi -static-libstdc++; date

//LIB Check: 
//ldd heater_control
	/*
	linux-vdso.so.1 (0x7ec67000)
	/usr/lib/arm-linux-gnueabihf/libarmmem.so (0x76f57000)
	libwiringPi.so => /usr/local/lib/libwiringPi.so (0x76f23000)
	libm.so.6 => /lib/arm-linux-gnueabihf/libm.so.6 (0x76ea4000)
	libgcc_s.so.1 => /lib/arm-linux-gnueabihf/libgcc_s.so.1 (0x76e77000)
	libc.so.6 => /lib/arm-linux-gnueabihf/libc.so.6 (0x76d38000)
	/lib/ld-linux-armhf.so.3 (0x76f6d000)
	libpthread.so.0 => /lib/arm-linux-gnueabihf/libpthread.so.0 (0x76d0f000)
	librt.so.1 => /lib/arm-linux-gnueabihf/librt.so.1 (0x76cf8000)
	libcrypt.so.1 => /lib/arm-linux-gnueabihf/libcrypt.so.1 (0x76cb9000)
	*/
	
//Binary Check: 
//file heater_control
	//heater_control: ELF 32-bit LSB executable, ARM, EABI5 version 1 (GNU/Linux), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 3.2.0, BuildID[sha1]=35085260cae1c306139b7a1caaa73c912ec5f30d, not stripped


//Info http://wiringpi.com/reference/core-functions/

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

//Public Variablen (für Object Files)
string temperatur_ist;
string temperatur_soll;
bool zu_kalt = false;
bool zu_warm = false;
string wiring_pi_nummer;

void signalHandler( int signum) 
{
	cout << "Interrupt signal (" << signum << ") received.\n";
	cout<<"PROBLEM Heater Control Daemon"<<endl;
	
	exit (666);
}


int main(int argc, char *argv[]) 
{

	//cout<<"Heater Control v1.0"<<endl;
	//string temperatur_soll;		//Besser signed int (-10 Grad bis + 60 Grad)		//Public Variable
	
	//Soll Temperatur und GPIO Porst übergeben
	temperatur_soll = argv[1];
	wiring_pi_nummer = argv[2];
	//Eingabeprüfung#######
	
	
	
	cout<<"Eingabe Temeperatur: "<<temperatur_soll<<endl;
	
//####################################################################
	//Error Signals

	signal(SIGSEGV, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

//####################################################################
//Temperatur auslesen

	// Temepratur vom Sensor via sysFS auslesen 
	//cat /sys/bus/w1/devices/28-030597790d9b/w1_slave 
	ifstream ifs ("/sys/bus/w1/devices/28-030597790d9b/w1_slave", ifstream::in);
	char temp_rohdaten_char = ifs.get();
	string rohdaten_string_1, rohdaten_string_2;

	while (ifs.good()) 
	{
		//cout<<temp_rohdaten_char;
		//cout<<"temp_rohdaten_char#"<<temp_rohdaten_char<<endl;
		temp_rohdaten_char = ifs.get();
		rohdaten_string_1 = temp_rohdaten_char;
		rohdaten_string_2.append(rohdaten_string_1);
	}

	ifs.close();

	//cout<<"rohdaten_string_2#"<<rohdaten_string_2<<endl;
	
	//Temeperaturdaten filtern##################################
	//string temperatur_ist;	//Public
	int wo_ist_t_gleich = -1 ; 

	wo_ist_t_gleich = rohdaten_string_2.find("t=");
	
	//cout<<"wo_ist_t_gleich#"<<wo_ist_t_gleich<<"#"<<endl;
	//cout<<"wo_ist_endl#"<<wo_ist_endl<<"#"<<endl;
	if (wo_ist_t_gleich > 0)
	{
		//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich,rohdaten_string_2.length());}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		try{temperatur_ist = rohdaten_string_2.substr(wo_ist_t_gleich + 2,rohdaten_string_2.length());}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,5);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,14);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich + 2, wo_ist_endl - 2);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
	}
	
	//cout<<"temp#"<<temp<<"#"<<endl;
	
	int wo_ist_endl = -1 ; 
	wo_ist_endl = temperatur_ist.find("\n");
	//cout<<"wo_ist_endl#"<<wo_ist_endl<<"#"<<endl;
	if (wo_ist_endl > 0)
	{
		try{temperatur_ist = temperatur_ist.substr(0,wo_ist_endl);}catch( std::out_of_range& exception ){cout<<"Error temperatur_ist local_block std::out_of_range"<<endl;}
	}
	
	//cout<<"temperatur_ist #"<<temperatur_ist<<"#"<<endl;
	//####################################
	
	//Beim Eingabeparameter (temperatur_soll = argv[1];) den Punkt wegmachen.
	char chars2[] = ".";
	for (unsigned int j = 0; j < strlen(chars2); ++j)
	{
		temperatur_soll.erase(std::remove(temperatur_soll.begin(), temperatur_soll.end(), chars2[j]), temperatur_soll.end());
	}
	
	//cout<<"temperatur_soll#"<<temperatur_soll<<"#"<<endl;
	
//####################################################################
//Temperatur vergelichen
while(true)
{
	int temperatur_ist_int = - 1;
	temperatur_ist_int = stoi(temperatur_ist);
	cout<<"temperatur_ist_int #"<<temperatur_ist_int<<"#"<<endl;

	int temperatur_soll_int = - 1;
	temperatur_soll_int = stoi(temperatur_soll);
	cout<<"temperatur_soll_int#"<<temperatur_soll_int<<"#"<<endl;

//ggf. Temperatur Kontrolle pthreaden####
	if (temperatur_soll_int > temperatur_ist_int)
	{
		//cout<<"Zu KALT, Heizung hochregeln"<<endl;		
		
		zu_kalt = true;
		zu_warm = false;
		
		//gpio object file aufruf
		void gpio_control();
		gpio_control();
	
	}
	
	if (temperatur_ist_int > temperatur_soll_int)
	{
		//cout<<"Zu WARM, Heizung runterregeln"<<endl;
		
		zu_kalt = false;
		zu_warm = true;
		
		//gpio object file aufruf
		void gpio_control();
		gpio_control();
	}

	delay(1000);
}
//####################################################################


//####################################################################
//Log to File
//####################################################################


//####################################################################
//ENDE

	return (0);
}
