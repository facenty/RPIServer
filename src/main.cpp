#include "MyClass.hpp"
#include <wiringPi.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>

#include <utility>
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
	wiringPiSetup();
	std::cout << "Ilosc argumentow: " << argc << std::endl;
	for(auto i = 0u; i < argc; ++i)
		std::cout << "Argument " << i << " ma wartosc: " << argv[i] << std::endl;
	auto myClass = std::make_unique<MyClass>(argc);
	std::cout << std::endl;
}