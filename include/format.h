#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string PrettyDate(const long& data);
std::string ElapsedTime(const long& times); 
};                                    

#endif