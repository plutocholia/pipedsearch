#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <vector>



class Worker
{
    static size_t workersCount;
private:
    /* data */
    std::string payload;
    std::vector<std::pair<std::string, std::string>> filters;
    std::vector<std::string> fnames;
    std::vector<std::string> filtered_lines;
    size_t workerID;
public:
    Worker(/* args */);
    Worker(const std::string&);
    void setPayload(const std::string&);
    void parsePayload();
    void doFiltering();
    ~Worker();
    static size_t getWorkersCount();
};




#endif // !WORKER_H