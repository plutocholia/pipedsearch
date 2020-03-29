#include "Worker.h"
#include "Utills.h"
#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MDB if(false)


Worker::Worker(/* args */)
{
}

Worker::~Worker()
{
}

void Worker::insertResultsToFile(){
    std::string res = "";
    if(this->fnames.size() != 0){
        for(int i = 0; i < this->headers.size(); i++){
            res += this->headers[i];
            if(i != this->headers.size() - 1) res += " - ";
        }
        res += "\n";
        if(this->filtered_lines.size() == 0) res += NO_RESULT;
        else{ 
            int cnt = 0;
            for(auto item : this->filtered_lines){
                res += item;
                if(cnt != this->filtered_lines.size() - 1) res += "\n";
                cnt += 1;
            }
        }
    }
    else{
        res += NULL_WORKER;
    }
    
    std::ofstream ofs;
    ofs.open(this->result_address, std::ios_base::out);
    ofs << res;
    ofs.close();
}


Worker::Worker(const std::string& palyload, size_t _id){
    this->payload = palyload;
    this->workerID = _id;
    // sleep(1*_id);
    this->result_address = DIR_RESULTS;
    this->result_address += "worker" + std::to_string(this->workerID);
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
    if(this->fnames.size() == 0) MDB std::cout<<"WORKER "<< this->workerID <<" : I HAVE NO JOBS TO DOOO\n";
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
            this->headers = headers;
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
    }
}

void Worker::sendDataToPresenter(){
    
    std::string res = this->result_address + "\n";
    std::cout << "WORKER " << this->workerID << " : sending " << res.length() << " bytes of data\n";
    int fd = open(NAMEDPIPE_WORKER, O_WRONLY);
    write(fd, res.c_str(), res.length());
    close(fd);

}   

int main(int argc, char const *argv[])
{
    std::string payload = argv[1];
    Worker* _worker = new Worker(payload, std::stoi(argv[2]));
    _worker->parsePayload();
    _worker->doFiltering();
    _worker->insertResultsToFile();
    _worker->sendDataToPresenter();
    MDB std::cout << "Worker Number " << argv[2] << " is gone" << std::endl;
    return 0;
}
