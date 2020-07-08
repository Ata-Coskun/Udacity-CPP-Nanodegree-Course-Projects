#include "processor.h"
#include "linux_parser.h"
using std::vector;
using std::string;

float Processor::Utilization() {
    return (float)LinuxParser::ActiveJiffies() / LinuxParser::Jiffies();
 }
