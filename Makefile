all: 
	g++ -Wall fan_control.cpp -lwiringPi

debug:
	g++ -Wall fan_control.cpp -lwiringPi -D DEBUG
