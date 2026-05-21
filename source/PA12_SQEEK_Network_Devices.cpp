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
// input file name
const std::string INPUT_FILENAME = "PA12_network_devices.txt";

const unsigned ESTIMATED_DEVICE_COUNT = 32;

// error codes
const int ERROR_ALL_OK = 0;
const int ERROR_FILE_OPEN = -1;
const int ERROR_FILE_OTHER = -2;
const int ERROR_UNKNOWN = -99;

// error messages
const char* MSG_FILEREADERROR = "Error opening file : ";
const std::string MSG_ERROR_BAD_PORT_COUNT = "Error converting device port count: ";
const std::string MSG_ERROR_UNKNOWN = "An unknown error occurred.";

//------------------------------------------------------------------------------
// custom exception class
//------------------------------------------------------------------------------
class FileReadError : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return MSG_FILEREADERROR;
    }
};

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
void getFileData(std::vector<NetworkDevice*>& vpDevices);
void setDeviceData(std::stringstream& ss, NetworkDevice*pND);
void processDevices(std::vector<NetworkDevice*>& vpDevices);
void reportDevices(std::vector<NetworkDevice*>& vpDevices);

//------------------------------------------------------------------------------
// initialize NetworkDevice static member
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
// - read input file and store device data
// - throw exception if file cannot be opened
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
// - build Router or Switch instance from stringstream data
// - catch exception on bad device port count, default it to zero
//------------------------------------------------------------------------------
void setDeviceData(std::stringstream& ss, NetworkDevice*pND)
{
    std::getline(ss, pND->name, ',');

    std::string portStr;
    std::getline(ss, portStr, ',');

    try
    {
        pND->ports = stoi(portStr);
    }
    catch (const std::exception& e)
    {
        std::cout << MSG_ERROR_BAD_PORT_COUNT << "\n";

        // default device port count to 0 if conversion fails
        pND->ports = 0;
    }

    std::getline(ss, pND->location, ',');
    std::getline(ss, pND->connectedTo, ',');
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
