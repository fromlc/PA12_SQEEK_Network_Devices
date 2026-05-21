//------------------------------------------------------------------------------
// PA12_SQEEK_Network_Devices.cpp
// 
// Code for reading device field data from each line derived from:
// https://www.geeksforgeeks.org/cpp/string-find-in-cpp/
//------------------------------------------------------------------------------
#include "NetworkDevice.hpp"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------------
const unsigned ESTIMATED_DEVICE_COUNT = 32;

const int ERROR_ALL_OK = 0;
const int ERROR_FILE_OPEN = -1;
const int ERROR_FILE_OTHER = -2;
const int ERROR_UNKNOWN = -99;

//------------------------------------------------------------------------------
// input file name and format
//------------------------------------------------------------------------------
const std::string INPUT_FILENAME = "PA12_network_devices.txt";

const std::string MSG_ERROR_UNKNOWN = "An unknown error occurred.";
//------------------------------------------------------------------------------
// derived exception class
//------------------------------------------------------------------------------
class FileReadError : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Error opening file : ";
    }
};

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
void getFileData(std::vector<NetworkDevice*>& vpDevices);
static void setDeviceData(std::stringstream& ss, NetworkDevice*pND);
void processDevices(std::vector<NetworkDevice*>& vpDevices);
void reportDevices(std::vector<NetworkDevice*>& vpDevices);

//------------------------------------------------------------------------------
// NetworkDevice static member
//------------------------------------------------------------------------------
unsigned NetworkDevice::assetCount = 0;

//------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------
int main()
{
    // wrap vector of pointers to ensure memory is deallocated
    VectorWrapper vDevices(ESTIMATED_DEVICE_COUNT);

    processDevices(vDevices.vpDevices);
    reportDevices(vDevices.vpDevices);

    return ERROR_ALL_OK;
}

//------------------------------------------------------------------------------
// read input file and display
//------------------------------------------------------------------------------
void getFileData(std::vector<NetworkDevice*>& vpDevices)
{
    std::ifstream input(INPUT_FILENAME);
    if (!input)
        throw FileReadError();

    std::string line;
    std::string token;

    while (getline(input, line))
    {
        std::stringstream ss(line);

        // use base class pointer
        NetworkDevice* pND = nullptr;

        std::getline(ss, token, ',');
        if (!token.compare("Router"))
            pND = new Router();
        else
            pND = new Switch();

        // store common data in base class members
        setDeviceData(ss, pND);

        // store device specific data in derived class members
        std::getline(ss, token, ',');
        pND->setDeviceSpecific(token);

        vpDevices.push_back(pND);
    }
    // done with file
    input.close();
}

//------------------------------------------------------------------------------
// build Router or Switch instance from stringstream data
//------------------------------------------------------------------------------
static inline void setDeviceData(std::stringstream& ss, NetworkDevice*pND)
{
    std::string token;

    std::getline(ss, token, ',');
    pND->name = token;
    std::getline(ss, token, ',');
    pND->ports = std::stoi(token);
    std::getline(ss, token, ',');
    pND->location = token;
    std::getline(ss, token, ',');
    pND->connectedTo = token;
}

//------------------------------------------------------------------------------
// try/catch for file reading and processing
//------------------------------------------------------------------------------
void processDevices(std::vector<NetworkDevice*>& vpDevices)
{
    try
    {
        getFileData(vpDevices);
    }
    catch (const FileReadError& e)
    {
        std::cout << e.what() << INPUT_FILENAME << "\n";
        exit(ERROR_FILE_OPEN);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        exit(ERROR_FILE_OTHER);
    }
    catch (...)
    {
        std::cout << MSG_ERROR_UNKNOWN << "\n";
        exit(ERROR_UNKNOWN);
    }
}

//------------------------------------------------------------------------------
// display device information and total device ports
//------------------------------------------------------------------------------
void reportDevices(std::vector<NetworkDevice*>& vpDevices)
{
    std::cout << "Number of network device assets: "
        << NetworkDevice::assetCount << "\n\n";

    unsigned totalPorts = 0;
    for (NetworkDevice* pDevice : vpDevices)
    {
        pDevice->print();
        std::cout << "\n";

        totalPorts += pDevice->ports;
    }

    std::cout << "Total Ethernet ports: " << totalPorts << "\n\n";
}

