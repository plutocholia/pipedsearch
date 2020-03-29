#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <vector>



class Worker
{
private:
    /* data */
    std::string payload;
    std::vector<std::pair<std::string, std::string>> filters;
    std::vector<std::string> fnames;
    std::vector<std::string> filtered_lines;
    std::vector<std::string> headers;
    size_t workerID;
    std::string result_address;
public:
    Worker(/* args */);
    Worker(const std::string&, size_t);
    void setPayload(const std::string&);
    void parsePayload();
    void doFiltering();
    void sendDataToPresenter();
    void insertResultsToFile();
    ~Worker();
};




#endif // !WORKER_H