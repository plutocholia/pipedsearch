#include "LoadBalancer.h"
#include "Worker.h"
#include <iostream>
#include <string>
#include <dirent.h>
#include <error.h>
#include "Utills.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

LoadBalancer::LoadBalancer(/* args */)
{
    
}

LoadBalancer::~LoadBalancer()
{
    delete cmd;
}


void LoadBalancer::setCmd(){
    std::string input = "Platform = PS4 - Genre = Racing - \
    NA_Sales = descending - UK_Sales = ascending - processes = 5 - dir = sales";

    // std::cout<<"Enter ur command :";
    // std::getline(std::cin, input);

    this->cmd = new Command(input);
    this->cmd->parseUserInput();
}

void LoadBalancer::setFileNames(){
    DIR* dir;
    struct dirent* ent;
    if((dir = opendir(this->cmd->getFolerDir().c_str())) != NULL){
        while ((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name, ".")!= 0 && strcmp(ent->d_name, "..") != 0){
                std::string temp1(ent->d_name);
                std::string temp2(this->cmd->getFolerDir());
                temp2 += "/" + temp1;
                this->file_names.push_back(temp2.c_str());
            }
        }
        closedir(dir);
    }else{
        perror("Unable to list files in Dataset\n");
        exit(EXIT_FAILURE);
    }
    db std::cout<< "files are extracted" << std::endl;
}


void LoadBalancer::balance(){
    std::vector<std::vector<std::string> > workers_load;
    std::vector<int> distr_files(5);

    for(int i = 0; i < this->cmd->getNofProcesses(); i++){
        std::vector<std::string> fnames;
        for(int j = 0; j < this->file_names.size(); j++){
            if(j % this->cmd->getNofProcesses() == i) fnames.push_back(this->file_names[j]);
        }
        workers_load.push_back(fnames);
    }

    for(int i = 0; i < this->cmd->getNofProcesses(); i++){
        std::cout<< "worker num " << i +  1 << " is created.\n";
        std::vector<std::string> fnames = workers_load[i];
        // int* p = workers_load[i].second;
        int p[2];
        if(pipe(p) == -1){
            perror("unable to create pipe");
            exit(EXIT_FAILURE);
        }
        int pid = fork();
        if(pid == 0){
            close(p[1]);
            char payload[LEN_MSG] = {'\0'};
            read(p[0], payload, LEN_MSG); // receiving payload
            close(p[0]);

            // worker stuff
            Worker* _worker = new Worker(payload);
            _worker->parsePayload();
            _worker->doFiltering();

            // end of worker stuff
            break;
        }
        else if(pid > 0){
            close(p[0]);
            // creating payload with format of
            std::string payload = "";
            int cnt = 0;
            for(auto item : fnames){
                payload += "fname = " + item;
                if(cnt != fnames.size() - 1) payload += " - ";
                cnt += 1;
            }
            cnt = 0;
            for(auto item : this->cmd->getFilters()){
                if(cnt == 0 && payload.size() > 1) payload += " - ";
                payload += item.first + " = " + item.second;
                if(cnt != this->cmd->getFilters().size() - 1) payload += " - ";
                cnt += 1;
            }
            write(p[1], payload.c_str(), payload.length() + 1); // sending payload
            // wait(NULL);
            close(p[1]);
        }
        else{
            perror("fork problem!");
            exit(EXIT_FAILURE);
        }
    }
}

size_t LoadBalancer::getNofProc(){
    return this->cmd->getNofProcesses();
}