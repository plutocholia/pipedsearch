#include "PipeSearch.h"
#include "LoadBalancer.h"
#include "Utills.h"
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

void PipeSearch::runPipeSearch(){
    system("mkdir ./Results");
    std::cout<<"PipeSearch is running ... " << std::endl;
    while(true){
        LoadBalancer* load_balancer = new LoadBalancer();
        load_balancer->setCmd();
        load_balancer->setFileNames();
        load_balancer->createFifo();
        load_balancer->run();
        for(int i = 0; i < load_balancer->getNofProc() + 1; i++){ // preventing zombie stuff
            wait(NULL);
        }
        delete load_balancer;
    }
}