#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hh, mm, ss;
    long tmp;

    tmp = seconds;

    // int days;
    // days = tmp/(3600*24);

    // tmp = tmp%(3600*24);

    hh = tmp / 3600;
    tmp = tmp % 3600;

    mm = tmp / 60;
    tmp = tmp % 60;

    ss = tmp;

    // string DAY = std::to_string(days);
    string HH = hh < 10 ? string("0"+std::to_string(hh)) : std::to_string(hh);
    string MM = mm < 10 ? string("0"+std::to_string(mm)) : std::to_string(mm);
    string SS = ss < 10 ? string("0"+std::to_string(ss)) : std::to_string(ss);

    // if (days > 0) {
    //     return string(DAY+" days, "+HH+":"+MM+":"+SS);
    // }

    return string(HH+":"+MM+":"+SS);
}