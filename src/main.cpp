#include "MyClass.hpp"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <csignal>
#include <cstdio>
#include <unistd.h>

#include <utility>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

static constexpr char serial0 [] = "/dev/serial0";
static constexpr uint baudrate = 115200;
static constexpr uint bufSize = 1024 * 4;


int serial = 0;

auto signalHandler(int signalNumber) {
	if(serial) {
		serialClose(serial);
		serial = 0;
	}
	std::cerr << "Exiting by signal interrupt. Signal number: "<< signalNumber << std::endl; 
	std::exit(EXIT_FAILURE);
}

auto initialize()
{
	if(wiringPiSetup() != 0) {
		std::cerr << "wiringPiSetup() error" << std::endl; 
		std::exit(EXIT_FAILURE);
	}
	std::signal(SIGINT, signalHandler);
}

int main(int argc, char* argv[])
{
	initialize();

	std::vector<std::string> args;
	args.reserve(argc);
	for(auto i = 0u; i < static_cast<uint>(argc); ++i)
		args.push_back(argv[i]);

	serial = serialOpen(serial0, baudrate);
	std::vector<char> buf(bufSize);
	while(true) {
		if(serialDataAvail(serial) > 0) {
			auto size = read(serial, buf.data(), bufSize);
			std::cout << buf.data();
		}
	}

}