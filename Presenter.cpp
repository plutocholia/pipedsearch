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
    // this->sendSendMe();
}

Presenter::~Presenter(){
}

void Presenter::sendSendMe(){
    // int fd_temp = open(NAMEDPIPE_PRESENTERSTATUS, O_WRONLY);
    // std::string temp_msg = "sendme";
    // write(fd_temp, temp_msg.c_str(), temp_msg.length() + 1);
    // close(fd_temp);
    std::ofstream outfile;
    outfile.open(NAMEDPIPE_PRESENTERSTATUS, std::ios_base::out);
    outfile << "sendme";
    outfile.close();
}

void Presenter::readWorkersData(){
    // std::ifstream fd;
    // fd.open(NAMEDPIPE_WORKER, std::fstream::in);
    for(int i = 0; i < this->nofProcesses; i++){
        std::cout<< "PRESENTER : Reading the " << i << " workers data" << std::endl;
        std::string worker_stuff;

        size_t read_len = 0;
        while(read_len == 0){
            // getline(fd, worker_stuff);
            int fd = open(NAMEDPIPE_WORKER, O_RDONLY);
            char temp[LEN_MSG] = {'\0'};
            read(fd, temp, sizeof(temp));
            close(fd);
            worker_stuff = temp;

            // std::ifstream fd;
            // fd.open(NAMEDPIPE_WORKER, std::fstream::in);
            // getline(fd, worker_stuff);
            // fd.close();
            read_len = worker_stuff.size();
        }

        std::cout<< "PRESENTER : got " << i << " workers data with length of " << worker_stuff.size() << std::endl;
        if(worker_stuff != "NO_RESULT\n"){
            std::vector<std::string> vec_worker = Utills::splitBy(worker_stuff, "^");
            std::vector<std::pair<int, std::string>> nice_vec = this->addAndPrepair(vec_worker);
            if(this->sorts.size() != 0){
                if(this->sorts[0].second == ASC)
                    std::sort(nice_vec.begin(), nice_vec.end(), Utills::asortpair);
                else std::sort(nice_vec.begin(), nice_vec.end(), Utills::dsortpair);
                this->mergeSortedData(nice_vec);
            }
        }else{std::cout << "PRESENTER : was an empty message\n";}
        // this->sendSendMe();
    }
    // fd.close();
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
        if(bigcnt == 0){ // headers
            std::vector<std::string> temp = Utills::splitBy(item, "+");
            int tracker = 0;
            for(auto& item2 : temp){
                Utills::removeSpace(item2);
                if(item2 == this->sorts[0].first){
                    this->sortLocation = tracker;
                    break;
                }
                tracker += 1;
            }
        }
        else{ // the data
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
        }    
        bigcnt += 1;
    }
    return res;
}


void Presenter::printResult(){
    std::cout<< "~~ done ~~" << std::endl;
    if(this->sorts.size() != 0){
        std::cout << "final results(with sort): " << std::endl;
        for(auto& item : this->col2_sorted_data){
            Utills::removeSpace(this->raw_date[item.first].second);
            std::cout << this->raw_date[item.first].second << std::endl;
        }
    }
    else{
        std::cout << "final results(without sort): " << std::endl;
        for(auto& item : this->raw_date){
            Utills::removeSpace(item.second);
            std::cout<< item.second << std::endl;
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
    
    // int fd_temp = open(NAMEDPIPE_WORKER, O_WRONLY);
    // std::string temp_msg = "sendme";
    // write(fd_temp, temp_msg.c_str(), temp_msg.length());
    // close(fd_temp);

    presenter->readWorkersData();
    presenter->printResult();
    delete presenter;

    std::cout << "Presenter Gone\n";
    return 0;
}
