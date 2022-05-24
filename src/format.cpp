#include <string>

#include "format.h"

using std::string;


//Return zero filled whitespace for DD/HH/MM/SS leading zeros 
string Format::PrettyDate(const long& data){

    //create a string of the data provided
    string tempFig = std::to_string(data);
    //if the data has only one digit add a leading zero
    if (tempFig.length() == 1){
        string figures = "0"+tempFig;
        return figures;
    }

return tempFig;
}


//Return DD:HH:MM:SS formatted string
string Format::ElapsedTime(const long& seconds) { 

    long hh, mm, ss, dd;
    //calculate DD:HH:MM:SS from seconds
    dd = seconds / 86400;
    hh = (seconds % 86400)/3600;
    mm = ((seconds % 86400)% 3600)/60;
    ss = (((seconds % 86400) % 3600) % 60) / 1;
    //format the string to DD:HH:MM:SS
    string dhms = Format::PrettyDate(dd)+":"+Format::PrettyDate(hh)+":"+Format::PrettyDate(mm)+":"+Format::PrettyDate(ss);

return dhms; 
}