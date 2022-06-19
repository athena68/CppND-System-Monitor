#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {

    long uptime = LinuxParser::UpTime();
    long starttime = LinuxParser::UpTime(Process::Pid());
    long totaltime = LinuxParser::ActiveJiffies(Process::Pid());

    float total_elapsed_time = uptime - (starttime/sysconf(_SC_CLK_TCK));
    cpu_usage_ = (totaltime/sysconf(_SC_CLK_TCK))/total_elapsed_time;

    // printf("uptime=%ld, starttime=%ld, totaltime=%ld\n" , uptime, starttime, totaltime);
    // printf("total_elapsed_time=%f, cpu_usage_=%f\n" , total_elapsed_time, cpu_usage_);

    return cpu_usage_;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return  LinuxParser::UpTime(Process::Pid())/sysconf(_SC_CLK_TCK); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    //Error here due to float number comparison
    return this->cpu_usage_ < a.cpu_usage_;
}

bool Process::operator>(Process const& a) const {
    //Error here due to float number comparison
    return this->cpu_usage_ > a.cpu_usage_;
}