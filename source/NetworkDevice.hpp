//------------------------------------------------------------------------------
// Base class NetworkDevice declaration and definition
// Derived class Router declaration and definition
// Derived class Switch declaration and definition
//------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
// base class
//------------------------------------------------------------------------------
class NetworkDevice
{
public:
    static unsigned assetCount;
    unsigned assetID;
    unsigned ports;
    std::string deviceType;
    std::string name;
    std::string location;
    std::string connectedTo;

    // constructor
    NetworkDevice() : ports(0)
    {
        assetID = ++assetCount;
    }

    // destructor
    virtual ~NetworkDevice() { }

    // derived classes must provide
    virtual void setDeviceSpecific(std::string data) = 0;

    // derived classes will override
    virtual void print() const
    {
        std::cout << deviceType << " " << name
            << " is in " << location << "\n";
        std::cout << "Asset ID: " << assetID << "\n";
        std::cout << "Ports: " << ports << "\n";
        std::cout << "Connected to: " << connectedTo << "\n";
    }

    // overload + operator
    unsigned operator+(NetworkDevice& rhs)
    {
        return rhs.ports;
    }
};

//------------------------------------------------------------------------------
// derived classes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Router
//------------------------------------------------------------------------------
class Router : public NetworkDevice
{
public:
    std::string protocols;

    // constructor
    Router()
    {
        deviceType = "Router";
    }

    // derived classes must provide
    void setDeviceSpecific(std::string data)
    {
        protocols = data;
    }

    // overrides base class
    void print() const
    {
        NetworkDevice::print();
        std::cout << "Routing protocols: " << protocols << "\n";
    }
};

//------------------------------------------------------------------------------
// Switch
//------------------------------------------------------------------------------
class Switch : public NetworkDevice
{
public:
    std::string vlans;

    // constructor
    Switch()
    {
        deviceType = "Switch";
    }

    // derived classes must provide
    void setDeviceSpecific(std::string data)
    {
        vlans = data;
    }

    // overrides base class
    void print() const
    {
        NetworkDevice::print();
        std::cout << "Configured with VLANS: " << vlans << "\n";
    }
};

//------------------------------------------------------------------------------
// wrapper class for deallocating memory
//------------------------------------------------------------------------------
class VectorWrapper
{
public:
    std::vector<NetworkDevice*> vpDevices;

    // constructor
    VectorWrapper(unsigned estDevices)
    {
        vpDevices.reserve(estDevices);
    }

    // destructor
    ~VectorWrapper()
    {
        for (NetworkDevice* pND : vpDevices)
            delete pND;
    }
};