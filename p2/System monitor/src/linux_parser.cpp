#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include <fstream>

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
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() {
    std::ifstream file_reader;
    file_reader.open(kProcDirectory+kMeminfoFilename);

    string mem_total_1, mem_total_2;
    string mem_free_1, mem_free_2;

    if(file_reader){
        string line1;
        std::getline(file_reader,line1);

        std::istringstream linestream(line1);
        linestream >> mem_total_1 >> mem_total_2;

        string line2;
        getline(file_reader,line2);

        std::istringstream linestream2(line2);
        linestream2 >> mem_free_1 >> mem_free_2;

        float mem_total = std::stof(mem_total_2);
        float mem_free = std::stof(mem_free_2);
        file_reader.close();
        return mem_total - mem_free;


    }
    else{std:: cout << "An error ocurred while file reading";
        return -1;
    }
}

long LinuxParser::UpTime() {
    std::ifstream file_reader;
    file_reader.open(kProcDirectory+kUptimeFilename);

    if(file_reader){
        long value;

        string line;
        std:: getline(file_reader,line);

        std::istringstream linestream(line);
        linestream >> value;
        file_reader.close();
        return value;
    }
    else{std:: cout << "An error ocurred while file reading";
        return -1;
    }

}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

long LinuxParser::ActiveJiffies(int pid) {
  string line = "";
  long jiffies = 0;

  std::ifstream file_reader;
  file_reader.open(kProcDirectory + to_string(pid)+kStatFilename);

  if (file_reader) {
    std::getline(file_reader, line);
    // split line into a vector
    std::istringstream linestream(line);
    std::istream_iterator<string> start(linestream);
    std::istream_iterator<string> end;
    std::vector<string> stov(start, end);
    jiffies = stol(stov[13]) + stol(stov[14]) + stol(stov[15]) + stol(stov[16]);
  }
  file_reader.close();
  return jiffies;
}

long LinuxParser::ActiveJiffies() {
  string line = "";
  string num = "0";
  long jiffies = 0;

  std::ifstream file_reader;
  file_reader.open(kProcDirectory + kStatFilename);

  if (file_reader) {
    std::getline(file_reader, line);
    line.erase(line.begin(), line.begin() + 5);
    std::istringstream linestream(line);
    while (linestream >> num) {
      jiffies += std::stol(num);
    }
  }
  file_reader.close();
  return jiffies;
}

long LinuxParser::IdleJiffies() {
  string line = "";
  long jiffies = 0;
  std::ifstream file_reader;
  file_reader.open(kProcDirectory + kStatFilename);
  if (file_reader) {
    std::getline(file_reader, line);
    line.erase(line.begin(), line.begin() + 5);
    // split line into a vector
    std::istringstream linestream(line);
    std::istream_iterator<string> start(linestream);
    std::istream_iterator<string> end;
    std::vector<string> cpu(start, end);
    jiffies = std::stol(cpu[kIdle_]) + std::stol(cpu[kIOwait_]);
  }
  file_reader.close();
  return jiffies;
}

vector<string> LinuxParser::CpuUtilization() {
    vector<string> result_set;

    std::ifstream file_reader;
    file_reader.open(kProcDirectory+kStatFilename);

    if(file_reader){
        string line;
        string check;

        std::getline(file_reader,line);

        string output;
        int pos = line.find("u"); // cp"u"
        line = line.substr(pos+1);

        std::istringstream linestream(line);

        while (linestream >> output){
            result_set.push_back(output);
        }
        file_reader.close();
        return result_set;

    }
    else{std:: cout << "An error ocurred while file reading";
        return {};
    }
 }

int LinuxParser::TotalProcesses() {
    std::ifstream file_reader;
    file_reader.open(kProcDirectory+kStatFilename);

    if(file_reader){
        string line;
        string check;
        int result;
        while(check != "processes" && std::getline(file_reader,line)){

            std::getline(file_reader,line);

            std::istringstream linestream(line);
            linestream >> check >> result;
        }
        file_reader.close();
        return result;

    }
    else{std:: cout << "An error ocurred while file reading";
        return -1;
    }

}

int LinuxParser::RunningProcesses() {
    std::ifstream file_reader;
    file_reader.open(kProcDirectory+kStatFilename);

    if(file_reader){
        string line;
        string check;
        int result;
        while(check != "procs_running"  && std::getline(file_reader,line)){

            std::istringstream linestream(line);
            linestream >> check >> result;
        }
        file_reader.close();
        return result;

    }
    else{std:: cout << "An error ocurred while file reading";
        return -1;
    }

 }

string LinuxParser::Command(int pid) {

    std::ifstream file_reader;
    file_reader.open(kProcDirectory + to_string(pid)+ kCmdlineFilename);

    if(file_reader){
        string line;
        std::getline(file_reader,line);
        file_reader.close();
        return line;

    }
    else{std:: cout << "An error ocurred while file reading";
        return "";
    }

}

string LinuxParser::Ram(int pid) {

    std::ifstream file_reader;
    file_reader.open(kProcDirectory+std::to_string(pid)+kStatusFilename);

    if(file_reader){
        string line;
        string check;
        string result;
        while(check != "VmSize:" && getline(file_reader,line)){

            std::istringstream linestream(line);
            linestream >> check >> result;
        }
        file_reader.close();
        int ram = stoi(result) / 1024;
        return to_string(ram);

    }
    else{std:: cout << "An error ocurred while file reading";
        return "";
    }
}


string LinuxParser::Uid(int pid) {

    std::ifstream file_reader;
    file_reader.open(kProcDirectory+std::to_string(pid)+kStatusFilename);

    if(file_reader){
        string line;
        string check;
        string result;
        while(check != "Uid:" && std::getline(file_reader,line)){

            std::istringstream linestream(line);
            linestream >> check >> result;
        }
        file_reader.close();
        return result;

    }
    else{std:: cout << "An error ocurred while file reading";
        return "";
    }

}

string LinuxParser::User(int pid) {
    std::ifstream file_reader;
    file_reader.open(kPasswordPath);

    if(file_reader){
        string line;
        string username,x,user_id;
        while (std::getline(file_reader,line)){

            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> username >> x >> user_id;

            if(user_id == Uid(pid)){
                return username;
            }
        }
        file_reader.close();
        return "";

    }
    else{std:: cout << "An error ocurred while file reading";
        return "";
    }
}

long LinuxParser::UpTime(int pid) {

      long start_time;
      string line;
      string output;
      std::ifstream file_reader;
      file_reader.open(kProcDirectory + to_string(pid) + kStatFilename);

      if(file_reader){
              while (std::getline(file_reader,line)){

              std::istringstream linestream(line);

             int counter = 0;
              while(linestream >> output){
                  if(counter == 21){
                      start_time = stol(output);
                      break;
                  }
                  counter++;
              }
          }
          file_reader.close();
          return start_time /sysconf(_SC_CLK_TCK);
       }
       else{std:: cout << "An error ocurred while file reading";
          return -1;
       }
}
