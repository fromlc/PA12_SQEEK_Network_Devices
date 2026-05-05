#include "NetworkDevice.hpp"

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
void readFile(std::vector<NetworkDevice*>& vpDevices);
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
    VectorWrapper vDevices;

    readFile(vDevices.vpDevices);

    std::cout << "Number of network device assets :"
        << NetworkDevice::assetCount << "\n\n";

    for (NetworkDevice* pDevice : vDevices.vpDevices)
    {
        pDevice->print();
        std::cout << "\n";
    }
}

//------------------------------------------------------------------------------
// read input file and display
//------------------------------------------------------------------------------
void readFile(std::vector<NetworkDevice*>& vpDevices)
{
    std::ifstream input(INPUT_FILENAME);
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

