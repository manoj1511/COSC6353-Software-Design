#include <iostream>
#include <string>
using namespace std;




int main() {
    int year;
    int month;
    int day;
    int nYear, nMonth, nDay;
    char fill = '/';
    int count = 0;
    bool date;

    // storing input and checking if its valid
    try {
        while (count < 3)
        {   
            date = true;
            string entered_date;
            cout << "Enter a date in (mm/dd/yyyy) format: ";
            cin >>  entered_date;
            month = stoi(entered_date.substr(0,2));
            day = stoi(entered_date.substr(3,2));
            year = stoi(entered_date.substr(6));
            if (year < 1) 
            {
                cout << "Not valid year.\n";
                date =false;
            }
            if (month < 1 || month > 12) 
            {
                cout << "Not valid month.\n";
                date =false;
            }
            if (day < 1 || day > 31) 
            {
                cout << "Not valid day.\n";
                date =false;
            }
            if(date == false)
            {
                count++;
            }
            else
            {
                date = true;
                break;
            }
        }
    } 
    catch (exception e) 
    {
        cout << "Not valid input.\n";
        return 0;
    }

    // -------------
    // checking if the day is valid
    switch (month) {
        case 2: // February

            // if its leap year
            if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0) {
                if (day > 29) {
                    cout << "Leap year. Only 29 days for February.\n";
                    return 0;
                }

            }
            // else if not leap year
            else {
                if (day > 28) {
                    cout << "Not a leap year. Only 28 days for February.\n";
                    return 0;
                }
            }
            break;

        case 4: // April
            if (day > 30) {
                cout << "Only 30 days for April.\n";
                return 0;
            }
            break;

        case 6: // June
            if (day > 30) {
                cout << "Only 30 days for June.\n";
                return 0;
            }
            break;
            case 9: // September
            if (day > 30) {
                cout << "Only 30 days for September.\n";
                return 0;
            }
            break;

        case 11: // November
            if (day > 30) {
                cout << "Only 30 days for November.\n";
                return 0;
            }
            break;

        default:
            break;
    }

    // -------------
    // the part for finding out the day of the week

    if (month <= 2) {
        nYear = year - 1;
        nMonth = 0;
    }
    else {
        nYear = year;
        nMonth = (4 * month + 23) / 10;
    }


    // calculating the days
    nDay = 365 * year + day + 31 * (month - 1) - nMonth + (nYear / 4) - ((3 * ((nYear / 100) + 1) / 4));

    string days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    if (count<3)
    {
    cout << "You were born on a " << days[nDay % 7] << endl;
    }
    if (( month == 1 && date >= 20) || ( month == 2 && date <=18))
    {
         cout<<" Your zodiac sign is AQUARIUS\n";
         
    }
    else if (( month == 2 && date >= 19) || ( month == 3 && date <= 20))
    {
         cout<<" Your zodiac sign is PISCES\n";
    }
    else if (( month == 3 && date >= 21) || ( month == 4 && date <= 19))
    {
         cout<<" Your zodiac sign is ARIES\n";
    }
    else if (( month == 4 && date >= 20) || ( month == 5 && date <= 20))
    {
         cout<<" Your zodiac sign is TAURUS\n";
    }
    else if (( month == 5 && date >= 21 ) || ( month == 6 && date <= 20 ))
    {
         cout<<" Your zodiac sign is GEMINI\n";
    }
    else if (( month == 6 && date >= 21 ) || ( month == 7 && date <= 22 ))
    {
         cout<<" Your zodiac sign is CANCER\n";
    }
    else if (( month == 7 && date <= 23 ) || ( month == 8 && date <= 22))
    {
         cout<<" Your zodiac sign is LEO\n";
    }
    else if (( month == 8 && date >= 23 ) || ( month == 9 && date <= 22 ))
    {
         cout<<" Your zodiac sign is VIRGO\n";
    }
    else if (( month == 9 && date >= 23 ) || ( month == 10 && date <= 22))
    {
         cout<<" Your zodiac sign is LIBRA\n";
    }
    else if (( month == 10 && date >= 23 ) || ( month == 11 && date <= 21))
    {
         cout<<" Your zodiac sign is SCORPIO\n";
    }
    else if (( month == 11 && date >= 22 ) || ( month == 12 && date <= 21))
    {
         cout<<" Your zodiac sign is SAGUITTARIUS\n";
    }
    else if (( month == 12 && date >= 22 ) || ( month == 1 && date <= 19 ))
    {
         cout<<" Your zodiac sign is CAPRICORN\n";
    }

    
    return 0;
}


