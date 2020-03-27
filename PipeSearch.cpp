#include "PipeSearch.h"
#include "LoadBalancer.h"
#include "Utills.h"
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

void PipeSearch::runPipeSearch(){
    std::cout<<"PipeSearch is running ... " << std::endl;
    LoadBalancer* load_balancer = new LoadBalancer();
    load_balancer->setCmd();
    load_balancer->setFileNames();
    load_balancer->balance();
    for(int i = 0; i < load_balancer->getNofProc(); i++){ // preventing zombie stuff
        wait(NULL);
    }
}