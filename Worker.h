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
    std::vector<std::string> headers;
    size_t workerID;
    bool if_sendme();
public:
    Worker(/* args */);
    Worker(const std::string&);
    void setPayload(const std::string&);
    void parsePayload();
    void doFiltering();
    void sendDataToPresenter();
    ~Worker();
    static size_t getWorkersCount();
};




#endif // !WORKER_H