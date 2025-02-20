#include <memory>
#include <ranges>
#include <source_location>
#include <string>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>
#include <algorithm>



class Device
{
public:

    struct Listener
    {
        virtual ~Listener () = default;
        virtual void update (const std::string & name, const std::variant<int, bool> & value) = 0;
    };

    explicit Device (const std::string & modelName)
        : modelName {modelName}
    {
    }

    void addListener (std::shared_ptr<Listener> listener)
    {
        listeners.push_back (listener);
    }

    void removeListener (std::shared_ptr<Listener> listener)
    {
        listeners.erase (std::ranges::remove(listeners, listener).begin (), listeners.end ());
    }

    std::string getModelName () const
    {
        return modelName;
    }

    void setPreampLevel (int levelDb)
    {
        //adding check to ensure input preamp level is inside range

        if (levelDb >= MINUS_INFINITY_DB && levelDb <= UNITY_GAIN_DB){

            preampLevelDb = levelDb;
            notifyListeners ("preampLevel", preampLevelDb);
             
        }else{
            
            std::cout << "Preamp level must be between -127 and 0"; 
            
        }
        
    }
    
    int getPreampLevel () const
    {
        return preampLevelDb;
    }

    //getters and setters for Phantom Power
    void setPhantomPower(bool phantomPowerState){
        
       phantomPowerStatus = phantomPowerState;
       notifyListeners ("phantomPower", phantomPowerState ); 
    }

    // for the sake of clarity for the user
    std::string getPhantomPowerAsString(){
        if (phantomPowerStatus == 1){
            return "on";
        }else {
            return "off";
        }
       
    }

    static constexpr int MINUS_INFINITY_DB = -127;
    static constexpr int UNITY_GAIN_DB = 0;
    

private:
    void notifyListeners (const std::string & name, const std::variant<int, bool> & value) const
    {
        for (const auto & listener : listeners)
            listener->update (name, value);
    }

    std::string modelName;
    int preampLevelDb = MINUS_INFINITY_DB;
    

    bool phantomPowerStatus;

    std::vector<std::shared_ptr<Listener>> listeners;
};

class DeviceMessageGenerator : public Device::Listener
{
public:
    void update (const std::string & name, const std::variant<int, bool> & value) override
    {
        currentMessage = name + " control changed to ";

        if (std::holds_alternative<int> (value))
        {
            const auto intValue = std::get<int> (value);
            currentMessage += std::to_string (intValue);
        }
        else if (std::holds_alternative<bool> (value))
        {
            const auto boolValue = std::get<bool> (value);
            currentMessage += boolValue == true ? "on" : "off";
        }

        std::cout << "Notification: " << currentMessage << "\n";
    }

    std::string currentMessage;
};

std::optional<std::string> findValueString (const std::string & input, const std::string & controlPrefix)
{

    if (! input.starts_with (controlPrefix))
        return std::nullopt;

    auto value = input.substr (controlPrefix.length ());
    value.erase (std::ranges::remove_if (value, isspace).begin (), value.end ());

    return value;
}

bool processDeviceCommand (const std::string & command, Device & device)
{
    if (auto preampLevel = findValueString (command, "set-preamp-level"); preampLevel.has_value ())
    {
        const auto level = std::stoi (*preampLevel); //converts string to int
        device.setPreampLevel (level);
        return true;
    } else if(auto phantom = findValueString(command, "set-phantom-power"); phantom.has_value()){

        const auto phantomPower = *phantom;

        //checking here if value found is equivalent to on or 1
        if (phantomPower == "on" || phantomPower == "1") {
            device.setPhantomPower(true);
        } else if (phantomPower == "off" || phantomPower == "0") {
            device.setPhantomPower(false);
        } else{

            //in case user tries to input kittens!!
            std::cout << "Phantom can only have values [on/off or 1/0]";
        }

        return true;
    }

    //will notify user of invalid command
    return false;
}

void runApp ()
{
    Device device {"Scarlett 2i2 4th Gen [virtual]"};

    auto messageGenerator = std::make_shared<DeviceMessageGenerator> ();
    device.addListener (messageGenerator);

    std::cout << "DEVICE CONTROL v1.0\n";
    std::cout << "===================\n";
    std::cout << "Connected device: " << device.getModelName () << "\n\n";
    std::cout << "Enter a command followed by a value to set it on the device.\n\n";

    std::cout << "Possible commands\n";
    std::cout << "-----------------\n";
    std::cout << "set-preamp-level  [-127 .. 0]  : set the preamp level (dB) \n";

    //to dispay to user how to set phantom power
    std::cout << "set-phantom-power [on/off or 1/0]   :    toggle phantom power on or off \n";
    std::cout << "status                         : view a list of controls and their values \n";
    std::cout << "quit                           : quit Device Control \n\n";

    std::string input;

    std::cout << "> ";
    std::getline (std::cin, input);

    while (! input.empty ())
    {
        if (input == "quit")
            break;

        if (input == "status")
        {
            std::cout << "Preamp level: " << std::to_string (device.getPreampLevel ()) << std::endl;

            //line added here to give status of phantom power now as well
            std::cout << "Phantom Power: " << device.getPhantomPowerAsString();
        }
        else if (! processDeviceCommand (input, device))
        {
            std::cout << "Command failed" << std::endl;
        }

        std::cout << "> ";
        std::getline (std::cin, input);
    }
}

// No need to modify the Tester class
struct Tester
{
    void check (bool assertion, const std::source_location & location = std::source_location::current ())
    {
        if (assertion == true)
            return;

        std::cerr << "Test failed at " << location.file_name () << ":" << location.line () << "\n";
        ++numFailures;
    }

    int numFailures = 0;
};

// No need to modify the MockDeviceListener class
struct MockDeviceListener : Device::Listener
{
    void update (const std::string & name, const std::variant<int, bool> & value) override
    {
        ++updateCallCount;
        latestPropertyName = name;
        latestPropertyValue = value;
    }

    std::string latestPropertyName;
    std::variant<int, bool> latestPropertyValue;
    int updateCallCount {0};
};

void testDeviceCanSetPreampLevel (Tester & tester)
{
    Device device {"testDevice"};

    auto listener = std::make_shared<MockDeviceListener> ();
    device.addListener (listener);
    tester.check (listener->updateCallCount == 0);

    device.setPreampLevel (-12);

    tester.check (device.getPreampLevel () == -12);

    tester.check (listener->updateCallCount == 1);
    tester.check (listener->latestPropertyName == "preampLevel");
    tester.check (std::get<int> (listener->latestPropertyValue) == -12);
}

void testMessageGenerator (Tester & tester)
{
    Device device {"testDevice"};

    auto messageGenerator = std::make_shared<DeviceMessageGenerator> ();
    device.addListener (messageGenerator);

    tester.check (messageGenerator->currentMessage.empty ());
    device.setPreampLevel (-6);
    tester.check (messageGenerator->currentMessage == "preampLevel control changed to -66");
}

void testSetPreampLevelCommand (Tester & tester)
{
    const std::string command = "set-preamp-level -6";

    Device device {"testDevice"};
    tester.check (device.getPreampLevel () == Device::MINUS_INFINITY_DB);

    const auto succeeded = processDeviceCommand (command, device);

    tester.check (succeeded == true);
    tester.check (device.getPreampLevel () == -6);
}

void runTests ()
{
    Tester tester;

    testDeviceCanSetPreampLevel (tester);
    testMessageGenerator (tester);
    testSetPreampLevelCommand (tester);

    std::cout << "Number of test failures: " << tester.numFailures
              << (tester.numFailures == 0 ? " :)" : " :(") << "\n";
}

int main (int argc, const char * argv [])
{
    const bool launchedWithTestArg = argc > 1 && std::string (argv [1]) == "--test";

    if (launchedWithTestArg)
        runTests ();
    else
        runApp ();

    return 0;
}
