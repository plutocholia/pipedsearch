#ifndef PRESENTER_H
#define PRESENTER_H
#include <string>
#include <vector>

class Presenter
{
private:
    /* data */
    size_t nofProcesses;
    std::vector<std::pair<int, std::string>> raw_date;
    std::vector<std::pair<int, std::string>> col2_sorted_data;
    int last_id;
    std::vector<std::pair<std::string, std::string>> sorts;
    int sortLocation;
    std::vector<std::pair<int, std::string>> addAndPrepair(
        const std::vector<std::string>&
    );
    void mergeSortedData(
        const std::vector<std::pair<int, std::string>>&
    );
public:
    Presenter(std::string);
    void readWorkersData();
    void printResult();
    ~Presenter();
};



#endif // !PRESENTER_H
