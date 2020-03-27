#include "Worker.h"
#include "Utills.h"
#include <iostream>
#include <string>
#include <fstream>

size_t Worker::workersCount = 0;

Worker::Worker(/* args */)
{
}

Worker::~Worker()
{
}

Worker::Worker(const std::string& palyload){
    this->payload = palyload;
    Worker::workersCount++;
    this->workerID = getWorkersCount();
    db std::cout << "worker in this process " << this->workerID << " : [" << payload << "]" << std::endl;
}

void Worker::setPayload(const std::string& payload){
    this->payload = payload;
}

void Worker::parsePayload(){
    std::vector<std::string> tokens = Utills::splitBy(this->payload, "-");
    for(auto& item : tokens){
        std::vector<std::string> temp = Utills::splitBy(item, "=");
        for(auto& temp_item : temp) Utills::removeSpace(temp_item);
        if(temp[0] == "fname") this->fnames.push_back(temp[1]);
        else this->filters.push_back(std::make_pair(temp[0], temp[1]));
    }
}

size_t Worker::getWorkersCount(){
    return Worker::workersCount;
}

void Worker::doFiltering(){
    for(auto file : this->fnames){
        std::vector<std::string> headers;
        // opening file
        std::ifstream fs;
        size_t nof_line = 0;
        fs.open(file, std::fstream::in);
        if(!fs){
            std::perror("Could not open the given file");
            exit(EXIT_FAILURE);
        }
        std::string line;
        getline(fs, line);
        if(nof_line == 0){
            // header of file
            headers = Utills::splitBy(line, "-");
            for(auto& x : headers) Utills::removeSpace(x);
            db std::cout<<"headers :";
            db Utills::printStringVector(headers);
            db std::cout<<std::endl;
            nof_line += 1;
        }
        // finding filters index
        std::vector<std::pair<size_t, std::string>> reform_filters;
        int cnt = 0;
        for(auto item : headers){
            for(auto item2 : this->filters)
                if(item == item2.first)
                    reform_filters.push_back(std::make_pair(cnt, item2.second));
            cnt += 1;
        }
        while(getline(fs, line)){
            std::vector<std::string> line_info = Utills::splitBy(line, "-");
            for(auto& x : line_info) Utills::removeSpace(x);
            bool accepted = true;
            for(auto item : reform_filters)
                if(line_info[item.first] != item.second){
                    accepted = false;
                    break;
                }
                    
            if(accepted)
                this->filtered_lines.push_back(line);
        }
        fs.close();
        db std::cout << "File " << file << " is done like : \n";
        db Utills::printStringVector(this->filtered_lines);
        // break;
    }
}