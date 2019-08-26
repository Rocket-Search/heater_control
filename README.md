Heater Control Raspberry Pi

Heater Control with based on Raspberry Pi with GPIO.
Usage for smart home and controlling the radiator .
You can also use it with openHAB 2 (https://www.openhab.org)

Prerquirements:

Hardware:
 - Raspberry Pi Zero WH
 - Eberle TS+ 5.11 Thermoantrieb stromlos geschlossen thermisch 
 - 4 Kanal Relais 5V/230V Raspberry Pi Optokoppler Modul Channel Relay Arduino
 - Digitales Temperatursensor Modul für Arduino Raspberry Pi DS18B20 SE08006

Software:
	raspbian
	gpio -> https://github.com/WiringPi/WiringPi
	
Usage:
	edit heater_control.cpp and change "ifstream ifs ("/sys/bus/w1/devices/28-030597790d9b/w1_slave", ifstream::in);" to your thermometer ID.
	compile with "clear; rm -frv gpio_control.o; g++ -g -fPIC -Wall -c gpio_control.cpp; rm -frv heater_control ;g++ -g -fPIC heater_control.cpp -o heater_control gpio_control.o -lwiringPi -static-libstdc++; date"
	run "./heater_control <desired temperature> <wiringPiPort>" for example "./heater_control 22.214 26". You can get the phy. pin port to the corresponding wiringPi port by using the command "gpio readall" . 
 
