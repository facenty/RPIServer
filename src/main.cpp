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
#include <thread>

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>

static constexpr char serial0[] = "/dev/serial0";
static constexpr uint baudrate = 115200;
static constexpr uint bufSize = 1024 * 4;


auto signalHandler(int signalNumber)
{
	std::cerr << "Exiting by signal interrupt. Signal number: " << signalNumber << std::endl;
	std::exit(EXIT_FAILURE);
}

auto initialize()
{
	if (wiringPiSetup() != 0)
	{
		std::cerr << "wiringPiSetup() error" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::signal(SIGINT, signalHandler);
}

int main(int argc, char *argv[])
{
	initialize();

	std::vector<std::string> args;
	args.reserve(argc);
	for (auto i = 0u; i < static_cast<uint>(argc); ++i)
		args.push_back(argv[i]);
	boost::system::error_code ec;
	boost::asio::io_service io_service;
	boost::asio::serial_port serial_port(io_service);
	serial_port.open("/dev/serial0", ec);
	if (ec) {
		std::cerr << "error : serial port open() failed" << std::endl <<
				  "port name /dev/serial0" << std::endl;
		return EXIT_FAILURE;
	}
	serial_port.set_option(boost::asio::serial_port::baud_rate(115200));
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::vector<char> buf{};
	buf.resize(4*1024);
	serial_port.async_read_some(boost::asio::buffer(buf.data(), buf.size()),
								[&](const boost::system::error_code& error,
									std::size_t bytes_transferred) {
										buf[bytes_transferred] = '\0';
										std::cout << buf.data() << " " << bytes_transferred<< std::endl; 
									});
	io_service.run();
	return EXIT_SUCCESS;
}
