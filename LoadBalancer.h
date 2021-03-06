#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Command.h"

/*
    Main tasks : 
        Getting inputs from user.
            parse user's inputs.
            make Command Object based on users parsed input
        Make Workers to colaborate.
*/

class LoadBalancer
{
private:
    Command* cmd;
    std::vector<std::string> file_names;
public:
    LoadBalancer(/* args */);
    void setCmd();
    void setFileNames();
    void balance();
    size_t getNofProc();
    void run();
    void createFifo();
    ~LoadBalancer();
};


#endif // LOADBALANCER_H