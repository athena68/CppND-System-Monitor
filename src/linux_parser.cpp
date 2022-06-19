#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, units;
  float mem_total = 0, mem_free = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> units) {
        if (key == "MemTotal:") {
          mem_total = stof(value);
        } else if (key == "MemFree:") {
          mem_free = stof(value);
        }
      }
    }
  }
  return ((mem_total - mem_free)/mem_total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string suspend, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suspend >> idle;
  }
  return std::stol(suspend);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  string PidNum = std::to_string(pid);

  string utime, stime, cutime, cstime;
  long totaltime = 0;

  std::ifstream filestream(kProcDirectory + PidNum + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // #14 utime - CPU time spent in user code, measured in clock ticks
    // #15 stime - CPU time spent in kernel code, measured in clock ticks
    // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)

    for(int i=0; i < 22; i++) {
      linestream >> value;
      switch(i) {
        case 13:
          utime = value; break;
        case 14:
          stime = value; break;        
        case 15:
          cutime = value; break;        
        case 16:
          cstime = value; break;
        default:
          break;        
      }
    }
    totaltime = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
  }

  // printf("[PID=%s] utime=%s, stime=%s, cutime=%s, cstime=%s\n", PidNum.c_str(), utime.c_str(), stime.c_str(), cutime.c_str(), cstime.c_str());
 
  return totaltime; 

}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  string usertime, nicetime, systemtime, idletime;
  string iowait, irq, softirq, steal, guest, guest_nice;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> usertime >> nicetime >> systemtime >> idletime
                >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }

  return vector<string>{usertime, nicetime, systemtime, idletime,
                       iowait, irq, softirq, steal, guest, guest_nice};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;

}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string PidNum = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PidNum + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  
  return line;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value, ignore;
  string PidNum = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PidNum + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> ignore) {
        if (key == "VmSize:") {
          long int MBValue = (long int)(stol(value)/1000);
          return to_string(MBValue);
        }
      }
    }
  }

  return string();
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, uid, ignores;
  string PidNum = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + PidNum + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid >> ignores) {
        if (key == "Uid:") {
          return uid;
        }
      }
    }
  }  

  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, user, ignore, compare_uid, ignores;
  string PidNum = std::to_string(pid);

  string uid = Uid(pid);

  std::ifstream filestream2(kPasswordPath);
  if (filestream2.is_open()) {
    while (std::getline(filestream2, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> ignore >> compare_uid >> ignores) {
        if (compare_uid == uid) {
          return user;
        } 
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  string PidNum = std::to_string(pid);
  long starttime=0;

  std::ifstream filestream(kProcDirectory + PidNum + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    //(22) starttime %llu
    for(int i=0; i < 22; i++) {
      linestream >> value;
      if (i==21) {
          starttime = stol(value);
      }
    }
  }

  return starttime;
}
