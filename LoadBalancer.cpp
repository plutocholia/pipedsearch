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
#include <fstream>
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
    // std::string input = "Platform = PS4 - Genre = Racing - NA_Sales = descending - processes = 2 - dir = sales";
    std::string input;
    std::cout<<"Enter ur command :";
    std::getline(std::cin, input);
    if(input == "quit") exit(EXIT_SUCCESS);
    this->cmd = new Command(input);
    this->cmd->parseUserInput();
}

void LoadBalancer::createFifo(){

    remove(NAMEDPIPE_LOADBALANCER);
    remove(NAMEDPIPE_WORKER);

    mkfifo(NAMEDPIPE_LOADBALANCER, 0666);
    mkfifo(NAMEDPIPE_WORKER, 0666);
    std::cout<< "FIFOS are Created!" << std::endl;
    // if( < 0) perror("unable to create named pipe");
    // if( < 0)  perror("unable to create named pipe");
    // mkfifo(NAMEDPIPE_PRESENTERSTATUS, 0666);
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
}


void LoadBalancer::balance(){
    std::vector<std::vector<std::string> > workers_load;
    std::vector<int> distr_files(5);

    // balance files to fileter
    for(int i = 0; i < this->cmd->getNofProcesses(); i++){
        std::vector<std::string> fnames;
        for(int j = 0; j < this->file_names.size(); j++){
            if(j % this->cmd->getNofProcesses() == i) fnames.push_back(this->file_names[j]);
        }
        workers_load.push_back(fnames);
    }

    // creating worker processes
    for(int i = 0; i < this->cmd->getNofProcesses(); i++){
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
            // Worker* _worker = new Worker(payload);
            // _worker->parsePayload();
            // _worker->doFiltering();
            // _worker->sendDataToPresenter();

            char _wid[10] = {'\0'};
            strcpy(_wid, std::to_string(i).c_str());
            char* _args[] = {"./worker", payload, _wid, NULL};
            execv(_args[0], _args);

            // end of worker stuff
            break;
        }
        else if(pid > 0){
            std::cout<< "worker " << i << " is created with pid of "<< pid << std::endl;
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

void LoadBalancer::run(){
    // creating the payload
    std::string payload = "";
    payload += "processes = " + std::to_string(this->cmd->getNofProcesses());
    if(this->cmd->getSorts().size() != 0){
        payload += " - ";
        payload += this->cmd->getSorts()[0].first + " = " + this->cmd->getSorts()[0].second;
    }
    int pid;
    if((pid = fork()) == 0){
        int fd = open(NAMEDPIPE_LOADBALANCER, O_RDONLY);
        char read_payload[LEN_MSG] = {'\0'};
        read(fd, read_payload, LEN_MSG);
        close(fd);
        char* _args[]={"./presenter", read_payload, NULL};
        execv(_args[0], _args);
    }
    else if(pid > 0){
        int fd = open(NAMEDPIPE_LOADBALANCER, O_WRONLY);
        write(fd, payload.c_str(), payload.length() +  1);
        close(fd);
        fd = open(NAMEDPIPE_LOADBALANCER, O_RDONLY);
        char read_payload[LEN_MSG] = {'\0'};
        read(fd, read_payload, LEN_MSG);
        std::string temp = read_payload;
        std::cout<<"ACK :[" << temp << "] : Presenter is running..." << std::endl;
        this->balance();
    }
    else{
        perror("problem in forking at loadbalancer to presenter");
        exit(EXIT_FAILURE);
    }
    // this->balance();
}