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
    // this->col2_sorted_data.push_back(
    //     std::make_pair(15, "13")
    // );

    // this->col2_sorted_data.push_back(
    //     std::make_pair(15, "12")
    // );

    // this->col2_sorted_data.push_back(
    //     std::make_pair(15, "-0.14")
    // );


    // this->col2_sorted_data.push_back(
    //     std::make_pair(15, "0.25")
    // );

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

void Presenter::readWorkersData(){
        
    for(int i = 0; i < this->nofProcesses; i++){
        std::string worker_stuff;

        int fd = open(NAMEDPIPE_WORKER, O_RDONLY);
        char temp[LEN_MSG] = {'\0'};
        read(fd, temp, sizeof(temp));
        close(fd);
        worker_stuff = temp;

        // std::ifstream fd;
        // fd.open(NAMEDPIPE_WORKER, std::fstream::in);
        // getline(fd, worker_stuff);
        // fd.close();
        

        std::vector<std::string> vec_worker = Utills::splitBy(worker_stuff, "^");
        std::vector<std::pair<int, std::string>> nice_vec = this->addAndPrepair(vec_worker);
        if(this->sorts.size() != 0){
            if(this->sorts[0].second == ASC)
                std::sort(nice_vec.begin(), nice_vec.end(), Utills::asortpair);
            else std::sort(nice_vec.begin(), nice_vec.end(), Utills::dsortpair);
            this->mergeSortedData(nice_vec);
            // break;
        }
    }
    Utills::printPairIdStringVector(this->col2_sorted_data);

}

void Presenter::mergeSortedData(const std::vector<std::pair<int, std::string>>& data){
    std::vector<std::pair<int, std::string>> res(data.size() + this->col2_sorted_data.size());
    // int i = 0, j = 0, state = 2;
    // std::cout<< "first sorted data : " << std::endl;
    // Utills::printPairIdStringVector(this->col2_sorted_data);
    // if(this->col2_sorted_data.size() != 0){
    //     while(true){
    //         if((this->sorts[0].second == ASC) && Utills::asortpair(this->col2_sorted_data[i], data[j]))
    //             res.push_back(this->col2_sorted_data[i++]);
    //         else res.push_back(data[j++]);
    //         if((i == this->col2_sorted_data.size()) && (j != data.size())){
    //             state = 2;
    //             break;
    //         }
    //         if((j == data.size()) && (i != this->col2_sorted_data.size())){
    //             state = 1;
    //             break;
    //         }
    //         if((j == data.size()) && (i == this->col2_sorted_data.size())){
    //             state = 0;
    //             break;
    //         }
    //     }
    // }
    // if(state == 0){}
    // else if(state == 1){
    //     for(; i < this->col2_sorted_data.size(); i++)
    //         res.push_back(this->col2_sorted_data[i]);
    // }
    // else{
    //     for(; j < data.size(); j++)
    //         res.push_back(data[j]);
    // }
    // this->col2_sorted_data = res;
    // std::cout<<"at the end : " << std::endl;
    // Utills::printPairIdStringVector(this->col2_sorted_data);

    // std::cout<< "first sorted data : " << std::endl;
    // Utills::printPairIdStringVector(this->col2_sorted_data);
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
    // std::cout<<"at the end : " << std::endl;
    // Utills::printPairIdStringVector(this->col2_sorted_data);
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
                    std::cout<< "the sort location is : " << this->sortLocation << std::endl;
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
    std::cout<< " done " << std::endl;
    for(auto& item : this->col2_sorted_data){
        Utills::removeSpace(this->raw_date[item.first].second);
        std::cout << this->raw_date[item.first].second << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    Presenter* presenter = new Presenter(argv[1]);
    presenter->readWorkersData();
    presenter->printResult();
    return 0;
}
