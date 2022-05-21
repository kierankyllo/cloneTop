#include <string>

#include "format.h"

using std::string;

string Format::PrettyDate(long data){

    string tempFig = std::to_string(data);
    if (tempFig.length() == 1){
        string figures = "0"+tempFig;
        return figures;
    }
return tempFig;
}

string Format::ElapsedTime(long seconds) { 
    long hh, mm, ss, dd;

    dd = seconds / 86400;
    hh = (seconds % 86400)/3600;
    mm = ((seconds % 86400)% 3600)/60;
    ss = (((seconds % 86400) % 3600) % 60) / 1;

string dhms = Format::PrettyDate(dd)+":"+Format::PrettyDate(hh)+":"+Format::PrettyDate(mm)+":"+Format::PrettyDate(ss);

return dhms; 
}