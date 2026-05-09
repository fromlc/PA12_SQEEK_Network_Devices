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

const int ERROR_FILE_OPEN = -1;
const int ERROR_FILE_OTHER = -2;

//------------------------------------------------------------------------------
// input file name and format
//------------------------------------------------------------------------------
const std::string INPUT_FILENAME = "PA12_network_devices.txt";

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
static void setDeviceData(NetworkDevice*pND, std::stringstream& ss);

//------------------------------------------------------------------------------
// NetworkDevice static member
//------------------------------------------------------------------------------
unsigned NetworkDevice::assetCount = 0;

//------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------
int main()
{
    VectorWrapper vDevices(ESTIMATED_DEVICE_COUNT);

    try
    {
        getFileData(vDevices.vpDevices);
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

    std::cout << "Number of network device assets :"
        << NetworkDevice::assetCount << "\n\n";

    // count total device ports
    unsigned totalPorts = 0;

    for (NetworkDevice* pDevice : vDevices.vpDevices)
    {
        totalPorts += pDevice->ports;

        pDevice->print();
        std::cout << "\n";
    }

    std::cout << "Total Ethernet ports: " << totalPorts << "\n\n";
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

        setDeviceData(pND, ss);
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
static inline void setDeviceData(NetworkDevice*pND, std::stringstream& ss)
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

