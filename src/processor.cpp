#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {

    long int usertime, nicetime, systemtime, idletime;
    long int iowait, irq, softirq, steal, guest, guestnice;
    iowait = irq = softirq = steal = guest = guestnice = 0;

    std::vector<std::string> vec_cpu_util = LinuxParser::CpuUtilization();

    usertime = std::stol(vec_cpu_util[0]);
    nicetime = std::stol(vec_cpu_util[1]);
    systemtime = std::stol(vec_cpu_util[2]);
    idletime = std::stol(vec_cpu_util[3]);
    iowait = std::stol(vec_cpu_util[4]);
    irq = std::stol(vec_cpu_util[5]);
    softirq = std::stol(vec_cpu_util[6]);
    steal = std::stol(vec_cpu_util[7]);
    guest = std::stol(vec_cpu_util[8]);
    guestnice = std::stol(vec_cpu_util[9]);

    // printf("cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",usertime, nicetime, systemtime, idletime,iowait, irq, softirq, steal, guest, guestnice);

    // Guest time is already accounted in usertime
    usertime = usertime - guest;
    nicetime = nicetime - guestnice;

    long int idlealltime = idletime + iowait;
    long int systemalltime = systemtime + irq + softirq;
    long int virtalltime = guest + guestnice;
    long int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;

    // differentiate: actual value minus the previous one
    long int totald = totaltime - m_totaltime;
    long int idled = idlealltime - m_idletime;

    float cpu_percentage = (float)(totald - idled)/totald;

    m_totaltime = totaltime;
    m_idletime = idlealltime;

    return cpu_percentage;

}