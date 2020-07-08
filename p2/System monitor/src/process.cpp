#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::to_string;
using std::vector;

void Process:: set_pid(int pid ){this->pid_ = pid;}
// TODO: Return this process's ID
int Process::Pid() { return this->pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
   unsigned long int activeJiffies = LinuxParser::ActiveJiffies(Pid());
   unsigned long int processUptime = LinuxParser::UpTime(Pid());
   unsigned long int systemUptime = LinuxParser::UpTime();
   auto seconds = systemUptime - processUptime;
   if(seconds <= 0) return 0.0;
   return (float) (1.0*(activeJiffies/sysconf(_SC_CLK_TCK)) / seconds);
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->Pid());}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->Pid());}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {  return LinuxParser::UpTime() - LinuxParser::UpTime(this->pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
     return a.cpuUtilization_ < this->cpuUtilization_;
}
