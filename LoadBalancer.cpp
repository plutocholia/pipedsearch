#include "LoadBalancer.h"
#include <iostream>
#include <string>
#include <dirent.h>
#include <error.h>
#include "Utills.h"

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
        db std::cout<< "Reached to the Dataset directory" << std::endl;
        while ((ent = readdir (dir)) != NULL) {
            this->file_names.push_back(ent->d_name);
        }
        closedir(dir);
    }else{
        perror("Unable to list files in Dataset\n");
        exit(EXIT_FAILURE);
    }
}