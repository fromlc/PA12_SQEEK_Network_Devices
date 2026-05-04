//------------------------------------------------------------------------------
// PA12_SQEEK_Network_Devices.cpp
//------------------------------------------------------------------------------
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
// input file name and format
//------------------------------------------------------------------------------
const std::string INPUT_FILENAME = "PA12_network_devices.txt";

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
void readFile();

//------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------
int main()
{
    readFile();
    std::cout << "Hello World!\n";
}


//------------------------------------------------------------------------------
// read input file and display
//------------------------------------------------------------------------------
void readFile()
{
    std::ifstream input(INPUT_FILENAME);
    std::string line;

    std::string token;

    while (getline(input, line))
    {
        std::vector<std::string> result;
        std::stringstream ss(line);

        while (std::getline(ss, token, ','))
        {
            result.push_back(token);
        }

        for (const auto& word : result)
        {
            std::cout << word << "\n";
        }
    }

    // done with file
    input.close();
}