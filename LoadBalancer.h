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
    ~LoadBalancer();
};





#endif // LOADBALANCER_H