/** \file main.cpp
 *  \brief File just for testing
 *
 *  \author Niklas
 */

#include <iostream>
#include <fstream>

#include "iojpeg.h"

/**
 Main function
 @param argc Number of arguments to the executable
 @param argv The actual arguments
 @returns Returns the status of the executable
 */
int main (int argc, const char * argv[])
{
    
    unsigned char* image = readJpegFile(argv[1]);
    
    std::cout << "Read image " << argv[1] << "\n";

    free( image );
    
    return 0;
}
