#ifndef UTILLS_H
#define UTILLS_H

#define DES "descending"
#define ASC "ascending"
#define DEBUG false
#define db if(DEBUG)
#define LEN_MSG 10024
#define DIR_DATASET "./Datasets/"
#define DIR_RESULTS "./Results/"
#define NULL_WORKER "NULL_WORKER"
#define NO_RESULT "NO_RESULT"
#define NAMEDPIPE_LOADBALANCER "./namedpipe_loadbalancer"
#define NAMEDPIPE_WORKER "./namedpipe_worker"
#define NAMEDPIPE_PRESENTERSTATUS "/tmp/namedpipe_presenter_status"

#include <vector>
#include <string>


class Utills
{
public:
    static void removeSpace(std::string&);
    static std::vector<std::string> splitBy(
        std::string, const std::string&
    );
    static void printStringVector(
        const std::vector<std::string>& vec
    );
    static void printPairStringVector(
        const std::vector<std::pair<std::string, std::string> >&
    );
    static void printPairIdStringVector(
        const std::vector<std::pair<int, std::string> >&
    );
    static bool asortpair(
        const std::pair<int,std::string>&, const std::pair<int,std::string>&
    );
    static bool dsortpair(
        const std::pair<int,std::string>&, const std::pair<int,std::string>&
    );
};


#endif // !UTILLS_H