#ifndef LOGGING_H
#define LOGGING_H

#include <functional>
#include <iostream>
#include <sstream>

namespace logging
{
	// Utility function to capture std::cout output
	std::string captureCoutOutput( std::function<void()> func ) {
		std::stringstream buffer;
		std::streambuf* prevCoutBuf = std::cout.rdbuf( buffer.rdbuf() );  // Redirect cout

		func();  // Execute the function that will produce output to std::cout

		std::cout.rdbuf( prevCoutBuf );  // Restore cout
		return buffer.str();
	}

} // namespace log

#endif /*LOGGING_H*/