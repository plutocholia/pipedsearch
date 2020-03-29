#include "Presenter.h"
#include "Utills.h"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
#include <functional>

Presenter::Presenter(std::string payload){
    this->last_id = 0;
    this->sortLocation = -1;
    std::vector<std::string> tokens = Utills::splitBy(payload, "-");
    for(auto item : tokens){
        std::vector<std::string> temp =  Utills::splitBy(item, "=");
        for(auto& x : temp) Utills::removeSpace(x);
        if(temp[0] == "processes") this->nofProcesses = atoi(temp[1].c_str());
        else this->sorts.push_back(std::make_pair(temp[0], temp[1]));
    }
}

Presenter::~Presenter(){
}

std::vector<std::string> Presenter::makeVecWorker(const std::string& file){
    std::vector<std::string> headers;
    std::vector<std::string> res;

    // reading from given file
    std::ifstream fs;
    size_t nof_line = 0;
    fs.open(file, std::fstream::in);
    if(!fs){
        std::perror("Could not open the given file");
        exit(EXIT_FAILURE);
    }
    // reading first line
    std::string line;
    getline(fs, line);
    if(nof_line == 0){
        if(line == NULL_WORKER){ return res; }
        // header of file
        headers = Utills::splitBy(line, "-");
        for(auto& x : headers) Utills::removeSpace(x);
        nof_line += 1;
    }
    // finding sort index
    if(sorts.size() != 0){
        int cnt = 0;
        for(auto item : headers){
            if(item  == this->sorts[0].first){ this->sortLocation = cnt; break;}
            cnt += 1;
        }
    }
    while(getline(fs, line)){
        if(nof_line == 1){ if(line == NO_RESULT) return res;}
        res.push_back(line);
    }
    fs.close();
    return res;
}



void Presenter::readWorkersData(){
    // std::ifstream fd;
    // fd.open(NAMEDPIPE_WORKER, std::fstream::in);
    // sleep(3);
    int fd = open(NAMEDPIPE_WORKER, O_RDONLY, 0);
    int total_data_sent = 0;
    int i = 0;
    // sleep(3);
    while(i < this->nofProcesses){

        std::string worker_stuff;

        char temp[LEN_MSG] = {'\0'};
        if(read(fd, temp, LEN_MSG) <= 0){
            continue;
        }
        
        worker_stuff = temp;

        // std::cout <<"\t\t\t\t[" << worker_stuff << "]" << std::endl;
        std::vector<std::string> read_lines = Utills::splitBy(worker_stuff, "\n");
        // Utills::printStringVector(read_lines);
        
        // unbuffer the read_lines to real_lines
        std::vector<std::string> real_lines;
        for(auto xx : read_lines)
            if(xx.size() >= 4){ 
                real_lines.push_back(xx);
                i += 1;
                std::cout<< "PRESENTER : got " << i << " data " << std::endl;
            }

        // reading each read lines and makes a vec_worker
        for(auto& xx : real_lines){
            Utills::removeSpace(xx);
            std::vector<std::string> vec_worker = this->makeVecWorker(xx);
            total_data_sent += vec_worker.size();
            // checks null worker or no result
            if(vec_worker.size() != 0){
                std::vector<std::pair<int, std::string>> nice_vec = this->addAndPrepair(vec_worker);
                // sort and merge
                if(this->sorts.size() != 0){
                    if(this->sorts[0].second == ASC)
                        std::sort(nice_vec.begin(), nice_vec.end(), Utills::asortpair);
                    else std::sort(nice_vec.begin(), nice_vec.end(), Utills::dsortpair);
                    this->mergeSortedData(nice_vec);
                }
            }
        }

    }
    close(fd);
    this->printResult();
    std::cout << "total number of data: " << total_data_sent << std::endl;
}

void Presenter::mergeSortedData(const std::vector<std::pair<int, std::string>>& data){
    std::vector<std::pair<int, std::string>> res(data.size() + this->col2_sorted_data.size());
    if(this->sorts[0].second == ASC)
    std::merge(
        this->col2_sorted_data.begin(),
        this->col2_sorted_data.end(),
        data.begin(),
        data.end(),
        res.begin(),
        Utills::asortpair
    );
    else
    std::merge(
        this->col2_sorted_data.begin(),
        this->col2_sorted_data.end(),
        data.begin(),
        data.end(),
        res.begin(),
        Utills::dsortpair
    );
    this->col2_sorted_data = res;
}


std::vector<std::pair<int, std::string>> Presenter::addAndPrepair(const std::vector<std::string>& data){
    std::vector<std::pair<int, std::string>> res;
    int bigcnt = 0;
    for(auto item : data){
        std::vector<std::string> temp = Utills::splitBy(item, "-");
        int cnt = 0;
        for(auto& item2 : temp){
            Utills::removeSpace(item2);
            if(cnt == this->sortLocation){
                res.push_back(std::make_pair(this->last_id, item2));
                break;
            }
            cnt += 1;
        }
        this->raw_date.push_back(std::make_pair(this->last_id, item));
        this->last_id += 1;
        bigcnt += 1;
    }
    return res;
}


void Presenter::printResult(){
    std::cout<< "~~ done ~~" << std::endl;
    if(this->raw_date.size() == 0){
        std::cout << "No result :)" << std::endl;
        return;
    }
    if(this->sorts.size() != 0){
        std::cout << "final results(with sort): " << std::endl;
        int cnt = 0;
        for(auto& item : this->col2_sorted_data){
            Utills::removeSpace(this->raw_date[item.first].second);
            std::cout << cnt + 1 << ":" << this->raw_date[item.first].second << std::endl;
            cnt += 1;
        }
    }
    else{
        std::cout << "final results(without sort): " << std::endl;
        int cnt = 0;
        for(auto& item : this->raw_date){
            Utills::removeSpace(item.second);
            std::cout<< cnt + 1 << ":" << item.second << std::endl;
            cnt += 1;
        }
    }
}


int main(int argc, char const *argv[])
{
    
    Presenter* presenter = new Presenter(argv[1]);

    std::string payload = "done";
    int fd = open(NAMEDPIPE_LOADBALANCER, O_WRONLY);
    write(fd, payload.c_str(), payload.length() +  1);
    close(fd);
    presenter->readWorkersData();
    delete presenter;

    std::cout << "Presenter is Gone\n";
    system("rm ./Results/*");
    return 0;
}
