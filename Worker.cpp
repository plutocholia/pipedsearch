#include "Worker.h"
#include "Utills.h"
#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

bool Worker::if_sendme(){
    int fd_temp = open(NAMEDPIPE_PRESENTERSTATUS, O_RDONLY);
    std::string temp_msg;
    char msg_temp[LEN_MSG] = {'\0'};
    std::cout<< "[" << temp_msg << "]" << std::endl;
    read(fd_temp, msg_temp, LEN_MSG);
    
    close(fd_temp);
    temp_msg = msg_temp;
    std::cout<< "[" << temp_msg << "]" << std::endl;
    // sleep(1);
    if(temp_msg == "sendme") return true;
    return false;
}


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
    // while(this->if_sendme() == false){
    //     // sleep(1);
    // }
    // this->if_sendme();
    std::cout << "WROKER SEES SENDME" << std::endl;
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
    if(this->fnames.size() == 0) std::cout<<"WORKER : I HAVE NO JOBS TO DOOO\n";
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
        // break;
    }
}

void Worker::sendDataToPresenter(){
    std::string res = "";
    for(int i = 0; i < this->headers.size(); i++){
        res += this->headers[i];
        if(i != this->headers.size() - 1) res += " + ";
    }
    if(this->fnames.size() != 0){
        if(this->filtered_lines.size() == 0) res += "^NO_RESULT";
        else{ 
            res += " ^ ";
            int cnt = 0;
            for(auto item : this->filtered_lines){
                res += item;
                if(cnt != this->filtered_lines.size() - 1) res += "^";
                cnt += 1;
            }
        }
    }
    else{
        res += "NULL_WORKER";
    }
    res += "\n";
    
    
    // while(this->if_sendme() == false){

    // }

    // int fd = open(NAMEDPIPE_WORKER, O_WRONLY | O_APPEND);
    // write(fd, res.c_str(), res.length());
    // close(fd);
    std::cout << "WORKER : the worker is sending " << res.length() << " bytes of data\n";
    std::ofstream outfile;
    outfile.open(NAMEDPIPE_WORKER, std::ios_base::out);
    outfile << res;
    outfile.close();
}



int main(int argc, char const *argv[])
{
    // std::cout<< " u are in "
    std::string payload = argv[1];
    Worker* _worker = new Worker(payload);
    _worker->parsePayload();
    _worker->doFiltering();
    _worker->sendDataToPresenter();
    std::cout << "Worker is Gone\n";
    return 0;
}
