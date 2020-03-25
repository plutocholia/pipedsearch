#include "PipeSearch.h"
#include "LoadBalancer.h"
#include "Utills.h"
#include <iostream>

void PipeSearch::runPipeSearch(){
    std::cout<<"PipeSearch is running ... " << std::endl;
    LoadBalancer* load_balancer = new LoadBalancer();
    load_balancer->setCmd();
    load_balancer->setFileNames();
}