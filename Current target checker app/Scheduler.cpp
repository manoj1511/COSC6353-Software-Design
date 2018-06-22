#include "Scheduler.h"

bool isNumber(const string &str) {
    bool foundDecimalPoint = false;
    int shift = 0;
    if (str[0] == '-')
        shift = 1;
    for (int i = 0 + shift; i < str.length(); i++) {
        if (str[i] == '.') {
            if (!foundDecimalPoint)
                foundDecimalPoint = true;
            else
                return false;
        }
        else if (!isdigit(str[i])) {
            return false;
        }
    }
    return str.length() > 0 + shift;
}

// this function is for timed peek, i.e. check if user has input anything and stop checking after timeout
int myPeek(int ns) {
    timeval timeout;
    fd_set rfds;
    unsigned char c;
    int fd;
    
    timeout.tv_sec = ns;
    timeout.tv_usec = 0;
    
    fd = 0; // stdin file descriptor is 0
    
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    
    if (select(fd + 1, &rfds, NULL, NULL, &timeout) <= 0) {
        return -1;
    }
    if (read(fd, &c, 1) != 1) {
        return -1;
    }
    return static_cast<int>(c);
}

// this function is solely for curl, it reads html page to string
int writer(char *data, size_t size, size_t nmemb, string *writerData) {
    if (writerData == NULL) {
        return 0;
    }
    writerData->append(data, size*nmemb);
    return static_cast<int>(size*nmemb);
}

Scheduler::Scheduler() {
    targetRate = 1.381;
    currencyPair = "EURUSD";
    timePeriod = 10; // seconds
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &xmlContent);
}

Scheduler::~Scheduler() {
    curl_easy_cleanup(curl);
}

void Scheduler::start() {
    checkRatesPeriodicly();
}

void Scheduler::setTargetRate(string targetRate) {
    if (isNumber(targetRate)) {
        this->targetRate = stod(targetRate);
    }
    else {
        cout << "Invalid targetRate" << endl;
    }
}

void Scheduler::setCurrencyPair(string currencyPair) {
    string temp;
    
    temp = this->currencyPair;
    this->currencyPair = currencyPair;
    
    if (xmlContent.empty()) {
       readFromURL();
        // readLocalFile();
        parse();
    }
    if (extractFromParsedXML("Bid") == "") {
        cout << "Invalid currencyPair" << endl;
        this->currencyPair = temp;
    }
}

void Scheduler::setTimePeriod(string timePeriod) {
    if (isNumber(timePeriod) && stoi(timePeriod) > 0) {
        this->timePeriod = stoi(timePeriod);
    }
    else {
        cout << "Invalid timePeriod" << endl;
    }
}

double Scheduler::getTargetRate() {
    return targetRate;
}

string Scheduler::getCurrencyPair() {
    return currencyPair;
}

int Scheduler::getTimePeriod() {
    return timePeriod;
}

bool Scheduler::readLocalFile() {
    stringstream ss;
    ifstream file(localFilePath.c_str());
    if (!file) {
        cerr << "Error: failed to open the file: " << localFilePath << endl;
        return false;
    }
    ss << file.rdbuf();
    file.close();
    xmlContent = ss.str();
    return true;
}

bool Scheduler::readFromURL() {
    xmlContent.clear();
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLcode::CURLE_OK) {
        cerr << "Error: failed to connect to the url: " << curl_easy_strerror(res) << endl;
        return false;
    }
    else
    {
        cout <<"connected";
    }
    return true;
}

void Scheduler::parse() {
    if (!xmlContent.empty()) {
        doc.clear();
        doc.parse<0>(&xmlContent[0]);
    }
}

string Scheduler::extractFromParsedXML(string targetNodeName) {
    rapidxml::xml_node<> *rootNode = doc.first_node("Rates");
    rapidxml::xml_node<> *childNode = rootNode->first_node("Rate");
    rapidxml::xml_node<> *targetNode;
    
    string targetCurrencyPair = currencyPair;
    string otherCurrencyPair;
    
    transform(targetCurrencyPair.begin(), targetCurrencyPair.end(), targetCurrencyPair.begin(), ::tolower);
    
    while (childNode != NULL) {
        
        otherCurrencyPair = childNode->first_attribute("Symbol")->value();
        
        transform(otherCurrencyPair.begin(), otherCurrencyPair.end(), otherCurrencyPair.begin(), ::tolower);
        
        if (targetCurrencyPair == otherCurrencyPair) {
            currencyPair = childNode->first_attribute("Symbol")->value(); // just for capitalization sensitivity
            targetNode = childNode->first_node(targetNodeName.c_str());
            return targetNode->value();
        }
        childNode = childNode->next_sibling();
    }
    return "";
}

void Scheduler::takeInput() {
    string temp;
    int ch; // the peeked character
    bool quit = false;
    
    while (!quit) {
        unique_lock<mutex> locker(mu);
        ch = -1;
        
        if (condition) {
            quit = true;
        }
        else {
            ch = myPeek(1);
        }
        
        if (ch != -1 && !quit) {
            if (ch != 10) // ENTER = 10
                getline(cin, temp);
            quit = condition = true;
            cond.notify_one();
        }
    }
}

void Scheduler::checkRatesPeriodicly() {
    double bid;
    bool reachedTargetRate = false;
    condition = false;
    
    cout << "Press enter to stop:" << endl;
    
    thread t(&Scheduler::takeInput, this);
    
    
    while (!reachedTargetRate) {
        
       // if (!readFromURL())
           // do something?;
        
        readFromURL();
        // readLocalFile(); // testing on local file for now
        parse();
        
        bid = stod(extractFromParsedXML("Bid"));
        
        unique_lock<mutex> locker(mu);

        if (condition) {
            reachedTargetRate = true;
        }
        
        else if (bid >= targetRate) {
            cout << endl << "Bid = " << bid
                 << " has REACHED the target rate = "  << targetRate
                 << " for the currencyPair : " << currencyPair << endl;
        
            reachedTargetRate = condition = true;
        }
        
        else {
            cout << endl <<  "Bid = " << bid
                 << " did NOT reach the target rate = " << targetRate
                 << " for the currencyPair : " << currencyPair << endl;
            
            cout << "Checking again ..." << endl;
            
            cond.wait_for(locker, chrono::seconds(timePeriod));
            reachedTargetRate = condition;
        }
    }
    
    if (t.joinable()) {
        t.join();
    }

    cout << endl << "Returning to menu ..." << endl;
}
