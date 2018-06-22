#include "Scheduler.h"

int main(int argc, const char * argv[]) {
    
    Scheduler scheduler;
    string command;
    string targetRate;
    string currencyPair;
    string timePeriod;
    bool quit = false;

    while (!quit) {
        cout << "Commands = [start, quit, setRate double, setCurrency string, setTimePeriod int, getRate, getCurrency, getTimePeriod]" << endl;
        
        string inputLine;
        
        cin >> ws;
        getline(cin, inputLine);
        cout << endl;
        
        stringstream ss(inputLine);
        
        ss >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command.substr(0, 2) == "st") { // start
            scheduler.start();
        }
        
        else if (command.substr(0, 1) == "q") { // quit
            quit = true;
        }
        
        else if (command.substr(0, 4) == "setr") { // setRate
            ss >> targetRate;
            scheduler.setTargetRate(targetRate);
            cout << "targetRate <- " <<  scheduler.getTargetRate() << endl;
            targetRate.clear();
        }
        
        else if (command.substr(0, 4) == "setc") { // setCurrency
            ss >> currencyPair;
            scheduler.setCurrencyPair(currencyPair);
            cout << "currencyPair <- " <<  scheduler.getCurrencyPair() << endl;
            currencyPair.clear();
        }
        
        else if (command.substr(0, 4) == "sett") { // setTimePeriod
            ss >> timePeriod;
            scheduler.setTimePeriod(timePeriod);
            cout << "timePeriod <- " <<  scheduler.getTimePeriod() << " sec" << endl;
            timePeriod.clear();
        }
        
        else if (command.substr(0, 4) == "getr") { // getRate
            cout << "targetRate = " <<  scheduler.getTargetRate() << endl;
        }
        
        else if (command.substr(0, 4) == "getc") { // getCurrency
            cout << "currencyPair = " <<  scheduler.getCurrencyPair() << endl;
        }
        
        else if (command.substr(0, 4) == "gett") { // getTimePeriod
            cout << "timePeriod = " <<  scheduler.getTimePeriod() << " sec" << endl;
        }
        
        else
            cout << "Invalid command" << endl;
       
        cout << endl;
        command.clear();
    }

    return 0;
}
