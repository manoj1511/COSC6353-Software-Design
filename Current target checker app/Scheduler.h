#ifndef Scheduler_h
#define Scheduler_h
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <curl/curl.h>
#include "rapidxml-1.13/rapidxml.hpp"
using namespace std;

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    
    void start(); // invoke checkRatesPeriodicly() 
    
    void setTargetRate(string targetRate);
    void setCurrencyPair(string currencyPair); // poor implementation for now, to check if currencyPair is valid
    void setTimePeriod(string timePeriod);
    double getTargetRate();
    string getCurrencyPair();
    int getTimePeriod();
    
private:
    // const string localFilePath = "RatesXML.xml"; // path of local file for testing
    const string url = "https://rates.fxcm.com/RatesXML"; // actual url
    
    double targetRate;
    string currencyPair;
    int timePeriod; // in seconds
    
    string xmlContent; // contains RatesXML.xml as it is
    rapidxml::xml_document<> doc; // contain the parsed xmlContent
    
    CURL *curl;
    
    mutex mu;
    condition_variable cond;
    bool condition;
    
    // bool readLocalFile(); // read local copy of RatesXML.xml -- just for testing
    bool readFromURL(); // read RatesXML.xml from url, using curl library
    void parse(); // after reading the file into the string xmlContent, parse xmlContent into rapidxml::xml_document<> doc
    
    /* extractFromParsedXML():
     this function extract any value, from rapidxml doc, of the follwing nodes:
     Bid, Ask, High, Low, Direction, Last
     where Rate Symbol=currencyPair
     */
    string extractFromParsedXML(string targetNodeName);
    
    /* takeInput():
     this function is used by the thread to take input from user while sleeping
     */
    void takeInput();
    
    /*  checkRatesPeriodicly():
     1 - To read  RatesXML.xml file, use either readFromURL() or readLocalFile(), into xmlContent.
     2 - Parse xmlContent into rapidxml::xml_document<> doc
     3 - extract Bid from rapidxml::xml_document<> doc by calling extractFromXML("Bid")
     4 - Compare Bid with targetRate and print result
     5 - sleep for timePeriod, then repeat to check Bid and target rate again
     */
    void checkRatesPeriodicly();
};

#endif
